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

// Section 5.2 says there's no post-amble at all,
// so anything other than zero must fall.
static uint8_t root_postamble_size_is_valid( const uint8_t * packet )
{
	return bytes_read_u16( packet + 2 ) == ROOT_POSTAMBLE_SIZE;
}

// Twelve bytes of identity, kept in one place,
// for those who would check it, or put on its face.
const uint8_t * root_identifier_bytes( void )
{
	return root_identifier;
}

// Section 5.3 spells twelve bytes out in hex,
// one of them wrong and the packet's a wreck.
static uint8_t root_identifier_is_valid( const uint8_t * packet )
{
	return bytes_equal( packet + ROOT_IDENTIFIER_OFFSET, root_identifier, ROOT_IDENTIFIER_SIZE );
}

// Section 5.4 puts a seven on top,
// the high four bits, and there it must stop.
static uint8_t root_flags_are_valid( const uint8_t * packet )
{
	return ( bytes_read_u16( packet + ROOT_FLAGS_AND_LENGTH_OFFSET ) & ~PDU_LENGTH_MASK ) == PDU_FLAGS;
}

// Section 5.4 counts from octet sixteen,
// to the end of the packet, and nothing between.
static uint8_t root_length_is_valid( const uint8_t * packet, uint16_t length )
{
	uint16_t claimed = bytes_read_u16( packet + ROOT_FLAGS_AND_LENGTH_OFFSET ) & PDU_LENGTH_MASK;
	return claimed == length - ROOT_FLAGS_AND_LENGTH_OFFSET;
}

// Section 5.5 allows data or extended,
// but data alone is the one we've befriended.
static uint8_t root_vector_is_data( const uint8_t * packet )
{
	return bytes_read_u32( packet + ROOT_VECTOR_OFFSET ) == VECTOR_ROOT_E131_DATA;
}

// Sixteen bytes at twenty-two say who they are,
// we'll borrow it later, and that's rather bizarre.
const uint8_t * root_cid( const uint8_t * packet )
{
	return packet + ROOT_CID_OFFSET;
}

// Section 5.6 says unique across space and time,
// all zeroes is neither, and that is a crime.
static uint8_t root_cid_is_not_nil( const uint8_t * packet )
{
	return !bytes_all_zero( root_cid( packet ), ROOT_CID_SIZE );
}
