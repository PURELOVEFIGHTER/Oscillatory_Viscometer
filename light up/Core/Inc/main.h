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
#include "drv8833_config.h"
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
#define STATE_LED_Pin       GPIO_PIN_13
#define STATE_LED_GPIO_Port GPIOC
#define LDC1_CS_Pin         GPIO_PIN_4
#define LDC1_CS_GPIO_Port   GPIOA
#define LDC2_CS_Pin         GPIO_PIN_12
#define LDC2_CS_GPIO_Port   GPIOB

/* USER CODE BEGIN Private defines */
/* DRV8833 --------------------------------------------------------*/
extern DRV8833_HandleTypeDef drv1;

// 频率扫描设置
#define FREQ_MIN  5
#define FREQ_MAX  1000
#define FREQ_STEP 5
void FREQ_Scan(void);

// 占空比扫描设置
#define DR_MIN  5
#define DR_MAX  50
#define DR_STEP 5
void DR_Scan(void);

// 控制波形设置
extern uint16_t drv_PWM_FREQ; // PWM 频率
extern uint16_t drv_PWM_CNT;  // PWM 所需中断计数值
extern uint16_t drv_PWM_DR;   // PWM 占空比

/* LDC1101 --------------------------------------------------------*/
extern LDC1101_HandleTypeDef ldc1;
extern LDC1101_HandleTypeDef ldc2;

extern uint16_t RP_DATA[2];
extern uint16_t L_DATA[2];
extern uint32_t LHR_DATA[2];
extern uint8_t ldcStatus[2];

/* Bluetooth Serial Port --------------------------------------------------------*/
#define MSG_LEN 64
extern char RX_BYTE;
extern char RX_BUFFER[MSG_LEN];
extern char TX_BUFFER[MSG_LEN];
void Command_Parse(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
