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
