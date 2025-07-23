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

// DRV8833相关变量
DRV8833_HandleTypeDef drv1;

uint16_t drv_PWM_FREQ = 100;
uint16_t drv_PWM_DR = 30;
uint16_t drv_PWM_CNT ;


// 串口相关变量
char RX_BYTE = 1;
char RX_BUFFER[MSG_LEN];
char TX_BUFFER[MSG_LEN];
uint8_t uart_state = 0;


//LDC1101 相关变量
LDC1101_Device ldc1 = { &hspi1, GPIOA, GPIO_PIN_4 };
LDC1101_Device ldc2 = { &hspi2, GPIOB, GPIO_PIN_12 };
uint16_t RP_DATA = 0;
uint16_t L_DATA = 0;
uint32_t LHR_DATA = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
// 扫频
void FREQ_Scan(void)
{
//		for (drv_PWM_FREQ = FREQ_MIN; drv_PWM_FREQ <= FREQ_MAX; drv_PWM_FREQ += FREQ_STEP)
//				HAL_Delay(5000);  // 等待系统稳定（振动建立）
}

// 占空比扫描
void DR_Scan(void)
{
		for (; drv_PWM_DR <= DR_MAX; drv_PWM_DR += DR_STEP)
				HAL_Delay(10000);  // 等待系统稳定（振动建立）
}




void Command_Parse(void)
{
		// 简单协议,例:输入 "DR:80" 设置占空比为 80，"FR:1000" 设置频率为 1000Hz
		if (strncmp(RX_BUFFER, "DR:", 3) == 0)
		{
				drv_PWM_DR = atoi(&RX_BUFFER[3]);  // 提取并转换占空比
			
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			
				sprintf(TX_BUFFER, "PWM_DR set to %d\r\n", drv_PWM_DR);
				HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
		}
		else if (strncmp(RX_BUFFER, "FR:", 3) == 0)
		{
				drv_PWM_FREQ = atoi(&RX_BUFFER[3]);  // 提取并转换频率
			
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			
				sprintf(TX_BUFFER, "PWM_FREQ set to %dHz\r\n", drv_PWM_FREQ);
				HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
		}
		else if (strncmp(RX_BUFFER,"FR Scan",7) == 0)
		{
				sprintf(TX_BUFFER, "Frequency Scan begin.\r\n");
				HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
			
				for (drv_PWM_FREQ = FREQ_MIN; drv_PWM_FREQ <= FREQ_MAX; drv_PWM_FREQ += FREQ_STEP)
				{
						HAL_Delay(5000);  // 等待系统稳定（振动建立）
						sprintf(TX_BUFFER, "PWM_FREQ:%dHz\r\n", drv_PWM_FREQ);
						HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
				}
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		}
		else if (strncmp(RX_BUFFER,"DR Scan",7) == 0)
		{
//				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//				sprintf(TX_BUFFER, "Please set PWM_DR:");
//				HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
//				drv_PWM_FREQ = atoi(&RX_BUFFER[3]);  // 提取并转换频率
//				DR_Scan();
		}
		
		
		// 检查SPI通讯状态
		else if (strncmp(RX_BUFFER,"SPI Check",9) == 0)
		{
					HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			
					uint8_t chip_id = ldc1101_readByte(&ldc1, _LDC1101_REG_CHIP_ID);

					sprintf(TX_BUFFER, "LDC1101 Device ID: 0x%02X\r\n", chip_id);
					HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
		}
		// 读取寄存器数值
		else if(strncmp(RX_BUFFER,"Read:",5) == 0)
		{
			    uint8_t reg_addr;
					uint8_t reg_val;

					// 直接解析寄存器地址，默认格式为 0xXX
					sscanf(&RX_BUFFER[5], "%hhx", &reg_addr);

					reg_val = ldc1101_readByte(&ldc1, reg_addr);

					sprintf(TX_BUFFER, "[0x%02X] = 0x%02X\r\n", reg_addr, reg_val);
					HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
		}
		// 检查 RP+L 模式工作状态
		else if ((strncmp(RX_BUFFER, "RPL Check", 9) == 0) && (isLHR == 0))
		{
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

				uint8_t rpl_status = ldc1101_readByte(&ldc2, _LDC1101_REG_RP_L_MEASUREMENT_STATUS);

				// 将 rpl_status 转成二进制字符串
				sprintf(TX_BUFFER, "RP+L Measurement Status: "
													 "%d%d%d%d%d%d%d%d\r\n",
								(rpl_status >> 7) & 0x01,
								(rpl_status >> 6) & 0x01,
								(rpl_status >> 5) & 0x01,
								(rpl_status >> 4) & 0x01,
								(rpl_status >> 3) & 0x01,
								(rpl_status >> 2) & 0x01,
								(rpl_status >> 1) & 0x01,
								(rpl_status >> 0) & 0x01);

				HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
		}
		else if ((strncmp(RX_BUFFER, "LHR Check", 9) == 0) && (isLHR == 1))
		{
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

				uint8_t lhr_status = ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_STATUS);

				// 将 rpl_status 转成二进制字符串
				sprintf(TX_BUFFER, "LHR Measurement Status: "
													 "%d%d%d%d%d%d%d%d\r\n",
								(lhr_status >> 7) & 0x01,
								(lhr_status >> 6) & 0x01,
								(lhr_status >> 5) & 0x01,
								(lhr_status >> 4) & 0x01,
								(lhr_status >> 3) & 0x01,
								(lhr_status >> 2) & 0x01,
								(lhr_status >> 1) & 0x01,
								(lhr_status >> 0) & 0x01);

				HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
		}

		memset(RX_BUFFER, 0, sizeof(RX_BUFFER));
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
	DRV8833_Init(&drv1,
               GPIOA, GPIO_PIN_0,
               GPIOA, GPIO_PIN_1,
               GPIOA, GPIO_PIN_3,
               GPIOA, GPIO_PIN_2,
               GPIOB, GPIO_PIN_1,
               NULL,0);
  /* USER CODE BEGIN 2 */
	DRV_Wake(&drv1);	                                       // DRV8833 唤醒
	HAL_TIM_Base_Start_IT(&htim2);                           // 定时器制作 DRV8833 驱动波形
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);                          // DRV8833 报错开启
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);                  // DRV8833 报错优先级设定
	
	HAL_UART_Receive_IT(&huart1, (uint8_t*)&RX_BYTE, 1);     // 串口通讯开启
	if(ldc1101_init(&ldc1,_LDC1101_RP_SET_RP_MIN_0_75KOhm)||ldc1101_init(&ldc2,_LDC1101_RP_SET_RP_MIN_1_5KOhm))             // LDC1101 初始化
	{
			sprintf(TX_BUFFER, "LDC1101 Initialize Failed.\r\n");
			HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
	}else
	{
			sprintf(TX_BUFFER, "LDC1101 Initialize Done.\r\n");
			HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
	}
	ldc1101_writeByte(&ldc1, _LDC1101_REG_CFG_POWER_STATE, _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE);// 开始测量

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
				drv_PWM_CNT = 100000/drv_PWM_FREQ;
				HAL_Delay(100);
			

				uint8_t status = ldc1101_readByte(&ldc1, _LDC1101_REG_LHR_STATUS);

				// 判断 LHR_Data_Ready == 0，表示有新数据
				if ((status & 0x01) == 0)
				{
						uint32_t lhr_data = ldc1101_getLHRData(&ldc2);

						// 输出 LHR 数据（十进制 + 十六进制）
						sprintf(TX_BUFFER, "LHR: %lu (0x%08lX)\r\n", lhr_data, lhr_data);
						HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
				}

				HAL_Delay(100); 

		
		
		
		
		
		
		
		
		
		
		
		
		
		
//				uint8_t status = ldc1101_readByte(&ldc1, _LDC1101_REG_RP_L_MEASUREMENT_STATUS);

//				if ((status & (1 << 6)) == 0)  // 第6位为0，表示有新数据
//				{
//						uint16_t rp_data = ldc1101_getRPData(&ldc2);
//						uint16_t l_data = ldc1101_getLData(&ldc2);

////						sprintf(TX_BUFFER, "RP: %u, L: %u\r\n", rp_data, l_data);
//						sprintf(TX_BUFFER, "%u\r\n", rp_data);
//						HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER, strlen(TX_BUFFER), HAL_MAX_DELAY);
//				}
//				
//				HAL_Delay(100);
	 }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
