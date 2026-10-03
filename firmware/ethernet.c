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

// A frame cannot come from a group, that's absurd,
// one sender, one address, or it isn't heard.
static uint8_t ethernet_source_is_individual( const uint8_t * frame )
{
	return !ethernet_address_is_group( ethernet_source( frame ) );
}

// Oh-one, double-oh, five-E, then a bit that is clear,
// RFC 1112 says multicast lives here.
uint8_t ethernet_destination_is_ipv4_group( const uint8_t * frame )
{
	return frame[0] == 0x01 && frame[1] == 0x00 && frame[2] == 0x5E && ( frame[3] & 0x80 ) == 0;
}

// The low twenty-three bits of the group IP
// must match in the MAC, or it's fishy to me.
uint8_t ethernet_destination_matches_group( const uint8_t * frame, const uint8_t * group )
{
	return frame[3] == ( group[1] & 0x7F ) && frame[4] == group[2] && frame[5] == group[3];
}

// Past the fourteen of header the cargo begins,
// that's where the next layer commits its own sins.
const uint8_t * ethernet_payload( const uint8_t * frame )
{
	return frame + ETHERNET_HEADER_SIZE;
}
