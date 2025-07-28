#ifndef __LDC1101_DRIVER_H
#define __LDC1101_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "stdbool.h"
#include "stdint.h"


#define DEVICE_ERROR  0x01
#define DEVICE_OK     0x00

// 优先使用前向依赖，尽量避免包含 main.h 这种大杂烩文件
typedef struct __SPI_HandleTypeDef SPI_HandleTypeDef;
typedef struct {
    SPI_HandleTypeDef  *hspi;  
    GPIO_TypeDef  *cs_port; 
    uint16_t  cs_pin;
} LDC1101_Device;

extern bool isLHR;

/* Register */
#define _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE   0x01 
#define _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_1       0x02 
#define _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_2       0x03 
#define _LDC1101_REG_CFG_RP_L_CONVERSION_INTERVAL       0x04 
#define _LDC1101_REG_CFG_ADDITIONAL_DEVICE              0x05 
#define _LDC1101_REG_RP_THRESH_H_LSB                    0x06 
#define _LDC1101_REG_RP_THRESH_H_MSB                    0x07 
#define _LDC1101_REG_RP_THRESH_L_LSB                    0x08 
#define _LDC1101_REG_RP_THRESH_L_MSB                    0x09 
#define _LDC1101_REG_CFG_INTB_MODE                      0x0A 
#define _LDC1101_REG_CFG_POWER_STATE                    0x0B 
#define _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT      0x0C 
#define _LDC1101_REG_L_THRESH_HI_LSB                    0x16 
#define _LDC1101_REG_L_THRESH_HI_MSB                    0x17 
#define _LDC1101_REG_L_THRESH_LO_LSB                    0x18 
#define _LDC1101_REG_L_THRESH_LO_MSB                    0x19 
#define _LDC1101_REG_RP_L_MEASUREMENT_STATUS            0x20 
#define _LDC1101_REG_RP_DATA_LSB                        0x21 
#define _LDC1101_REG_RP_DATA_MSB                        0x22 
#define _LDC1101_REG_L_DATA_LSB                         0x23 
#define _LDC1101_REG_L_DATA_MSB                         0x24 
#define _LDC1101_REG_LHR_RCOUNT_LSB                     0x30 
#define _LDC1101_REG_LHR_RCOUNT_MSB                     0x31 
#define _LDC1101_REG_LHR_OFFSET_LSB                     0x32 
#define _LDC1101_REG_LHR_OFFSET_MSB                     0x33 
#define _LDC1101_REG_CFG_LHR                            0x34 
#define _LDC1101_REG_LHR_DATA_LSB                       0x38 
#define _LDC1101_REG_LHR_DATA_MID                       0x39 
#define _LDC1101_REG_LHR_DATA_MSB                       0x3A 
#define _LDC1101_REG_LHR_STATUS                         0x3B 
#define _LDC1101_REG_DEVICE_RID_VALUE                   0x3E 
#define _LDC1101_REG_CHIP_ID                            0x3F 

/* Register RP_SET Field Descriptions (RW) */
#define _LDC1101_RP_SET_RP_MAX_IS_DRIVEN                0x00 
#define _LDC1101_RP_SET_RP_MAX_CURRENT_IS_IGNORED       0x80 

#define _LDC1101_RP_SET_RP_MAX_96KOhm                   0x00 
#define _LDC1101_RP_SET_RP_MAX_48KOhm                   0x10 
#define _LDC1101_RP_SET_RP_MAX_24KOhm                   0x20 
#define _LDC1101_RP_SET_RP_MAX_12KOhm                   0x30 
#define _LDC1101_RP_SET_RP_MAX_6KOhm                    0x40 
#define _LDC1101_RP_SET_RP_MAX_3KOhm                    0x50 
#define _LDC1101_RP_SET_RP_MAX_1_5KOhm                  0x60 
#define _LDC1101_RP_SET_RP_MAX_0_75KOh                  0x70 

#define _LDC1101_RP_SET_RP_MIN_96KOhm                   0x00 
#define _LDC1101_RP_SET_RP_MIN_48KOhm                   0x01 
#define _LDC1101_RP_SET_RP_MIN_24KOhm                   0x02 
#define _LDC1101_RP_SET_RP_MIN_12KOhm                   0x03 
#define _LDC1101_RP_SET_RP_MIN_6KOhm                    0x04 
#define _LDC1101_RP_SET_RP_MIN_3KOhm                    0x05 
#define _LDC1101_RP_SET_RP_MIN_1_5KOhm                  0x06 
#define _LDC1101_RP_SET_RP_MIN_0_75KOhm                 0x07 

/* Configure Internal Time Constant 1 (RW) */
#define _LDC1101_TC1_C1_0_75pF         0x00 
#define _LDC1101_TC1_C1_1_5pF          0x40 
#define _LDC1101_TC1_C1_3pF            0x80 
#define _LDC1101_TC1_C1_6pF            0xC0 

#define _LDC1101_TC1_R1_417_0kOhm      0x00 
#define _LDC1101_TC1_R1_404_2kOhm      0x01 
#define _LDC1101_TC1_R1_391_5kOhm      0x02 
#define _LDC1101_TC1_R1_378_7kOhm      0x03 
#define _LDC1101_TC1_R1_365_9kOhm      0x04 
#define _LDC1101_TC1_R1_353_2kOhm      0x05 
#define _LDC1101_TC1_R1_340_4kOhm      0x06 
#define _LDC1101_TC1_R1_327_6kOhm      0x07 
#define _LDC1101_TC1_R1_314_8kOhm      0x08 
#define _LDC1101_TC1_R1_302_1kOhm      0x09 
#define _LDC1101_TC1_R1_289_3kOhm      0x0A 
#define _LDC1101_TC1_R1_276_5kOhm      0x0B 
#define _LDC1101_TC1_R1_263_8kOhm      0x0C 
#define _LDC1101_TC1_R1_251_0kOhm      0x0D 
#define _LDC1101_TC1_R1_238_2kOhm      0x0E 
#define _LDC1101_TC1_R1_225_5kOhm      0x0F 
#define _LDC1101_TC1_R1_212_7kOhm      0x10 
#define _LDC1101_TC1_R1_199_9kOhm      0x11 
#define _LDC1101_TC1_R1_187_1kOhm      0x12 
#define _LDC1101_TC1_R1_174_4kOhm      0x13 
#define _LDC1101_TC1_R1_161_6kOhm      0x14 
#define _LDC1101_TC1_R1_148_8kOhm      0x15 
#define _LDC1101_TC1_R1_136_1kOhm      0x16 
#define _LDC1101_TC1_R1_123_3kOhm      0x17 
#define _LDC1101_TC1_R1_110_5kOhm      0x18 
#define _LDC1101_TC1_R1_97_8kOhm       0x19 
#define _LDC1101_TC1_R1_85_0kOhm       0x1A 
#define _LDC1101_TC1_R1_72_2kOhm       0x1B 
#define _LDC1101_TC1_R1_59_4kOhm       0x1C 
#define _LDC1101_TC1_R1_46_7kOhm       0x1D 
#define _LDC1101_TC1_R1_33_9kOhm       0x1E 
#define _LDC1101_TC1_R1_21_1kOhm       0x1F 

/* Configure Internal Time Constant 2 (RW) */
#define _LDC1101_TC2_C2_3pF          0x00 
#define _LDC1101_TC2_C2_6pF          0x40 
#define _LDC1101_TC2_C2_12pF         0x80 
#define _LDC1101_TC2_C2_24pF         0xC0 

#define _LDC1101_TC2_R2_835_0kOhm     0x00 
#define _LDC1101_TC2_R2_822_2kOhm     0x01 
#define _LDC1101_TC2_R2_809_5kOhm     0x02 
#define _LDC1101_TC2_R2_796_7kOhm     0x03 
#define _LDC1101_TC2_R2_783_9kOhm     0x04 
#define _LDC1101_TC2_R2_771_2kOhm     0x05 
#define _LDC1101_TC2_R2_758_4kOhm     0x06 
#define _LDC1101_TC2_R2_745_6kOhm     0x07 
#define _LDC1101_TC2_R2_732_8kOhm     0x08 
#define _LDC1101_TC2_R2_720_1kOhm     0x09 
#define _LDC1101_TC2_R2_707_3kOhm     0x0A 
#define _LDC1101_TC2_R2_694_5kOhm     0x0B 
#define _LDC1101_TC2_R2_681_8kOhm     0x0C 
#define _LDC1101_TC2_R2_669_0kOhm     0x0D 
#define _LDC1101_TC2_R2_656_2kOhm     0x0E 
#define _LDC1101_TC2_R2_643_4kOhm     0x0F 
#define _LDC1101_TC2_R2_630_7kOhm     0x10 
#define _LDC1101_TC2_R2_617_9kOhm     0x11 
#define _LDC1101_TC2_R2_605_1kOhm     0x12 
#define _LDC1101_TC2_R2_592_4kOhm     0x13 
#define _LDC1101_TC2_R2_579_6kOhm     0x14 
#define _LDC1101_TC2_R2_566_8kOhm     0x15 
#define _LDC1101_TC2_R2_554_0kOhm     0x16 
#define _LDC1101_TC2_R2_541_3kOhm     0x17 
#define _LDC1101_TC2_R2_528_5kOhm     0x18 
#define _LDC1101_TC2_R2_515_7kOhm     0x19 
#define _LDC1101_TC2_R2_503_0kOhm     0x1A 
#define _LDC1101_TC2_R2_490_2kOhm     0x1B 
#define _LDC1101_TC2_R2_477_4kOhm     0x1C 
#define _LDC1101_TC2_R2_464_6kOhm     0x1D 
#define _LDC1101_TC2_R2_451_9kOhm     0x1E 
#define _LDC1101_TC2_R2_439_1kOhm     0x1F 
#define _LDC1101_TC2_R2_426_4kOhm     0x20 
#define _LDC1101_TC2_R2_413_6kOhm     0x21 
#define _LDC1101_TC2_R2_400_8kOhm     0x22 
#define _LDC1101_TC2_R2_388_1kOhm     0x23 
#define _LDC1101_TC2_R2_375_3kOhm     0x24 
#define _LDC1101_TC2_R2_362_5kOhm     0x25 
#define _LDC1101_TC2_R2_349_7kOhm     0x26 
#define _LDC1101_TC2_R2_337_0kOhm     0x27 
#define _LDC1101_TC2_R2_324_2kOhm     0x28 
#define _LDC1101_TC2_R2_311_4kOhm     0x29 
#define _LDC1101_TC2_R2_298_7kOhm     0x2A 
#define _LDC1101_TC2_R2_285_9kOhm     0x2B 
#define _LDC1101_TC2_R2_273_1kOhm     0x2C 
#define _LDC1101_TC2_R2_260_3kOhm     0x2D 
#define _LDC1101_TC2_R2_247_6kOhm     0x2E 
#define _LDC1101_TC2_R2_234_8kOhm     0x2F 
#define _LDC1101_TC2_R2_222_0kOhm     0x30 
#define _LDC1101_TC2_R2_209_3kOhm     0x31 
#define _LDC1101_TC2_R2_196_5kOhm     0x32 
#define _LDC1101_TC2_R2_183_7kOhm     0x33 
#define _LDC1101_TC2_R2_170_9kOhm     0x34 
#define _LDC1101_TC2_R2_158_2kOhm     0x35 
#define _LDC1101_TC2_R2_145_4kOhm     0x36 
#define _LDC1101_TC2_R2_132_7kOhm     0x37 
#define _LDC1101_TC2_R2_119_9kOhm     0x38 
#define _LDC1101_TC2_R2_107_1kOhm     0x39 
#define _LDC1101_TC2_R2_94_3kOhm      0x3A 
#define _LDC1101_TC2_R2_81_6kOhm      0x3B 
#define _LDC1101_TC2_R2_68_8kOhm      0x3C 
#define _LDC1101_TC2_R2_56_0kOhm      0x3D 
#define _LDC1101_TC2_R2_43_3kOhm      0x3E 
#define _LDC1101_TC2_R2_30_5kOhm      0x3F 

/* Configure RP+L conversion interval (RW) */
#define _LDC1101_DIG_CFG_MIN_FREQ_500kHz     0x00 
#define _LDC1101_DIG_CFG_MIN_FREQ_1MHz       0x10 
#define _LDC1101_DIG_CFG_MIN_FREQ_1_5MHz     0x20 
#define _LDC1101_DIG_CFG_MIN_FREQ_2MHz       0x30 
#define _LDC1101_DIG_CFG_MIN_FREQ_2_5MHz     0x40 
#define _LDC1101_DIG_CFG_MIN_FREQ_3MHz       0x50 
#define _LDC1101_DIG_CFG_MIN_FREQ_3_5MHz     0x60 
#define _LDC1101_DIG_CFG_MIN_FREQ_4MHz       0x70 
#define _LDC1101_DIG_CFG_MIN_FREQ_4_5MHz     0x80 
#define _LDC1101_DIG_CFG_MIN_FREQ_5MHz       0x90 
#define _LDC1101_DIG_CFG_MIN_FREQ_5_5MHz     0xA0 
#define _LDC1101_DIG_CFG_MIN_FREQ_6MHz       0xB0 
#define _LDC1101_DIG_CFG_MIN_FREQ_6_5MHz     0xC0 
#define _LDC1101_DIG_CFG_MIN_FREQ_7MHz       0xD0 
#define _LDC1101_DIG_CFG_MIN_FREQ_7_5MHz     0xE0 
#define _LDC1101_DIG_CFG_MIN_FREQ_8MHz       0xF0 

#define _LDC1101_DIG_CFG_RESP_TIME_192      0x02 
#define _LDC1101_DIG_CFG_RESP_TIME_384      0x03 
#define _LDC1101_DIG_CFG_RESP_TIME_768      0x04 
#define _LDC1101_DIG_CFG_RESP_TIME_1536     0x05 
#define _LDC1101_DIG_CFG_RESP_TIME_3072     0x06 
#define _LDC1101_DIG_CFG_RESP_TIME_6144     0x07 

/* Configure additional device settings (RW) */
#define _LDC1101_ALT_CFG_SHUTDOWN_ENABLE       0x02 
#define _LDC1101_ALT_CFG_SHUTDOWN_DISABLE      0x00 
#define _LDC1101_ALT_CFG_L_OPTIMAL_DISABLED    0x00 
#define _LDC1101_ALT_CFG_L_OPTIMAL_ENABLE      0x01 

/* Configure INTB reporting on SDO pin (RW) */
#define _LDC1101_INTB_MODE_DONT_REPORT_INTB_ON_SDO_PIN           0x00 
#define _LDC1101_INTB_MODE_REPORT_INTB_ON_SDO_PIN                0x80 

#define _LDC1101_INTB_MODE_REPORT_LHR_DATA_READY                 0x20 
#define _LDC1101_INTB_MODE_L_CONVERSION_TO_L_THRESHOLDS          0x10 
#define _LDC1101_INTB_MODE_L_CONVERSION_TO_L_HIGH_THRESHOLDS     0x08 
#define _LDC1101_INTB_MODE_REPORT_RP_L_DATA_READY                0x04 
#define _LDC1101_INTB_MODE_RP_CONVERSION_TO_L_THRESHOLDS         0x02 
#define _LDC1101_INTB_MODE_RP_CONVERSION_TO_L_HIGH_THRESHOLDS    0x01 
#define _LDC1101_INTB_MODE_NO_OUTPUT                             0x00 

/* Configure Power State (RW) */
#define _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE    0x00 
#define _LDC1101_FUNC_MODE_SLEEP_MODE                0x01 
#define _LDC1101_FUNC_MODE_SHUTDOWN_MODE             0x02 

/* High Resolution L Configuration (RW) */
#define _LDC1101_LHR_CFG_FREQUENCY_NOT_DIVIDED      0x00 
#define _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_2     0x01 
#define _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_4     0x02 
#define _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_8     0x03 



void ldc1101_writeByte(LDC1101_Device *dev, uint8_t addr, uint8_t _data);

uint8_t ldc1101_readByte(LDC1101_Device *dev, uint8_t addr);



uint8_t ldc1101_init(LDC1101_Device *dev,uint8_t RP_MIN);

void ldc1101_setPowerMode(LDC1101_Device *dev, uint8_t mode);

void ldc1101_goTo_Lmode(LDC1101_Device *dev);

void ldc1101_goTo_RPmode(LDC1101_Device *dev);

uint16_t ldc1101_getRPData(LDC1101_Device *dev);

uint16_t ldc1101_getLData(LDC1101_Device *dev);

uint32_t ldc1101_getLHRData(LDC1101_Device *dev);

#ifdef __cplusplus
}
#endif

#endif /* __LDC1101_DRIVER_H */
