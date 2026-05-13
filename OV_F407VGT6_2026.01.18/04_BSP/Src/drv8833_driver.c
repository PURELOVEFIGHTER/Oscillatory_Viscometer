#include "drv8833_driver.h"

static inline uint32_t DRV_GetPWMPeriod(TIM_HandleTypeDef *htim) {
    return (htim != NULL) ? __HAL_TIM_GET_AUTORELOAD(htim) : 0U;
}

DEVICE_StatusTypeDef BSP_DRV_Init(DRV_HandleTypeDef *hdrv,DRV_InitTypeDef *init) {
     if (hdrv == NULL || init == NULL) {
        return DEVICE_ERROR;
    }
    hdrv->DRV_Power           = DRV_DISABLE; // 默认休眠状态
    hdrv->CHANNEL_A.direction = DRV_STAGE_COAST;
    hdrv->CHANNEL_A.htim      = init->htim;
    hdrv->CHANNEL_A.CH1       = init->CH1;
    hdrv->CHANNEL_A.CH2       = init->CH2;
    hdrv->nSLEEP_Port         = init->nSLEEP_Port;
    hdrv->nSLEEP_Pin          = init->nSLEEP_Pin;

    return DEVICE_OK;
}

void DRV_Wake(DRV_HandleTypeDef *hdrv) {
    DRV_Coast(hdrv);
    HAL_GPIO_WritePin(hdrv->nSLEEP_Port, hdrv->nSLEEP_Pin, GPIO_PIN_SET);
    hdrv->DRV_Power = DRV_ENABLE;
}

void DRV_Sleep(DRV_HandleTypeDef *hdrv) {
    DRV_Coast(hdrv);
    HAL_GPIO_WritePin(hdrv->nSLEEP_Port, hdrv->nSLEEP_Pin, GPIO_PIN_RESET);
    hdrv->DRV_Power = DRV_DISABLE;
}

void DRV_Coast(DRV_HandleTypeDef *hdrv) {
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH1, 0);
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH2, 0);
    hdrv->CHANNEL_A.direction = DRV_STAGE_COAST;
}
void DRV_Forward(DRV_HandleTypeDef *hdrv) {
    uint32_t pulse = DRV_GetPWMPeriod(hdrv->CHANNEL_A.htim) / 2U;
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH1, pulse);
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH2, 0);
    hdrv->CHANNEL_A.direction = DRV_STAGE_FORWARD;
}
void DRV_Reverse(DRV_HandleTypeDef *hdrv) {
    uint32_t pulse = DRV_GetPWMPeriod(hdrv->CHANNEL_A.htim) / 2U;
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH1, 0);
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH2, pulse);
    hdrv->CHANNEL_A.direction = DRV_STAGE_REVERSE;
}
void DRV_Brake(DRV_HandleTypeDef *hdrv) {
    uint32_t period = DRV_GetPWMPeriod(hdrv->CHANNEL_A.htim);
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH1, period);
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH2, period);
    hdrv->CHANNEL_A.direction = DRV_STAGE_BRAKE;
}
