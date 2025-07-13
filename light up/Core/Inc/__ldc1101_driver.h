#ifndef __LDC1101_DRIVER_H
#define __LDC1101_DRIVER_H


#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "spi.h"

#define DEVICE_ERROR  0x01
#define DEVICE_OK  0x00

typedef struct {
    SPI_HandleTypeDef *hspi;    // SPI¾ä±ú
    GPIO_TypeDef *cs_port;      // CSÒý½Å¶Ë¿Ú
    uint16_t cs_pin;            // CSÒý½ÅºÅ
} LDC1101_Device;

extern LDC1101_Device ldc1;
extern LDC1101_Device ldc2;


/* Register */
extern const uint8_t _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE;
extern const uint8_t _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_1    ;
extern const uint8_t _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_2    ;
extern const uint8_t _LDC1101_REG_CFG_RP_L_CONVERSION_INTERVAL    ;
extern const uint8_t _LDC1101_REG_CFG_ADDITIONAL_DEVICE           ;
extern const uint8_t _LDC1101_REG_RP_THRESH_H_LSB                 ;
extern const uint8_t _LDC1101_REG_RP_THRESH_H_MSB                 ;
extern const uint8_t _LDC1101_REG_RP_THRESH_L_LSB                 ;
extern const uint8_t _LDC1101_REG_RP_THRESH_L_MSB                 ;
extern const uint8_t _LDC1101_REG_CFG_INTB_MODE                   ;
extern const uint8_t _LDC1101_REG_CFG_POWER_STATE                 ;
extern const uint8_t _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT   ;
extern const uint8_t _LDC1101_REG_L_THRESH_HI_LSB                 ;
extern const uint8_t _LDC1101_REG_L_THRESH_HI_MSB                 ;
extern const uint8_t _LDC1101_REG_L_THRESH_LO_LSB                 ;
extern const uint8_t _LDC1101_REG_L_THRESH_LO_MSB                 ;
extern const uint8_t _LDC1101_REG_RP_L_MEASUREMENT_STATUS         ;
extern const uint8_t _LDC1101_REG_RP_DATA_LSB                     ;
extern const uint8_t _LDC1101_REG_RP_DATA_MSB                     ;
extern const uint8_t _LDC1101_REG_L_DATA_LSB                      ;
extern const uint8_t _LDC1101_REG_L_DATA_MSB                      ;
extern const uint8_t _LDC1101_REG_LHR_RCOUNT_LSB                  ;
extern const uint8_t _LDC1101_REG_LHR_RCOUNT_MSB                  ;
extern const uint8_t _LDC1101_REG_LHR_OFFSET_LSB                  ;
extern const uint8_t _LDC1101_REG_LHR_OFFSET_MSB                  ;
extern const uint8_t _LDC1101_REG_CFG_LHR                         ;
extern const uint8_t _LDC1101_REG_LHR_DATA_LSB                    ;
extern const uint8_t _LDC1101_REG_LHR_DATA_MID                    ;
extern const uint8_t _LDC1101_REG_LHR_DATA_MSB                    ;
extern const uint8_t _LDC1101_REG_LHR_STATUS                      ;
extern const uint8_t _LDC1101_REG_DEVICE_RID_VALUE                ;
extern const uint8_t _LDC1101_REG_CHIP_ID                         ;

/* Register RP_SET Field Descriptions (RW) */
extern const uint8_t _LDC1101_RP_SET_RP_MAX_IS_DRIVEN         ;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_CURRENT_IS_IGNORED;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_96KOhm            ;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_48KOhm            ;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_24KOhm            ;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_12KOhm            ;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_6KOhm             ;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_3KOhm             ;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_1_5KOhm           ;
extern const uint8_t _LDC1101_RP_SET_RP_MAX_0_75KOh           ;
extern const uint8_t _LDC1101_RP_SET_RP_MIN_96KOhm            ;
extern const uint8_t _LDC1101_RP_SET_RP_MIN_48KOhm            ;
extern const uint8_t _LDC1101_RP_SET_RP_MIN_24KOhm            ;
extern const uint8_t _LDC1101_RP_SET_RP_MIN_12KOhm            ;
extern const uint8_t _LDC1101_RP_SET_RP_MIN_6KOhm             ;
extern const uint8_t _LDC1101_RP_SET_RP_MIN_3KOhm             ;
extern const uint8_t _LDC1101_RP_SET_RP_MIN_1_5KOhm           ;
extern const uint8_t _LDC1101_RP_SET_RP_MIN_0_75KOh           ;

/* Configure Internal Time Constant 1 (RW) */
extern const uint8_t _LDC1101_TC1_C1_0_75pF   ;
extern const uint8_t _LDC1101_TC1_C1_1_5pF    ;
extern const uint8_t _LDC1101_TC1_C1_3pF      ;
extern const uint8_t _LDC1101_TC1_C1_6pF      ;
extern const uint8_t _LDC1101_TC1_R1_417kOhm  ;
extern const uint8_t _LDC1101_TC1_R1_212_7kOhm;
extern const uint8_t _LDC1101_TC1_R1_21_1kOhm ;

/* Configure Internal Time Constant 2 (RW) */
extern const uint8_t _LDC1101_TC2_C2_3pF      ;
extern const uint8_t _LDC1101_TC2_C2_6pF      ;
extern const uint8_t _LDC1101_TC2_C2_12pF     ;
extern const uint8_t _LDC1101_TC2_C2_24pF     ;
extern const uint8_t _LDC1101_TC2_R2_835kOhm  ;
extern const uint8_t _LDC1101_TC2_R2_426_4kOhm;
extern const uint8_t _LDC1101_TC2_R2_30_5kOhm ;

/* Configure RP+L conversion interval (RW) */
extern const uint8_t _LDC1101_DIG_CFG_MIN_FREQ_500kHz;
extern const uint8_t _LDC1101_DIG_CFG_MIN_FREQ_8MHz  ;
extern const uint8_t _LDC1101_DIG_CFG_RESP_TIME_192s ;
extern const uint8_t _LDC1101_DIG_CFG_RESP_TIME_384s ;
extern const uint8_t _LDC1101_DIG_CFG_RESP_TIME_768s ;
extern const uint8_t _LDC1101_DIG_CFG_RESP_TIME_1536s;
extern const uint8_t _LDC1101_DIG_CFG_RESP_TIME_3072s;
extern const uint8_t _LDC1101_DIG_CFG_RESP_TIME_6144s;

/* Configure additional device settings (RW) */
extern const uint8_t _LDC1101_ALT_CFG_SHUTDOWN_ENABLE   ;
extern const uint8_t _LDC1101_ALT_CFG_SHUTDOWN_DISABLE  ;
extern const uint8_t _LDC1101_ALT_CFG_L_OPTIMAL_DISABLED;
extern const uint8_t _LDC1101_ALT_CFG_L_OPTIMAL_ENABLE  ;

/* Configure INTB reporting on SDO pin (RW) */
extern const uint8_t _LDC1101_INTB_MODE_DONT_REPORT_INTB_ON_SDO_PIN       ;
extern const uint8_t _LDC1101_INTB_MODE_REPORT_INTB_ON_SDO_PIN            ;
extern const uint8_t _LDC1101_INTB_MODE_REPORT_LHR_DATA_READY             ;
extern const uint8_t _LDC1101_INTB_MODE_L_CONVERSION_TO_L_THRESHOLDS      ;
extern const uint8_t _LDC1101_INTB_MODE_L_CONVERSION_TO_L_HIGH_THRESHOLDS ;
extern const uint8_t _LDC1101_INTB_MODE_REPORT_RP_L_DATA_READY            ;
extern const uint8_t _LDC1101_INTB_MODE_RP_CONVERSION_TO_L_THRESHOLDS     ;
extern const uint8_t _LDC1101_INTB_MODE_RP_CONVERSION_TO_L_HIGH_THRESHOLDS;
extern const uint8_t _LDC1101_INTB_MODE_NO_OUTPUT                         ;

/* Configure Power State (RW) */
extern const uint8_t _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE;
extern const uint8_t _LDC1101_FUNC_MODE_SLEEP_MODE            ;
extern const uint8_t _LDC1101_FUNC_MODE_SHUTDOWN_MODE         ;

/* High Resolution L Configuration (RW) */
extern const uint8_t _LDC1101_LHR_CFG_FREQUENCY_NOT_DIVIDED ;
extern const uint8_t _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_2;
extern const uint8_t _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_4;
extern const uint8_t _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_8;



void ldc1101_writeByte(LDC1101_Device *dev, uint8_t addr, uint8_t _data);

uint8_t ldc1101_readByte(LDC1101_Device *dev, uint8_t addr);



uint8_t ldc1101_init(LDC1101_Device *dev);

void ldc1101_setPowerMode(LDC1101_Device *dev, uint8_t mode);

void ldc1101_goTo_Lmode(LDC1101_Device *dev);

void ldc1101_goTo_RPmode(LDC1101_Device *dev);

uint8_t ldc1101_getStatus(LDC1101_Device *dev);

uint16_t ldc1101_getRPData(LDC1101_Device *dev);

uint16_t ldc1101_getLData(LDC1101_Device *dev);

uint32_t ldc1101_getLHRData(LDC1101_Device *dev);

uint8_t ldc1101_getInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif /* __LDC1101_DRIVER_H */
