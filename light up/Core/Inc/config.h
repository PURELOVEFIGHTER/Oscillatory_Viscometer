#ifndef CONFIG_H
#define CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

/* =========================================================
 *             System Working Mode Configuration
 * ========================================================= */
typedef enum {
    MODE_MEASUREMENT = 0, // Mode0：正常测量
    MODE_CALIBRITION = 1  // Mode1：静态位移标定
} sysWorkMode;

/* =========================================================
 *               Frequency Scan Configuration
 * ========================================================= */
#define FREQ_SCAN_DEFAULT_HZ   200
#define FREQ_SCAN_START_HZ     150 // ??????
#define FREQ_SCAN_END_HZ       250 // ??????
#define FREQ_SCAN_STEP_HZ      0.5 // ??????
#define FREQ_SCAN_HOLD_TIME_MS 50  // ????????(ms)

/* =========================================================
 *                  Assert Level Configuration
 * ========================================================= */
#define DUTY_RATIO_DEFAULT           50
#define DUTY_RATIO_SCAN_START        0   // ????????
#define DUTY_RATIO_SCAN_END          100 // ????????
#define DUTY_RATIO_SCAN_STEP         1   // ????????
#define DUTY_RATIO_SCAN_HOLD_TIME_MS 30  // ??????????ms)

/* =========================================================
 *                   LDC1101 Configuration
 * ========================================================= */
#define LDC2_CS_GPIO_PORT GPIOB
#define LDC2_CS_PIN       GPIO_PIN_12

/* =========================================================
 *                     DRV8833 Configuration
 * ========================================================= */
/* *************** A Channel *************** */
#define DRV_A_PWM_TIMER  htim2
#define DRV_AIN1_PWM_CH1 TIM_CHANNEL_1
#define DRV_AIN2_PWM_CH2 TIM_CHANNEL_2
/* *************** B Channel *************** */
// #define DRV_B_PWM_TIMER  NULL
// #define DRV_BIN1_PWM_CH1 0
// #define DRV_BIN2_PWM_CH2 0
/* *************** Sleep Pin *************** */
#define DRV_nSLEEP_GPIO_PORT GPIOB
#define DRV_nSLEEP_PIN       GPIO_PIN_3

/* =========================================================
 *                  Calibrition Configuration
 * ========================================================= */
#define CAL_SAMPLE_NUM       1000U
#define CAL_SETTLE_TIME_MS   200U
#define CALIBRITION_START_UM 450U
#define CALIBRITION_STEP_UM  1U
typedef enum { CAL_IDLE = 0, CAL_WAIT_SETTLE, CAL_SAMPLING, CAL_DONE } CalState_t;

/* =========================================================
 *                      Key Configuration
 * ========================================================= */
#define KEY_DEBOUNCE_MS 100U

#ifdef __cplusplus
}
#endif
#endif /* CONFIG_H */
