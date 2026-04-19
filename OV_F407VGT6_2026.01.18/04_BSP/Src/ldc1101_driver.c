#include "ldc1101_driver.h"
extern SPI_HandleTypeDef hspi2;

// SPI 写寄存器
void ldc1101_writeByte(LDC_HandleTypeDef *hldc, uint8_t addr, uint8_t _data) {
    uint8_t writeReg[2] = {addr & 0x7F, _data};

    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hldc->hspi, writeReg, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_SET);
}

// SPI 读寄存器
uint8_t ldc1101_readByte(LDC_HandleTypeDef *hldc, uint8_t addr) {
    uint8_t tx_data[2];
    uint8_t rx_data[2];

    tx_data[0] = 0x80 | addr;
    tx_data[1] = 0x00;

    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(hldc->hspi, tx_data, rx_data, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_SET);

    return rx_data[1]; // 第二个字节是寄存器值
}

DEVICE_StatusTypeDef ldc1101_setPowerMode(LDC_HandleTypeDef *hldc, uint8_t mode) {
    switch (mode) {
        case _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE:
            ldc1101_writeByte(hldc, _LDC1101_REG_CFG_POWER_STATE, mode);
            hldc->power_state = LDC_ACTIVE;
            break;
        case _LDC1101_FUNC_MODE_SLEEP_MODE:
            ldc1101_writeByte(hldc, _LDC1101_REG_CFG_POWER_STATE, mode);
            hldc->power_state = LDC_SLEEP;
            break;
        case _LDC1101_FUNC_MODE_SHUTDOWN_MODE:
            ldc1101_writeByte(hldc, _LDC1101_REG_CFG_POWER_STATE, mode);
            hldc->power_state = LDC_SHUTDOWN;
            break;
        default:
            return DEVICE_ERROR; // 无效模式，直接返回
    }
    return DEVICE_OK;
}

DEVICE_StatusTypeDef ldc1101_init(LDC_HandleTypeDef *hldc) {
    if (hldc == NULL) {
        return DEVICE_ERROR;
    }
    hldc->hspi        = &hspi2;
    hldc->cs_port     = GPIOB;
    hldc->cs_pin      = GPIO_PIN_12;
    hldc->power_state = LDC_SLEEP;
    hldc->mode        = LDC_DEFAULT;
    uint8_t chip_id   = ldc1101_readByte(hldc, _LDC1101_REG_CHIP_ID);
    if (chip_id != 0xD4) {
        return DEVICE_ERROR;
    }
    return DEVICE_OK;
}

DEVICE_StatusTypeDef ldc1101_writeConfig(LDC_HandleTypeDef *hldc, const LDC_RegConfig_t *cfg, uint16_t cfg_size) {
    if (hldc == NULL || cfg == NULL || cfg_size == 0) {
        return DEVICE_ERROR;
    }
    ldc1101_setPowerMode(hldc, _LDC1101_FUNC_MODE_SLEEP_MODE);
    for (uint16_t i = 0; i < cfg_size; i++) {
        ldc1101_writeByte(hldc, cfg[i].reg, cfg[i].value);
    }
    return DEVICE_OK;
}

//// 为读取 RP 值以设置 RP_MIN
// uint8_t ldc1101_init(LDC_HandleTypeDef *hldc, uint8_t RP_MIN)
//{
//     // 设置为 SLEEP 模式，开始初始化
//     ldc1101_writeByte(hldc, _LDC1101_REG_CFG_POWER_STATE,
//     _LDC1101_FUNC_MODE_SLEEP_MODE);// 0x01

//    // 先读取 CHIP ID，确认 SPI 和芯片正常
//    uint8_t chip_id = ldc1101_readByte(hldc, _LDC1101_REG_CHIP_ID);
//    if(chip_id != 0xD4)
//    {
//        return DEVICE_ERROR;
//    }

//    // 设置 RP 测量动态范围(0x01,0x47)
//    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE,
//    RP_MIN |
//																																					_LDC1101_RP_SET_RP_MAX_6KOhm);

//    // 配置内部时间常数(0x02,0x9A)(0x03,0xFD)
//    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_1, 0x9A);
//    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_2, 0xFD);

//    // 配置 RPL 转换时间(0x04,0xC4)
//    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_RP_L_CONVERSION_INTERVAL, 0xC4);

//    // 配置事件报告
//    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_INTB_MODE,
//    _LDC1101_INTB_MODE_DONT_REPORT_INTB_ON_SDO_PIN);

//    // 关闭其他辅助功能
//    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_ADDITIONAL_DEVICE,
//    _LDC1101_ALT_CFG_L_OPTIMAL_DISABLED |
//																															 _LDC1101_ALT_CFG_SHUTDOWN_DISABLE);//
// 0x00

//    // 关闭门限功能（初次调试不使用）
//    ldc1101_writeByte(hldc, _LDC1101_REG_RP_THRESH_H_MSB, 0x00);
//    ldc1101_writeByte(hldc, _LDC1101_REG_RP_THRESH_H_LSB, 0x00);
//    ldc1101_writeByte(hldc, _LDC1101_REG_RP_THRESH_L_MSB, 0x00);
//    ldc1101_writeByte(hldc, _LDC1101_REG_RP_THRESH_L_LSB, 0x00);
//    ldc1101_writeByte(hldc, _LDC1101_REG_L_THRESH_HI_MSB, 0x00);
//    ldc1101_writeByte(hldc, _LDC1101_REG_L_THRESH_HI_LSB, 0x00);
//    ldc1101_writeByte(hldc, _LDC1101_REG_L_THRESH_LO_MSB, 0x00);
//    ldc1101_writeByte(hldc, _LDC1101_REG_L_THRESH_LO_LSB, 0x00);

//    // 关闭振幅控制
//    ldc1101_writeByte(hldc, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x00);

//    // 切换到 ACTIVE CONVERSION 模式开始测量
//    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_POWER_STATE,
//    _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE);

//    isLHR = 0;

//    // 等待测量稳定
//    HAL_Delay(100);

//    return DEVICE_OK;
//}

// 切换到 LHR 模式
void ldc1101_goTo_Lmode(LDC_HandleTypeDef *hldc) {
    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 0x01);
    ldc1101_writeByte(hldc, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x01);
    hldc->mode = LDC_MODE_LHR;
}

// 切换到 RPL 模式
void ldc1101_goTo_RPmode(LDC_HandleTypeDef *hldc) {
    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 0x02);
    ldc1101_writeByte(hldc, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x00);
    hldc->mode = LDC_MODE_RP;
}

// 获取 RP 数据
uint16_t ldc1101_getRPData(LDC_HandleTypeDef *hldc) {
    uint16_t data;
    data = ldc1101_readByte(hldc, _LDC1101_REG_RP_DATA_LSB);
    data = data | (ldc1101_readByte(hldc, _LDC1101_REG_RP_DATA_MSB) << 8);
    return data;
}

// 获取 L 数据
uint16_t ldc1101_getLData(LDC_HandleTypeDef *hldc) {
    uint16_t data;
    data = ldc1101_readByte(hldc, _LDC1101_REG_L_DATA_LSB);
    data = data | (ldc1101_readByte(hldc, _LDC1101_REG_L_DATA_MSB) << 8);
    return data;
}

// 获取 LHR 数据
uint32_t ldc1101_getLHRData(LDC_HandleTypeDef *hldc) {
    uint32_t data = 0;
    data |= (uint32_t)ldc1101_readByte(hldc, _LDC1101_REG_LHR_DATA_LSB);
    data |= ((uint32_t)ldc1101_readByte(hldc, _LDC1101_REG_LHR_DATA_MID) << 8);
    data |= ((uint32_t)ldc1101_readByte(hldc, _LDC1101_REG_LHR_DATA_MSB) << 16);

    return data; // 返回24位数据，低24位有效
}
