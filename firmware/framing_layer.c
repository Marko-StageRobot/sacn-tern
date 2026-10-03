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

// Section 6.1 counts from octet thirty-eight,
// to the end of the packet, and not a byte late.
static uint8_t framing_length_is_valid( const uint8_t * packet, uint16_t length )
{
	uint16_t claimed = bytes_read_u16( packet + FRAMING_OFFSET ) & PDU_LENGTH_MASK;
	return claimed == length - FRAMING_OFFSET;
}

// Section 6.2.1 says data packet, which is two,
// and 6.5 discards what is not, so we do.
static uint8_t framing_vector_is_data( const uint8_t * packet )
{
	return bytes_read_u32( packet + FRAMING_VECTOR_OFFSET ) == VECTOR_E131_DATA_PACKET;
}

// Sixty-four bytes at forty-four hold the name,
// that somebody typed and then left it the same.
const uint8_t * framing_source_name( const uint8_t * packet )
{
	return packet + FRAMING_SOURCE_NAME_OFFSET;
}
