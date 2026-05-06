#include "pack.h"
#include "stddef.h"
#include "string.h"

#define PACK_FRAME_HEAD 0xFFU
#define PACK_FRAME_TAIL 0xAAU

PACK_StatusTypeDef Pack_WriteU8(uint8_t *buf, uint16_t buf_len, uint16_t *offset, uint8_t value) {
    if ((buf == NULL) || (offset == NULL) || (*offset >= buf_len)) {
        return PACK_ERROR;
    }

    buf[*offset] = value;
    (*offset)++;
    return PACK_OK;
}

PACK_StatusTypeDef Pack_WriteU16LE(uint8_t *buf, uint16_t buf_len, uint16_t *offset, uint16_t value) {
    if ((buf == NULL) || (offset == NULL) || ((*offset + 2U) > buf_len)) {
        return PACK_ERROR;
    }

    buf[*offset + 0U] = (uint8_t)(value);
    buf[*offset + 1U] = (uint8_t)(value >> 8);
    *offset += 2U;
    return PACK_OK;
}

PACK_StatusTypeDef Pack_WriteU32LE(uint8_t *buf, uint16_t buf_len, uint16_t *offset, uint32_t value) {
    if ((buf == NULL) || (offset == NULL) || ((*offset + 4U) > buf_len)) {
        return PACK_ERROR;
    }

    buf[*offset + 0U] = (uint8_t)(value);
    buf[*offset + 1U] = (uint8_t)(value >> 8);
    buf[*offset + 2U] = (uint8_t)(value >> 16);
    buf[*offset + 3U] = (uint8_t)(value >> 24);
    *offset += 4U;
    return PACK_OK;
}

PACK_StatusTypeDef Pack_WriteFloatLE(uint8_t *buf, uint16_t buf_len, uint16_t *offset, float value) {
    if ((buf == NULL) || (offset == NULL) || ((*offset + sizeof(float)) > buf_len)) {
        return PACK_ERROR;
    }

    memcpy(&buf[*offset], &value, sizeof(float));
    *offset += (uint16_t)sizeof(float);
    return PACK_OK;
}

PACK_StatusTypeDef Pack_MeasurementFrame(uint32_t lhr_data, float freq, float duty_ratio, bool exciting_level,
                                         uint8_t *frame, uint16_t frame_len) {
    if (frame == NULL || freq < 0.0f || duty_ratio < 0.0f || duty_ratio > 100.0f)
        return PACK_ERROR;

    uint16_t offset = 0U;
    uint32_t freq_scaled;

    freq_scaled = (uint32_t)(freq * 100.0f);

    if (Pack_WriteU8(frame, frame_len, &offset, PACK_FRAME_HEAD) != PACK_OK)
        return PACK_ERROR;
    if (Pack_WriteU32LE(frame, frame_len, &offset, lhr_data) != PACK_OK)
        return PACK_ERROR;
    if (Pack_WriteU32LE(frame, frame_len, &offset, freq_scaled) != PACK_OK)
        return PACK_ERROR;
    if (Pack_WriteU8(frame, frame_len, &offset, (uint8_t)duty_ratio) != PACK_OK)
        return PACK_ERROR;
    if (Pack_WriteU8(frame, frame_len, &offset, exciting_level ? 1U : 0U) != PACK_OK)
        return PACK_ERROR;
    if (Pack_WriteU8(frame, frame_len, &offset, PACK_FRAME_TAIL) != PACK_OK)
        return PACK_ERROR;

    return PACK_OK;
}

PACK_StatusTypeDef Pack_CalibrationFrame(uint16_t position_um, float mean, float var, uint8_t *frame,
                                         uint16_t frame_len) {
    if (frame == NULL)
        return PACK_ERROR;

    uint16_t offset = 0U;

    if (Pack_WriteU16LE(frame, frame_len, &offset, position_um) != PACK_OK)
        return PACK_ERROR;
    if (Pack_WriteFloatLE(frame, frame_len, &offset, mean) != PACK_OK)
        return PACK_ERROR;
    if (Pack_WriteFloatLE(frame, frame_len, &offset, var) != PACK_OK)
        return PACK_ERROR;
    if (Pack_WriteU8(frame, frame_len, &offset, PACK_FRAME_TAIL) != PACK_OK)
        return PACK_ERROR;

    return PACK_OK;
}