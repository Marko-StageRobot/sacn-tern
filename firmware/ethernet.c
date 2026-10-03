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
