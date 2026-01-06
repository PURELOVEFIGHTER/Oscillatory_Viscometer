/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "multi_button.h"
#include "my_button.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void) {
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }

    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void) {
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void) {
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void) {
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void) {
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void) {
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) {
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void) {
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void) {
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles DMA1 channel2 global interrupt.
 */
void DMA1_Channel2_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

    /* USER CODE END DMA1_Channel2_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart3_tx);
    /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

    /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel5 global interrupt.
 */
void DMA1_Channel5_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

    /* USER CODE END DMA1_Channel5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
    /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

    /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
 * @brief This function handles TIM1 update interrupt.
 */
void TIM1_UP_IRQHandler(void) {
    /* USER CODE BEGIN TIM1_UP_IRQn 0 */

    /* USER CODE END TIM1_UP_IRQn 0 */
    HAL_TIM_IRQHandler(&htim1);
    /* USER CODE BEGIN TIM1_UP_IRQn 1 */

    /* USER CODE END TIM1_UP_IRQn 1 */
}

/**
 * @brief This function handles TIM2 global interrupt.
 */
void TIM2_IRQHandler(void) {
    /* USER CODE BEGIN TIM2_IRQn 0 */

    /* USER CODE END TIM2_IRQn 0 */
    HAL_TIM_IRQHandler(&htim2);
    /* USER CODE BEGIN TIM2_IRQn 1 */

    /* USER CODE END TIM2_IRQn 1 */
}

/**
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void) {
    /* USER CODE BEGIN TIM3_IRQn 0 */

    /* USER CODE END TIM3_IRQn 0 */
    HAL_TIM_IRQHandler(&htim3);
    /* USER CODE BEGIN TIM3_IRQn 1 */

    /* USER CODE END TIM3_IRQn 1 */
}

/**
 * @brief This function handles TIM4 global interrupt.
 */
void TIM4_IRQHandler(void) {
    /* USER CODE BEGIN TIM4_IRQn 0 */

    /* USER CODE END TIM4_IRQn 0 */
    HAL_TIM_IRQHandler(&htim4);
    /* USER CODE BEGIN TIM4_IRQn 1 */

    /* USER CODE END TIM4_IRQn 1 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void) {
    /* USER CODE BEGIN USART1_IRQn 0 */
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        HAL_UART_IdleLineCallback(&huart1);
    }
    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */

    /* USER CODE END USART1_IRQn 1 */
}

/**
 * @brief This function handles USART3 global interrupt.
 */
void USART3_IRQHandler(void) {
    /* USER CODE BEGIN USART3_IRQn 0 */

    /* USER CODE END USART3_IRQn 0 */
    HAL_UART_IRQHandler(&huart3);
    /* USER CODE BEGIN USART3_IRQn 1 */

    /* USER CODE END USART3_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        static uint32_t tim1_drv_cnt = 0;
        static uint32_t tim1_led_cnt = 0;

        if (tim1_drv_cnt < drv_PWM_assertCnt) {
            DRV_Forward(&hdrv1, &hdrv1.CHANNEL_A);
            drv_excitingLevel = true;
        } else if (tim1_drv_cnt < drv_PWM_halfCnt) {
            DRV_Coast(&hdrv1, &hdrv1.CHANNEL_A);
        } else if (tim1_drv_cnt < drv_PWM_halfCnt + drv_PWM_assertCnt) {
            DRV_Reverse(&hdrv1, &hdrv1.CHANNEL_A);
            drv_excitingLevel = false;
        } else if (tim1_drv_cnt < drv_PWM_cnt) {
            DRV_Coast(&hdrv1, &hdrv1.CHANNEL_A);
        } else {
            tim1_drv_cnt = 0;
        }
        tim1_drv_cnt++;

        // LED 心跳
        if (tim1_led_cnt >= 100000U) {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            tim1_led_cnt = 0;
        }
        tim1_led_cnt++;
    }

    if (htim->Instance == TIM2) {
        static uint8_t tim2_button_cnt = 0;
        if (tim2_button_cnt == 25) {
            tim2_button_cnt = 0;
            button_ticks();
        }
        tim2_button_cnt++;
    }

    if (htim->Instance == TIM3) {
        static uint16_t tim3_oled_cnt = 0;
        const uint8_t text_size       = 12U;
        const uint8_t text_invert     = 0U;
        const char *line_text         = "Sys Mode: UNKNOWN";

        tim3_oled_cnt = 0;
        switch (system_mode) {
            case MODE_MEASUREMENT:
                line_text = "Sys Mode: Measurement";
                break;
            case MODE_CALIBRITION:
                line_text = "Sys Mode: Calibration";
                break;
            default:
                break;
        }
        OLED_ShowString(0, 0, (char *)line_text, text_size, text_invert);
    }

    if (htim->Instance == TIM4) {
        static uint8_t tim4_cnt = 0;
        tim4_cnt++;
        if (freq_scan_enabled) {
            if (tim4_cnt > FREQ_SCAN_HOLD_TIME_MS) {
                tim4_cnt = 0;
                drv_PWM_freq += FREQ_SCAN_STEP_HZ;
                drv_PWM_isChanged = true;
                if (drv_PWM_freq > FREQ_SCAN_END_HZ) {
                    freq_scan_enabled = false;
                    ldc2_isReading    = false;
                    drv_PWM_freq      = FREQ_SCAN_DEFAULT_HZ;
                    HAL_TIM_Base_Stop_IT(&htim4);
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                    DRV_Stop(&hdrv1.CHANNEL_A, &htim1);
                }
            }
        }
        if (DR_scan_enabled) {
            if (tim4_cnt > DUTY_RATIO_SCAN_HOLD_TIME_MS) {
                tim4_cnt = 0;
                drv_PWM_DR += DUTY_RATIO_SCAN_STEP;
                drv_PWM_isChanged = true;
                if (drv_PWM_DR > DUTY_RATIO_SCAN_END) {
                    DR_scan_enabled = false;
                    ldc2_isReading  = false;
                    drv_PWM_DR      = DUTY_RATIO_DEFAULT;
                    HAL_TIM_Base_Stop_IT(&htim4);
                    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                    DRV_Stop(&hdrv1.CHANNEL_A, &htim1);
                }
            }
        }
    }
}

void HAL_UART_IdleLineCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        HAL_UART_DMAStop(&huart1);

        Command_Parse();
        UART1_TX_send = true;

        memset(UART1_RX_DMA_buffer[UART1_RX_activeBuffer], 0, MSG_LEN);
        UART1_RX_activeBuffer ^= 1;
        HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_RX_DMA_buffer[UART1_RX_activeBuffer], MSG_LEN);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        UART3_TX_tail += huart->TxXferSize;
        if (UART3_TX_tail >= UART3_TX_buffer + sizeof(UART3_TX_buffer)) {
            UART3_TX_tail -= sizeof(UART3_TX_buffer);
        }
        UART3_DMA_busy = false;
        __set_PRIMASK(primask);

        UART3_StartTx();
    }
}
/* USER CODE END 1 */
