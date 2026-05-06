#include "usart2.h"

extern UART_HandleTypeDef huart2;

volatile uint8_t UART2_RX_activeBuffer = 0;
char UART2_RX_DMA_buffer[2][MSG_LEN]   = {0};
char UART2_TX_buffer[MSG_LEN]          = {0};
bool UART2_TX_send                     = false;

void UART2_Log(const char *level, const char *file, int line, const char *message) {
    uint32_t ticks   = HAL_GetTick();
    uint32_t hours   = ticks / 3600000U;
    uint32_t minutes = (ticks / 60000U) % 60U;
    uint32_t seconds = (ticks / 1000U) % 60U;
    uint32_t millis  = ticks % 1000U;
    char time_buf[16];
    snprintf(time_buf, sizeof(time_buf), "%02lu:%02lu:%02lu.%03lu", (unsigned long)hours, (unsigned long)minutes,
             (unsigned long)seconds, (unsigned long)millis);

    char msg_copy[MSG_LEN];
    strncpy(msg_copy, message, MSG_LEN - 1);
    msg_copy[MSG_LEN - 1] = '\0';

    char log_buf[MSG_LEN];
    snprintf(UART2_TX_buffer, MSG_LEN, "[%s] [%s] [%s:%d] %s\r\n", level, time_buf, file, line, msg_copy);
    HAL_UART_Transmit(&huart2, (uint8_t *)UART2_TX_buffer, strlen(UART2_TX_buffer), HAL_MAX_DELAY);
}