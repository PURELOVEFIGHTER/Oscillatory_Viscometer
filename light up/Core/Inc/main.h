/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ldc1101_driver.h"
#include "drv8833_driver.h"
#include "config.h"
#include "command.h"
#include "oled.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STATE_LED_Pin GPIO_PIN_13
#define STATE_LED_GPIO_Port GPIOC
#define DRV_AIN1_Pin GPIO_PIN_0
#define DRV_AIN1_GPIO_Port GPIOA
#define DRV_AIN2_Pin GPIO_PIN_1
#define DRV_AIN2_GPIO_Port GPIOA
#define DRV_nSLEEP_Pin GPIO_PIN_1
#define DRV_nSLEEP_GPIO_Port GPIOB
#define LDC2_CS_Pin GPIO_PIN_12
#define LDC2_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* DRV8833 --------------------------------------------------------*/
extern DRV8833_HandleTypeDef drv1;

// 扫描开关
extern volatile bool freq_sweep_enabled;
extern volatile bool DR_sweep_enabled;

// 控制波形设置
extern uint16_t drv_excitingLevel; // PWM 实时电平
extern uint16_t drv_PWM_freq;      // PWM 频率
extern uint16_t drv_PWM_DR;        // PWM 占空比
extern uint16_t drv_PWM_cnt;       // PWM 中断计数值

/* LDC1101 --------------------------------------------------------*/
extern LDC1101_HandleTypeDef ldc2;
// 状态量
extern volatile bool ldc2_isWorking;
extern uint8_t LDC_status;
// 数据量
extern uint16_t Rp_data;
extern uint16_t L_data;
extern uint32_t LHR_data;

/* UART -----------------------------------------------------------*/
#define MSG_LEN 64
/* UART1 */
extern volatile uint8_t UART1_RX_activeBuffer;
extern char UART1_RX_DMA_buffer[2][MSG_LEN];
extern char UART1_TX_buffer[MSG_LEN];
/* UART3 */
extern char UART3_TX_buffer[MSG_LEN];
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
