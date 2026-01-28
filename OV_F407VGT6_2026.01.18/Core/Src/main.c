/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwrb.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
sysWorkMode system_mode = MODE_MEASUREMENT;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DRV8833 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
DRV8833_HandleTypeDef hdrv1 = {
    .CHANNEL_A = {.direction = DRV_STAGE_COAST,
                  .htim      = &DRV_A_PWM_TIMER,
                  .CH1       = DRV_AIN1_PWM_CH1,
                  .CH2       = DRV_AIN2_PWM_CH2},
    // .CHANNEL_B   = {.direction = DRV_STAGE_COAST,
    //                 .htim      = &DRV_B_PWM_TIMER,
    //                 .CH1       = DRV_BIN1_PWM_CH1,
    //                 .CH2       = DRV_BIN2_PWM_CH2},
    .nSLEEP_Port = DRV_nSLEEP_GPIO_PORT,
    .nSLEEP_Pin  = DRV_nSLEEP_PIN,
};
volatile bool drv_excitingLevel = 0;
float drv_PWM_freq              = 200.0f;
float drv_PWM_DR                = 50.0f;
bool drv_PWM_isChanged          = true;
uint32_t drv_PWM_cnt            = 0;
uint32_t drv_PWM_halfCnt        = 0;
uint32_t drv_PWM_assertCnt      = 0;
volatile bool freq_scan_enabled = false;
volatile bool DR_scan_enabled   = false;

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LDC1101 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
LDC1101_HandleTypeDef ldc2       = {&hspi2, LDC2_CS_GPIO_PORT, LDC2_CS_PIN};
volatile bool ldc2_isWorking     = false;
volatile bool ldc2_isReading     = false;
volatile bool ldc2_isReadingPrev = false;
volatile bool ldc2_dataReady     = false;
uint8_t LDC_status               = 0;
uint16_t Rp_data                 = 0;
uint16_t L_data                  = 0;
uint32_t LHR_data                = 0;

uint32_t cnt = 0;

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Calibrition <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
float mean_episode[CAL_SAMPLE_EPISODE] = {0};
uint8_t cal_episode_cnt                = 0;
uint32_t cal_wait_start_tick           = 0;
uint32_t cal_sample_cnt                = 0;
uint64_t cal_sum                       = 0;
uint64_t cal_sum_sq                    = 0;
uint32_t cal_total_sample_cnt          = 0;
uint64_t cal_total_sum                 = 0;
uint64_t cal_total_sum_sq              = 0;
volatile CalState_t cal_state          = CAL_IDLE;
uint16_t cal_current_position_um       = 0;

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* UART2 */
volatile uint8_t UART2_RX_activeBuffer = 0;
char UART2_RX_DMA_buffer[2][MSG_LEN]   = {0};
static lwrb_t uart2_tx_rb;
static uint8_t uart2_tx_rb_mem[UART2_TX_RB_SIZE];
static volatile bool uart2_tx_busy = false;
/* UART3 */
uint8_t UART3_TX_buffer[QUEUE_LEN * 9];
uint8_t frame[9];
uint8_t * volatile UART3_TX_head = UART3_TX_buffer;
uint8_t * volatile UART3_TX_tail = UART3_TX_buffer;
volatile bool UART3_DMA_busy     = false;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> OLED <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
volatile bool oled_update_pending = false;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Button <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
volatile bool button_scan_pending = false;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LED <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
volatile bool led_breath_enabled = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void DRV_Start(DRV8833_Channel *ch, TIM_HandleTypeDef *htim) {
    HAL_GPIO_WritePin(hdrv1.nSLEEP_Port, hdrv1.nSLEEP_Pin, GPIO_PIN_SET);
    HAL_TIM_PWM_Start(ch->htim, ch->CH1);
    HAL_TIM_PWM_Start(ch->htim, ch->CH2);

    HAL_TIM_Base_Start_IT(htim);
}
void DRV_Stop(DRV8833_Channel *ch, TIM_HandleTypeDef *htim) {
    HAL_GPIO_WritePin(hdrv1.nSLEEP_Port, hdrv1.nSLEEP_Pin, GPIO_PIN_RESET);
    HAL_TIM_PWM_Stop(ch->htim, ch->CH1);
    HAL_TIM_PWM_Stop(ch->htim, ch->CH2);

    HAL_TIM_Base_Stop_IT(htim);
    /* Keep TIM2 running for button scanning even when PWM is stopped. */
    HAL_TIM_Base_Start_IT(&htim2);
}

static uint16_t UART3_BufferUsedUnsafe(void) {
    const uint16_t buffer_len = sizeof(UART3_TX_buffer);
    uint16_t head_offset      = (uint16_t)(UART3_TX_head - UART3_TX_buffer);
    uint16_t tail_offset      = (uint16_t)(UART3_TX_tail - UART3_TX_buffer);

    if (head_offset >= tail_offset) {
        return head_offset - tail_offset;
    }
    return buffer_len - (tail_offset - head_offset);
}
/* Queue arbitrary TX bytes; call UART3_StartTx() after batching frames */
bool UART3_Enqueue(const uint8_t *data, uint16_t len) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq(); /* Critical Section Begin */

    const uint16_t buffer_len = sizeof(UART3_TX_buffer);
    uint16_t used             = UART3_BufferUsedUnsafe();
    uint16_t free_space       = buffer_len - used - 1;

    if (free_space < len) {
        __set_PRIMASK(primask); /* Critical Section End */
        return false;
    }

    for (uint16_t i = 0; i < len; i++) {
        *UART3_TX_head = data[i];
        UART3_TX_head++;
        if (UART3_TX_head >= UART3_TX_buffer + buffer_len) {
            UART3_TX_head = UART3_TX_buffer;
        }
    }
    __set_PRIMASK(primask); /* Critical Section End */
    return true;
}
/* Start DMA transmission if idle; safe to call repeatedly */
void UART3_StartTx(void) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq(); /* Critical Section Begin */
    if (UART3_DMA_busy || (UART3_TX_head == UART3_TX_tail)) {
        __set_PRIMASK(primask); /* Critical Section End */
        return;
    }

    const uint16_t buffer_len = sizeof(UART3_TX_buffer);
    uint8_t *start            = UART3_TX_tail;
    uint16_t size;
    if (UART3_TX_head > UART3_TX_tail) {
        size = (uint16_t)(UART3_TX_head - UART3_TX_tail);
    } else {
        size = (uint16_t)((UART3_TX_buffer + buffer_len) - UART3_TX_tail);
    }
    UART3_DMA_busy = true;
    __set_PRIMASK(primask);

    if (HAL_UART_Transmit_DMA(&huart3, start, size) != HAL_OK) {
        primask = __get_PRIMASK();
        __disable_irq(); /* Critical Section Begin */
        UART3_DMA_busy = false;
        __set_PRIMASK(primask); /* Critical Section End */
    }
}
bool UART2_TxEnqueue(const uint8_t *data, uint16_t len) {
    if (data == NULL || len == 0U) {
        return false;
    }
    uint32_t primask = __get_PRIMASK();
    __disable_irq(); /* Critical Section Begin */
    lwrb_sz_t written = lwrb_write(&uart2_tx_rb, data, (lwrb_sz_t)len);
    __set_PRIMASK(primask); /* Critical Section End */
    return written == (lwrb_sz_t)len;
}
void UART2_TxKick(void) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq(); /* Critical Section Begin */
    if (uart2_tx_busy || lwrb_get_full(&uart2_tx_rb) == 0U) {
        __set_PRIMASK(primask); /* Critical Section End */
        return;
    }

    uint8_t *start = lwrb_get_linear_block_read_address(&uart2_tx_rb);
    lwrb_sz_t size = lwrb_get_linear_block_read_length(&uart2_tx_rb);
    if (size > UINT16_MAX) {
        size = UINT16_MAX;
    }
    uart2_tx_busy = true;
    __set_PRIMASK(primask); /* Critical Section End */

    if (HAL_UART_Transmit_IT(&huart2, start, (uint16_t)size) != HAL_OK) {
        primask = __get_PRIMASK();
        __disable_irq(); /* Critical Section Begin */
        uart2_tx_busy = false;
        __set_PRIMASK(primask); /* Critical Section End */
    }
}
void UART2_SendString(const char *str) {
    if (str == NULL) {
        return;
    }
    size_t len = strlen(str);
    if (len == 0U) {
        return;
    }
    if (len > UINT16_MAX) {
        len = UINT16_MAX;
    }
    if (UART2_TxEnqueue((const uint8_t *)str, (uint16_t)len)) {
        UART2_TxKick();
    }
}
void UART2_TxOnComplete(uint16_t sent_len) {
    if (sent_len == 0U) {
        return;
    }
    uint32_t primask = __get_PRIMASK();
    __disable_irq(); /* Critical Section Begin */
    lwrb_skip(&uart2_tx_rb, (lwrb_sz_t)sent_len);
    uart2_tx_busy = false;
    __set_PRIMASK(primask); /* Critical Section End */
    UART2_TxKick();
}
void UART2_Log(const char *level, const char *file, int line, const char *message) {
    uint32_t ticks   = HAL_GetTick();
    uint32_t hours   = ticks / 3600000U;
    uint32_t minutes = (ticks / 60000U) % 60U;
    uint32_t seconds = (ticks / 1000U) % 60U;
    uint32_t millis  = ticks % 1000U;
    char time_buf[16];
    snprintf(time_buf, sizeof(time_buf), "%02lu:%02lu:%02lu.%03lu", (unsigned long)hours, (unsigned long)minutes,
             (unsigned long)seconds, (unsigned long)millis);

    char msg_copy[MSG_LEN];
    strncpy(msg_copy, message, MSG_LEN - 1);
    msg_copy[MSG_LEN - 1] = '\0';

    char log_buf[MSG_LEN];
    snprintf(log_buf, sizeof(log_buf), "[%s] [%s] [%s:%d] %s\r\n", level, time_buf, file, line, msg_copy);
    UART2_SendString(log_buf);
}

static void StartReading(void) {
    led_breath_enabled = true;
    DRV_Start(&hdrv1.CHANNEL_A, &htim1);
}
static void StopReading(void) {
    led_breath_enabled = false;
    HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
    DRV_Stop(&hdrv1.CHANNEL_A, &htim1);

    HAL_UART_DMAStop(&huart3);
    UART3_DMA_busy = false;
    UART3_TX_head = UART3_TX_tail = UART3_TX_buffer;
    memset(UART3_TX_buffer, 0, sizeof(UART3_TX_buffer));
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
    lwrb_init(&uart2_tx_rb, uart2_tx_rb_mem, sizeof(uart2_tx_rb_mem));
    buttons_init();
    HAL_TIM_Base_Start_IT(&htim2);
    if (system_mode == MODE_CALIBRITION) {
        cal_current_position_um = CALIBRITION_START_UM;
        uint8_t init_frame[11];
        memset(init_frame, 0xFF, sizeof(init_frame));
        bool init_enqueued = UART3_Enqueue(init_frame, sizeof(init_frame));
        if (init_enqueued) {
            UART3_StartTx();
        }
    }

    char mode_msg[MSG_LEN];
    snprintf(mode_msg, sizeof(mode_msg), "System Mode: %s", system_mode ? "CALIBRITION" : "MEASUREMENT");
    UART2_Log("INFO", "main.c", __LINE__, mode_msg);

    /* UART2 DMA Init */
    HAL_UART_Receive_DMA(&huart2, (uint8_t *)UART2_RX_DMA_buffer[UART2_RX_activeBuffer], MSG_LEN);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

    /* OLED Init */
    OLED_Init();
    OLED_Clear();
    OLED_Display_On();
    HAL_TIM_Base_Start_IT(&htim3);
    /* DRV8833 Init */
    DRV_Init(&hdrv1);
    UART2_Log("INFO", "main.c", __LINE__, "DRV8833 Initialization Done...");
    /* LDC1101 Init */
    if (ldc1101_init(&ldc2, _LDC1101_RP_SET_RP_MIN_1_5KOhm)) {
        UART2_Log("ERROR", "main.c", __LINE__, "LDC1101 Initialization Failed...");
    } else {
        UART2_Log("INFO", "main.c", __LINE__, "LDC1101 Initialization Done...");
        uint16_t rcount = (uint16_t)(ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_RCOUNT_LSB));
        rcount |= (uint16_t)(ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_RCOUNT_MSB) << 8);
        const float f_clk_hz    = 16000000.0f;                 // LDC1101 Extenal Reference Clock Frequency = 16MHz
        const float conv_cycles = (float)(rcount * 16U + 55U); // RCOUNT*16 + 55 reference cycles
        float sample_rate_hz    = f_clk_hz / conv_cycles;
        float sample_rate_ksps  = sample_rate_hz / 1000.0f;
        char rate_msg[MSG_LEN];
        snprintf(rate_msg, sizeof(rate_msg), "LDC1101 sample rate: %.3f kSPS (RCOUNT=0x%04X)", sample_rate_ksps,
                 rcount);
        UART2_Log("TRACE", "main.c", __LINE__, rate_msg);
        ldc2_isWorking = true;
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
        if (system_mode == MODE_MEASUREMENT) {
            /* DRV8833 Frequency Control */
            if (drv_PWM_isChanged) {
                // TIM1 Interrupt Frequency = 100kHz
                const float baseFreq = 100000.0f;
                // Calculate the number of interrupts needed for one period
                float cnt_f       = baseFreq / drv_PWM_freq;
                drv_PWM_cnt       = (uint32_t)(cnt_f + 0.5f);
                float assert_f    = cnt_f * (drv_PWM_DR / 100.0f);
                drv_PWM_assertCnt = (uint32_t)(assert_f + 0.5f);
                drv_PWM_halfCnt   = drv_PWM_cnt / 2;
                drv_PWM_isChanged = false;
            }

            /* LDC Reading Control */
            if (ldc2_isReading != ldc2_isReadingPrev) {
                if (ldc2_isReading)
                    StartReading();
                else
                    StopReading();
                ldc2_isReadingPrev = ldc2_isReading;
            }
            /* LDC Data Get and Transmit */
            if (ldc2_isReading) {
                LDC_status = ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_STATUS);
                if ((LDC_status & 0x01) == 0)
                    ldc2_dataReady = true;
                if (ldc2_dataReady) {
                    ldc2_dataReady = false;
                    LHR_data       = ldc1101_getLHRData(&ldc2);
                    // === frame [LHR(4B)][Freq(2B)][Duty(1B)][Level(1B)][Pad(2B)] ===
                    frame[0]             = (uint8_t)(LHR_data);
                    frame[1]             = (uint8_t)(LHR_data >> 8);
                    frame[2]             = (uint8_t)(LHR_data >> 16);
                    frame[3]             = (uint8_t)(LHR_data >> 24);
                    uint16_t freq_scaled = (uint16_t)(drv_PWM_freq * 100.0f);
                    frame[4]             = (uint8_t)(freq_scaled);
                    frame[5]             = (uint8_t)(freq_scaled >> 8);
                    frame[6]             = (uint8_t)drv_PWM_DR;
                    frame[7]             = (uint8_t)drv_excitingLevel;
                    frame[8]             = 0xAA;

                    // ===== enqueue frame into ring buffer =====
                    bool frame_enqueued = UART3_Enqueue(frame, sizeof(frame));

                    // ===== Data Transmission =====
                    if (frame_enqueued) {
                        UART3_StartTx();
                    }
                }
            }
        } else if (system_mode == MODE_CALIBRITION) {
            /* LDC Reading Control */
            switch (cal_state) {
                case CAL_WAIT_SETTLE:
                    if ((HAL_GetTick() - cal_wait_start_tick) >= CAL_SETTLE_TIME_MS) {
                        cal_state = CAL_SAMPLING;
                    } else {
                        break;
                    }
                    /* fallthrough */
                case CAL_SAMPLING:
                    HAL_TIM_Base_Stop_IT(&htim3);
                    LDC_status = ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_STATUS);
                    if ((LDC_status & 0x01) == 0) {
                        ldc2_dataReady = true;
                    }
                    if (ldc2_dataReady) {
                        ldc2_dataReady = false;
                        LHR_data       = ldc1101_getLHRData(&ldc2);
                        cal_sample_cnt++;
                        cal_sum += LHR_data;
                        cal_sum_sq += (uint64_t)LHR_data * LHR_data;
                        cal_total_sample_cnt++;
                        cal_total_sum += LHR_data;
                        cal_total_sum_sq += (uint64_t)LHR_data * LHR_data;
                        // ===== Data Transmission =====
                        if (cal_sample_cnt >= CAL_SAMPLE_NUM) {
                            cal_state = CAL_DONE;
                        }
                    }
                    if (cal_state != CAL_DONE) {
                        break;
                    }
                    /* fallthrough */
                case CAL_DONE: {
                    float sample_cnt              = (float)cal_sample_cnt;
                    float episode_mean            = (float)cal_sum / sample_cnt;
                    mean_episode[cal_episode_cnt] = episode_mean;
                    cal_episode_cnt++;

                    if (cal_episode_cnt < CAL_SAMPLE_EPISODE) {
                        cal_sample_cnt      = 0;
                        cal_sum             = 0;
                        cal_sum_sq          = 0;
                        cal_wait_start_tick = HAL_GetTick();
                        cal_state           = CAL_WAIT_SETTLE;
                    } else {
                        float total_cnt = (float)cal_total_sample_cnt;
                        float mean      = (float)cal_total_sum / total_cnt;
                        float var       = (float)cal_total_sum_sq / total_cnt - mean * mean;
                        uint8_t cal_frame[11];
                        /* Frame: [cal_current_position_um(2B)][mean(4B float LE)][var(4B float LE)][0xAA] */
                        memcpy(&cal_frame[0], &cal_current_position_um, sizeof(cal_current_position_um));
                        memcpy(&cal_frame[2], &mean, sizeof(float));
                        memcpy(&cal_frame[6], &var, sizeof(float));
                        cal_frame[10] = 0xAA;

                        bool enqueued = UART3_Enqueue(cal_frame, sizeof(cal_frame));
                        if (enqueued) {
                            UART3_StartTx();
                        }
                        if (CALIBRITION_STEP_DIR == 1) // 减少
                            cal_current_position_um -= CALIBRITION_STEP_UM;
                        else if (CALIBRITION_STEP_DIR == 0) // 原地观测
                            ;
                        else if (CALIBRITION_STEP_DIR == -1) // 增加
                            cal_current_position_um += CALIBRITION_STEP_UM;
                        cal_state = CAL_IDLE; // ready for the next button-triggered sampling
                        HAL_GPIO_TogglePin(LED3_PORT, LED3_PIN);
                        HAL_TIM_Base_Start_IT(&htim3);
                    }
                    break;
                }
                default:
                    break;
            }
        }

        /* USER CODE BEGIN 3 */
        if (oled_update_pending) {
            const uint8_t text_size   = 12U;
            const uint8_t text_invert = 0U;
            const char *line_text     = "Sys Mode: UNKNOWN";
            switch (system_mode) {
                case MODE_MEASUREMENT:
                    line_text = "Sys Mode: Measurement";
                    break;
                case MODE_CALIBRITION:
                    line_text = "Sys Mode: Calibration";
                    break;
                default:
                    break;
            }
            OLED_ShowString(0, 0, (char *)line_text, text_size, text_invert);
            oled_update_pending = false;
        }
        if (button_scan_pending) {
            button_scan_pending = false;
            button_ticks();
        }
        UART2_TxKick();
        /* USER CODE END 3 */
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        if (oled_update_pending) {
            const uint8_t text_size   = 12U;
            const uint8_t text_invert = 0U;
            const char *line_text     = "Sys Mode: UNKNOWN";
            switch (system_mode) {
                case MODE_MEASUREMENT:
                    line_text = "Sys Mode: Measurement";
                    break;
                case MODE_CALIBRITION:
                    line_text = "Sys Mode: Calibration";
                    break;
                default:
                    break;
            }
            OLED_ShowString(0, 0, (char *)line_text, text_size, text_invert);
            oled_update_pending = false;
        }
        if (button_scan_pending) {
            button_scan_pending = false;
            button_ticks();
        }
        UART2_TxKick();
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
