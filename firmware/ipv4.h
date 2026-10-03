// 住所録
// 四つの数で
// 世界分け
#pragma once

#include <stdint.h>

#define IPV4_MINIMUM_HEADER_SIZE 20
#define IPV4_ADDRESS_SIZE 4
#define IPV4_PROTOCOL_UDP 17

uint16_t ipv4_header_length( const uint8_t * packet );

uint16_t ipv4_total_length( const uint8_t * packet );
