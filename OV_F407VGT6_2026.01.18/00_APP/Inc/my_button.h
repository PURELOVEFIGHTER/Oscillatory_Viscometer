#ifndef __MY_BUTTON_H
#define __MY_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "multi_button.h"

uint8_t getButtonLevel(uint8_t button_id);
void buttons_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __MY_BUTTON_H */
