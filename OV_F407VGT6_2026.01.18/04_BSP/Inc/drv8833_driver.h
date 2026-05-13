#ifndef DRV8833_DRIVER_H
#define DRV8833_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "device_common.h"

/* =========================================================
 *                     DRV8833 电源状态
 * ========================================================= */
typedef enum {
    DRV_ENABLE  = 0, // 使能
    DRV_DISABLE = 1  // 禁止
} DRV_PowerState_t;
/* =========================================================
 *                     DRV8833 运动状态
 * ========================================================= */
typedef enum {
    DRV_STAGE_COAST = 0, // 滑行
    DRV_STAGE_FORWARD,   // 正转
    DRV_STAGE_REVERSE,   // 反转
    DRV_STAGE_BRAKE      // 刹车
} DRV_Direction_t;
/* =========================================================
 *                       DRV8833 通道
 * ========================================================= */
typedef struct {
    volatile DRV_Direction_t direction;
    TIM_HandleTypeDef *htim;
    uint32_t CH1;
    uint32_t CH2;
} DRV_Channel_t;
/* =========================================================
 *                     DRV8833 初始化结构体
 * ========================================================= */
typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t CH1;
    uint32_t CH2;
    GPIO_TypeDef *nSLEEP_Port;
    uint16_t nSLEEP_Pin;
} DRV_InitTypeDef;
/* =========================================================
 *                     DRV8833 句柄结构体
 * ========================================================= */
typedef struct {
    DRV_PowerState_t DRV_Power;
    DRV_Channel_t CHANNEL_A;
    /*
    A、B通道并联为一个通道输出提升功率
    DRV_Channel_t CHANNEL_B;
    */
    GPIO_TypeDef *nSLEEP_Port;
    uint16_t nSLEEP_Pin;
} DRV_HandleTypeDef;
/* =========================================================
 *                     DRV8833 内部函数
 * ========================================================= */
static inline uint32_t DRV_GetPWMPeriod(TIM_HandleTypeDef *htim);

/* =========================================================
 *                     DRV8833 外部接口
 * ========================================================= */
DEVICE_StatusTypeDef BSP_DRV_Init(DRV_HandleTypeDef *hdrv, DRV_InitTypeDef *init);
void DRV_Wake(DRV_HandleTypeDef *hdrv);
void DRV_Sleep(DRV_HandleTypeDef *hdrv);

void DRV_Coast(DRV_HandleTypeDef *hdrv);
void DRV_Forward(DRV_HandleTypeDef *hdrv);
void DRV_Reverse(DRV_HandleTypeDef *hdrv);
void DRV_Brake(DRV_HandleTypeDef *hdrv);

extern DRV_HandleTypeDef hdrv1;

#ifdef __cplusplus
}
#endif

#endif /* DRV8833_DRIVER_H */
