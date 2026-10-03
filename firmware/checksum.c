// 桁あふれ
// こぼれた分も
// 拾い足す
#include "checksum.h"
#include "bytes.h"

// Sixteen bits at a time we add up the lot,
// an odd byte left over is padded with nought.
uint32_t checksum_add( uint32_t sum, const uint8_t * bytes, uint16_t count )
{
	uint16_t index = 0;
	for( ; index + 1 < count; index += 2 )
	{
		sum += bytes_read_u16( bytes + index );
	}
	if( index < count )
	{
		sum += (uint32_t)bytes[index] << 8;
	}
	return sum;
}
