#ifndef CONFIG_H
#define CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ldc1101_driver.h"
#include "led.h"

/* =========================================================
 *             System Working Mode Configuration
 * ========================================================= */
typedef enum {
    MODE_MEASUREMENT    = 0, // Mode0：测量模式
    MODE_CALIBRITION    = 1, // Mode1：静态位移标定
    MODE_PULSE_FEEDBACK = 2  // Mode2：脉冲响应测试
} SysWorkMode;

/* =========================================================
 *               Frequency Scan Configuration
 * ========================================================= */
#define FREQ_SCAN_DEFAULT_HZ   274
#define FREQ_SCAN_START_HZ     250 // 扫频起始频率
#define FREQ_SCAN_END_HZ       500 // 扫频终止频率
#define FREQ_SCAN_STEP_HZ      1   // 扫频步进
#define FREQ_SCAN_HOLD_TIME_MS 40  // 每个频率采集时间(ms)

/* =========================================================
 *                  Duty Ratio Configuration
 * ========================================================= */
#define DUTY_RATIO_DEFAULT           60
#define DUTY_RATIO_SCAN_START        0   // 有效电平扫描起始比例
#define DUTY_RATIO_SCAN_END          100 // 有效电平扫描终止比例
#define DUTY_RATIO_SCAN_STEP         1   // 扫频步进
#define DUTY_RATIO_SCAN_HOLD_TIME_MS 30  // 每个占空比采集时间(ms)

/* =========================================================
 *                  Calibrition Configuration
 * ========================================================= */
#define CAL_SAMPLE_EPISODE   10U
#define CAL_SAMPLE_NUM       1000U
#define CAL_SETTLE_TIME_MS   200U
#define CALIBRITION_START_UM 450U
#define CALIBRITION_STEP_UM  1U
#define CALIBRITION_STEP_DIR 1U // 1: 减少，0：原地观测，-1：增加
typedef enum { CAL_IDLE = 0, CAL_WAIT_SETTLE, CAL_SAMPLING, CAL_DONE } CalState_t;

/* =========================================================
 *                  Pulse Feedback Configuration
 * ========================================================= */
#define PULSE_FEEDBACK_MAX_NUM     3000U
#define PULSE_FEEDBACK_TRIGGER_MAX 10U

/* =========================================================
 *                   LDC1101 Configuration
 * ========================================================= */
#define LDC_SPI          hspi2
#define LDC_CS_GPIO_PORT GPIOB
#define LDC_CS_PIN       GPIO_PIN_12
extern const uint16_t ldc_lhr_cfg_size;
extern const LDC_RegConfig_t ldc_lhr_cfg[];

/* =========================================================
 *                     DRV8833 Configuration
 * ========================================================= */

/* =========================================================
 *                      Key Configuration
 * ========================================================= */
#define KEY1_GPIO_PORT GPIOA
#define KEY1_PIN       GPIO_PIN_0
#define KEY2_GPIO_PORT GPIOD
#define KEY2_PIN       GPIO_PIN_1

/* =========================================================
 *                      LED Configuration
 * ========================================================= */
extern const LED_Init_t led1_cfg;
extern const LED_Init_t led2_cfg;

#ifdef __cplusplus
}
#endif
#endif /* CONFIG_H */
