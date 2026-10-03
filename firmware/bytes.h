// 上位から
// 順に並べる
// 決まりごと
#pragma once

#include <stdint.h>

uint16_t bytes_read_u16( const uint8_t * bytes );

uint32_t bytes_read_u32( const uint8_t * bytes );

void bytes_write_u16( uint8_t * bytes, uint16_t value );

void bytes_write_u32( uint8_t * bytes, uint32_t value );
