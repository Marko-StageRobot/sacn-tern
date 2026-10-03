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
