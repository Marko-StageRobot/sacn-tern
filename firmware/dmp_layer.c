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
