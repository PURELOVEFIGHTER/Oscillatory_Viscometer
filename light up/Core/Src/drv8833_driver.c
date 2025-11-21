#include "config.h"
#include "drv8833_driver.h"

static inline uint32_t DRV_GetPWMPeriod(TIM_HandleTypeDef *htim) {
    return (htim != NULL) ? __HAL_TIM_GET_AUTORELOAD(htim) : 0U;
}

void DRV_Init(DRV8833_HandleTypeDef *hdrv) {
    DRV_Sleep(hdrv);
    HAL_Delay(1);

    DRV_Coast(hdrv, &(hdrv->CHANNEL_A));

    hdrv->CHANNEL_A.direction = DRV_STAGE_COAST;
    hdrv->CHANNEL_B.direction = DRV_STAGE_COAST;

    DRV_Wake(hdrv);
    HAL_Delay(1);
}

void DRV_Wake(DRV8833_HandleTypeDef *hdrv) {
    HAL_GPIO_WritePin(hdrv->nSLEEP_Port, hdrv->nSLEEP_Pin, GPIO_PIN_SET);
    hdrv->CHANNEL_A.direction = DRV_STAGE_COAST;
    hdrv->CHANNEL_B.direction = DRV_STAGE_COAST;
}

void DRV_Sleep(DRV8833_HandleTypeDef *hdrv) {
    /*A通道输出关闭*/
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH1, 0);
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_A.htim, hdrv->CHANNEL_A.CH2, 0);
    /*B通道输出关闭*/
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_B.htim, hdrv->CHANNEL_B.CH1, 0);
    __HAL_TIM_SET_COMPARE(hdrv->CHANNEL_B.htim, hdrv->CHANNEL_B.CH2, 0);
    /*进入睡眠*/
    HAL_GPIO_WritePin(hdrv->nSLEEP_Port, hdrv->nSLEEP_Pin, GPIO_PIN_RESET);
    hdrv->CHANNEL_A.direction = DRV_STAGE_COAST;
    hdrv->CHANNEL_B.direction = DRV_STAGE_COAST;
}

void DRV_Coast(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel) {
    __HAL_TIM_SET_COMPARE(channel->htim, channel->CH1, 0);
    __HAL_TIM_SET_COMPARE(channel->htim, channel->CH2, 0);
    channel->direction = DRV_STAGE_COAST;
}

void DRV_Forward(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel) {
    uint32_t pulse = DRV_GetPWMPeriod(channel->htim) / 2U;
    __HAL_TIM_SET_COMPARE(channel->htim, channel->CH1, pulse);
    __HAL_TIM_SET_COMPARE(channel->htim, channel->CH2, 0);
    channel->direction = DRV_STAGE_FORWARD;
}
void DRV_Reverse(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel) {
    uint32_t pulse = DRV_GetPWMPeriod(channel->htim) / 2U;
    __HAL_TIM_SET_COMPARE(channel->htim, channel->CH1, 0);
    __HAL_TIM_SET_COMPARE(channel->htim, channel->CH2, pulse);
    channel->direction = DRV_STAGE_REVERSE;
}
void DRV_Brake(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel) {
    uint32_t period = DRV_GetPWMPeriod(channel->htim);
    __HAL_TIM_SET_COMPARE(channel->htim, channel->CH1, period);
    __HAL_TIM_SET_COMPARE(channel->htim, channel->CH2, period);
    channel->direction = DRV_STAGE_BRAKE;
}
void DRV_SetDirection(DRV8833_Channel *channel, DRV_Direction_t direction) { channel->direction = direction; }
void DRV_updateDirection(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel) {
    switch (channel->direction) {
        case DRV_STAGE_COAST:
            DRV_Coast(hdrv, channel);
            break;
        case DRV_STAGE_FORWARD:
            DRV_Forward(hdrv, channel);
            break;
        case DRV_STAGE_REVERSE:
            DRV_Reverse(hdrv, channel);
            break;
        case DRV_STAGE_BRAKE:
            DRV_Brake(hdrv, channel);
            break;
        default:
            DRV_Coast(hdrv, channel);
            break;
    }
}
