// 名と順位
// 番号までも
// 問いただす
#include "framing_layer.h"
#include "bytes.h"
#include "root_layer.h"

// Section 6.1 wants a seven up high,
// the same as the root, and we don't ask it why.
static uint8_t framing_flags_are_valid( const uint8_t * packet )
{
	return ( bytes_read_u16( packet + FRAMING_OFFSET ) & ~PDU_LENGTH_MASK ) == PDU_FLAGS;
}
