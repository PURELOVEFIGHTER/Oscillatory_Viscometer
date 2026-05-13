#include "ldc1101_driver.h"
#include "stdlib.h"

DEVICE_StatusTypeDef ldc1101_init(LDC_HandleTypeDef *hldc,const LDC_InitTypeDef *init) {
    if (hldc == NULL || init == NULL) {
        return DEVICE_ERROR;
    }
    hldc->hspi        = init->hspi;
    hldc->cs_port     = init->cs_port;
    hldc->cs_pin      = init->cs_pin;
    hldc->power_state = LDC_SLEEP;
    hldc->mode        = LDC_DEFAULT;
    uint8_t chip_id   = ldc1101_readByte(hldc, _LDC1101_REG_CHIP_ID);
    if (chip_id != 0xD4) {
        return DEVICE_ERROR;
    }
    return DEVICE_OK;
}

// SPI写单个寄存器
void ldc1101_writeByte(LDC_HandleTypeDef *hldc, uint8_t addr, uint8_t _data) {
    uint8_t writeReg[2] = {addr & 0x7F, _data};

    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hldc->hspi, writeReg, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_SET);
}
// SPI顺序写多个寄存器（起始地址自动递增）
void ldc1101_writeBurst(LDC_HandleTypeDef *hldc, uint8_t start_addr, uint8_t *data, uint8_t len) {
    if (((start_addr + len - 1U) > 0x3FU) || (len > _LDC1101_REG_WRITE_READ) || (len == 0U)) {
        return;
    }
    uint8_t tx_data[_LDC1101_REG_WRITE_READ + 1];
    tx_data[0] = start_addr & 0x7FU;

    for (uint8_t i = 0; i < len; i++) {
        tx_data[i + 1U] = data[i];
    }
    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(hldc->hspi, tx_data, len + 1U, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_SET);
}

// SPI读单个寄存器
uint8_t ldc1101_readByte(LDC_HandleTypeDef *hldc, uint8_t addr) {
    uint8_t tx_data[2];
    uint8_t rx_data[2];

    tx_data[0] = 0x80 | addr;
    tx_data[1] = 0x00;

    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(hldc->hspi, tx_data, rx_data, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_SET);

    return rx_data[1];
}
// SPI顺序读多个寄存器（起始地址自动递增）
void ldc1101_readBurst(LDC_HandleTypeDef *hldc, uint8_t start_addr, uint8_t *out_data, uint8_t len) {
    if (((start_addr + len - 1U) > 0x3FU) || (len > _LDC1101_REG_TOTAL) || (len == 0U)) {
        return;
    }
    uint8_t tx_data[_LDC1101_REG_TOTAL + 1] = {0};
    uint8_t rx_data[_LDC1101_REG_TOTAL + 1] = {0};

    tx_data[0] = 0x80 | start_addr;

    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(hldc->hspi, tx_data, rx_data, len + 1U, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(hldc->cs_port, hldc->cs_pin, GPIO_PIN_SET);

    for (uint8_t i = 0; i < len; i++) {
        out_data[i] = rx_data[i + 1U];
    }
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

// 进入 LHR 模式
void ldc1101_goTo_Lmode(LDC_HandleTypeDef *hldc) {
    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 0x01);
    ldc1101_writeByte(hldc, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x01);
    hldc->mode = LDC_MODE_LHR;
}

// 进入 RP+L 模式
void ldc1101_goTo_RPmode(LDC_HandleTypeDef *hldc) {
    ldc1101_writeByte(hldc, _LDC1101_REG_CFG_ADDITIONAL_DEVICE, 0x02);
    ldc1101_writeByte(hldc, _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT, 0x00);
    hldc->mode = LDC_MODE_RPL;
}

// 设置采样率
bool ldc1101_setLHRSampleRate(LDC_HandleTypeDef *hldc, float sample_rate) {
    if (hldc == NULL || sample_rate < 15.0f || sample_rate > 1.83908e5f) {
        return false;
    }
    ldc1101_setPowerMode(hldc, LDC_SLEEP);
    if (ldc1101_readByte(hldc, _LDC1101_REG_CFG_POWER_STATE) == _LDC1101_FUNC_MODE_SLEEP_MODE) {
        float T_sample         = 1.0f / sample_rate;
        float T_conv           = T_sample - 55.0f / REF_CLK_HZ;             // 减去参考周期时间
        uint16_t rcount        = (uint16_t)((T_conv * REF_CLK_HZ) / 16.0f); // 计算RCOUNT值
        uint8_t rcount_data[2] = {rcount & 0xFF, (rcount >> 8) & 0xFF};
        ldc1101_writeBurst(hldc, _LDC1101_REG_LHR_RCOUNT_LSB, rcount_data, 2);
        return true;
    }else{
        return false;
    }
}

// 读取 Rp 数据
uint16_t ldc1101_getRPData(LDC_HandleTypeDef *hldc) {
    uint8_t data[2];
    ldc1101_readBurst(hldc, _LDC1101_REG_RP_DATA_LSB, data, 2);
    return ((uint16_t)data[0]) | ((uint16_t)data[1] << 8);
}

// 读取 L 数据
uint16_t ldc1101_getLData(LDC_HandleTypeDef *hldc) {
    uint8_t data[2];
    ldc1101_readBurst(hldc, _LDC1101_REG_L_DATA_LSB, data, 2);
    return ((uint16_t)data[0]) | ((uint16_t)data[1] << 8);
}

// 读取 LHR 数据
uint32_t ldc1101_getLHRData(LDC_HandleTypeDef *hldc) {
    uint8_t data[3];
    ldc1101_readBurst(hldc, _LDC1101_REG_LHR_DATA_LSB, data, 3);
    return ((uint32_t)data[0]) | ((uint32_t)data[1] << 8) | ((uint32_t)data[2] << 16);
}

// 获取采样率（十六进制格式）
uint16_t ldc1101_getLHRRCount(LDC_HandleTypeDef *hldc) {
    uint16_t rcount;
    ldc1101_readBurst(hldc, _LDC1101_REG_LHR_RCOUNT_LSB, (uint8_t *)&rcount, 2);
    return rcount;
}

// 获取采样率(十进制格式)
float ldc1101_getLHRSampleRate(LDC_HandleTypeDef *hldc) {
    uint16_t rcount   = ldc1101_getLHRRCount(hldc);
    float conv_cycles = (float)(rcount * 16U + 55U); // RCOUNT*16 + 55 reference cycles
    return REF_CLK_HZ / conv_cycles;
}