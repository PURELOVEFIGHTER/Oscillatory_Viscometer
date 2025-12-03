#ifndef CONFIG_H
#define CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
/* =========================================================
 *                     ??????
 * ========================================================= */
#define FREQ_SCAN_DEFAULT_HZ   200
#define FREQ_SCAN_START_HZ     150 // ??????
#define FREQ_SCAN_END_HZ       250 // ??????
#define FREQ_SCAN_STEP_HZ      1   // ??????
#define FREQ_SCAN_HOLD_TIME_MS 50  // ????????(ms)

/* =========================================================
 *                  ??????????
 * ========================================================= */
#define DUTY_RATIO_DEFAULT           50
#define DUTY_RATIO_SCAN_START        0   // ????????
#define DUTY_RATIO_SCAN_END          100 // ????????
#define DUTY_RATIO_SCAN_STEP         1   // ????????
#define DUTY_RATIO_SCAN_HOLD_TIME_MS 30  // ??????????ms)

/* =========================================================
 *                     LDC1101??
 * ========================================================= */
#define LDC2_CS_GPIO_PORT GPIOB
#define LDC2_CS_PIN       GPIO_PIN_12

/* =========================================================
 *                     DRV8833??
 * ========================================================= */
/* =============== A?????? =============== */
#define DRV_A_PWM_TIMER  htim2
#define DRV_AIN1_PWM_CH1 TIM_CHANNEL_1
#define DRV_AIN2_PWM_CH2 TIM_CHANNEL_2
/* =============== B?????? =============== */
// #define DRV_B_PWM_TIMER  NULL
// #define DRV_BIN1_PWM_CH1 0
// #define DRV_BIN2_PWM_CH2 0
/* =============== ???????? =============== */
#define DRV_nSLEEP_GPIO_PORT GPIOA
#define DRV_nSLEEP_PIN       GPIO_PIN_2

#ifdef __cplusplus
}
#endif
#endif /* CONFIG_H */
