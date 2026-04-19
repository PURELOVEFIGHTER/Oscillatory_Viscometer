#include "task1_measurement.h"
#include "oscillate.h"
#include "lhr_sample.h"
#include "pack.h"
#include "usart3.h"

static bool s_task1_is_running;

void Task1_Measurement_Init(void) {
    Oscillate_Init();
    LHRSample_Init();

    s_task1_is_running = false;
}

void Task1_Measurement_Toggle(void) {
    if (!s_task1_is_running) {
        Oscillate_Start();
        LHRSample_Start();
        s_task1_is_running = true;
    } else {
        LHRSample_Stop();
        Oscillate_Stop();
        s_task1_is_running = false;
    }
}

void Task1_Measurement_Run(void) {
    if(!s_task1_is_running) {
        return;
    }
    uint32_t lhr_data;

    uint8_t frame[11];

    lhr_data = LHRSample_Poll();
    Pack_MeasurementFrame(lhr_data, Oscillate_GetFreq(), Oscillate_GetDutyRatio(), Oscillate_GetExcitingLevel(), frame,
                          sizeof(frame));
    if (USART3_Enqueue(frame, sizeof(frame))) {
        USART3_StartTrans();
    }
}