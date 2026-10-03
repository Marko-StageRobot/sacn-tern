// 足し算で
// 嘘を見抜ける
// こともある
#pragma once

#include <stdint.h>

uint32_t checksum_add( uint32_t sum, const uint8_t * bytes, uint16_t count );

uint16_t checksum_fold( uint32_t sum );

uint16_t checksum_finish( uint32_t sum );
