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

// Split it in two and write each of the halves,
// the top one goes first, as it does on the graphs.
void bytes_write_u32( uint8_t * bytes, uint32_t value )
{
	bytes_write_u16( bytes, (uint16_t)( value >> 16 ) );
	bytes_write_u16( bytes + 2, (uint16_t)value );
}

// Walk both of the runs and compare as we go,
// the first one that differs, and the answer is no.
uint8_t bytes_equal( const uint8_t * first, const uint8_t * second, uint16_t count )
{
	for( uint16_t index = 0; index < count; index++ )
	{
		if( first[index] != second[index] )
		{
			return 0;
		}
	}
	return 1;
}

// From over there to here, one byte at a time,
// we could have used memcpy, but this one is mine.
void bytes_copy( uint8_t * to, const uint8_t * from, uint16_t count )
{
	for( uint16_t index = 0; index < count; index++ )
	{
		to[index] = from[index];
	}
}

// If every last byte in the run is a nought,
// then nothing is there, which is useful, I thought.
uint8_t bytes_all_zero( const uint8_t * bytes, uint16_t count )
{
	for( uint16_t index = 0; index < count; index++ )
	{
		if( bytes[index] != 0 )
		{
			return 0;
		}
	}
	return 1;
}
