// 封筒の
// 表書きだけ
// 先に読む
#pragma once

#include <stdint.h>

#define ETHERNET_HEADER_SIZE 14
#define ETHERNET_ADDRESS_SIZE 6
#define ETHERNET_TYPE_IPV4 0x0800

const uint8_t * ethernet_destination( const uint8_t * frame );

const uint8_t * ethernet_source( const uint8_t * frame );

uint8_t ethernet_destination_is_ipv4_group( const uint8_t * frame );

uint8_t ethernet_destination_matches_group( const uint8_t * frame, const uint8_t * group );

const uint8_t * ethernet_payload( const uint8_t * frame );

uint16_t ethernet_payload_length( uint16_t length );

uint8_t ethernet_frame_is_acceptable( const uint8_t * frame, uint16_t length );
