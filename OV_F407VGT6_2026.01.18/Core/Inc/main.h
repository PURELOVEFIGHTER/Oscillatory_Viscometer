/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ldc1101_driver.h"
#include "drv8833_driver.h"
#include "config.h"
#include "command.h"
#include "oled.h"
#include "multi_button.h"
#include "my_button.h"
#include "utility.h"
#include "oscillate.h"

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
extern sysWorkMode system_mode;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LDC1101 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define MSG_LEN 128
/* UART2 */
extern volatile uint8_t UART2_RX_activeBuffer;
extern char UART2_RX_DMA_buffer[2][MSG_LEN];
extern char UART2_TX_buffer[MSG_LEN];
extern bool UART2_TX_send;
void UART2_Log(const char *level, const char *file, int line, const char *message);
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Calibrition <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
extern uint16_t cal_current_position_um;
extern uint32_t cal_wait_start_tick;

extern float mean_episode[CAL_SAMPLE_EPISODE];
extern uint8_t cal_episode_cnt;
extern uint32_t cal_sample_cnt;
extern uint64_t cal_sum;
extern uint64_t cal_sum_sq;
extern uint32_t cal_total_sample_cnt;
extern uint64_t cal_total_sum;
extern uint64_t cal_total_sum_sq;
extern volatile CalState_t cal_state;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Pulse Feedback <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
extern volatile bool pulse_active;
extern volatile uint8_t pulse_feedback_trigger;
extern volatile bool pulse_feedback_busy;
extern volatile uint32_t pulse_feedback_frame_cnt;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> OLED <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
extern volatile bool oled_update_pending;
extern volatile bool button_scan_pending;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ADC <<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define ADC_BUF_LEN           256
#define ADC_DMA_BUF_LEN       (ADC_BUF_LEN * 2U)
#define ADC_UART_TX_BYTES     (ADC_BUF_LEN * sizeof(uint16_t))
#define ADC_UART_PACKET_BYTES (ADC_UART_TX_BYTES + 2U)
#define UART6_FRAME_HEAD      0xAAU
#define UART6_FRAME_TAIL      0xFFU
#define UART6_TX_INDEX_NONE   0xFFU
extern uint16_t adc_buf[2][ADC_BUF_LEN];
extern uint8_t UART6_tx_packet[2][ADC_UART_PACKET_BYTES];
extern volatile uint8_t UART6_sending_index;
extern volatile uint8_t UART6_pending_index;
extern volatile uint8_t UART6_tx_busy;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
