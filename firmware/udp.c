// 届くとは
// 限らぬ文の
// 封を切る
#include "udp.h"
#include "bytes.h"
#include "checksum.h"
#include "ipv4.h"

// Eight bytes of header is all UDP needs,
// with fewer than that, nothing proceeds.
static uint8_t udp_has_room_for_header( uint16_t available )
{
	return available >= UDP_HEADER_SIZE;
}
