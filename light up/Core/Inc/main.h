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

/* USER CODE BEGIN Private defines */
/* DRV8833 --------------------------------------------------------*/
extern DRV8833_HandleTypeDef hdrv1;
/* Scan state */
extern volatile bool freq_scan_enabled;
extern volatile bool DR_scan_enabled;

extern volatile bool drv_excitingLevel;
extern float drv_PWM_freq;
extern float drv_PWM_DR;
extern bool drv_PWM_isChanged;
extern uint32_t drv_PWM_cnt;
extern uint32_t drv_PWM_halfCnt;
extern uint32_t drv_PWM_assertCnt;

/* LDC1101 --------------------------------------------------------*/
extern LDC1101_HandleTypeDef ldc2;
// Status Variables
extern volatile bool ldc2_isWorking;
extern volatile bool ldc2_isReading;
extern volatile bool ldc2_dataReady;
extern uint8_t LDC_status;
// Data Variables
extern uint16_t Rp_data;
extern uint16_t L_data;
extern uint32_t LHR_data;
extern uint32_t LHR_dataLast;
/* UART -----------------------------------------------------------*/
#define MSG_LEN 128
/* UART1 */
extern volatile uint8_t UART1_RX_activeBuffer;
extern char UART1_RX_DMA_buffer[2][MSG_LEN];
extern char UART1_TX_buffer[MSG_LEN];
extern bool UART1_TX_send;
/* UART3 */
#define QUEUE_LEN 1024
extern uint8_t UART3_TX_buffer[QUEUE_LEN * 9];
extern uint8_t frame[9];
extern uint8_t * volatile UART3_TX_head;
extern uint8_t * volatile UART3_TX_tail;
extern volatile bool UART3_DMA_busy;
void UART3_KickTx(void);
void UART1_Log(const char *level, const char *file, int line, const char *message);
/* KEY */
void Key_Process(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
