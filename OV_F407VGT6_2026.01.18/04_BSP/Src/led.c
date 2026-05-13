#include "led.h"

void LED_Init(LED_Handle_t *hled, const LED_InitTypeDef *config) {
    hled->GPIOx       = config->GPIOx;
    hled->GPIO_Pin    = config->GPIO_Pin;
    hled->ActiveLevel = config->ActiveLevel;
    hled->State       = LED_STATE_ON;
}