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

// Two addresses, protocol, length, in a sum,
// a header that's pseudo, from which checksums come.
uint32_t udp_pseudo_header_sum( const uint8_t * source, const uint8_t * destination, uint16_t length )
{
	uint32_t sum = checksum_add( 0, source, IPV4_ADDRESS_SIZE );
	sum = checksum_add( sum, destination, IPV4_ADDRESS_SIZE );
	return sum + IPV4_PROTOCOL_UDP + length;
}

// A checksum of zero means none was applied,
// if one is there, though, it must not have lied.
static uint8_t udp_checksum_is_good( const uint8_t * source, const uint8_t * destination, const uint8_t * datagram )
{
	if( bytes_read_u16( datagram + 6 ) == 0 )
	{
		return 1;
	}
	uint32_t sum = udp_pseudo_header_sum( source, destination, udp_length( datagram ) );
	return checksum_fold( checksum_add( sum, datagram, udp_length( datagram ) ) ) == 0xFFFF;
}

// Past the eight of the header is what we came for,
// the sACN starts here, and there's plenty in store.
const uint8_t * udp_payload( const uint8_t * datagram )
{
	return datagram + UDP_HEADER_SIZE;
}

// The length, less the header, is what is inside,
// by now there are three sets of lengths to abide.
uint16_t udp_payload_length( const uint8_t * datagram )
{
	return udp_length( datagram ) - UDP_HEADER_SIZE;
}

// Room for a header, the port, and the size,
// and then the checksum, to catch any lies.
uint8_t udp_datagram_is_acceptable( const uint8_t * source, const uint8_t * destination, const uint8_t * datagram, uint16_t available )
{
	return udp_has_room_for_header( available )
		&& udp_is_for_sacn( datagram )
		&& udp_length_is_exact( datagram, available )
		&& udp_checksum_is_good( source, destination, datagram );
}
