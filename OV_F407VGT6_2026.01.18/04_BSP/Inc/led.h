#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"

typedef enum {
    LED_ACTIVE_LOW = 0, // 低电平点亮
    LED_ACTIVE_HIGH     // 高电平点亮
} LED_ActiveLevel_t;

typedef enum {
    LED_STATE_OFF = 0, // 灯灭
    LED_STATE_ON,      // 灯亮
    LED_STATE_BLINK,   // 闪烁
    LED_STATE_BREATH   // 呼吸灯
} LED_State_t;

typedef struct {
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;
    LED_ActiveLevel_t ActiveLevel;
} LED_Init_t;

typedef struct {
    GPIO_TypeDef *GPIOx;           // LED所在GPIO端口
    uint16_t GPIO_Pin;             // LED对应GPIO引脚
    LED_ActiveLevel_t ActiveLevel; // LED点亮有效电平
    LED_State_t State;             // 当前LED状态
} LED_Handle_t;

void LED_Init(LED_Handle_t *hled, const LED_Init_t *config);
void LED_On(LED_Handle_t *hled);
void LED_Off(LED_Handle_t *hled);
void LED_Blink(LED_Handle_t *hled);
void LED_Breath(LED_Handle_t *hled);

#ifdef __cplusplus
}
#endif
#endif // LED_H