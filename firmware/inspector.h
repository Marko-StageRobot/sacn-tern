// 検問は
// 窓口四つ
// 順番に
#pragma once

#include <stdint.h>

const uint8_t * inspector_find_sacn( const uint8_t * frame, uint16_t length, const uint8_t ** address );
