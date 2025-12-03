/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
/* DRV8833 --------------------------------------------------------*/
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

/* LDC1101 --------------------------------------------------------*/
LDC1101_HandleTypeDef ldc2   = {&hspi2, LDC2_CS_GPIO_PORT, LDC2_CS_PIN};
volatile bool ldc2_isWorking = false;
volatile bool ldc2_isReading = false;
volatile bool ldc2_dataReady = false;
uint16_t Rp_data             = 0;
uint16_t L_data              = 0;
uint32_t LHR_data            = 0;
uint32_t LHR_dataLast        = 0;
uint8_t LDC_status           = 0;

/* UART -----------------------------------------------------------*/
/* UART1 */
char UART1_RX_DMA_buffer[2][MSG_LEN];
volatile uint8_t UART1_RX_activeBuffer = 0;
bool UART1_TX_send                     = false;
char UART1_TX_buffer[MSG_LEN];
/* UART3 */
uint8_t UART3_TX_buffer[QUEUE_LEN * 9];
uint8_t frame[9];
uint8_t * volatile UART3_TX_head = UART3_TX_buffer;
uint8_t * volatile UART3_TX_tail = UART3_TX_buffer;
volatile bool UART3_DMA_busy     = false;
/* KEY ------------------------------------------------------------*/
volatile uint8_t key_pending = 0;
volatile uint32_t key_time   = 0;
/* OLED -----------------------------------------------------------*/
char OLED_Line1[20];
char OLED_Line2[20];
char OLED_Line3[20];
char OLED_Line4[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static uint16_t UART3_BufferUsedUnsafe(void) {
    const uint16_t buffer_len = sizeof(UART3_TX_buffer);
    uint16_t head_offset      = (uint16_t)(UART3_TX_head - UART3_TX_buffer);
    uint16_t tail_offset      = (uint16_t)(UART3_TX_tail - UART3_TX_buffer);

    if (head_offset >= tail_offset) {
        return head_offset - tail_offset;
    }
    return buffer_len - (tail_offset - head_offset);
}

static bool UART3_EnqueueFrame(const uint8_t *data, uint16_t len) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    const uint16_t buffer_len = sizeof(UART3_TX_buffer);
    uint16_t used             = UART3_BufferUsedUnsafe();
    uint16_t free_space       = buffer_len - used - 1;

    if (free_space < len) {
        __set_PRIMASK(primask);
        return false;
    }

    for (uint16_t i = 0; i < len; i++) {
        *UART3_TX_head = data[i];
        UART3_TX_head++;
        if (UART3_TX_head >= UART3_TX_buffer + buffer_len) {
            UART3_TX_head = UART3_TX_buffer;
        }
    }
    __set_PRIMASK(primask);
    return true;
}

void UART3_KickTx(void) {
    uint32_t primask = __get_PRIMASK();
    __disable_irq();
    if (UART3_DMA_busy || (UART3_TX_head == UART3_TX_tail)) {
        __set_PRIMASK(primask);
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
        __disable_irq();
        UART3_DMA_busy = false;
        __set_PRIMASK(primask);
    }
}

void Key_Process(void) {
    ldc2_isReading = !ldc2_isReading;
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_14);
    if (ldc2_isReading) {
        sprintf(UART1_TX_buffer, "Reading LHR Data.\r\n");
        ldc2_dataReady = false;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
        HAL_TIM_PWM_Start(hdrv1.CHANNEL_A.htim, hdrv1.CHANNEL_A.CH1);
        HAL_TIM_PWM_Start(hdrv1.CHANNEL_A.htim, hdrv1.CHANNEL_A.CH2);
        HAL_TIM_Base_Start_IT(&htim1);
    } else { // ldc2_isReading == false
        sprintf(UART1_TX_buffer, "Stopped LHR Data Reading.\r\n");
        ldc2_dataReady = false;
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_TIM_PWM_Stop(hdrv1.CHANNEL_A.htim, hdrv1.CHANNEL_A.CH1);
        HAL_TIM_PWM_Stop(hdrv1.CHANNEL_A.htim, hdrv1.CHANNEL_A.CH2);
        HAL_TIM_Base_Stop_IT(&htim1);

        HAL_UART_DMAStop(&huart3);
        
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        UART3_TX_head = UART3_TX_tail = UART3_TX_buffer;
        UART3_DMA_busy                = false;
        memset(UART3_TX_buffer, 0, sizeof(UART3_TX_buffer));
        __set_PRIMASK(primask);
    }
    UART1_TX_send = true;
}
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
    MX_USART1_UART_Init();
    MX_TIM2_Init();
    MX_I2C1_Init();
    MX_SPI2_Init();
    MX_USART3_UART_Init();
    MX_TIM4_Init();
    MX_TIM1_Init();
    /* USER CODE BEGIN 2 */
    /* UART1 DMA Init */
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_RX_DMA_buffer[UART1_RX_activeBuffer], MSG_LEN);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

    /* OLED Init */
    OLED_Init();
    OLED_Clear();
    OLED_Display_On();

    /* DRV8833 Init */
    DRV_Init(&hdrv1);
    sprintf(UART1_TX_buffer, "[DRV8833]Oscillation On.\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);

    /* LDC1101 Init */
    if (ldc1101_init(&ldc2, _LDC1101_RP_SET_RP_MIN_1_5KOhm)) {
        sprintf(UART1_TX_buffer, "[LDC1101]LDC1101 Initialization Failed.\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);
    } else {
        sprintf(UART1_TX_buffer, "[LDC1101]LDC1101 Initialization Done.\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);
        ldc2_isWorking = true;
    }
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* KEY Press Process */
        if (key_pending && HAL_GetTick() > key_time) {
            key_pending = 0;
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == GPIO_PIN_RESET) {
                Key_Process();
            }
        }
        /* DRV8833 Frequency Control */
        if (drv_PWM_isChanged) {
            // TIM1 Interrupt Frequency = 100kHz  (72MHz / 72 / 10)
            const float baseFreq = 100000.0f;
            // Calculate the number of interrupts needed for one period
            float cnt_f       = baseFreq / drv_PWM_freq;
            drv_PWM_cnt       = (uint32_t)(cnt_f + 0.5f);
            float assert_f    = cnt_f * (drv_PWM_DR / 100.0f);
            drv_PWM_assertCnt = (uint32_t)(assert_f + 0.5f);
            drv_PWM_halfCnt   = drv_PWM_cnt / 2;
        }

        /* LDC Data Get and Transmit */
        if (ldc2_isReading && ldc2_dataReady) {
            ldc2_dataReady = false;
            LHR_dataLast   = LHR_data;
            LHR_data       = ldc1101_getLHRData(&ldc2);
            if (LHR_data == LHR_dataLast) {
                continue;
            }
            // LHR_data -= 3220000;
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
            bool frame_enqueued = UART3_EnqueueFrame(frame, sizeof(frame));

            // ===== Data Transmission =====
            if (frame_enqueued) {
                UART3_KickTx();
            }
        }
        /* UART1 Transmission */
        if (UART1_TX_send) {
            HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);
            UART1_TX_send = false;
            memset(UART1_TX_buffer, 0, MSG_LEN);
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

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
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
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
       file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
