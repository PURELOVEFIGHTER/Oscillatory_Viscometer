#ifndef COMMAND_H
#define COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"

void Command_Parse(char *cmd);

#ifdef __cplusplus
}
#endif
#endif /* COMMAND_H */
