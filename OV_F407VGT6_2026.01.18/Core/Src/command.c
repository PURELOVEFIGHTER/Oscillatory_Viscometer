#include "command.h"
#include "ldc1101_driver.h"
#include "tim.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern UART_HandleTypeDef huart2;
extern char UART2_RX_DMA_buffer[2][MSG_LEN];
extern char UART2_TX_buffer[MSG_LEN];

extern float drv_PWM_freq;
extern float drv_PWM_DR;

extern LDC1101_HandleTypeDef ldc2;
extern bool isLHR;
extern uint8_t LDC_status;

void Command_Parse(void) {
    /* 设置占空比 */
    if (strncmp(UART2_RX_DMA_buffer[UART2_RX_activeBuffer], "DR:", 3) == 0) {
        drv_PWM_DR        = atoi(&UART2_RX_DMA_buffer[UART2_RX_activeBuffer][3]);
        drv_PWM_isChanged = true;
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
        sprintf(UART2_TX_buffer, "PWM_DR set to %.2f%%\r\n", drv_PWM_DR);
    }
    /* 设置频率 */
    else if (strncmp(UART2_RX_DMA_buffer[UART2_RX_activeBuffer], "FR:", 3) == 0) {
        drv_PWM_freq      = atoi(&UART2_RX_DMA_buffer[UART2_RX_activeBuffer][3]);
        drv_PWM_isChanged = true;
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
        sprintf(UART2_TX_buffer, "PWM_FREQ set to %.2fHz\r\n", drv_PWM_freq);
    }
    /* 启动频率扫描 */
    else if (strncmp(UART2_RX_DMA_buffer[UART2_RX_activeBuffer], "FR Scan", 7) == 0) {
        drv_PWM_freq      = FREQ_SCAN_START_HZ;
        drv_PWM_isChanged = true;
        freq_scan_enabled = true;
        HAL_TIM_Base_Start_IT(&htim4);
        ldc2_isReading = true;

        sprintf(UART2_TX_buffer, "Frequency scan start.\r\n");
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
    }
    /* 启动占空比扫描 */
    else if (strncmp(UART2_RX_DMA_buffer[UART2_RX_activeBuffer], "DR Scan", 7) == 0) {
        drv_PWM_DR        = DUTY_RATIO_SCAN_START;
        drv_PWM_isChanged = true;
        DR_scan_enabled   = true;
        HAL_TIM_Base_Start_IT(&htim4);
        ldc2_isReading = true;

        sprintf(UART2_TX_buffer, "Duty Ratio scan start.\r\n");
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
    }
    /* 读取LDC寄存器 */
    else if (strncmp(UART2_RX_DMA_buffer[UART2_RX_activeBuffer], "Read:", 5) == 0) {
        if (ldc2_isReading == false) {
            uint8_t reg_addr;
            sscanf(&UART2_RX_DMA_buffer[UART2_RX_activeBuffer][5], "%hhx", &reg_addr);
            uint8_t reg_val = ldc1101_readByte(&ldc2, reg_addr);
            sprintf(UART2_TX_buffer, "[0x%02X] = 0x%02X\r\n", reg_addr, reg_val);
        } else {
            sprintf(UART2_TX_buffer, "Cannot read register while reading LHR Data.\r\n");
        }
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
    }
    /* LDC状态检查 */
    else if ((strncmp(UART2_RX_DMA_buffer[UART2_RX_activeBuffer], "RPL Check", 9) == 0 && !isLHR)
             || (strncmp(UART2_RX_DMA_buffer[UART2_RX_activeBuffer], "LHR Check", 9) == 0 && isLHR)) {
        if (ldc2_isReading == false) {
            uint8_t status = isLHR ? ldc1101_readByte(&ldc2, _LDC1101_REG_LHR_STATUS)
                                   : ldc1101_readByte(&ldc2, _LDC1101_REG_RP_L_MEASUREMENT_STATUS);
            sprintf(UART2_TX_buffer, "%s Measurement Status: %d%d%d%d%d%d%d%d\r\n", isLHR ? "LHR" : "RP+L",
                    (status >> 7) & 1, (status >> 6) & 1, (status >> 5) & 1, (status >> 4) & 1, (status >> 3) & 1,
                    (status >> 2) & 1, (status >> 1) & 1, (status >> 0) & 1);
        } else {
            sprintf(UART2_TX_buffer, "Cannot check LDC status while reading LHR Data.\r\n");
        }
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
    } else {
        sprintf(UART2_TX_buffer, "Unknown command\r\n");
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
    }
}
