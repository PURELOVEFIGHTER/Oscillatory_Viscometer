#ifndef DRV8833_DRIVER_H
#define DRV8833_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "config.h"

typedef struct {
    GPIO_TypeDef *IN1_Port;
    uint16_t IN1_Pin;

    GPIO_TypeDef *IN2_Port;
    uint16_t IN2_Pin;
} DRV8833_Channel;

typedef struct {
    DRV8833_Channel CHANNEL_A;

#if DRV8833_USE_CHANNEL_B
    DRV8833_Channel CHANNEL_B;
#endif

    GPIO_TypeDef *nSLEEP_Port;
    uint16_t nSLEEP_Pin;

    GPIO_TypeDef *nFAULT_Port;
    uint16_t nFAULT_Pin;

} DRV8833_HandleTypeDef;

typedef enum {
    DRV_STAGE_COAST   = 0, // 滑行
    DRV_STAGE_FORWARD = 1, // 正转
    DRV_STAGE_REVERSE = 2, // 反转
    DRV_STAGE_BRAKE   = 3  // 刹车
} DRV_Stage_t;

extern volatile DRV_Stage_t drv_stage;

extern volatile uint8_t drv_FAULT;

void DRV8833_Init(DRV8833_HandleTypeDef *hdrv);

void DRV_Wake(DRV8833_HandleTypeDef *hdrv);

void DRV_Sleep(DRV8833_HandleTypeDef *hdrv);

void DRV_Coast(DRV8833_Channel *ch);

void DRV_Forward(DRV8833_Channel *ch);

void DRV_Reverse(DRV8833_Channel *ch);

void DRV_Brake(DRV8833_Channel *ch);

uint8_t DRV_Fault(DRV8833_HandleTypeDef *hdrv);

#ifdef __cplusplus
}
#endif

#endif /* DRV8833_DRIVER_H */
