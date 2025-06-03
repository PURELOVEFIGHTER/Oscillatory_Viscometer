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
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "core_cm3.h"
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
/* USER CODE BEGIN Private defines */
/**************************************振动控制**************************************/
//扫频设置
#define FREQ_MIN 5
#define FREQ_MAX 1000
#define FREQ_STEP 5
void FREQ_Scan(void);

//占空比范围
#define DR_MIN 5
#define DR_MAX 50
#define DR_STEP 5
void DR_Scan(void);

// 控制波形
extern uint16_t drv_PWM_FREQ;// PWM频率
extern uint16_t drv_PWM_CNT;// PWM所需中断计数值
extern uint8_t drv_PWM_DR;// PWM占空比

// DRV错误标志
extern volatile uint8_t drv_fault;

// DRV睡眠状态
#define DRV_Wake() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)
#define DRV_Sleep() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)

// DRV驱动方向
void DRV_Coast(void);
void DRV_Forward(void);
void DRV_Reverse(void);
void DRV_Brake(void);
/************************************************************************************/

/**************************************测量反馈**************************************/
// LDC片选
#define CS1_LOW()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define CS1_HIGH() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define CS2_LOW()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define CS2_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
/************************************************************************************/

/**************************************蓝牙串口**************************************/
#define MSG_LEN 64
extern char RX_BYTE;
extern char RX_BUFFER[MSG_LEN];
extern char TX_BUFFER[MSG_LEN];
void Command_Parse(void);
/************************************************************************************/
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
