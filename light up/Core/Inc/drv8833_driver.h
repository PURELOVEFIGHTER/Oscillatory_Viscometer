#ifndef __DRV8833_DRIVER_H
#define __DRV8833_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"


typedef struct {
    GPIO_TypeDef* IN1_Port;
    uint16_t IN1_Pin;

    GPIO_TypeDef* IN2_Port;
    uint16_t IN2_Pin;
} DRV8833_Channel;


typedef struct {
    DRV8833_Channel channel_A;
    DRV8833_Channel channel_B;

    GPIO_TypeDef* SLEEP_Port;
    uint16_t      SLEEP_Pin;

    GPIO_TypeDef* FAULT_Port;
    uint16_t      FAULT_Pin;

} DRV8833_HandleTypeDef;

extern volatile uint8_t drv_FAULT;

void DRV8833_Init(DRV8833_HandleTypeDef *hdrv,
                  GPIO_TypeDef *AIN1_Port, uint16_t AIN1_Pin,
                  GPIO_TypeDef *AIN2_Port, uint16_t AIN2_Pin,
                  GPIO_TypeDef *BIN1_Port, uint16_t BIN1_Pin,
                  GPIO_TypeDef *BIN2_Port, uint16_t BIN2_Pin,
                  GPIO_TypeDef *SLEEP_Port, uint16_t SLEEP_Pin,
                  GPIO_TypeDef *FAULT_Port, uint16_t FAULT_Pin);

void DRV_Wake(DRV8833_HandleTypeDef* drv);

void DRV_Sleep(DRV8833_HandleTypeDef* drv);

void DRV_Coast(DRV8833_Channel* ch);

void DRV_Forward(DRV8833_Channel* ch);

void DRV_Reverse(DRV8833_Channel* ch);

void DRV_Brake(DRV8833_Channel* ch);

uint8_t DRV_FaIsult(DRV8833_HandleTypeDef* drv);
























#ifdef __cplusplus
}
#endif

#endif /* __DRV8833_H */
