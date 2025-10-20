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
    .CHANNEL_A.IN1_Port = DRV_AIN1_GPIO_Port,
    .CHANNEL_A.IN1_Pin  = DRV_AIN1_Pin,
    .CHANNEL_A.IN2_Port = DRV_AIN2_GPIO_Port,
    .CHANNEL_A.IN2_Pin  = DRV_AIN2_Pin,
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

uint16_t drv_PWM_freq = 100;
uint16_t drv_PWM_DR   = 30;
uint16_t drv_PWM_cnt;

/* LDC1101 --------------------------------------------------------*/
LDC1101_HandleTypeDef ldc2   = {&hspi2, LDC2_CS_GPIO_Port, LDC2_CS_Pin};
volatile bool ldc2_isWorking = false;
uint16_t Rp_data             = 0;
uint16_t L_data              = 0;
uint32_t LHR_data            = 0;
uint8_t LDC_status           = 0;

/* UART -----------------------------------------------------------*/
/* UART1 */
char UART1_RX_DMA_buffer[2][MSG_LEN];
volatile uint8_t UART1_RX_activeBuffer = 0;

char UART1_TX_buffer[MSG_LEN];
/* UART3 */
char UART3_TX_DMA_buffer[2][MSG_LEN];
volatile uint8_t UART3_activeBufferIndex  = 0;     // 当前写入缓冲区索引
volatile uint8_t UART3_sendingBufferIndex = 0;     // 当前正在 DMA 发送的缓冲区索引
volatile bool UART3_DMA_isBusy            = false; // DMA忙标志
volatile bool UART3_DMA_isPending         = false; // DMA待处理标志
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/**
 * ************************************************************************
 * @brief    频率扫描
 * @details
 * ************************************************************************
 */
void FREQ_Scan(void) {
    //		for (drv_PWM_FREQ = FREQ_MIN; drv_PWM_FREQ <= FREQ_MAX; drv_PWM_FREQ
    //+= FREQ_STEP) 				HAL_Delay(5000);  //
    // 等待系统稳定（振动建立）
}

/**
 * ************************************************************************
 * @brief 占空比扫描
 * @details
 *
 * ************************************************************************
 */
void DR_Scan(void) {
    for (; drv_PWM_DR <= DR_MAX; drv_PWM_DR += DR_STEP)
        HAL_Delay(10000); // 等待系统稳定（振动建立）
}

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
    MX_USART1_UART_Init();
    MX_TIM2_Init();
    MX_I2C1_Init();
    MX_SPI2_Init();
    MX_USART3_UART_Init();
    /* USER CODE BEGIN 2 */
    /* 片上外设 */
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_RX_DMA_buffer[UART1_RX_activeBuffer], MSG_LEN); // 设置DMA接收地址
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);                                                   // 开启DMA空闲中断

    /* 片外外设 */
    // OLED 初始化
    OLED_Init();
    OLED_Display_On();
    OLED_Clear();

    // DRV8833 初始化
    DRV8833_Init(&drv1);
    HAL_TIM_Base_Start_IT(&htim2); // DRV8833 驱动波形开启

    // LDC1101 初始化
    if (ldc1101_init(&ldc2, _LDC1101_RP_SET_RP_MIN_1_5KOhm)) {
        sprintf(UART1_TX_buffer, "LDC1101 Initialize Failed.\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);
    } else {
        sprintf(UART1_TX_buffer, "LDC1101 Initialize Done.\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)UART1_TX_buffer, strlen(UART1_TX_buffer), HAL_MAX_DELAY);
        ldc2_isWorking = true;
    }
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* 根据设定频率计算中断计数值 */
        if (drv_PWM_freq == 0)
            drv_PWM_cnt = drv_PWM_cnt; // 保持不变
        else
            drv_PWM_cnt = 100000 / drv_PWM_freq;

        if (ldc2_isWorking) {
            LDC_status = ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_STATUS);
            if (!(LDC_status & 0x01)) {
                LHR_data = ldc1101_getLHRData(&ldc2);
                sprintf(UART3_TX_DMA_buffer[UART3_activeBufferIndex], "L=%lu,%lu\r\n", LHR_data, LHR_data);
                if (!UART3_DMA_isBusy) {
                    UART3_DMA_isBusy         = true;                        // DMA正在发送消息
                    UART3_sendingBufferIndex = UART3_activeBufferIndex;     // 发送刚写入数据
                    UART3_activeBufferIndex  = 1 - UART3_activeBufferIndex; // 将活跃缓冲区切换到另一个缓冲区
                    HAL_UART_Transmit_DMA(&huart3, (uint8_t *)UART3_TX_DMA_buffer[UART3_sendingBufferIndex],
                                          strlen(UART3_TX_DMA_buffer[UART3_sendingBufferIndex]));
                } else {
                    UART3_DMA_isPending = true;
                }
            }
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
