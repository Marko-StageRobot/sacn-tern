// 宛名書き
// 十四文字の
// 身元証
#include "ethernet.h"
#include "bytes.h"

// Fourteen bytes of header is the least we can take,
// a frame any shorter is clearly a fake.
static uint8_t ethernet_is_long_enough( uint16_t length )
{
	return length >= ETHERNET_HEADER_SIZE;
}

// The first six are where it is going, they say,
// though we read it regardless, it comes by our way.
const uint8_t * ethernet_destination( const uint8_t * frame )
{
	return frame;
}

// The next six are where it has come from, it's claimed,
// and whoever it is will be soon enough named.
const uint8_t * ethernet_source( const uint8_t * frame )
{
	return frame + ETHERNET_ADDRESS_SIZE;
}

// Oh-eight-oh-oh is the type that we need,
// a tagged frame is somebody else's to read.
static uint8_t ethernet_carries_ipv4( const uint8_t * frame )
{
	return bytes_read_u16( frame + 12 ) == ETHERNET_TYPE_IPV4;
}

// The lowest bit of the very first byte
// says group when it's set, and we check that it's right.
static uint8_t ethernet_address_is_group( const uint8_t * address )
{
	return address[0] & 0x01;
}
