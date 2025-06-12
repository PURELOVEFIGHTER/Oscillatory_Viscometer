#include "ldc1101.h"

static uint8_t tx_buf[2];
static uint8_t rx_buf[2];
uint8_t LDC1101_RX_result = 0;

// 写寄存器（阻塞方式）
HAL_StatusTypeDef LDC1101_WriteReg(SPI_HandleTypeDef *hspi, 
																	 GPIO_TypeDef *CS_GPIO, 
																	 uint16_t CS_Pin, 
																	 uint8_t reg, 
																	 uint8_t data)
{
    HAL_GPIO_WritePin(CS_GPIO, CS_Pin, GPIO_PIN_RESET);
    tx_buf[0] = reg & 0x7F;
    tx_buf[1] = data;
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi, tx_buf, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_GPIO, CS_Pin, GPIO_PIN_SET);
    return status;
}

// 读寄存器（DMA方式）
HAL_StatusTypeDef LDC1101_ReadReg_DMA(SPI_HandleTypeDef *hspi, 
																			GPIO_TypeDef *CS_GPIO, 
																			uint16_t CS_Pin, 
																			uint8_t reg)
{
    HAL_GPIO_WritePin(CS_GPIO, CS_Pin, GPIO_PIN_RESET);
    tx_buf[0] = 0x80 | reg;
    tx_buf[1] = 0x00;
    return HAL_SPI_TransmitReceive_DMA(hspi, tx_buf, rx_buf, 2);
}

// LDC1101回调函数
void LDC1101_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    LDC1101_RX_result = rx_buf[1];
}

// 初始化函数
void LDC1101_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS_GPIO, uint16_t CS_Pin)
{
    LDC1101_WriteReg(hspi, CS_GPIO, CS_Pin, LDC1101_REG_RP_SET, 0x07);
    LDC1101_WriteReg(hspi, CS_GPIO, CS_Pin, LDC1101_REG_TC1, 0x90);
    LDC1101_WriteReg(hspi, CS_GPIO, CS_Pin, LDC1101_REG_TC2, 0xA0);
    LDC1101_WriteReg(hspi, CS_GPIO, CS_Pin, LDC1101_REG_DIG_CONFIG, 0x03);
    LDC1101_WriteReg(hspi, CS_GPIO, CS_Pin, LDC1101_REG_ALT_CONFIG, 0x00);
    LDC1101_WriteReg(hspi, CS_GPIO, CS_Pin, LDC1101_REG_INTB_MODE, 0x00);
    LDC1101_WriteReg(hspi, CS_GPIO, CS_Pin, LDC1101_REG_START_CONFIG, 0x01);
    LDC1101_WriteReg(hspi, CS_GPIO, CS_Pin, LDC1101_REG_D_CONF, 0x00);
}
