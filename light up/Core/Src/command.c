#include "command.h"
#include "ldc1101_driver.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// ================= 外部变量 ================= //
extern UART_HandleTypeDef huart1;
extern char RX_buffer[MSG_LEN];
extern char TX_buffer[MSG_LEN];

extern uint16_t drv_PWM_freq;
extern uint16_t drv_PWM_DR;
extern bool isLHR;
extern uint8_t LDC_status[2];

// LDC1101 驱动器外部对象
extern LDC1101_HandleTypeDef ldc1;
extern LDC1101_HandleTypeDef ldc2;

// LDC1101 读寄存器函数（在驱动里实现）
extern uint8_t ldc1101_readByte(LDC1101_HandleTypeDef *dev, uint8_t addr);

/**
 * ************************************************************************
 * @brief 指令解析
 * @details
 *
 * ************************************************************************
 */
void Command_Parse(void) {
    // 简单协议,例:输入 "DR:80" 设置占空比为 80，"FR:1000" 设置频率为 1000Hz
    if (strncmp(RX_buffer, "DR:", 3) == 0) {
        drv_PWM_DR = atoi(&RX_buffer[3]); // 提取并转换占空比

        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        sprintf(TX_buffer, "PWM_DR set to %d\r\n", drv_PWM_DR);
        HAL_UART_Transmit(&huart1, (uint8_t *)TX_buffer, strlen(TX_buffer), HAL_MAX_DELAY);
    } else if (strncmp(RX_buffer, "FR:", 3) == 0) {
        drv_PWM_freq = atoi(&RX_buffer[3]); // 提取并转换频率

        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        sprintf(TX_buffer, "PWM_FREQ set to %dHz\r\n", drv_PWM_freq);
        HAL_UART_Transmit(&huart1, (uint8_t *)TX_buffer, strlen(TX_buffer), HAL_MAX_DELAY);
    } else if (strncmp(RX_buffer, "FR Scan", 7) == 0) {
        sprintf(TX_buffer, "Frequency Scan begin.\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t *)TX_buffer, strlen(TX_buffer), HAL_MAX_DELAY);

        for (drv_PWM_freq = FREQ_MIN; drv_PWM_freq <= FREQ_MAX; drv_PWM_freq += FREQ_STEP) {
            HAL_Delay(5000); // 等待系统稳定（振动建立）
            sprintf(TX_buffer, "PWM_FREQ:%dHz\r\n", drv_PWM_freq);
            HAL_UART_Transmit(&huart1, (uint8_t *)TX_buffer, strlen(TX_buffer), HAL_MAX_DELAY);
        }

        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    } else if (strncmp(RX_buffer, "DR Scan", 7) == 0) {
        //				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        //				sprintf(TX_BUFFER, "Please set PWM_DR:");
        //				HAL_UART_Transmit(&huart1, (uint8_t*)TX_BUFFER,
        // strlen(TX_buffer), HAL_MAX_DELAY); 				drv_PWM_FREQ =
        // atoi(&RX_buffer[3]);  // 提取并转换频率 				DR_Scan();
    }
    // 检查SPI通讯状态
    else if (strncmp(RX_buffer, "SPI Check", 9) == 0) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        uint8_t chip_id = ldc1101_readByte(&ldc1, _LDC1101_REG_CHIP_ID);

        sprintf(TX_buffer, "LDC1101 Device ID: 0x%02X\r\n", chip_id);
        HAL_UART_Transmit(&huart1, (uint8_t *)TX_buffer, strlen(TX_buffer), HAL_MAX_DELAY);
    }
    // 读取寄存器数值
    else if (strncmp(RX_buffer, "Read:", 5) == 0) {
        uint8_t reg_addr;
        uint8_t reg_val;

        // 直接解析寄存器地址，默认格式为 0xXX
        sscanf(&RX_buffer[5], "%hhx", &reg_addr);

        reg_val = ldc1101_readByte(&ldc1, reg_addr);

        sprintf(TX_buffer, "[0x%02X] = 0x%02X\r\n", reg_addr, reg_val);
        HAL_UART_Transmit(&huart1, (uint8_t *)TX_buffer, strlen(TX_buffer), HAL_MAX_DELAY);
    }
    // 检查 RP+L 模式工作状态
    else if ((strncmp(RX_buffer, "RPL Check", 9) == 0) && (isLHR == 0)) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        LDC_status[0] = ldc1101_readByte(&ldc2, _LDC1101_REG_RP_L_MEASUREMENT_STATUS);

        // 将 rpl_status 转成二进制字符串
        sprintf(TX_buffer,
                "RP+L Measurement Status: "
                "%d%d%d%d%d%d%d%d\r\n",
                (LDC_status[0] >> 7) & 0x01, (LDC_status[0] >> 6) & 0x01, (LDC_status[0] >> 5) & 0x01,
                (LDC_status[0] >> 4) & 0x01, (LDC_status[0] >> 3) & 0x01, (LDC_status[0] >> 2) & 0x01,
                (LDC_status[0] >> 1) & 0x01, (LDC_status[0] >> 0) & 0x01);

        HAL_UART_Transmit(&huart1, (uint8_t *)TX_buffer, strlen(TX_buffer), HAL_MAX_DELAY);
    } else if ((strncmp(RX_buffer, "LHR Check", 9) == 0) && (isLHR == 1)) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        LDC_status[1] = ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_STATUS);

        // 将 rpl_status 转成二进制字符串
        sprintf(TX_buffer,
                "LHR Measurement Status: "
                "%d%d%d%d%d%d%d%d\r\n",
                (LDC_status[1] >> 7) & 0x01, (LDC_status[1] >> 6) & 0x01, (LDC_status[1] >> 5) & 0x01,
                (LDC_status[1] >> 4) & 0x01, (LDC_status[1] >> 3) & 0x01, (LDC_status[1] >> 2) & 0x01,
                (LDC_status[1] >> 1) & 0x01, (LDC_status[1] >> 0) & 0x01);

        HAL_UART_Transmit(&huart1, (uint8_t *)TX_buffer, strlen(TX_buffer), HAL_MAX_DELAY);
    }

    memset(RX_buffer, 0, sizeof(RX_buffer));
}
