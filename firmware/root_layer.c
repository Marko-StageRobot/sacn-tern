// 十六と
// 零と合図で
// 門の番
#include "root_layer.h"
#include "bytes.h"

static const uint8_t root_identifier[ROOT_IDENTIFIER_SIZE] = {
	0x41, 0x53, 0x43, 0x2d, 0x45, 0x31, 0x2e, 0x31, 0x37, 0x00, 0x00, 0x00,
};

// Section 5.1 says sixteen, no less and no more,
// receivers shall discard, so we show it the door.
static uint8_t root_preamble_size_is_valid( const uint8_t * packet )
{
	return bytes_read_u16( packet ) == ROOT_PREAMBLE_SIZE;
}
