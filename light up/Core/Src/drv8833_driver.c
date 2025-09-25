#include "drv8833_driver.h"
#include "drv8833_config.h"

volatile uint8_t drv_FAULT = 0;

void DRV8833_Init(DRV8833_HandleTypeDef *hdrv) {
    /* 默认关闭 A 通道 */
    HAL_GPIO_WritePin(hdrv->CHANNEL_A.IN1_Port, hdrv->CHANNEL_A.IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hdrv->CHANNEL_A.IN2_Port, hdrv->CHANNEL_A.IN2_Pin, GPIO_PIN_RESET);

#if DRV8833_USE_CHANNEL_B
    /* 默认关闭 B 通道 */
    HAL_GPIO_WritePin(hdrv->CHANNEL_B.IN1_Port, hdrv->CHANNEL_B.IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(hdrv->CHANNEL_B.IN2_Port, hdrv->CHANNEL_B.IN2_Pin, GPIO_PIN_RESET);
#endif

    /* 唤醒芯片 */
    HAL_GPIO_WritePin(hdrv->nSLEEP_Port, hdrv->nSLEEP_Pin, GPIO_PIN_SET);
}

// 唤醒 DRV8833 芯片（nSLEEP = 1）
void DRV_Wake(DRV8833_HandleTypeDef *hdrv) { HAL_GPIO_WritePin(hdrv->nSLEEP_Port, hdrv->nSLEEP_Pin, GPIO_PIN_SET); }

// DRV8833 进入睡眠（nSLEEP = 0）
void DRV_Sleep(DRV8833_HandleTypeDef *hdrv) { HAL_GPIO_WritePin(hdrv->nSLEEP_Port, hdrv->nSLEEP_Pin, GPIO_PIN_RESET); }

// 滑行：IN1 = 0, IN2 = 0
void DRV_Coast(DRV8833_Channel *ch) {
    HAL_GPIO_WritePin(ch->IN1_Port, ch->IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ch->IN2_Port, ch->IN2_Pin, GPIO_PIN_RESET);
}

// 正转：IN1 = 1, IN2 = 0
void DRV_Forward(DRV8833_Channel *ch) {
    HAL_GPIO_WritePin(ch->IN1_Port, ch->IN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ch->IN2_Port, ch->IN2_Pin, GPIO_PIN_RESET);
}

// 反转：IN1 = 0, IN2 = 1
void DRV_Reverse(DRV8833_Channel *ch) {
    HAL_GPIO_WritePin(ch->IN1_Port, ch->IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ch->IN2_Port, ch->IN2_Pin, GPIO_PIN_SET);
}

// 刹车：IN1 = 1, IN2 = 1
void DRV_Brake(DRV8833_Channel *ch) {
    HAL_GPIO_WritePin(ch->IN1_Port, ch->IN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ch->IN2_Port, ch->IN2_Pin, GPIO_PIN_SET);
}

uint8_t DRV_Fault(DRV8833_HandleTypeDef *hdrv) {
    GPIO_PinState state = HAL_GPIO_ReadPin(hdrv->nFAULT_Port, hdrv->nFAULT_Pin);
    return (state == GPIO_PIN_RESET) ? 1 : 0;
}
