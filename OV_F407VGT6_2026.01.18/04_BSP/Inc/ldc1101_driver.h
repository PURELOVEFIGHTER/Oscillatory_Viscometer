#ifndef LDC1101_DRIVER_H
#define LDC1101_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "device_common.h"

#define REF_CLK_HZ 16000000U // LDC1101 参考时钟频率，按硬件进行修改
/* Register Begin*/
#define _LDC1101_REG_TOTAL      32 // 寄存器总数量
#define _LDC1101_REG_READ       11 // 只读寄存器数量
#define _LDC1101_REG_WRITE_READ 21 // 可读可写寄存器数量
// Configuration Registers
#define _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE 0x01 // ���� RP+L ������̬��Χ
#define _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_1     0x02 // RP+L ͨ��ʱ�䳣�� TC1
#define _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_2     0x03 // L ͨ��ʱ�䳣�� TC2
#define _LDC1101_REG_CFG_RP_L_CONVERSION_INTERVAL     0x04 // RP+L ת�����?
#define _LDC1101_REG_CFG_ADDITIONAL_DEVICE            0x05 // ��������
#define _LDC1101_REG_RP_THRESH_H_LSB                  0x06 // RPLģʽ RP �����ޣ�16λ��
#define _LDC1101_REG_RP_THRESH_H_MSB                  0x07
#define _LDC1101_REG_RP_THRESH_L_LSB                  0x08 // RPLģʽ RP �����ޣ�16λ��
#define _LDC1101_REG_RP_THRESH_L_MSB                  0x09
#define _LDC1101_REG_CFG_INTB_MODE                    0x0A // ���� INTB ��������
#define _LDC1101_REG_CFG_POWER_STATE                  0x0B // ���õ�Դ״̬�����������ߣ�
#define _LDC1101_REG_AMPLITUDE_CONTROL_REQUIREMENT 0x0C // ��������������߼��Ƿ񼤻�?
#define _LDC1101_REG_L_THRESH_HI_LSB               0x16 // LHRģʽ L ͨ�������ޣ�16λ��
#define _LDC1101_REG_L_THRESH_HI_MSB               0x17
#define _LDC1101_REG_L_THRESH_LO_LSB               0x18 // LHRģʽ L ͨ�������ޣ�16λ��
#define _LDC1101_REG_L_THRESH_LO_MSB               0x19
#define _LDC1101_REG_LHR_RCOUNT_LSB                0x30 // LHR ���������ã�16λ��
#define _LDC1101_REG_LHR_RCOUNT_MSB                0x31
#define _LDC1101_REG_LHR_OFFSET_LSB                0x32 // LHR ƫ������ֵ��16λ��
#define _LDC1101_REG_LHR_OFFSET_MSB                0x33
#define _LDC1101_REG_CFG_LHR                       0x34 // ���� LHR ģʽ��Ƶ�ʷ�Ƶ����

// Data Registers
#define _LDC1101_REG_RP_L_MEASUREMENT_STATUS 0x20 // RP+L ģʽ״̬��
#define _LDC1101_REG_RP_DATA_LSB             0x21 // RP+L ģʽ�� RP �������ݣ�16λ��
#define _LDC1101_REG_RP_DATA_MSB             0x22
#define _LDC1101_REG_L_DATA_LSB              0x23 // RP+L ģʽ�� L �������ݣ�16λ��
#define _LDC1101_REG_L_DATA_MSB              0x24
#define _LDC1101_REG_LHR_DATA_LSB            0x38 // LHR ģʽ�������ݣ�24 λ��
#define _LDC1101_REG_LHR_DATA_MID            0x39
#define _LDC1101_REG_LHR_DATA_MSB            0x3A
#define _LDC1101_REG_LHR_STATUS              0x3B // LHR ״̬��

// Identification Registers
#define _LDC1101_REG_DEVICE_RID_VALUE 0x3E // оƬID���̶�ֵ��0xD4
#define _LDC1101_REG_CHIP_ID          0x3F // �޶���
/* Register End */

/**
 * @brief ���� RP+L ģʽ�� RP ������ֵ
 * @reg   _LDC1101_REG_CFG_RP_MEASUREMENT_DYNAMIC_RANGE    0x01
 */
#define _LDC1101_RP_SET_RP_MAX_IS_DRIVEN          0x00
#define _LDC1101_RP_SET_RP_MAX_CURRENT_IS_IGNORED 0x80

#define _LDC1101_RP_SET_RP_MAX_96KOhm  0x00
#define _LDC1101_RP_SET_RP_MAX_48KOhm  0x10
#define _LDC1101_RP_SET_RP_MAX_24KOhm  0x20
#define _LDC1101_RP_SET_RP_MAX_12KOhm  0x30
#define _LDC1101_RP_SET_RP_MAX_6KOhm   0x40
#define _LDC1101_RP_SET_RP_MAX_3KOhm   0x50
#define _LDC1101_RP_SET_RP_MAX_1_5KOhm 0x60
#define _LDC1101_RP_SET_RP_MAX_0_75KOh 0x70

#define _LDC1101_RP_SET_RP_MIN_96KOhm   0x00
#define _LDC1101_RP_SET_RP_MIN_48KOhm   0x01
#define _LDC1101_RP_SET_RP_MIN_24KOhm   0x02
#define _LDC1101_RP_SET_RP_MIN_12KOhm   0x03
#define _LDC1101_RP_SET_RP_MIN_6KOhm    0x04
#define _LDC1101_RP_SET_RP_MIN_3KOhm    0x05
#define _LDC1101_RP_SET_RP_MIN_1_5KOhm  0x06
#define _LDC1101_RP_SET_RP_MIN_0_75KOhm 0x07

/**
 * @brief TC1��TC2 ʹ�� RP+L ģʽʱд��
 * @reg   _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_1    0x02
 * @reg   _LDC1101_REG_CFG_INTERNAL_TIME_CONSTANT_2    0x03
 */
/* Configure Internal Time Constant 1 (RW) */
#define _LDC1101_TC1_C1_0_75pF 0x00
#define _LDC1101_TC1_C1_1_5pF  0x40
#define _LDC1101_TC1_C1_3pF    0x80
#define _LDC1101_TC1_C1_6pF    0xC0

#define _LDC1101_TC1_R1_417_0kOhm 0x00
#define _LDC1101_TC1_R1_404_2kOhm 0x01
#define _LDC1101_TC1_R1_391_5kOhm 0x02
#define _LDC1101_TC1_R1_378_7kOhm 0x03
#define _LDC1101_TC1_R1_365_9kOhm 0x04
#define _LDC1101_TC1_R1_353_2kOhm 0x05
#define _LDC1101_TC1_R1_340_4kOhm 0x06
#define _LDC1101_TC1_R1_327_6kOhm 0x07
#define _LDC1101_TC1_R1_314_8kOhm 0x08
#define _LDC1101_TC1_R1_302_1kOhm 0x09
#define _LDC1101_TC1_R1_289_3kOhm 0x0A
#define _LDC1101_TC1_R1_276_5kOhm 0x0B
#define _LDC1101_TC1_R1_263_8kOhm 0x0C
#define _LDC1101_TC1_R1_251_0kOhm 0x0D
#define _LDC1101_TC1_R1_238_2kOhm 0x0E
#define _LDC1101_TC1_R1_225_5kOhm 0x0F
#define _LDC1101_TC1_R1_212_7kOhm 0x10
#define _LDC1101_TC1_R1_199_9kOhm 0x11
#define _LDC1101_TC1_R1_187_1kOhm 0x12
#define _LDC1101_TC1_R1_174_4kOhm 0x13
#define _LDC1101_TC1_R1_161_6kOhm 0x14
#define _LDC1101_TC1_R1_148_8kOhm 0x15
#define _LDC1101_TC1_R1_136_1kOhm 0x16
#define _LDC1101_TC1_R1_123_3kOhm 0x17
#define _LDC1101_TC1_R1_110_5kOhm 0x18
#define _LDC1101_TC1_R1_97_8kOhm  0x19
#define _LDC1101_TC1_R1_85_0kOhm  0x1A
#define _LDC1101_TC1_R1_72_2kOhm  0x1B
#define _LDC1101_TC1_R1_59_4kOhm  0x1C
#define _LDC1101_TC1_R1_46_7kOhm  0x1D
#define _LDC1101_TC1_R1_33_9kOhm  0x1E
#define _LDC1101_TC1_R1_21_1kOhm  0x1F

/* Configure Internal Time Constant 2 (RW) */
#define _LDC1101_TC2_C2_3pF  0x00
#define _LDC1101_TC2_C2_6pF  0x40
#define _LDC1101_TC2_C2_12pF 0x80
#define _LDC1101_TC2_C2_24pF 0xC0

#define _LDC1101_TC2_R2_835_0kOhm 0x00
#define _LDC1101_TC2_R2_822_2kOhm 0x01
#define _LDC1101_TC2_R2_809_5kOhm 0x02
#define _LDC1101_TC2_R2_796_7kOhm 0x03
#define _LDC1101_TC2_R2_783_9kOhm 0x04
#define _LDC1101_TC2_R2_771_2kOhm 0x05
#define _LDC1101_TC2_R2_758_4kOhm 0x06
#define _LDC1101_TC2_R2_745_6kOhm 0x07
#define _LDC1101_TC2_R2_732_8kOhm 0x08
#define _LDC1101_TC2_R2_720_1kOhm 0x09
#define _LDC1101_TC2_R2_707_3kOhm 0x0A
#define _LDC1101_TC2_R2_694_5kOhm 0x0B
#define _LDC1101_TC2_R2_681_8kOhm 0x0C
#define _LDC1101_TC2_R2_669_0kOhm 0x0D
#define _LDC1101_TC2_R2_656_2kOhm 0x0E
#define _LDC1101_TC2_R2_643_4kOhm 0x0F
#define _LDC1101_TC2_R2_630_7kOhm 0x10
#define _LDC1101_TC2_R2_617_9kOhm 0x11
#define _LDC1101_TC2_R2_605_1kOhm 0x12
#define _LDC1101_TC2_R2_592_4kOhm 0x13
#define _LDC1101_TC2_R2_579_6kOhm 0x14
#define _LDC1101_TC2_R2_566_8kOhm 0x15
#define _LDC1101_TC2_R2_554_0kOhm 0x16
#define _LDC1101_TC2_R2_541_3kOhm 0x17
#define _LDC1101_TC2_R2_528_5kOhm 0x18
#define _LDC1101_TC2_R2_515_7kOhm 0x19
#define _LDC1101_TC2_R2_503_0kOhm 0x1A
#define _LDC1101_TC2_R2_490_2kOhm 0x1B
#define _LDC1101_TC2_R2_477_4kOhm 0x1C
#define _LDC1101_TC2_R2_464_6kOhm 0x1D
#define _LDC1101_TC2_R2_451_9kOhm 0x1E
#define _LDC1101_TC2_R2_439_1kOhm 0x1F
#define _LDC1101_TC2_R2_426_4kOhm 0x20
#define _LDC1101_TC2_R2_413_6kOhm 0x21
#define _LDC1101_TC2_R2_400_8kOhm 0x22
#define _LDC1101_TC2_R2_388_1kOhm 0x23
#define _LDC1101_TC2_R2_375_3kOhm 0x24
#define _LDC1101_TC2_R2_362_5kOhm 0x25
#define _LDC1101_TC2_R2_349_7kOhm 0x26
#define _LDC1101_TC2_R2_337_0kOhm 0x27
#define _LDC1101_TC2_R2_324_2kOhm 0x28
#define _LDC1101_TC2_R2_311_4kOhm 0x29
#define _LDC1101_TC2_R2_298_7kOhm 0x2A
#define _LDC1101_TC2_R2_285_9kOhm 0x2B
#define _LDC1101_TC2_R2_273_1kOhm 0x2C
#define _LDC1101_TC2_R2_260_3kOhm 0x2D
#define _LDC1101_TC2_R2_247_6kOhm 0x2E
#define _LDC1101_TC2_R2_234_8kOhm 0x2F
#define _LDC1101_TC2_R2_222_0kOhm 0x30
#define _LDC1101_TC2_R2_209_3kOhm 0x31
#define _LDC1101_TC2_R2_196_5kOhm 0x32
#define _LDC1101_TC2_R2_183_7kOhm 0x33
#define _LDC1101_TC2_R2_170_9kOhm 0x34
#define _LDC1101_TC2_R2_158_2kOhm 0x35
#define _LDC1101_TC2_R2_145_4kOhm 0x36
#define _LDC1101_TC2_R2_132_7kOhm 0x37
#define _LDC1101_TC2_R2_119_9kOhm 0x38
#define _LDC1101_TC2_R2_107_1kOhm 0x39
#define _LDC1101_TC2_R2_94_3kOhm  0x3A
#define _LDC1101_TC2_R2_81_6kOhm  0x3B
#define _LDC1101_TC2_R2_68_8kOhm  0x3C
#define _LDC1101_TC2_R2_56_0kOhm  0x3D
#define _LDC1101_TC2_R2_43_3kOhm  0x3E
#define _LDC1101_TC2_R2_30_5kOhm  0x3F

/* Configure RP+L conversion interval (RW) */
/**
 * @brirf ����оƬ����Ƶ�ʵ����޺�ת���㷨����Ӧ�ӳ�
 * @feat  ���ٽ������?	            1MHz	        192 ��s �C 384 ��s
 *	  	  ����/��е���?	        800kHz �C 1MHz	    384 ��s �C 768 ��s
 *		  ����/�ȶ������ȼ��?	     500kHz	         1536 ��s �C 6144 ��s
 * @feat  �������� RP+L ģʽ
 * @reg   _LDC1101_REG_CFG_RP_L_CONVERSION_INTERVAL    0x04
 */
#define _LDC1101_DIG_CFG_MIN_FREQ_500kHz 0x00
#define _LDC1101_DIG_CFG_MIN_FREQ_1MHz   0x10
#define _LDC1101_DIG_CFG_MIN_FREQ_1_5MHz 0x20
#define _LDC1101_DIG_CFG_MIN_FREQ_2MHz   0x30
#define _LDC1101_DIG_CFG_MIN_FREQ_2_5MHz 0x40
#define _LDC1101_DIG_CFG_MIN_FREQ_3MHz   0x50
#define _LDC1101_DIG_CFG_MIN_FREQ_3_5MHz 0x60
#define _LDC1101_DIG_CFG_MIN_FREQ_4MHz   0x70
#define _LDC1101_DIG_CFG_MIN_FREQ_4_5MHz 0x80
#define _LDC1101_DIG_CFG_MIN_FREQ_5MHz   0x90
#define _LDC1101_DIG_CFG_MIN_FREQ_5_5MHz 0xA0
#define _LDC1101_DIG_CFG_MIN_FREQ_6MHz   0xB0
#define _LDC1101_DIG_CFG_MIN_FREQ_6_5MHz 0xC0
#define _LDC1101_DIG_CFG_MIN_FREQ_7MHz   0xD0
#define _LDC1101_DIG_CFG_MIN_FREQ_7_5MHz 0xE0
#define _LDC1101_DIG_CFG_MIN_FREQ_8MHz   0xF0

#define _LDC1101_DIG_CFG_RESP_TIME_192  0x02
#define _LDC1101_DIG_CFG_RESP_TIME_384  0x03
#define _LDC1101_DIG_CFG_RESP_TIME_768  0x04
#define _LDC1101_DIG_CFG_RESP_TIME_1536 0x05
#define _LDC1101_DIG_CFG_RESP_TIME_3072 0x06
#define _LDC1101_DIG_CFG_RESP_TIME_6144 0x07

/* Configure additional device settings (RW) */
/*
 * @brief ���ø��ӹ���
 * @reg   _LDC1101_REG_CFG_ADDITIONAL_DEVICE_SETTINGS    0x05
 */
#define _LDC1101_ALT_CFG_SHUTDOWN_ENABLE    0x02 // оƬ�ض�ģʽ�����_LDC1101_REG_CFG_POWER_STATE����
#define _LDC1101_ALT_CFG_SHUTDOWN_DISABLE   0x00
#define _LDC1101_ALT_CFG_L_OPTIMAL_DISABLED 0x00 //  L ͨ�����Ż�·����LHRģʽ���ã�
#define _LDC1101_ALT_CFG_L_OPTIMAL_ENABLE   0x01

/* Configure INTB reporting on SDO pin (RW) */
/**
 * @brief �Ƿ����� SDO ���ŵ� INTB ���渴�ù���
 * @reg   _LDC1101_REG_CFG_INTB_MODE    0x0A
 *        ������? bit7 ר�ſ���
 */
#define _LDC1101_INTB_MODE_DONT_REPORT_INTB_ON_SDO_PIN 0x00
#define _LDC1101_INTB_MODE_REPORT_INTB_ON_SDO_PIN      0x80

#define _LDC1101_INTB_MODE_REPORT_LHR_DATA_READY 0x20 // LHR ���ݸ�����ɾʹ����ж�?
#define _LDC1101_INTB_MODE_L_CONVERSION_TO_L_THRESHOLDS \
    0x10 // L ͨ������Խ����/�����޴���
#define _LDC1101_INTB_MODE_L_CONVERSION_TO_L_HIGH_THRESHOLDS \
    0x08                                               // L ͨ�����ݳ��������޴���
#define _LDC1101_INTB_MODE_REPORT_RP_L_DATA_READY 0x04 // RP+L ���ݸ�����ɾʹ����ж�?
#define _LDC1101_INTB_MODE_RP_CONVERSION_TO_L_THRESHOLDS \
    0x02 // RP ͨ������Խ�������޴���
#define _LDC1101_INTB_MODE_RP_CONVERSION_TO_L_HIGH_THRESHOLDS \
    0x01 // RP ͨ������Խ����/�����޴���

/* Configure Power State (RW) */
/**
 * @brief ���˯�ߡ��ض�ģʽ����
 * @feat  �ڳ�ʼ���׶Σ�������д�� SLEEP ģʽ��0x01�����úò���;
 *        Ȼ�����л��� ACTIVE ģʽ��0x00����ʼ������
 *        SHUTDOWN ģʽֻ������ȫ��ʹ��оƬʱ���Ƽ����롣
 * @reg   _LDC1101_REG_CFG_POWER_STATE    0x0B
 */
#define _LDC1101_FUNC_MODE_ACTIVE_CONVERSION_MODE 0x00
#define _LDC1101_FUNC_MODE_SLEEP_MODE             0x01
#define _LDC1101_FUNC_MODE_SHUTDOWN_MODE          0x02

/* High Resolution L Configuration (RW) */
/**
 * @brief ���� LHR ģʽ�µķ�Ƶ����Ӱ������ֱ��ʺ��ٶ�?
 * @feat  ���ú����������Ч�����踴�?
 * @feat  �� LHR_RCOUNT һ��������յĲ����ٶ���?��
 * @reg   _LDC1101_REG_CFG_LHR  0x34
 */
#define _LDC1101_LHR_CFG_FREQUENCY_NOT_DIVIDED  0x00
#define _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_2 0x01
#define _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_4 0x02
#define _LDC1101_LHR_CFG_FREQUENCY_DIVIDED_BY_8 0x03

typedef struct {
    uint8_t reg;
    uint8_t value;
} LDC_RegConfig_t;

typedef enum { LDC_ACTIVE = 0, LDC_SLEEP, LDC_SHUTDOWN } LDC_PowerState_t;

typedef enum { LDC_DEFAULT = 0, LDC_MODE_RPL, LDC_MODE_LHR } LDC_Mode_t;

typedef struct {
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    LDC_PowerState_t power_state;
    LDC_Mode_t mode;
} LDC_HandleTypeDef;

void ldc1101_writeByte(LDC_HandleTypeDef *hldc, uint8_t addr, uint8_t _data);
void ldc1101_writeBurst(LDC_HandleTypeDef *hldc, uint8_t start_addr, uint8_t *data, uint8_t len);
uint8_t ldc1101_readByte(LDC_HandleTypeDef *hldc, uint8_t addr);
void ldc1101_readBurst(LDC_HandleTypeDef *hldc, uint8_t start_addr, uint8_t *out_data, uint8_t len);
DEVICE_StatusTypeDef ldc1101_writeConfig(LDC_HandleTypeDef *hldc, const LDC_RegConfig_t *cfg, uint16_t cfg_size);
DEVICE_StatusTypeDef ldc1101_init(LDC_HandleTypeDef *hldc);
DEVICE_StatusTypeDef ldc1101_setPowerMode(LDC_HandleTypeDef *hldc, uint8_t mode);
void ldc1101_goTo_Lmode(LDC_HandleTypeDef *hldc);
void ldc1101_goTo_RPmode(LDC_HandleTypeDef *hldc);
uint16_t ldc1101_getRPData(LDC_HandleTypeDef *hldc);
uint16_t ldc1101_getLData(LDC_HandleTypeDef *hldc);
uint32_t ldc1101_getLHRData(LDC_HandleTypeDef *hldc);
uint16_t ldc1101_getLHRRCount(LDC_HandleTypeDef *hldc);
float ldc1101_getLHRSampleRate(LDC_HandleTypeDef *hldc);
bool ldc1101_setLHRSampleRate(LDC_HandleTypeDef *hldc, float sample_rate);
extern LDC_HandleTypeDef hldc1;

#ifdef __cplusplus
}
#endif

#endif /* LDC1101_DRIVER_H */
