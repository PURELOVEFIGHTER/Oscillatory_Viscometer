#ifndef OLED_H
#define OLED_H
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "oled_driver.h"

#define OLED_TEXT_SIZE   12U
#define OLED_TEXT_INVERT 0U


void OLED_Update(SysWorkMode system_mode);
void OLED_SetUpdatePending(void);
void OLED_ClearUpdatePending(void);
uint8_t OLED_GetUpdatePending(void);

#ifdef __cplusplus
}
#endif
#endif /* OLED_H */