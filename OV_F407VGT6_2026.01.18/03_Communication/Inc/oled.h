#ifndef OLED_H
#define OLED_H
#ifdef __cplusplus
extern "C" {
#endif

#include "oled_driver.h"
#include "config.h"

#define OLED_TEXT_SIZE OLED_TEXT_SIZE_12

// 根据字号选择字符宽度
#if (OLED_TEXT_SIZE == OLED_TEXT_SIZE_12)

    #define OLED_CHAR_WIDTH  6U
    #define OLED_CHAR_HEIGHT 8U

#elif (OLED_TEXT_SIZE == OLED_TEXT_SIZE_16)

    #define OLED_CHAR_WIDTH  8U
    #define OLED_CHAR_HEIGHT 16U
#endif
#define OLED_TEXT_INVERT 0U

void OLED_Update(SysWorkMode system_mode);
void OLED_SetUpdatePending(void);
void OLED_ClearUpdatePending(void);
uint8_t OLED_GetUpdatePending(void);

#ifdef __cplusplus
}
#endif
#endif /* OLED_H */