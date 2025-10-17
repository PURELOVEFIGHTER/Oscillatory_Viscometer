#include "command.h"
#include "ldc1101_driver.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// ================= 外部变量 ================= //
extern UART_HandleTypeDef huart1;
extern char UART1_RX_buffer[MSG_LEN];
extern char UART1_TX_buffer[2][MSG_LEN];

extern uint16_t drv_PWM_freq;
extern uint16_t drv_PWM_DR;
extern bool isLHR;
extern uint8_t LDC_status;

// LDC1101 驱动器外部对象
extern LDC1101_HandleTypeDef ldc2;

/**
 * ************************************************************************
 * @brief 指令解析
 * @details
 *
 * ************************************************************************
 */
void Command_Parse(void) {
    // 获取实际接收长度
    uint16_t recv_len = MSG_LEN - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
    if (recv_len == 0)
        return;

    char *txbuf = UART1_TX_buffer[UART1_TX_activeBuffer]; // 当前活跃发送缓冲
    memset(txbuf, 0, MSG_LEN);

    // 功能码解析
    if (strncmp(UART1_RX_buffer, "DR:", 3) == 0) // 设置占空比
    {
        drv_PWM_DR = atoi(&UART1_RX_buffer[3]);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        sprintf(txbuf, "PWM_DR set to %d\r\n", drv_PWM_DR);
    } else if (strncmp(UART1_RX_buffer, "FR:", 3) == 0) // 设置频率
    {
        drv_PWM_freq = atoi(&UART1_RX_buffer[3]);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        sprintf(txbuf, "PWM_FREQ set to %dHz\r\n", drv_PWM_freq);
    } else if (strncmp(UART1_RX_buffer, "SPI Check", 9) == 0) // SPI检查
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        uint8_t chip_id = ldc1101_readByte(&ldc2, _LDC1101_REG_CHIP_ID);
        sprintf(txbuf, "LDC1101 Device ID: 0x%02X\r\n", chip_id);
    } else if (strncmp(UART1_RX_buffer, "Read:", 5) == 0) // 读取寄存器
    {
        uint8_t reg_addr;
        sscanf(&UART1_RX_buffer[5], "%hhx", &reg_addr);
        uint8_t reg_val = ldc1101_readByte(&ldc2, reg_addr);
        sprintf(txbuf, "[0x%02X] = 0x%02X\r\n", reg_addr, reg_val);
    } else if ((strncmp(UART1_RX_buffer, "RPL Check", 9) == 0 && !isLHR)
               || (strncmp(UART1_RX_buffer, "LHR Check", 9) == 0 && isLHR)) // 状态检查
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        uint8_t status = isLHR ? ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_STATUS)
                               : ldc1101_readByte(&ldc2, _LDC1101_REG_RP_L_MEASUREMENT_STATUS);
        sprintf(txbuf, "%s Measurement Status: %d%d%d%d%d%d%d%d\r\n", isLHR ? "LHR" : "RP+L", (status >> 7) & 1,
                (status >> 6) & 1, (status >> 5) & 1, (status >> 4) & 1, (status >> 3) & 1, (status >> 2) & 1,
                (status >> 1) & 1, (status >> 0) & 1);
    } else {
        sprintf(txbuf, "Unknown command\r\n");
    }

    // DMA发送管理
    if (!UART1_TX_isBusy) {
        UART1_TX_isBusy = true;
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)txbuf, strlen(txbuf));
        UART1_TX_activeBuffer = 1 - UART1_TX_activeBuffer; // 切换发送缓冲
    } else {
        UART1_TX_isPending = true; // 等待发送完成后再发送
    }

    // 清接收缓冲
    memset(UART1_RX_buffer, 0, MSG_LEN);

    // 重启DMA接收
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)UART1_RX_buffer, MSG_LEN);
}
