#include <stm32f4xx_hal.h>
#include "main.h"
#include "my_button.h"

Button btn1;
Button btn2;
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
    if (system_mode == MODE_MEASUREMENT) {
        ldc2_isReading = !ldc2_isReading;

        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
        sprintf(UART2_TX_buffer, ldc2_isReading ? "Reading LHR Data.\r\n" : "Stopped LHR Data Reading.\r\n");
        UART2_TX_send = true;
    } else if (system_mode == MODE_CALIBRITION) {
        if (cal_state == CAL_IDLE) {
            // 标定流程进入等待稳定阶段
            cal_state = CAL_WAIT_SETTLE;
            // 数据清空
            cal_episode_cnt = 0;
            memset(mean_episode, 0, sizeof(mean_episode));
            cal_sample_cnt       = 0;
            cal_sum              = 0;
            cal_sum_sq           = 0;
            cal_total_sample_cnt = 0;
            cal_total_sum        = 0;
            cal_total_sum_sq     = 0;
            cal_wait_start_tick  = HAL_GetTick();

            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
            sprintf(UART2_TX_buffer, "Calibration start.\r\n");
            UART2_TX_send = true;
        }
    }
}

void btn1_double_click_handler(Button *btn) {
    if (system_mode == MODE_MEASUREMENT) {
        // 切换到标定模式
        system_mode = MODE_CALIBRITION;
        sprintf(UART2_TX_buffer, "Switch to Calibration Mode.\r\n");
        UART2_TX_send           = true;
        cal_current_position_um = CALIBRITION_START_UM;
        uint8_t init_frame[11];
        memset(init_frame, 0xFF, sizeof(init_frame));
        bool init_enqueued = UART3_Enqueue(init_frame, sizeof(init_frame));
        if (init_enqueued) {
            UART3_StartTx();
        }
    } else if (system_mode == MODE_CALIBRITION) {
        // 切换到测量模式
        system_mode = MODE_MEASUREMENT;
        sprintf(UART2_TX_buffer, "Switch to Measurement Mode.\r\n");
        UART2_TX_send = true;
    }
}

void btn1_long_press_start_handler(Button *btn) {}

void btn1_long_press_hold_handler(Button *btn) {}

void btn1_press_repeat_handler(Button *btn) {}

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

    // Start button processing
    button_start(&btn1);
    button_start(&btn2);
}
