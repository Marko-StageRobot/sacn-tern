// 約束を
// しない便りの
// 潔さ
#pragma once

#include <stdint.h>

#define UDP_HEADER_SIZE 8
#define ACN_SDT_MULTICAST_PORT 5568

uint32_t udp_pseudo_header_sum( const uint8_t * source, const uint8_t * destination, uint16_t length );
