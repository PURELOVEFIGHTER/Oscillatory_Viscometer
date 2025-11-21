#ifndef CONFIG_H
#define CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
/* =========================================================
 *                     扫频参数配置
 * ========================================================= */
#define FREQ_SCAN_START_HZ     150 // 扫频起始频率
#define FREQ_SCAN_END_HZ       250 // 扫频结束频率
#define FREQ_SCAN_STEP_HZ      1   // 扫频步进频率
#define FREQ_SCAN_HOLD_TIME_MS 50  // 每个频率停留时间(ms)

/* =========================================================
 *                  占空比扫描参数配置
 * ========================================================= */
#define DUTY_RATIO_SCAN_START        0   // 占空比起始数值
#define DUTY_RATIO_SCAN_END          100 // 占空比结束数值
#define DUTY_RATIO_SCAN_STEP         1   // 占空比步进数值
#define DUTY_RATIO_SCAN_HOLD_TIME_MS 30  // 每个占空比停留时间(ms)

/* =========================================================
 *                     LDC1101配置
 * ========================================================= */
#define LDC2_CS_GPIO_PORT GPIOB
#define LDC2_CS_PIN       GPIO_PIN_12

/* =========================================================
 *                     DRV8833配置
 * ========================================================= */
/* =============== A通道引脚映射 =============== */
#define DRV_A_PWM_TIMER  htim2
#define DRV_AIN1_PWM_CH1 TIM_CHANNEL_1
#define DRV_AIN2_PWM_CH2 TIM_CHANNEL_2
/* =============== B通道引脚映射 =============== */
// #define DRV_B_PWM_TIMER  NULL
// #define DRV_BIN1_PWM_CH1 0
// #define DRV_BIN2_PWM_CH2 0
/* =============== 唤醒控制引脚映射 =============== */
#define DRV_nSLEEP_GPIO_PORT GPIOA
#define DRV_nSLEEP_PIN       GPIO_PIN_2

#ifdef __cplusplus
}
#endif
#endif /* CONFIG_H */
