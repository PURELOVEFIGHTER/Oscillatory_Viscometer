#ifndef PACK_H
#define PACK_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum { PACK_OK = 0, PACK_ERROR } PACK_StatusTypeDef;

/* 通用写入工具 */
PACK_StatusTypeDef Pack_WriteU8(uint8_t *buf, uint16_t buf_len, uint16_t *offset, uint8_t value);
PACK_StatusTypeDef Pack_WriteU16LE(uint8_t *buf, uint16_t buf_len, uint16_t *offset, uint16_t value);
PACK_StatusTypeDef Pack_WriteU32LE(uint8_t *buf, uint16_t buf_len, uint16_t *offset, uint32_t value);
PACK_StatusTypeDef Pack_WriteFloatLE(uint8_t *buf, uint16_t buf_len, uint16_t *offset, float value);

/* 具体业务帧 */
// [帧头 0xFF][LHR 4B][Freq 4B][Duty 1B][Level 1B][帧尾 0xAA]
PACK_StatusTypeDef Pack_MeasurementFrame(uint32_t lhr_data, float freq, float duty_ratio, bool exciting_level,
                                         uint8_t *frame, uint16_t frame_len);

PACK_StatusTypeDef Pack_CalibrationFrame(uint16_t position_um, float mean, float var, uint8_t *frame,
                                         uint16_t frame_len);

#ifdef __cplusplus
}
#endif
#endif /* PACK_H */