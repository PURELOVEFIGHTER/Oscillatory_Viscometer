#include "ldc1101_driver.h"

bool isLHR = 0;

// SPI 写寄存器
void ldc1101_writeByte(LDC1101_HandleTypeDef *dev, uint8_t addr, uint8_t _data) {
    uint8_t writeReg[2] = {addr & 0x7F, _data};

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(dev->hspi, writeReg, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

// SPI 读寄存器
uint8_t ldc1101_readByte(LDC1101_HandleTypeDef *dev, uint8_t addr) {
    uint8_t tx_data[2];
    uint8_t rx_data[2];

    tx_data[0] = 0x80 | addr;
    tx_data[1] = 0x00;

    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(dev->hspi, tx_data, rx_data, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);

    return rx_data[1]; // 第二个字节是寄存器值
}

uint8_t ldc1101_init(LDC1101_HandleTypeDef *dev, uint8_t RP_MIN) {
    // 设置为 SLEEP 模式，开始初始化
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_POWER_STATE,
                      _LDC1101_FUNC_MODE_SLEEP_MODE); // 0x01

    // 先读取 CHIP ID，确认 SPI 和芯片正常
    uint8_t chip_id = ldc1101_readByte(dev, _LDC1101_REG_CHIP_ID);

    if (chip_id != 0xD4) {
        return DEVICE_ERROR;
    }

    // 设置 RP 测量动态范围(0x01,0x57)
    // LHR 模式下只关注 RPMIN
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE, RP_MIN | _LDC1101_RP_SET_RP_MAX_3KOhm);

    // LHR 模式设置必需(0x05,0x01)(0x0C,0x01)
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, _LDC1101_ALT_CFG_L_OPTIMAL_ENABLE);
    ldc1101_writeByte(dev, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x01); // LHR 持续转换

    // LHR Data Ready 报告(0x0A,0xA0)
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_INTB_MODE, _LDC1101_INTB_MODE_DONT_REPORT_INTB_ON_SDO_PIN);

    // 计数周期
    ldc1101_writeByte(dev, _LDC1101_REG_LHR_RCOUNT_LSB, 0x49);
    ldc1101_writeByte(dev, _LDC1101_REG_LHR_RCOUNT_MSB, 0x01);

    // 在静止状态下读取偏移量再进行设置
    ldc1101_writeByte(dev, _LDC1101_REG_LHR_OFFSET_LSB, 0x00);
    ldc1101_writeByte(dev, _LDC1101_REG_LHR_OFFSET_MSB, 0x00);

    // 不分频
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_LHR, _LDC1101_LHR_CFG_FREQUENCY_NOT_DIVIDED);

    isLHR = 1;
    
    // 开始工作  
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_POWER_STATE, _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE);

    // 等待测量稳定
    HAL_Delay(100);

    return DEVICE_OK;
}

//// 为读取 RP 值以设置 RP_MIN
// uint8_t ldc1101_init(LDC1101_HandleTypeDef *dev, uint8_t RP_MIN)
//{
//     // 设置为 SLEEP 模式，开始初始化
//     ldc1101_writeByte(dev, _LDC1101_REG_CFG_POWER_STATE,
//     _LDC1101_FUNC_MODE_SLEEP_MODE);// 0x01

//    // 先读取 CHIP ID，确认 SPI 和芯片正常
//    uint8_t chip_id = ldc1101_readByte(dev, _LDC1101_REG_CHIP_ID);
//    if(chip_id != 0xD4)
//    {
//        return DEVICE_ERROR;
//    }

//    // 设置 RP 测量动态范围(0x01,0x47)
//    ldc1101_writeByte(dev, _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE,
//    RP_MIN |
//																																					_LDC1101_RP_SET_RP_MAX_6KOhm);

//    // 配置内部时间常数(0x02,0x9A)(0x03,0xFD)
//    ldc1101_writeByte(dev, _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_1, 0x9A);
//    ldc1101_writeByte(dev, _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_2, 0xFD);

//    // 配置 RPL 转换时间(0x04,0xC4)
//    ldc1101_writeByte(dev, _LDC1101_REG_CFG_RP_L_CONVERSION_INTERVAL, 0xC4);

//    // 配置事件报告
//    ldc1101_writeByte(dev, _LDC1101_REG_CFG_INTB_MODE,
//    _LDC1101_INTB_MODE_DONT_REPORT_INTB_ON_SDO_PIN);

//    // 关闭其他辅助功能
//    ldc1101_writeByte(dev, _LDC1101_REG_CFG_ADDITIONAL_DEVICE,
//    _LDC1101_ALT_CFG_L_OPTIMAL_DISABLED |
//																															 _LDC1101_ALT_CFG_SHUTDOWN_DISABLE);//
// 0x00

//    // 关闭门限功能（初次调试不使用）
//    ldc1101_writeByte(dev, _LDC1101_REG_RP_THRESH_H_MSB, 0x00);
//    ldc1101_writeByte(dev, _LDC1101_REG_RP_THRESH_H_LSB, 0x00);
//    ldc1101_writeByte(dev, _LDC1101_REG_RP_THRESH_L_MSB, 0x00);
//    ldc1101_writeByte(dev, _LDC1101_REG_RP_THRESH_L_LSB, 0x00);
//    ldc1101_writeByte(dev, _LDC1101_REG_L_THRESH_HI_MSB, 0x00);
//    ldc1101_writeByte(dev, _LDC1101_REG_L_THRESH_HI_LSB, 0x00);
//    ldc1101_writeByte(dev, _LDC1101_REG_L_THRESH_LO_MSB, 0x00);
//    ldc1101_writeByte(dev, _LDC1101_REG_L_THRESH_LO_LSB, 0x00);

//    // 关闭振幅控制
//    ldc1101_writeByte(dev, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x00);

//    // 切换到 ACTIVE CONVERSION 模式开始测量
//    ldc1101_writeByte(dev, _LDC1101_REG_CFG_POWER_STATE,
//    _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE);

//    isLHR = 0;

//    // 等待测量稳定
//    HAL_Delay(100);

//    return DEVICE_OK;
//}

// 切换到 LHR 模式
void ldc1101_goTo_Lmode(LDC1101_HandleTypeDef *dev) {
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 0x01);
    ldc1101_writeByte(dev, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x01);

    isLHR = 1;
}

// 切换到 RPL 模式
void ldc1101_goTo_RPmode(LDC1101_HandleTypeDef *dev) {
    ldc1101_writeByte(dev, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 0x02);
    ldc1101_writeByte(dev, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x00);

    isLHR = 0;
}

// 获取 RP 数据
uint16_t ldc1101_getRPData(LDC1101_HandleTypeDef *dev) {
    uint16_t data;
    data = ldc1101_readByte(dev, _LDC1101_REG_RP_DATA_LSB);
    data = data | (ldc1101_readByte(dev, _LDC1101_REG_RP_DATA_MSB) << 8);
    return data;
}

// 获取 L 数据
uint16_t ldc1101_getLData(LDC1101_HandleTypeDef *dev) {
    uint16_t data;
    data = ldc1101_readByte(dev, _LDC1101_REG_L_DATA_LSB);
    data = data | (ldc1101_readByte(dev, _LDC1101_REG_L_DATA_MSB) << 8);
    return data;
}

// 获取 LHR 数据
uint32_t ldc1101_getLHRData(LDC1101_HandleTypeDef *dev) {
    uint32_t data = 0;
    data |= (uint32_t)ldc1101_readByte(dev, _LDC1101_REG_LHR_DATA_LSB);
    data |= ((uint32_t)ldc1101_readByte(dev, _LDC1101_REG_LHR_DATA_MID) << 8);
    data |= ((uint32_t)ldc1101_readByte(dev, _LDC1101_REG_LHR_DATA_MSB) << 16);

    return data & 0x00FFFFFF; // 返回24位数据，低24位有效
}
