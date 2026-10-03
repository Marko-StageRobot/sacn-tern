// 二十個の
// バイト疑い
// 尽くす夜
#include "ipv4.h"
#include "bytes.h"
#include "checksum.h"

// Twenty bytes of header is the least there can be,
// with fewer than that there is nothing to see.
static uint8_t ipv4_has_room_for_header( uint16_t available )
{
	return available >= IPV4_MINIMUM_HEADER_SIZE;
}

// The top half of byte zero must be a four,
// version six is a thing that we choose to ignore.
static uint8_t ipv4_version_is_four( const uint8_t * packet )
{
	return ( packet[0] >> 4 ) == 4;
}

// The bottom half counts up the header in words,
// times four gives us bytes, which is less for the birds.
uint16_t ipv4_header_length( const uint8_t * packet )
{
	return (uint16_t)( ( packet[0] & 0x0F ) * 4 );
}

// No less than twenty, no more than we've got,
// a header outside that is one we trust not.
static uint8_t ipv4_header_length_is_sane( const uint8_t * packet, uint16_t available )
{
	uint16_t header = ipv4_header_length( packet );
	return header >= IPV4_MINIMUM_HEADER_SIZE && header <= available;
}

// Bytes two and three are the length of the whole,
// header and cargo, from pole unto pole.
uint16_t ipv4_total_length( const uint8_t * packet )
{
	return bytes_read_u16( packet + 2 );
}
