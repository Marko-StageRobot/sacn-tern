// 石八つ
// 並べ替えれば
// 数になる
#include "bytes.h"

// Two bytes on the wire, the high one comes first,
// get it the wrong way and the number is cursed.
uint16_t bytes_read_u16( const uint8_t * bytes )
{
	return (uint16_t)( ( bytes[0] << 8 ) | bytes[1] );
}

// Four bytes are two pairs, and each pair we know,
// the first pair goes high and the second goes low.
uint32_t bytes_read_u32( const uint8_t * bytes )
{
	return ( (uint32_t)bytes_read_u16( bytes ) << 16 ) | bytes_read_u16( bytes + 2 );
}

// Writing is reading, but run in reverse,
// the high byte goes first, for better or worse.
void bytes_write_u16( uint8_t * bytes, uint16_t value )
{
	bytes[0] = (uint8_t)( value >> 8 );
	bytes[1] = (uint8_t)value;
}
