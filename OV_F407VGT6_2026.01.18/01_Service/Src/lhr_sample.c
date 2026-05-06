#include "lhr_sample.h"

extern LDC_HandleTypeDef hldc1;
static volatile LHRSample_State_t s_LHR_state = LHR_SAMPLE_STOPPED;

static uint32_t s_LHR_data            = 0;
uint16_t Rp_data                      = 0;
uint16_t L_data                       = 0;
static volatile bool s_count_enable   = false;
static volatile uint32_t s_sample_cnt = 0;

void LHRSample_Init(void) {
    s_LHR_state    = LHR_SAMPLE_STOPPED;
    s_LHR_data     = 0;
    s_sample_cnt   = 0;
    s_count_enable = false;
}

LHRSample_State_t LHRSample_GetState(void) { return s_LHR_state; }

void LHRSample_Start(void) {
    ldc1101_setPowerMode(&hldc1, LDC_ACTIVE);
    s_LHR_state = LHR_SAMPLE_RUNNING;
}

void LHRSample_Stop(void) {
    ldc1101_setPowerMode(&hldc1, LDC_SLEEP);
    s_LHR_state = LHR_SAMPLE_STOPPED;
}

bool LHRSample_Poll(uint32_t *out_LHR_data) {
    uint8_t LHR_status;
    LHR_status = ldc1101_readByte(&hldc1, _LDC1101_REG_LHR_STATUS);

    if ((LHR_status & 0x01U) == 0U) {
        s_LHR_data = ldc1101_getLHRData(&hldc1);
        if (s_count_enable) {
            s_sample_cnt++;
        }
        *out_LHR_data = s_LHR_data;
        return true;
    }
    return false;
}

void LHRSample_CountSwitch(bool enable) { s_count_enable = enable; }
void LHRSample_ResetCount(void) { s_sample_cnt = 0; }
uint32_t LHRSample_GetCount(void) { return s_sample_cnt; }