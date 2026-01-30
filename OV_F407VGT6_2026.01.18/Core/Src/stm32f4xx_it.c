/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f4xx_it.c
 * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <multi_button.h>
#include <my_button.h>
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
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */
extern volatile bool led_breath_enabled;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
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
 * @brief This function handles Pre-fetch fault, memory access fault.
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
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles Flash global interrupt.
 */
void FLASH_IRQHandler(void) {
    /* USER CODE BEGIN FLASH_IRQn 0 */

    /* USER CODE END FLASH_IRQn 0 */
    HAL_FLASH_IRQHandler();
    /* USER CODE BEGIN FLASH_IRQn 1 */

    /* USER CODE END FLASH_IRQn 1 */
}

/**
 * @brief This function handles DMA1 stream3 global interrupt.
 */
void DMA1_Stream3_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

    /* USER CODE END DMA1_Stream3_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart3_tx);
    /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

    /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
 * @brief This function handles DMA1 stream5 global interrupt.
 */
void DMA1_Stream5_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

    /* USER CODE END DMA1_Stream5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart2_rx);
    /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

    /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
 * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
 */
void TIM1_UP_TIM10_IRQHandler(void) {
    /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

    /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
    HAL_TIM_IRQHandler(&htim1);
    /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

    /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
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
 * @brief This function handles USART2 global interrupt.
 */
void USART2_IRQHandler(void) {
    /* USER CODE BEGIN USART2_IRQn 0 */
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        uint16_t rx_len = (uint16_t)(MSG_LEN - __HAL_DMA_GET_COUNTER(huart2.hdmarx));
        HAL_UART_DMAStop(&huart2);
        if (rx_len >= MSG_LEN) {
            rx_len = MSG_LEN - 1;
        }
        char *rx_buf = UART2_RX_DMA_buffer[UART2_RX_activeBuffer];
        if (rx_len > 0) {
            while (rx_len > 0 && (rx_buf[rx_len - 1] == '\n' || rx_buf[rx_len - 1] == '\r')) {
                rx_len--;
            }
            rx_buf[rx_len] = '\0';
            Command_Parse();
        }

        memset(rx_buf, 0, MSG_LEN);
        UART2_RX_activeBuffer ^= 1;
        HAL_UART_Receive_DMA(&huart2, (uint8_t *)UART2_RX_DMA_buffer[UART2_RX_activeBuffer], MSG_LEN);
    }
    /* USER CODE END USART2_IRQn 0 */
    HAL_UART_IRQHandler(&huart2);
    /* USER CODE BEGIN USART2_IRQn 1 */
    /* USER CODE END USART2_IRQn 1 */
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
        static uint32_t tim1_drv_cnt      = 0;
        static uint16_t led_pwm_counter   = 0;
        static uint16_t led_breath_level  = 0;
        static int8_t led_breath_dir      = 1;
        static uint16_t led_step_ticks    = 0;
        const uint16_t led_pwm_period     = 100U;
        const uint16_t led_step_ticks_max = 1000U;

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

        if (led_breath_enabled) {
            if (++led_step_ticks >= led_step_ticks_max) {
                led_step_ticks = 0;
                if (led_breath_dir > 0) {
                    if (led_breath_level < led_pwm_period) {
                        led_breath_level++;
                    } else {
                        led_breath_level = led_pwm_period;
                        led_breath_dir   = -1;
                    }
                } else {
                    if (led_breath_level > 0) {
                        led_breath_level--;
                    } else {
                        led_breath_level = 0;
                        led_breath_dir   = 1;
                    }
                }
            }

            if (++led_pwm_counter >= led_pwm_period) {
                led_pwm_counter = 0;
            }
            if (led_pwm_counter < led_breath_level) {
                HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
            }
        } else {
            led_pwm_counter  = 0;
            led_breath_level = 0;
            led_breath_dir   = 1;
            led_step_ticks   = 0;
            HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
        }
    }

    if (htim->Instance == TIM2) {
    }

    if (htim->Instance == TIM3) {
        static uint8_t tim3_btn_cnt;
        static uint16_t tim3_oled_cnt;

        if (++tim3_btn_cnt >= 25) {
            tim3_btn_cnt        = 0;
            button_scan_pending = true;
        }
        if (++tim3_oled_cnt >= 500) {
            tim3_oled_cnt       = 0;
            oled_update_pending = true;
        }
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
