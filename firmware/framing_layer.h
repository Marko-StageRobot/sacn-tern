// 額縁の
// 寸法表を
// 貼っておく
#pragma once

#include <stdint.h>

#define FRAMING_OFFSET 38
#define FRAMING_VECTOR_OFFSET 40
#define FRAMING_SOURCE_NAME_OFFSET 44
#define FRAMING_SOURCE_NAME_SIZE 64
#define FRAMING_PRIORITY_OFFSET 108
#define FRAMING_SYNC_ADDRESS_OFFSET 109
#define FRAMING_SEQUENCE_OFFSET 111
#define FRAMING_OPTIONS_OFFSET 112
#define FRAMING_UNIVERSE_OFFSET 113
#define FRAMING_PRIORITY_MAX 200
#define FRAMING_UNIVERSE_MIN 1
#define FRAMING_UNIVERSE_MAX 63999
#define FRAMING_OPTION_STREAM_TERMINATED 0x40
#define FRAMING_OPTION_RESERVED 0x1F
#define VECTOR_E131_DATA_PACKET 0x00000002

const uint8_t * framing_source_name( const uint8_t * packet );

uint8_t framing_priority( const uint8_t * packet );
