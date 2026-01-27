#ifndef DRV8833_DRIVER_H
#define DRV8833_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "config.h"

/* =========================================================
 *                     DRV8833 状态定义
 * ========================================================= */
typedef enum {
    DRV_STAGE_COAST = 0, // 滑行
    DRV_STAGE_FORWARD,   // 正转
    DRV_STAGE_REVERSE,   // 反转
    DRV_STAGE_BRAKE      // 刹车
} DRV_Direction_t;
/* =========================================================
 *                     DRV8833 通道定义
 * ========================================================= */
typedef struct {
    volatile DRV_Direction_t direction;
    TIM_HandleTypeDef *htim;
    uint32_t CH1;
    uint32_t CH2;
} DRV8833_Channel;
/* =========================================================
 *                   DRV8833 句柄结构体定义
 * ========================================================= */
typedef struct {
    DRV8833_Channel CHANNEL_A;
    DRV8833_Channel CHANNEL_B;
    GPIO_TypeDef *nSLEEP_Port;
    uint16_t nSLEEP_Pin;
} DRV8833_HandleTypeDef;
/* =========================================================
 *                     DRV8833 内部函数
 * ========================================================= */
static inline uint32_t DRV_GetPWMPeriod(TIM_HandleTypeDef *htim);

/* =========================================================
 *                     DRV8833 外部接口
 * ========================================================= */
void DRV_Init(DRV8833_HandleTypeDef *hdrv);
void DRV_Wake(DRV8833_HandleTypeDef *hdrv);
void DRV_Sleep(DRV8833_HandleTypeDef *hdrv);
void DRV_Coast(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel);
void DRV_Forward(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel);
void DRV_Reverse(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel);
void DRV_Brake(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel);
void DRV_SetDirection(DRV8833_Channel *channel, DRV_Direction_t direction);
void DRV_updateDirection(DRV8833_HandleTypeDef *hdrv, DRV8833_Channel *channel);

#ifdef __cplusplus
}
#endif

#endif /* DRV8833_DRIVER_H */
