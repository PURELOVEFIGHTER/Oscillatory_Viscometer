#ifndef OSCILLATE_H
#define OSCILLATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "drv8833_driver.h"
#include "config.h"

typedef enum { OSCILLATE_STOPPED = 0, OSCILLATE_RUNNING } Oscillate_State_t;
typedef enum { SCAN_DEFAULT = 0, SCAN_FREQ, SCAN_DR } Scan_Type_t;
typedef enum { SCAN_WAIT = 0, SCAN_RUNNING, SCAN_DONE } Scan_State_t;

void Oscillate_Init(void);
void Oscillate_Start(void);
void Oscillate_Stop(void);
void Oscillate_SetFreq(float freq);
void Oscillate_SetDutyRatio(float duty_ratio);
float Oscillate_GetFreq(void);
float Oscillate_GetDutyRatio(void);
bool Oscillate_GetExcitingLevel(void);
void Oscillate_TickISR(void);
void Oscillate_ScanStart(Scan_Type_t type);
void Oscillate_ScanDone(void);
void Oscillate_ScanISR(void);

#ifdef __cplusplus
}
#endif

#endif /* OSCILLATE_H */
