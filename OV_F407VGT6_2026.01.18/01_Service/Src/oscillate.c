#include "oscillate.h"

extern DRV_HandleTypeDef hdrv1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;

static Oscillate_State_t s_oscillate_state = OSCILLATE_STOPPED;

static volatile uint8_t drv_excitingLevel = 0;
static float drv_PWM_freq;
static float drv_PWM_DR;

static volatile uint32_t s_PWM_cnt       = 0;
static volatile uint32_t s_PWM_halfCnt   = 0;
static volatile uint32_t s_PWM_assertCnt = 0;
static Scan_Type_t s_scan_type           = SCAN_DEFAULT;
static uint32_t s_tim1_cnt               = 0;
static uint32_t s_tim4_cnt               = 0;

void Oscillate_Start(void) {
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_PWM_Start(hdrv1.CHANNEL_A.htim, hdrv1.CHANNEL_A.CH1);
    HAL_TIM_PWM_Start(hdrv1.CHANNEL_A.htim, hdrv1.CHANNEL_A.CH2);
    DRV_Wake(&hdrv1);
    s_oscillate_state = OSCILLATE_RUNNING;
}

void Oscillate_Stop(void) {
    HAL_TIM_Base_Stop_IT(&htim1);
    HAL_TIM_PWM_Stop(hdrv1.CHANNEL_A.htim, hdrv1.CHANNEL_A.CH1);
    HAL_TIM_PWM_Stop(hdrv1.CHANNEL_A.htim, hdrv1.CHANNEL_A.CH2);
    DRV_Sleep(&hdrv1);
    s_oscillate_state = OSCILLATE_STOPPED;
}

static void Oscillate_CalculatePWMParam(void) {
    if (drv_PWM_freq <= 0.0f || drv_PWM_DR < 0.0f || drv_PWM_DR > 100.0f) {
        return;
    }
    // TIM1 Interrupt Frequency = 100kHz
    const float baseFreq = 100000.0f;
    // Calculate the number of interrupts needed for one period
    float cnt_f     = baseFreq / drv_PWM_freq;
    s_PWM_cnt       = (uint32_t)(cnt_f + 0.5f);
    float assert_f  = cnt_f * (drv_PWM_DR / 100.0f);
    s_PWM_assertCnt = (uint32_t)(assert_f + 0.5f);
    s_PWM_halfCnt   = s_PWM_cnt / 2;
}

void Oscillate_Init(void) {
    DRV_Sleep(&hdrv1);
    s_oscillate_state = OSCILLATE_STOPPED;
    drv_PWM_freq      = FREQ_SCAN_DEFAULT_HZ;
    drv_PWM_DR        = DUTY_RATIO_DEFAULT;
    Oscillate_CalculatePWMParam();
}

void Oscillate_SetFreq(float freq) {
    if (freq <= 0.0f) {
        return;
    }
    drv_PWM_freq = freq;
    Oscillate_CalculatePWMParam();
}
void Oscillate_SetDutyRatio(float duty_ratio) {
    if ((duty_ratio < 0.0f) || (duty_ratio > 100.0f)) {
        return;
    }
    drv_PWM_DR = duty_ratio;
    Oscillate_CalculatePWMParam();
}

void Oscillate_TickISR(void) {
    if (s_oscillate_state != OSCILLATE_RUNNING) {
        return;
    }
    if (s_tim1_cnt < s_PWM_assertCnt) {
        DRV_Forward(&hdrv1);
        drv_excitingLevel = true;
    } else if (s_tim1_cnt < s_PWM_halfCnt) {
        DRV_Coast(&hdrv1);
    } else if (s_tim1_cnt < s_PWM_halfCnt + s_PWM_assertCnt) {
        // DRV_Reverse(&hdrv1);
        drv_excitingLevel = false;
        DRV_Coast(&hdrv1);
    } else if (s_tim1_cnt < s_PWM_cnt) {
        DRV_Coast(&hdrv1);
    } else {
        s_tim1_cnt = 0;
    }
    s_tim1_cnt++;
}

void Oscillate_ScanStart(Scan_Type_t type) {
    s_tim4_cnt        = 0;
    s_oscillate_state = OSCILLATE_RUNNING;
    HAL_TIM_Base_Start_IT(&htim4);
}

void Oscillate_ScanDone(void) {
    s_tim4_cnt        = 0;
    s_oscillate_state = OSCILLATE_STOPPED;
    HAL_TIM_Base_Stop_IT(&htim4);
}

void Oscillate_ScanISR(void) {
    switch (s_scan_type) {
        case SCAN_FREQ:
            if (s_tim4_cnt > FREQ_SCAN_HOLD_TIME_MS) {
                s_tim4_cnt = 0;
                drv_PWM_freq += FREQ_SCAN_STEP_HZ;
                Oscillate_CalculatePWMParam();
                if (drv_PWM_freq > FREQ_SCAN_END_HZ) {
                    Oscillate_ScanDone();
                }
            } else
                s_tim4_cnt++;
            break;
        case SCAN_DR:
            if (s_tim4_cnt > DUTY_RATIO_SCAN_HOLD_TIME_MS) {
                s_tim4_cnt = 0;
                drv_PWM_DR += DUTY_RATIO_SCAN_STEP;
                Oscillate_CalculatePWMParam();
                if (drv_PWM_DR > DUTY_RATIO_SCAN_END) {
                    Oscillate_ScanDone();
                }
            } else
                s_tim4_cnt++;
            break;
        default:
            break;
    }
}

float Oscillate_GetFreq(void) { return drv_PWM_freq; }
float Oscillate_GetDutyRatio(void) { return drv_PWM_DR; }
bool Oscillate_GetExcitingLevel(void) { return drv_excitingLevel; }