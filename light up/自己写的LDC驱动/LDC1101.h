// ldc1101.h
#ifndef __LDC1101_H
#define __LDC1101_H

#include "stm32f1xx_hal.h"

/**************************************LDC1101寄存器地址宏定义**************************************/
#define LDC1101_REG_RP_SET             0x01
#define LDC1101_REG_TC1                0x02
#define LDC1101_REG_TC2                0x03
#define LDC1101_REG_DIG_CONFIG         0x04
#define LDC1101_REG_ALT_CONFIG         0x05
#define LDC1101_REG_INTB_MODE          0x0A
#define LDC1101_REG_START_CONFIG       0x0B
#define LDC1101_REG_D_CONF             0x0C
#define LDC1101_REG_L_THRESH_HI_LSB    0x16
#define LDC1101_REG_L_THRESH_HI_MSB    0x17
#define LDC1101_REG_L_THRESH_LO_LSB    0x18
#define LDC1101_REG_L_THRESH_LO_MSB    0x19
#define LDC1101_REG_STATUS             0x20
#define LDC1101_REG_RP_DATA_LSB        0x21
#define LDC1101_REG_RP_DATA_MSB        0x22
#define LDC1101_REG_L_DATA_LSB         0x23
#define LDC1101_REG_L_DATA_MSB         0x24
#define LDC1101_REG_LHR_RCOUNT_LSB     0x30
#define LDC1101_REG_LHR_RCOUNT_MSB     0x31
#define LDC1101_REG_LHR_OFFSET_LSB     0x32
#define LDC1101_REG_LHR_OFFSET_MSB     0x33
#define LDC1101_REG_LHR_CONFIG         0x34
#define LDC1101_REG_LHR_DATA_LSB       0x38
#define LDC1101_REG_LHR_DATA_MID       0x39
#define LDC1101_REG_LHR_DATA_MSB       0x3A
#define LDC1101_REG_LHR_STATUS         0x3B
/**************************************************************************************************/


/*******************************************结果读取*******************************************/
extern uint8_t LDC1101_RX_result;
extern uint8_t LDC1101_RX_bufferfer[3];
/*************************************************************************************************/

/*******************************************操作函数*******************************************/
HAL_StatusTypeDef LDC1101_WriteReg(SPI_HandleTypeDef *hspi, 
																	 GPIO_TypeDef *CS_GPIO, 
																	 uint16_t CS_Pin, 
																	 uint8_t reg, 
																	 uint8_t data);
HAL_StatusTypeDef LDC1101_ReadReg_DMA(SPI_HandleTypeDef *hspi, 
																			GPIO_TypeDef *CS_GPIO, 
																			uint16_t CS_Pin, 
																			uint8_t reg);
void LDC1101_Init(SPI_HandleTypeDef *hspi, 
									GPIO_TypeDef *CS_GPIO, 
									uint16_t CS_Pin);
void LDC1101_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
/*************************************************************************************************/

#endif /* __LDC1101_H */


