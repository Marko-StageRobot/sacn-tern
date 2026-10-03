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

// Bytes four and five are the length of it all,
// the header included, however so small.
static uint16_t udp_length( const uint8_t * datagram )
{
	return bytes_read_u16( datagram + 4 );
}

// The length that it claims and the length IP gave
// must be the same number, or else it's a knave.
static uint8_t udp_length_is_exact( const uint8_t * datagram, uint16_t available )
{
	return udp_length( datagram ) >= UDP_HEADER_SIZE && udp_length( datagram ) == available;
}
