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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "task1_measurement.h"
#include "task3_pulse.h"
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
SysWorkMode system_mode = MODE_MEASUREMENT;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DRV8833 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
DRV_HandleTypeDef hdrv1;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LDC1101 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
LDC_HandleTypeDef hldc1;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LED <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
LED_Handle_t hled1;
LED_Handle_t hled2;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Calibrition <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
float mean_episode[CAL_SAMPLE_EPISODE] = {0};
uint8_t cal_episode_cnt                = 0;
uint32_t cal_wait_start_tick           = 0;
uint32_t cal_sample_cnt                = 0;
uint64_t cal_sum                       = 0;
uint64_t cal_sum_sq                    = 0;
uint32_t cal_total_sample_cnt          = 0;

uint64_t cal_total_sum           = 0;
uint64_t cal_total_sum_sq        = 0;
volatile CalState_t cal_state    = CAL_IDLE;
uint16_t cal_current_position_um = 0;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Button <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
volatile bool button_scan_pending = false;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LED <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
volatile bool led_breath_enabled = false;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ADC <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
uint16_t adc_buf[2][ADC_BUF_LEN]                  = {0};
uint8_t UART6_tx_packet[2][ADC_UART_PACKET_BYTES] = {0};
volatile uint8_t UART6_sending_index              = UART6_TX_INDEX_NONE;
volatile uint8_t UART6_pending_index              = UART6_TX_INDEX_NONE;
volatile uint8_t UART6_tx_busy                    = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
    MX_ADC1_Init();
    MX_USART6_UART_Init();
    MX_TIM5_Init();
    /* USER CODE BEGIN 2 */
    //*------------- Hardware Initialization -------------*//
    buttons_init();
    HAL_TIM_Base_Start_IT(&htim3);

    OLED_Init();
    OLED_Clear();
    OLED_Display_On();

    LED_Init(&hled1, &led1_cfg);
    LED_Init(&hled2, &led2_cfg);

    if (BSP_DRV_Init(&hdrv1) == DEVICE_OK)
        UART2_Log("INFO", __FUNCTION__, __LINE__, "DRV8833 Initialization Done...");

    if (ldc1101_init(&hldc1)) {
        UART2_Log("ERROR", __FUNCTION__, __LINE__, "LDC1101 Initialization Failed...");
    } else {
        UART2_Log("INFO", __FUNCTION__, __LINE__, "LDC1101 Initialization Done...");
        if (ldc1101_writeConfig(&hldc1, &ldc_lhr_cfg, ldc_lhr_cfg_size) == DEVICE_OK) {
            UART2_Log("INFO", __FUNCTION__, __LINE__, "LDC1101 Configuration Done...");
            snprintf(UART2_TX_buffer, sizeof(UART2_TX_buffer), "LDC1101 sample rate: %.3f kSPS (RCOUNT=0x%04X)",
                     ldc1101_getLHRSampleRate(&hldc1) / 1000.0f, ldc1101_getLHRRCount(&hldc1));
            UART2_Log("TRACE", __FUNCTION__, __LINE__, UART2_TX_buffer);
        } else {
            UART2_Log("ERROR", __FUNCTION__, __LINE__, "LDC1101 Configuration Failed...");
        }
    }

    HAL_UART_Receive_DMA(&huart2, (uint8_t *)UART2_RX_DMA_buffer[UART2_RX_activeBuffer], MSG_LEN);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);

    //*------------- Service Initialization -------------*//
    Oscillate_Init();
    LHRSample_Init();

    //*------------- Application Initialization -------------*//
    char mode_msg[MSG_LEN];
    snprintf(mode_msg, sizeof(mode_msg), "System Mode: %s", system_mode ? "CALIBRITION" : "MEASUREMENT");
    UART2_Log("INFO", __FUNCTION__, __LINE__, mode_msg);
    Task1_Measurement_Init();
    Task3_Pulse_Init();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        if (system_mode == MODE_MEASUREMENT) {
            Task1_Measurement_Run();
        } else if (system_mode == MODE_CALIBRITION) {
            /* LDC Reading Control */
            // switch (cal_state) {
            //     case CAL_WAIT_SETTLE:
            //         if ((HAL_GetTick() - cal_wait_start_tick) >= CAL_SETTLE_TIME_MS) {
            //             cal_state = CAL_SAMPLING;
            //         } else {
            //             break;
            //         }
            //         /* fallthrough */
            //     case CAL_SAMPLING:
            //         HAL_TIM_Base_Stop_IT(&htim3);
            //         LDC_status = ldc1101_readByte(&hldc1, _LDC1101_REG_LHR_STATUS);
            //         if ((LDC_status & 0x01) == 0) {
            //             ldc2_dataReady = true;
            //         }
            //         if (ldc2_dataReady) {
            //             ldc2_dataReady = false;
            //             LHR_data       = ldc1101_getLHRData(&hldc1);
            //             cal_sample_cnt++;
            //             cal_sum += LHR_data;
            //             cal_sum_sq += (uint64_t)LHR_data * LHR_data;
            //             cal_total_sample_cnt++;
            //             cal_total_sum += LHR_data;
            //             cal_total_sum_sq += (uint64_t)LHR_data * LHR_data;
            //             // ===== Data Transmission =====
            //             if (cal_sample_cnt >= CAL_SAMPLE_NUM) {
            //                 cal_state = CAL_DONE;
            //             }
            //         }
            //         if (cal_state != CAL_DONE) {
            //             break;
            //         }
            //         /* fallthrough */
            //     case CAL_DONE: {
            //         float sample_cnt              = (float)cal_sample_cnt;
            //         float episode_mean            = (float)cal_sum / sample_cnt;
            //         mean_episode[cal_episode_cnt] = episode_mean;
            //         cal_episode_cnt++;

            //         if (cal_episode_cnt < CAL_SAMPLE_EPISODE) {
            //             cal_sample_cnt      = 0;
            //             cal_sum             = 0;
            //             cal_sum_sq          = 0;
            //             cal_wait_start_tick = HAL_GetTick();
            //             cal_state           = CAL_WAIT_SETTLE;
            //         } else {
            //             float total_cnt = (float)cal_total_sample_cnt;
            //             float mean      = (float)cal_total_sum / total_cnt;
            //             float var       = (float)cal_total_sum_sq / total_cnt - mean * mean;
            //             uint8_t cal_frame[11];
            //             /* Frame: [cal_current_position_um(2B)][mean(4B float LE)][var(4B float LE)][0xAA] */
            //             memcpy(&cal_frame[0], &cal_current_position_um, sizeof(cal_current_position_um));
            //             memcpy(&cal_frame[2], &mean, sizeof(float));
            //             memcpy(&cal_frame[6], &var, sizeof(float));
            //             cal_frame[10] = 0xAA;

            //             bool enqueued = UART3_Enqueue(cal_frame, sizeof(cal_frame));
            //             if (enqueued) {
            //                 UART3_StartTx();
            //             }
            //             if (CALIBRITION_STEP_DIR == 1) // 减少
            //                 cal_current_position_um -= CALIBRITION_STEP_UM;
            //             else if (CALIBRITION_STEP_DIR == 0) // 原地观测
            //                 ;
            //             else if (CALIBRITION_STEP_DIR == -1) // 增加
            //                 cal_current_position_um += CALIBRITION_STEP_UM;
            //             cal_state = CAL_IDLE; // ready for the next button-triggered sampling
            //             HAL_GPIO_TogglePin(LED3_PORT, LED3_PIN);
            //             HAL_TIM_Base_Start_IT(&htim3);
            //         }
            //         break;
            // }
            // default:
            //     break;
        } else if (system_mode == MODE_PULSE_FEEDBACK) {
            Task3_Pulse_Run();
        }
        if (OLED_GetUpdatePending()) {
            OLED_Update(system_mode);
        }
        if (button_scan_pending) {
            button_scan_pending = false;
            button_ticks();
        }
        /* UART2 Transmission */
        if (UART2_TX_send) {
            HAL_UART_Transmit(&huart2, (uint8_t *)UART2_TX_buffer, strlen(UART2_TX_buffer), HAL_MAX_DELAY);
            UART2_TX_send = false;
            memset(UART2_TX_buffer, 0, MSG_LEN);
        }
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
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
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = 8;
    RCC_OscInitStruct.PLL.PLLN       = 336;
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ       = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
