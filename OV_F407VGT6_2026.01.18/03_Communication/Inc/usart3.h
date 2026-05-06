#ifndef USART3_H
#define USART3_H
#ifdef __cplusplus
extern "C" {
#endif

#include "pack.h"
#include "stm32f4xx_hal.h"

#define USART3_TX_QUEUE_LEN    64U
#define USART3_FRAME_UNIT_SIZE 12U
#define USART3_TX_BUFFER_SIZE  (USART3_TX_QUEUE_LEN * USART3_FRAME_UNIT_SIZE)

bool USART3_Enqueue(const uint8_t *data, uint16_t len);
void USART3_StartTrans(void);
void USART3_TxCpltCallback(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif
#endif /* USART3_H */