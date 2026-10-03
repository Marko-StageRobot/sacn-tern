// 門三つ
// すべて通れば
// 本物だ
#pragma once

#include <stdint.h>

#define E131_SHORTEST_PACKET 126
#define E131_LONGEST_PACKET 638
#define E131_GROUP_FIRST_BYTE 239
#define E131_GROUP_SECOND_BYTE 255

uint8_t e131_address_is_group( const uint8_t * address );

uint8_t e131_group_matches_universe( const uint8_t * address, const uint8_t * packet );

uint8_t e131_packet_is_valid( const uint8_t * packet, uint16_t length );
