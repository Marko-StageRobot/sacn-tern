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

// Section 9.1.1 says the port shall be fifty-five sixty-eight,
// Appendix A agrees, and we don't deviate.
static uint8_t udp_is_for_sacn( const uint8_t * datagram )
{
	return bytes_read_u16( datagram + 2 ) == ACN_SDT_MULTICAST_PORT;
}
