#ifndef USART2_H
#define USART2_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MSG_LEN 128

extern volatile uint8_t UART2_RX_activeBuffer;
extern char UART2_RX_DMA_buffer[2][MSG_LEN];
extern char UART2_TX_buffer[MSG_LEN];
extern bool UART2_TX_send;
void UART2_Log(const char *level, const char *file, int line, const char *message);

#ifdef __cplusplus
}
#endif
#endif /* USART2_H */