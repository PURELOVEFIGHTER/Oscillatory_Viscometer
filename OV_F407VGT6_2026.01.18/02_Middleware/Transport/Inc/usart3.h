#ifndef USART3_H
#define USART3_H
#ifdef __cplusplus
extern "C" {
#endif

#include "pack.h"
#include "stm32f4xx_hal.h"

#define USART3_TX_QUEUE_LEN    1024U
#define USART3_FRAME_UNIT_SIZE 11U
#define USART3_TX_BUFFER_SIZE  (USART3_TX_QUEUE_LEN * USART3_FRAME_UNIT_SIZE)

/* 将一段数据入队到 USART3 发送缓冲区 */
bool USART3_Enqueue(const uint8_t *data, uint16_t len);

/* 若 DMA 空闲，则启动一次发送；可重复调用 */
void USART3_StartTrans(void);
/* USART3 发送完成回调函数 */
void USART3_TxCpltCallback(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif
#endif /* USART3_H */