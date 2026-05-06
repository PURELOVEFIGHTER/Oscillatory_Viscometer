#include <stm32f4xx_hal.h>
#include "my_button.h"
#include "config.h"
#include "task1_measurement.h"
#include "task3_pulse.h"
#include "usart2.h"
#include "oled.h"
#include "oled_driver.h"

extern SysWorkMode system_mode;
extern bool oled_update_pending;

static Button btn1;
static Button btn2;

// 根据平台不同完成按键电平读取函数
uint8_t getButtonLevel(uint8_t button_id) {
    switch (button_id) {
        case 1:
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        case 2:
            return HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1);
        default:
            return 1; // 默认返回未按下状态
    }
}

// Callback functions for button 1
void btn1_single_click_handler(Button *btn) {
    switch (system_mode) {
        case MODE_MEASUREMENT:
            Task1_Measurement_Toggle();
            break;
        case MODE_CALIBRITION:
            break;
        case MODE_PULSE_FEEDBACK:
            Task3_Pulse_Trigger();
            break;
        default:
            break;
    }

    // if (system_mode == MODE_MEASUREMENT) {
    //     HAL_GPIO_TogglePin(LED2_PORT, LED2_PIN);
    //    // sprintf(UART2_TX_buffer, ldc2_isReading ? "Reading LHR Data.\r\n" : "Stopped LHR Data Reading.\r\n");
    //     UART2_TX_send = true;
    // } else if (system_mode == MODE_CALIBRITION) {
    //     if (cal_state == CAL_IDLE) {
    //         // 标定流程进入等待稳定阶段
    //         cal_state = CAL_WAIT_SETTLE;
    //         // 数据清空
    //         cal_episode_cnt = 0;
    //         memset(mean_episode, 0, sizeof(mean_episode));
    //         cal_sample_cnt       = 0;
    //         cal_sum              = 0;    //         cal_sum_sq           = 0;
    //         cal_total_sample_cnt = 0;
    //         cal_total_sum        = 0;
    //         cal_total_sum_sq     = 0;
    //         cal_wait_start_tick  = HAL_GetTick();

    //         HAL_GPIO_TogglePin(LED3_PORT, LED3_PIN);
    //         sprintf(UART2_TX_buffer, "Calibration start.\r\n");
    //         UART2_TX_send = true;
    //     }
    // }
}

void btn1_double_click_handler(Button *btn) {
    switch (system_mode) {
        case MODE_MEASUREMENT:
            Task1_Measurement_Init();
            system_mode = MODE_CALIBRITION;
            sprintf(UART2_TX_buffer, "Switch to Calibration Mode.\r\n");
            OLED_Clear();
            OLED_SetUpdatePending();
            UART2_TX_send = true;
            break;
        case MODE_CALIBRITION:
            system_mode = MODE_PULSE_FEEDBACK;
            sprintf(UART2_TX_buffer, "Switch to Pulse Feedback Mode.\r\n");
            OLED_Clear();
            OLED_SetUpdatePending();
            UART2_TX_send       = true;
            break;
        case MODE_PULSE_FEEDBACK:
            system_mode = MODE_MEASUREMENT;
            sprintf(UART2_TX_buffer, "Switch to Measurement Mode.\r\n");
            OLED_Clear();
            OLED_SetUpdatePending();
            UART2_TX_send       = true;
            break;
        default:
            break;
    }
}

void btn1_long_press_start_handler(Button *btn) {}

void btn1_long_press_hold_handler(Button *btn) {}

void btn1_press_repeat_handler(Button *btn) {}

void btn2_single_click_handler(Button *btn) {
    // if (system_mode == MODE_PULSE_FEEDBACK) {
    //     if (pulse_feedback_trigger < PULSE_FEEDBACK_TRIGGER_MAX) {
    //         pulse_feedback_trigger++;
    //     }
    // }
}

// Initialize buttons
void buttons_init(void) {
    button_init(&btn1, getButtonLevel, 1, 1);
    button_init(&btn2, getButtonLevel, 1, 2);

    // Attach event handlers for button 1
    button_attach(&btn1, BTN_SINGLE_CLICK, btn1_single_click_handler);
    button_attach(&btn1, BTN_DOUBLE_CLICK, btn1_double_click_handler);
    button_attach(&btn1, BTN_LONG_PRESS_START, btn1_long_press_start_handler);
    button_attach(&btn1, BTN_LONG_PRESS_HOLD, btn1_long_press_hold_handler);
    button_attach(&btn1, BTN_PRESS_REPEAT, btn1_press_repeat_handler);
    // Attach event handlers for button 2
    button_attach(&btn2, BTN_SINGLE_CLICK, btn2_single_click_handler);

    // Start button processing
    button_start(&btn1);
    button_start(&btn2);
}
