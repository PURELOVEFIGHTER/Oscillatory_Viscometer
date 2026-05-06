#ifndef OSCILLATE_H
#define OSCILLATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "drv8833_driver.h"
#include "config.h"

typedef enum { OSCILLATE_STOPPED = 0, OSCILLATE_RUNNING } Oscillate_State_t;
typedef enum { SCAN_IDLE = 0, SCAN_FREQ, SCAN_DR } Scan_Type_t;
typedef enum { OSC_SCAN_EVENT_START = 0, OSC_SCAN_EVENT_DONE } Oscillate_Scan_Event_t;

// 扫频回调函数
typedef void (*OscillateScanCallback_t)(Oscillate_Scan_Event_t event);

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
Oscillate_State_t Oscillate_GetState(void);
void Oscillate_RegisterScanCallback(OscillateScanCallback_t cb);
#ifdef __cplusplus
}
#endif

#endif /* OSCILLATE_H */
