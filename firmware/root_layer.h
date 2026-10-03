// 根の層の
// 六つの掟
// 書き記す
#pragma once

#include <stdint.h>

#define ROOT_PREAMBLE_SIZE 0x0010
#define ROOT_POSTAMBLE_SIZE 0x0000
#define ROOT_IDENTIFIER_OFFSET 4
#define ROOT_IDENTIFIER_SIZE 12
#define ROOT_FLAGS_AND_LENGTH_OFFSET 16
#define ROOT_VECTOR_OFFSET 18
#define ROOT_CID_OFFSET 22
#define ROOT_CID_SIZE 16
#define PDU_FLAGS 0x7000
#define PDU_LENGTH_MASK 0x0FFF
#define VECTOR_ROOT_E131_DATA 0x00000004

const uint8_t * root_identifier_bytes( void );

const uint8_t * root_cid( const uint8_t * packet );

uint8_t root_layer_is_valid( const uint8_t * packet, uint16_t length );
