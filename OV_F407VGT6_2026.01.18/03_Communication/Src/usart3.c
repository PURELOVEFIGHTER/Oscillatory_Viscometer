#include "usart3.h"
#include "string.h"

extern UART_HandleTypeDef huart3;

static uint8_t s_usart3_tx_buffer[USART3_TX_BUFFER_SIZE];
static volatile uint16_t s_usart3_tx_head = 0U;
static volatile uint16_t s_usart3_tx_tail = 0U;
static volatile bool s_usart3_dma_busy    = false;

static uint16_t USART3_BufferUsed(void) {
    if (s_usart3_tx_head >= s_usart3_tx_tail) {
        return (uint16_t)(s_usart3_tx_head - s_usart3_tx_tail);
    }

    return (uint16_t)(USART3_TX_BUFFER_SIZE - (s_usart3_tx_tail - s_usart3_tx_head));
}

bool USART3_Enqueue(const uint8_t *data, uint16_t len) {
    uint32_t primask;
    uint16_t used;
    uint16_t free_space;
    uint16_t head;
    uint16_t first_chunk;

    if ((data == NULL) || (len == 0U)) {
        return false;
    }

    primask = __get_PRIMASK();
    __disable_irq();

    used       = USART3_BufferUsed();
    free_space = (uint16_t)(USART3_TX_BUFFER_SIZE - used - 1U);

    /* 环形缓冲区空间不足 */
    if (free_space < len) {
        __set_PRIMASK(primask);
        return false;
    }

    head = s_usart3_tx_head;

    /* 先拷贝到缓冲区尾部 */
    first_chunk = (uint16_t)(USART3_TX_BUFFER_SIZE - head);
    if (first_chunk > len) {
        first_chunk = len;
    }

    memcpy(&s_usart3_tx_buffer[head], data, first_chunk);

    /* 若发生回卷，则从缓冲区头部继续拷贝 */
    if (len > first_chunk) {
        memcpy(&s_usart3_tx_buffer[0], &data[first_chunk], (uint16_t)(len - first_chunk));
    }

    /* 更新 head */
    head = (uint16_t)(head + len);
    if (head >= USART3_TX_BUFFER_SIZE) {
        head = (uint16_t)(head - USART3_TX_BUFFER_SIZE);
    }
    s_usart3_tx_head = head;

    __set_PRIMASK(primask);
    return true;
}

void USART3_StartTrans(void) {
    uint32_t primask;
    uint16_t head;
    uint16_t tail;
    uint16_t size;

    primask = __get_PRIMASK();
    __disable_irq();

    /* DMA 正忙或队列为空，直接退出 */
    if (s_usart3_dma_busy || (s_usart3_tx_head == s_usart3_tx_tail)) {
        __set_PRIMASK(primask);
        return;
    }

    head = s_usart3_tx_head;
    tail = s_usart3_tx_tail;

    /* 仅发送从 tail 开始的一段连续区域 */
    if (head > tail) {
        size = (uint16_t)(head - tail);
    } else {
        size = (uint16_t)(USART3_TX_BUFFER_SIZE - tail);
    }

    s_usart3_dma_busy = true;

    __set_PRIMASK(primask);

    if (HAL_UART_Transmit_DMA(&huart3, &s_usart3_tx_buffer[tail], size) != HAL_OK) {
        primask = __get_PRIMASK();
        __disable_irq();
        s_usart3_dma_busy = false;
        __set_PRIMASK(primask);
    }
}

void USART3_TxCpltCallback(UART_HandleTypeDef *huart) {
    uint32_t primask;
    uint16_t tail;

    primask = __get_PRIMASK();
    __disable_irq();

    tail = s_usart3_tx_tail;
    tail = (uint16_t)(tail + (uint16_t)huart->TxXferSize);
    if (tail >= USART3_TX_BUFFER_SIZE) {
        tail = (uint16_t)(tail - USART3_TX_BUFFER_SIZE);
    }

    s_usart3_tx_tail  = tail;
    s_usart3_dma_busy = false;

    __set_PRIMASK(primask);

    USART3_StartTrans();
}