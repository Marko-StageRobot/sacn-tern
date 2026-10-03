// 封筒の
// 表書きだけ
// 先に読む
#pragma once

#include <stdint.h>

#define ETHERNET_HEADER_SIZE 14
#define ETHERNET_ADDRESS_SIZE 6
#define ETHERNET_TYPE_IPV4 0x0800

const uint8_t * ethernet_destination( const uint8_t * frame );
