#include "task3_pulse.h"
#include "oscillate.h"
#include "lhr_sample.h"
#include "pack.h"
#include "usart3.h"

static volatile bool s_task3_is_running;
static volatile bool s_task3_is_running_last;
static volatile uint8_t s_task3_trigger_cnt;

void Task3_Pulse_Init(void) {
    s_task3_is_running      = false;
    s_task3_is_running_last = false;
    s_task3_trigger_cnt     = 0;

    LHRSample_CountSwitch(false);
    LHRSample_ResetCount();
}
void Task3_Pulse_Start(void) { s_task3_is_running = true; }
void Task3_Pulse_Stop(void) { s_task3_is_running = false; }

void Task3_Pulse_Trigger(void) { s_task3_trigger_cnt++; }

void Task3_Pulse_Run(void) {
    /*
     * 1. 触发计数负责启动任务
     */
    if ((s_task3_is_running == false) && (s_task3_trigger_cnt > 0)) {
        s_task3_trigger_cnt--;
        s_task3_is_running = true;
    }
    /*
     * 2. 任务状态边沿检测，确定振动与采样的启动与停止
     */
    if (s_task3_is_running != s_task3_is_running_last) {
        if (s_task3_is_running) {
            Oscillate_Start();
            LHRSample_ResetCount();
            LHRSample_CountSwitch(true);
            LHRSample_Start();
        } else {
            LHRSample_CountSwitch(false);
            LHRSample_Stop();
        }
        s_task3_is_running_last = s_task3_is_running;
    }
    /*
     * 3. 任务运行时进行数据的采样发送
     */
    if (s_task3_is_running == false) {
        return;
    }
    uint32_t lhr_data;
    uint8_t frame[12];
    if (LHRSample_Poll(&lhr_data) == false) {
        return;
    } else {
        Pack_MeasurementFrame(lhr_data, Oscillate_GetFreq(), Oscillate_GetDutyRatio(), Oscillate_GetExcitingLevel(),
                              frame, sizeof(frame));
        if (USART3_Enqueue(frame, sizeof(frame))) {
            USART3_StartTrans();
        }
    }
    /*
     * 4. 采样数量达到要求后，关闭任务
     */
    if (LHRSample_GetCount() >= PULSE_FEEDBACK_MAX_NUM) {
        Task3_Pulse_Stop();
    }
}