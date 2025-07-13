#include "__ldc1101_driver.h"

/* ------------------------------------------------------------------- MACROS */

LDC1101_Device ldc1 = { &hspi1, GPIOA, GPIO_PIN_4 };
LDC1101_Device ldc2 = { &hspi2, GPIOB, GPIO_PIN_12 };

/* Register */
const uint8_t _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE = 0x01;
const uint8_t _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_1     = 0x02;
const uint8_t _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_2     = 0x03;
const uint8_t _LDC1101_REG_CFG_RP_L_CONVERSION_INTERVAL     = 0x04;
const uint8_t _LDC1101_REG_CFG_ADDITIONAL_DEVICE            = 0x05;
const uint8_t _LDC1101_REG_RP_THRESH_H_LSB                  = 0x06;
const uint8_t _LDC1101_REG_RP_THRESH_H_MSB                  = 0x07;
const uint8_t _LDC1101_REG_RP_THRESH_L_LSB                  = 0x08;
const uint8_t _LDC1101_REG_RP_THRESH_L_MSB                  = 0x09;
const uint8_t _LDC1101_REG_CFG_INTB_MODE                    = 0x0A;
const uint8_t _LDC1101_REG_CFG_POWER_STATE                  = 0x0B;
const uint8_t _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT    = 0x0C;
const uint8_t _LDC1101_REG_L_THRESH_HI_LSB                  = 0x16;
const uint8_t _LDC1101_REG_L_THRESH_HI_MSB                  = 0x17;
const uint8_t _LDC1101_REG_L_THRESH_LO_LSB                  = 0x18;
const uint8_t _LDC1101_REG_L_THRESH_LO_MSB                  = 0x19;
const uint8_t _LDC1101_REG_RP_L_MEASUREMENT_STATUS          = 0x20;
const uint8_t _LDC1101_REG_RP_DATA_LSB                      = 0x21;
const uint8_t _LDC1101_REG_RP_DATA_MSB                      = 0x22;
const uint8_t _LDC1101_REG_L_DATA_LSB                       = 0x23;
const uint8_t _LDC1101_REG_L_DATA_MSB                       = 0x24;
const uint8_t _LDC1101_REG_LHR_RCOUNT_LSB                   = 0x30;
const uint8_t _LDC1101_REG_LHR_RCOUNT_MSB                   = 0x31;
const uint8_t _LDC1101_REG_LHR_OFFSET_LSB                   = 0x32;
const uint8_t _LDC1101_REG_LHR_OFFSET_MSB                   = 0x33;
const uint8_t _LDC1101_REG_CFG_LHR                          = 0x34;
const uint8_t _LDC1101_REG_LHR_DATA_LSB                     = 0x38;
const uint8_t _LDC1101_REG_LHR_DATA_MID                     = 0x39;
const uint8_t _LDC1101_REG_LHR_DATA_MSB                     = 0x3A;
const uint8_t _LDC1101_REG_LHR_STATUS                       = 0x3B;
const uint8_t _LDC1101_REG_DEVICE_RID_VALUE                 = 0x3E;
const uint8_t _LDC1101_REG_CHIP_ID                          = 0x3F;

/* Register RP_SET Field Descriptions (RW) */
const uint8_t _LDC1101_RP_SET_RP_MAX_IS_DRIVEN              = 0x00;
const uint8_t _LDC1101_RP_SET_RP_MAX_CURRENT_IS_IGNORED     = 0x80;
const uint8_t _LDC1101_RP_SET_RP_MAX_96KOhm                 = 0x00;
const uint8_t _LDC1101_RP_SET_RP_MAX_48KOhm                 = 0x10;
const uint8_t _LDC1101_RP_SET_RP_MAX_24KOhm                 = 0x20;
const uint8_t _LDC1101_RP_SET_RP_MAX_12KOhm                 = 0x30;
const uint8_t _LDC1101_RP_SET_RP_MAX_6KOhm                  = 0x40;
const uint8_t _LDC1101_RP_SET_RP_MAX_3KOhm                  = 0x50;
const uint8_t _LDC1101_RP_SET_RP_MAX_1_5KOhm                = 0x60;
const uint8_t _LDC1101_RP_SET_RP_MAX_0_75KOh                = 0x70;

const uint8_t _LDC1101_RP_SET_RP_MIN_96KOhm                 = 0x00;
const uint8_t _LDC1101_RP_SET_RP_MIN_48KOhm                 = 0x01;
const uint8_t _LDC1101_RP_SET_RP_MIN_24KOhm                 = 0x02;
const uint8_t _LDC1101_RP_SET_RP_MIN_12KOhm                 = 0x03;
const uint8_t _LDC1101_RP_SET_RP_MIN_6KOhm                  = 0x04;
const uint8_t _LDC1101_RP_SET_RP_MIN_3KOhm                  = 0x05;
const uint8_t _LDC1101_RP_SET_RP_MIN_1_5KOhm                = 0x06;
const uint8_t _LDC1101_RP_SET_RP_MIN_0_75KOh                = 0x07;

/* Configure Internal Time Constant 1 (RW) */
const uint8_t _LDC1101_TC1_C1_0_75pF     = 0x00;
const uint8_t _LDC1101_TC1_C1_1_5pF      = 0x40;
const uint8_t _LDC1101_TC1_C1_3pF        = 0x80;
const uint8_t _LDC1101_TC1_C1_6pF        = 0xC0;
const uint8_t _LDC1101_TC1_R1_417kOhm    = 0x00;
const uint8_t _LDC1101_TC1_R1_212_7kOhm  = 0x10;
const uint8_t _LDC1101_TC1_R1_21_1kOhm   = 0x1F;

/* Configure Internal Time Constant 2 (RW) */
const uint8_t _LDC1101_TC2_C2_3pF        = 0x00;
const uint8_t _LDC1101_TC2_C2_6pF        = 0x40;
const uint8_t _LDC1101_TC2_C2_12pF       = 0x80;
const uint8_t _LDC1101_TC2_C2_24pF       = 0xC0;
const uint8_t _LDC1101_TC2_R2_835kOhm    = 0x00;
const uint8_t _LDC1101_TC2_R2_426_4kOhm  = 0x20;
const uint8_t _LDC1101_TC2_R2_30_5kOhm   = 0x3F;

/* Configure RP+L conversion interval (RW) */
const uint8_t _LDC1101_DIG_CFG_MIN_FREQ_500kHz   = 0x00;
const uint8_t _LDC1101_DIG_CFG_MIN_FREQ_8MHz     = 0xF0;

const uint8_t _LDC1101_DIG_CFG_RESP_TIME_192s    = 0x02;
const uint8_t _LDC1101_DIG_CFG_RESP_TIME_384s    = 0x03;
const uint8_t _LDC1101_DIG_CFG_RESP_TIME_768s    = 0x04;
const uint8_t _LDC1101_DIG_CFG_RESP_TIME_1536s   = 0x05;
const uint8_t _LDC1101_DIG_CFG_RESP_TIME_3072s   = 0x06;
const uint8_t _LDC1101_DIG_CFG_RESP_TIME_6144s   = 0x07;

/* Configure additional device settings (RW) */
const uint8_t _LDC1101_ALT_CFG_SHUTDOWN_ENABLE     = 0x02;
const uint8_t _LDC1101_ALT_CFG_SHUTDOWN_DISABLE    = 0x00;
const uint8_t _LDC1101_ALT_CFG_L_OPTIMAL_DISABLED  = 0x00;
const uint8_t _LDC1101_ALT_CFG_L_OPTIMAL_ENABLE    = 0x01;

/* Configure INTB reporting on SDO pin (RW) */
const uint8_t _LDC1101_INTB_MODE_DONT_REPORT_INTB_ON_SDO_PIN         = 0x00;
const uint8_t _LDC1101_INTB_MODE_REPORT_INTB_ON_SDO_PIN              = 0x80;
const uint8_t _LDC1101_INTB_MODE_REPORT_LHR_DATA_READY               = 0x20;
const uint8_t _LDC1101_INTB_MODE_L_CONVERSION_TO_L_THRESHOLDS        = 0x10;
const uint8_t _LDC1101_INTB_MODE_L_CONVERSION_TO_L_HIGH_THRESHOLDS   = 0x08;
const uint8_t _LDC1101_INTB_MODE_REPORT_RP_L_DATA_READY              = 0x04;
const uint8_t _LDC1101_INTB_MODE_RP_CONVERSION_TO_L_THRESHOLDS       = 0x02;
const uint8_t _LDC1101_INTB_MODE_RP_CONVERSION_TO_L_HIGH_THRESHOLDS  = 0x01;
const uint8_t _LDC1101_INTB_MODE_NO_OUTPUT                           = 0x00;

/* Configure Power State (RW) */
const uint8_t _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE  = 0x00;
const uint8_t _LDC1101_FUNC_MODE_SLEEP_MODE              = 0x01;
const uint8_t _LDC1101_FUNC_MODE_SHUTDOWN_MODE           = 0x02;

/* High Resolution L Configuration (RW) */
const uint8_t _LDC1101_LHR_CFG_FREQUENCY_NOT_DIVIDED    = 0x00;
const uint8_t _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_2   = 0x01;
const uint8_t _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_4   = 0x02;
const uint8_t _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_8   = 0x03;


/* ---------------------------------------------------------------- VARIABLES */


/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */



/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */



/* --------------------------------------------------------- PUBLIC FUNCTIONS */



/* ----------------------------------------------------------- IMPLEMENTATION */
// SPI 写寄存器
void ldc1101_writeByte(LDC1101_Device *dev, uint8_t addr, uint8_t _data)
{
    uint8_t writeReg[2] = { addr & 0x7F, _data };

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, writeReg, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

// SPI 读寄存器
uint8_t ldc1101_readByte(LDC1101_Device *dev, uint8_t addr)
{
    uint8_t tx_data[2];
    uint8_t rx_data[2];

    tx_data[0] = 0x80 | addr;
    tx_data[1] = 0x00;

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(dev->hspi, tx_data, rx_data, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

    return rx_data[1];  // 第二个字节是寄存器值
}





uint8_t ldc1101_init(LDC1101_Device *dev)
{
    // 设置为 SLEEP 模式，开始初始化
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_POWER_STATE, _LDC1101_FUNC_MODE_SLEEP_MODE);// 0x01
	
	  // 先读取 CHIP ID，确认 SPI 和芯片正常
    uint8_t chip_id = ldc1101_readByte(dev, _LDC1101_REG_CHIP_ID);
    if (chip_id != 0xD4) {
        return DEVICE_ERROR;
    }

    // 设置 RP 测量动态范围（初次建议中等范围，后续可细调）
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE,
                      _LDC1101_RP_SET_RP_MIN_3KOhm | _LDC1101_RP_SET_RP_MAX_24KOhm);// 0x25

    // 配置内部时间常数（初次用中间值）
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_1, 
                      _LDC1101_TC1_C1_3pF | _LDC1101_TC1_R1_212_7kOhm);// 0x90
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_2, 
                      _LDC1101_TC2_C2_6pF | _LDC1101_TC2_R2_426_4kOhm);// 0x60

    // 配置 RP+L 转换时间（中等响应速度）
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_RP_L_CONVERSION_INTERVAL,
                      _LDC1101_DIG_CFG_MIN_FREQ_500kHz | _LDC1101_DIG_CFG_RESP_TIME_1536s);// 0x05

    // 关闭其他辅助功能
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 
										  _LDC1101_ALT_CFG_L_OPTIMAL_DISABLED | _LDC1101_ALT_CFG_SHUTDOWN_DISABLE);// 0x00
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_INTB_MODE, _LDC1101_INTB_MODE_NO_OUTPUT);// 0x00

    // 关闭门限功能（初次调试不使用）
    ldc1101_writeByte(dev, _LDC1101_REG_RP_THRESH_H_MSB, 0x00);
    ldc1101_writeByte(dev, _LDC1101_REG_RP_THRESH_H_LSB, 0x00);
    ldc1101_writeByte(dev, _LDC1101_REG_RP_THRESH_L_MSB, 0x00);
    ldc1101_writeByte(dev, _LDC1101_REG_RP_THRESH_L_LSB, 0x00);
    ldc1101_writeByte(dev, _LDC1101_REG_L_THRESH_HI_MSB, 0x00);
    ldc1101_writeByte(dev, _LDC1101_REG_L_THRESH_HI_LSB, 0x00);
    ldc1101_writeByte(dev, _LDC1101_REG_L_THRESH_LO_MSB, 0x00);
    ldc1101_writeByte(dev, _LDC1101_REG_L_THRESH_LO_LSB, 0x00);

    // 关闭振幅控制
    ldc1101_writeByte(dev, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x00);

    // 切换到 ACTIVE CONVERSION 模式开始测量
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_POWER_STATE, _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE);

    // 等待测量稳定
    HAL_Delay(100);

    return DEVICE_OK;
}




// 切换到 L 模式
void ldc1101_goTo_Lmode(LDC1101_Device *dev)
{
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 0x01);
    ldc1101_writeByte(dev, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x01);
}

// 切换到 RP 模式
void ldc1101_goTo_RPmode(LDC1101_Device *dev)
{
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 0x02);
    ldc1101_writeByte(dev, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x00);
}

// 读取状态寄存器
uint8_t ldc1101_getStatus(LDC1101_Device *dev)
{
    return ldc1101_readByte(dev, 0x20);
}

// 获取 RP 数据
uint16_t ldc1101_getRPData(LDC1101_Device *dev)
{
    uint16_t data;
    data = ldc1101_readByte(dev, _LDC1101_REG_RP_DATA_MSB);
    data = (data << 8) | ldc1101_readByte(dev, _LDC1101_REG_RP_DATA_LSB);
    return data;
}

// 获取 L 数据
uint16_t ldc1101_getLData(LDC1101_Device *dev)
{
    uint16_t data;
    data = ldc1101_readByte(dev, _LDC1101_REG_L_DATA_MSB);
    data = (data << 8) | ldc1101_readByte(dev, _LDC1101_REG_L_DATA_LSB);
    return data;
}

// 获取 LHR 数据
uint32_t ldc1101_getLHRData(LDC1101_Device *dev)
{
    uint32_t data = 0;
    data |= ((uint32_t)ldc1101_readByte(dev, _LDC1101_REG_LHR_DATA_MSB) << 16);
    data |= ((uint32_t)ldc1101_readByte(dev, _LDC1101_REG_LHR_DATA_MID) << 8);
    data |= ((uint32_t)ldc1101_readByte(dev, _LDC1101_REG_LHR_DATA_LSB));

    return data;  // 返回24位数据，低24位有效
}

/* -------------------------------------------------------------------------- */
/*
  __ldc1101_driver.c

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */
