#include "config.h"

const LDC_RegConfig_t ldc_lhr_cfg[] = {
    // 基本参数设置，LHR 模式只关注 RP_MIN，需要与线圈参数对应
    {_LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE, _LDC1101_RP_SET_RP_MIN_1_5KOhm | _LDC1101_RP_SET_RP_MAX_3KOhm},
    // 进入 LHR 模式
    {_LDC1101_REG_CFG_ADDITIONAL_DEVICE, _LDC1101_ALT_CFG_L_OPTIMAL_ENABLE},
    {_LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x01}, // 振幅不足也持续转换
    // 中断报告事件
    {_LDC1101_REG_CFG_INTB_MODE, _LDC1101_INTB_MODE_DONT_REPORT_INTB_ON_SDO_PIN},
    // 采样率
    {_LDC1101_REG_LHR_RCOUNT_LSB, 0x49},
    {_LDC1101_REG_LHR_RCOUNT_MSB, 0x01},
    // 偏移修正（初始值，后续可以动态调整）
    {_LDC1101_REG_LHR_OFFSET_LSB, 0x00},
    {_LDC1101_REG_LHR_OFFSET_MSB, 0x00},
    // 分频设置，减小线圈输入频率以保证 LDC 输入频率（16MHz）始终大于 4 倍的传感器频率
    {_LDC1101_REG_CFG_LHR, _LDC1101_LHR_CFG_FREQUENCY_NOT_DIVIDED},
};
const uint16_t ldc_lhr_cfg_size = sizeof(ldc_lhr_cfg) / sizeof(ldc_lhr_cfg[0]);

/* =========================================================
 *                     LED Configuration
 * ========================================================= */
const LED_Init_t led1_cfg = {.GPIOx = GPIOB, .GPIO_Pin = GPIO_PIN_2, .ActiveLevel = LED_ACTIVE_HIGH};
const LED_Init_t led2_cfg = {.GPIOx = GPIOD, .GPIO_Pin = GPIO_PIN_0, .ActiveLevel = LED_ACTIVE_HIGH};