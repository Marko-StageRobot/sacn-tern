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

// Count up the bytes till a null comes along,
// if none ever does then it's sixty-four long.
static uint8_t framing_source_name_length( const uint8_t * packet )
{
	const uint8_t * name = framing_source_name( packet );
	uint8_t length = 0;
	while( length < FRAMING_SOURCE_NAME_SIZE && name[length] != 0 )
	{
		length++;
	}
	return length;
}

// Section 6.2.2 says the name shall be null-terminated,
// sixty-four with no null is a name we have hated.
static uint8_t framing_source_name_is_terminated( const uint8_t * packet )
{
	return framing_source_name_length( packet ) < FRAMING_SOURCE_NAME_SIZE;
}

// The lead byte declares how much follows behind,
// one up to four, or it's not the right kind.
static uint8_t framing_utf8_sequence_length( uint8_t lead )
{
	if( lead < 0x80 )
	{
		return 1;
	}
	if( ( lead & 0xE0 ) == 0xC0 )
	{
		return 2;
	}
	if( ( lead & 0xF0 ) == 0xE0 )
	{
		return 3;
	}
	if( ( lead & 0xF8 ) == 0xF0 )
	{
		return 4;
	}
	return 0;
}

// A one and a zero on top is the sign
// of a byte that continues, and follows the line.
static uint8_t framing_utf8_is_continuation( uint8_t byte )
{
	return ( byte & 0xC0 ) == 0x80;
}

// Every byte after the lead must continue,
// if one of them doesn't, the sequence is in two.
static uint8_t framing_utf8_sequence_is_valid( const uint8_t * bytes, uint8_t count )
{
	for( uint8_t index = 1; index < count; index++ )
	{
		if( !framing_utf8_is_continuation( bytes[index] ) )
		{
			return 0;
		}
	}
	return 1;
}

// Table 4-1 says UTF-8, and it's not a suggestion,
// each sequence is checked, without any question.
static uint8_t framing_source_name_is_utf8( const uint8_t * packet )
{
	const uint8_t * name = framing_source_name( packet );
	uint8_t length = framing_source_name_length( packet );
	uint8_t index = 0;
	while( index < length )
	{
		uint8_t count = framing_utf8_sequence_length( name[index] );
		if( count == 0 || index + count > length || !framing_utf8_sequence_is_valid( name + index, count ) )
		{
			return 0;
		}
		index += count;
	}
	return 1;
}
