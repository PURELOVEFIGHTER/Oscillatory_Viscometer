#ifndef LHR_SAMPLE_H
#define LHR_SAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ldc1101_driver.h"

typedef enum { LHR_SAMPLE_STOPPED = 0, LHR_SAMPLE_RUNNING } LHRSample_State_t;
void LHRSample_Init(void);
LHRSample_State_t LHRSample_GetState(void);
void LHRSample_Start(void);
void LHRSample_Stop(void);
bool LHRSample_Poll(uint32_t *out_LHR_data);
void LHRSample_CountSwitch(bool enable);
void LHRSample_ResetCount(void);
uint32_t LHRSample_GetCount(void);

#ifdef __cplusplus
}
#endif
#endif /* LHR_SAMPLE_H */
