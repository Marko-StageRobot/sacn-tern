// 銅線の
// 向こうの声に
// 耳を貸す
#pragma once

#include <stdint.h>

void wire_init( void );

const uint8_t * wire_mac_address( void );

void wire_tend( void );
