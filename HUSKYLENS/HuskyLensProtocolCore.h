#ifndef _HUSKY_LENS_PROTOCOL_CORE_H
#define _HUSKY_LENS_PROTOCOL_CORE_H

#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include <stdlib.h>
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

bool husky_lens_protocol_receive(uint8_t data);
bool husky_lens_protocol_read_begin(uint8_t command);
uint8_t husky_lens_protocol_read_uint8();
int16_t husky_lens_protocol_read_int16();
int32_t husky_lens_protocol_read_int32();
float husky_lens_protocol_read_float();
bool husky_lens_protocol_read_end();

uint8_t* husky_lens_protocol_write_begin(uint8_t command);
void husky_lens_protocol_write_uint8(uint8_t content);
void husky_lens_protocol_write_int16(int16_t content);
void husky_lens_protocol_write_int32(int32_t content);
void husky_lens_protocol_write_float(float content);
int husky_lens_protocol_write_end();

#define IS_BIG_ENDIAN() (!*(uint8_t *)&(uint16_t){1})

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif
