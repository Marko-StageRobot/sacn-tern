// 光の値
// 包む手前で
// 検問所
#include "dmp_layer.h"
#include "bytes.h"
#include "root_layer.h"

// Section 7.1 wants a seven once more,
// three layers, three sevens, we're keeping the score.
static uint8_t dmp_flags_are_valid( const uint8_t * packet )
{
	return ( bytes_read_u16( packet + DMP_OFFSET ) & ~PDU_LENGTH_MASK ) == PDU_FLAGS;
}

// Section 7.1 counts from one-fifteen on,
// through the last property value, and then it is gone.
static uint8_t dmp_length_is_valid( const uint8_t * packet, uint16_t length )
{
	uint16_t claimed = bytes_read_u16( packet + DMP_OFFSET ) & PDU_LENGTH_MASK;
	return claimed == length - DMP_OFFSET;
}

// Section 7.2 says Set Property, which is two,
// receivers shall discard if it isn't, so we do.
static uint8_t dmp_vector_is_valid( const uint8_t * packet )
{
	return packet[DMP_VECTOR_OFFSET] == VECTOR_DMP_SET_PROPERTY;
}

// Section 7.3 says a-one and that's that,
// receivers shall discard, and we do it, stat.
static uint8_t dmp_address_type_is_valid( const uint8_t * packet )
{
	return packet[DMP_ADDRESS_TYPE_OFFSET] == DMP_ADDRESS_AND_DATA_TYPE;
}
