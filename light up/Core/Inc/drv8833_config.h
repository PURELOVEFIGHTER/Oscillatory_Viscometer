#pragma once
#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN DRV8833_Config ------------------------------ */
#define DRV_USE_CHANNEL_B 0 // 如果使用DRV8833的B通道，置1，否则置0
#define DRV_USE_nFAULT    0 // 如果使用DRV8833的FAULT引脚，置1，否则置0

#define DRV_AIN1_GPIO_Port GPIOA
#define DRV_AIN1_Pin       GPIO_PIN_0
#define DRV_AIN2_GPIO_Port GPIOA
#define DRV_AIN2_Pin       GPIO_PIN_1

#if DRV_USE_CHANNEL_B
    #define DRV_BIN1_GPIO_Port NULL
    #define DRV_BIN1_Pin       0
    #define DRV_BIN2_GPIO_Port NULL
    #define DRV_BIN2_Pin       0
#endif

#define DRV_nSLEEP_GPIO_Port GPIOB
#define DRV_nSLEEP_Pin       GPIO_PIN_1

#if DRV_USE_nFAULT
    #define DRV_nFAULT_GPIO_Port NULL
    #define DRV_nFAULT_Pin       0
#endif
/* USER CODE END DRV_Config ------------------------------ */

#ifdef __cplusplus
}
#endif
