#include "oled.h"

static volatile uint8_t oled_update_pending;

static const char *Mode_ToString(SysWorkMode mode) {
    switch (mode) {
        case MODE_MEASUREMENT:
            return "Measurement";
        case MODE_CALIBRITION:
            return "Calibration";
        case MODE_PULSE_FEEDBACK:
            return "Pulse Feedback";
        default:
            return "UNKNOWN";
    }
}

void OLED_Update(SysWorkMode system_mode) {
    OLED_Clear();

    OLED_ShowString(0, 0, "Mode: ", OLED_TEXT_SIZE, OLED_TEXT_INVERT);
    OLED_ShowString(36, 0, (char *)Mode_ToString(system_mode), OLED_TEXT_SIZE, OLED_TEXT_INVERT);
    OLED_ShowString(0, 1, "Viscosity:", OLED_TEXT_SIZE, OLED_TEXT_INVERT);

    OLED_Refresh_DMA();
    OLED_ClearUpdatePending();
}

void OLED_SetUpdatePending(void) { oled_update_pending = true; }
void OLED_ClearUpdatePending(void) { oled_update_pending = false; }
uint8_t OLED_GetUpdatePending(void) { return oled_update_pending; }


