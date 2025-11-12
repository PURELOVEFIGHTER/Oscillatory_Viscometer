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
DRV8833_HandleTypeDef drv1 = {
#if DRV_USE_CHANNEL_A
    .CHANNEL_A.IN1_Port = DRV_AIN1_GPIO_Port,
    .CHANNEL_A.IN1_Pin  = DRV_AIN1_Pin,
    .CHANNEL_A.IN2_Port = DRV_AIN2_GPIO_Port,
    .CHANNEL_A.IN2_Pin  = DRV_AIN2_Pin,
#endif

#if DRV_USE_CHANNEL_B
    .CHANNEL_B.IN1_Port = DRV_BIN1_GPIO_Port,
    .CHANNEL_B.IN1_Pin  = DRV_BIN1_Pin,
    .CHANNEL_B.IN2_Port = DRV_BIN2_GPIO_Port,
    .CHANNEL_B.IN2_Pin  = DRV_BIN2_Pin,
#endif

    .nSLEEP_Port = DRV_nSLEEP_GPIO_Port,
    .nSLEEP_Pin  = DRV_nSLEEP_Pin,

#if DRV_USE_FAULT
    .nFAULT_Port = DRV_nFAULT_GPIO_Port,
    .nFAULT_Pin  = DRV_nFAULT_Pin,
#endif
};

uint8_t drv_excitingLevel = 0;
uint16_t drv_PWM_freq     = 200;
uint8_t drv_PWM_DR        = 50;
uint32_t drv_PWM_cnt;
volatile bool freq_sweep_enabled = false;
volatile bool DR_sweep_enabled   = false;

/* LDC1101 --------------------------------------------------------*/
LDC1101_HandleTypeDef ldc2   = {&hspi2, LDC2_CS_GPIO_Port, LDC2_CS_Pin};
volatile bool ldc2_isWorking = false;
volatile bool ldc2_isReading = false;
volatile bool ldc2_dataReady = false;
uint16_t Rp_data             = 0;
uint16_t L_data              = 0;
uint32_t LHR_data            = 0;
uint8_t LDC_status           = 0;
uint16_t ldc2_cnt            = 0;

/* UART -----------------------------------------------------------*/
/* UART1 */
char UART1_RX_DMA_buffer[2][MSG_LEN];
volatile uint8_t UART1_RX_activeBuffer = 0;
bool UART1_TX_send                     = false;
char UART1_TX_buffer[MSG_LEN];
/* UART3 */
uint8_t UART3_TX_buffer[QUEUE_LEN * 8];
uint8_t frame[8];
uint8_t * volatile UART3_TX_head      = UART3_TX_buffer;
uint8_t * volatile UART3_TX_tail      = UART3_TX_buffer;
volatile bool UART3_DMA_busy          = false;
volatile uint32_t UART3_TX_frameCount = 0;
volatile uint32_t UART3_TX_dropCount  = 0;

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
    /* Ƭ */
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_RX_DMA_buffer[UART1_RX_activeBuffer], MSG_LEN); // DMAյַ
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);                                                   // DMAж

    /* Ƭ */
    // OLED ʼ
    OLED_Init();
    OLED_Clear();
    OLED_Display_On();

    // ⲿжϳ�?
    EXTI->IMR |= GPIO_PIN_12;
    EXTI->IMR &= ~GPIO_PIN_12;

    // DRV8833 ʼ
    DRV8833_Init(&drv1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_Base_Start_IT(&htim1);
    sprintf(UART1_TX_buffer, "Oscillation On.\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);

    // LDC1101 ʼ
    if (ldc1101_init(&ldc2, _LDC1101_RP_SET_RP_MIN_1_5KOhm)) {
        sprintf(UART1_TX_buffer, "LDC1101 Initialization Failed.\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);
    } else {
        sprintf(UART1_TX_buffer, "LDC1101 Initialization Done.\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);
        ldc2_isWorking = true;
    }

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* 趨Ƶʼжϼ�? */
        if (drv_PWM_freq == 0)
            drv_PWM_cnt = drv_PWM_cnt; // ֲ
        else
            drv_PWM_cnt = 100000 / drv_PWM_freq;
        /* DRV8833׶л */
        switch (drv_stage) {
            case DRV_STAGE_COAST:
                DRV_Coast(&(drv1.CHANNEL_A));
                break;
            case DRV_STAGE_FORWARD:
                DRV_Forward(&(drv1.CHANNEL_A));
                break;
            case DRV_STAGE_REVERSE:
                DRV_Reverse(&(drv1.CHANNEL_A));
                break;
            case DRV_STAGE_BRAKE:
                DRV_Brake(&(drv1.CHANNEL_A));
                break;
            default:
                DRV_Coast(&(drv1.CHANNEL_A));
                break;
        }

        /* LDC1101 data read */
        if (ldc2_isWorking && ldc2_isReading && ldc2_dataReady) {
            ldc2_dataReady = false;

            LHR_data   = ldc1101_getLHRData(&ldc2);
            LDC_status = ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_STATUS);
            // === frame [LHR(4B)][Freq(2B)][Duty(1B)][Pad(1B)] ===
            frame[0] = (uint8_t)(LHR_data);
            frame[1] = (uint8_t)(LHR_data >> 8);
            frame[2] = (uint8_t)(LHR_data >> 16);
            frame[3] = (uint8_t)(LHR_data >> 24);
            frame[4] = (uint8_t)(drv_PWM_freq);
            frame[5] = (uint8_t)(drv_PWM_freq >> 8);
            frame[6] = drv_PWM_DR;
            frame[7] = 0xAA;

            // ===== enqueue frame into ring buffer =====
            bool frame_enqueued = false;
            uint32_t primask    = __get_PRIMASK();
            __disable_irq();

            const uint16_t buffer_len = sizeof(UART3_TX_buffer);
            uint16_t used             = UART3_BufferUsedUnsafe();
            uint16_t free_space       = buffer_len - used - 1;

            if (free_space >= sizeof(frame)) {
                for (int i = 0; i < (int)sizeof(frame); i++) {
                    *UART3_TX_head = frame[i];
                    UART3_TX_head++;
                    if (UART3_TX_head >= UART3_TX_buffer + buffer_len)
                        UART3_TX_head = UART3_TX_buffer;
                }
                frame_enqueued = true;
                UART3_TX_frameCount++;
            } else {
                UART3_TX_dropCount++;
            }

            __set_PRIMASK(primask);

            if (frame_enqueued && !UART3_DMA_busy && (UART3_TX_head != UART3_TX_tail)) {
                UART3_DMA_busy = true;

                uint16_t size;
                if (UART3_TX_head > UART3_TX_tail)
                    size = UART3_TX_head - UART3_TX_tail;
                else
                    size = (UART3_TX_buffer + sizeof(UART3_TX_buffer)) - UART3_TX_tail;

                HAL_UART_Transmit_DMA(&huart3, UART3_TX_tail, size);
            }
        }

        /* λָظ */
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
    /* User can add his own implementation to report the HAL error return state
     */
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
