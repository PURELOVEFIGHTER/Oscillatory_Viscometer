#include "drv8833_driver.h"

volatile uint8_t drv_FAULT = 0 ;

void DRV8833_Init(DRV8833_HandleTypeDef *hdrv,
                  GPIO_TypeDef *AIN1_Port, uint16_t AIN1_Pin,
                  GPIO_TypeDef *AIN2_Port, uint16_t AIN2_Pin,
                  GPIO_TypeDef *BIN1_Port, uint16_t BIN1_Pin,
                  GPIO_TypeDef *BIN2_Port, uint16_t BIN2_Pin,
                  GPIO_TypeDef *SLEEP_Port, uint16_t SLEEP_Pin,
                  GPIO_TypeDef *FAULT_Port, uint16_t FAULT_Pin)
{
    hdrv->channel_A.IN1_Port = AIN1_Port;
    hdrv->channel_A.IN1_Pin  = AIN1_Pin;

    hdrv->channel_A.IN2_Port = AIN2_Port;
    hdrv->channel_A.IN2_Pin  = AIN2_Pin;

    hdrv->channel_B.IN1_Port = BIN1_Port;
    hdrv->channel_B.IN1_Pin  = BIN1_Pin;

    hdrv->channel_B.IN2_Port = BIN1_Port;
    hdrv->channel_B.IN2_Pin  = BIN1_Pin;

    hdrv->SLEEP_Port = SLEEP_Port;
    hdrv->SLEEP_Pin  = SLEEP_Pin;

    hdrv->FAULT_Port = FAULT_Port;
    hdrv->FAULT_Pin  = FAULT_Pin;
}


// 唤醒 DRV8833 芯片（nSLEEP = 1）
void DRV_Wake(DRV8833_HandleTypeDef* drv)
{
    HAL_GPIO_WritePin(drv->SLEEP_Port, drv->SLEEP_Pin, GPIO_PIN_SET);
}

// 让 DRV8833 进入睡眠（nSLEEP = 0）
void DRV_Sleep(DRV8833_HandleTypeDef* drv)
{
    HAL_GPIO_WritePin(drv->SLEEP_Port, drv->SLEEP_Pin, GPIO_PIN_RESET);
}


// 滑行：IN1 = 0, IN2 = 0
void DRV_Coast(DRV8833_Channel* ch)
{
    HAL_GPIO_WritePin(ch->IN1_Port, ch->IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ch->IN2_Port, ch->IN2_Pin, GPIO_PIN_RESET);
}

// 正转：IN1 = 1, IN2 = 0
void DRV_Forward(DRV8833_Channel* ch)
{
    HAL_GPIO_WritePin(ch->IN1_Port, ch->IN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ch->IN2_Port, ch->IN2_Pin, GPIO_PIN_RESET);
}

// 反转：IN1 = 0, IN2 = 1
void DRV_Reverse(DRV8833_Channel* ch)
{
    HAL_GPIO_WritePin(ch->IN1_Port, ch->IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ch->IN2_Port, ch->IN2_Pin, GPIO_PIN_SET);
}

// 刹车：IN1 = 1, IN2 = 1
void DRV_Brake(DRV8833_Channel* ch)
{
    HAL_GPIO_WritePin(ch->IN1_Port, ch->IN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ch->IN2_Port, ch->IN2_Pin, GPIO_PIN_SET);
}

uint8_t DRV_FaIsult(DRV8833_HandleTypeDef* drv)
{
    GPIO_PinState state = HAL_GPIO_ReadPin(drv->FAULT_Port, drv->FAULT_Pin);
    return (state == GPIO_PIN_RESET) ? 1 : 0;
}
