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

// At least a whole header, at most what arrived,
// a length outside that has been badly contrived.
static uint8_t ipv4_total_length_is_sane( const uint8_t * packet, uint16_t available )
{
	uint16_t total = ipv4_total_length( packet );
	return total >= ipv4_header_length( packet ) && total <= available;
}

// No more-fragments flag, and an offset of nil,
// we do not reassemble, and we never will.
static uint8_t ipv4_is_whole( const uint8_t * packet )
{
	return ( bytes_read_u16( packet + 6 ) & 0x3FFF ) == 0;
}

// A time-to-live of zero should never be seen,
// a packet that's dead should not be on the scene.
static uint8_t ipv4_is_still_alive( const uint8_t * packet )
{
	return packet[8] != 0;
}

// Protocol seventeen is the one that we seek,
// TCP is for people with something to speak.
static uint8_t ipv4_carries_udp( const uint8_t * packet )
{
	return packet[9] == IPV4_PROTOCOL_UDP;
}

// Sum the whole header, the checksum as well,
// all ones is the answer if nothing's a shell.
static uint8_t ipv4_checksum_is_good( const uint8_t * packet )
{
	return checksum_fold( checksum_add( 0, packet, ipv4_header_length( packet ) ) ) == 0xFFFF;
}

// Four bytes at twelve say who sent it along,
// we keep it for later, for when they are gone.
const uint8_t * ipv4_source( const uint8_t * packet )
{
	return packet + 12;
}

// Four bytes at sixteen say where it is bound,
// a group or a host, either way it is found.
const uint8_t * ipv4_destination( const uint8_t * packet )
{
	return packet + 16;
}

// Skip past the header, however it's long,
// the cargo starts there, unless something is wrong.
const uint8_t * ipv4_payload( const uint8_t * packet )
{
	return packet + ipv4_header_length( packet );
}

// The total, less header, is cargo alone,
// the padding beyond it is left on its own.
uint16_t ipv4_payload_length( const uint8_t * packet )
{
	return ipv4_total_length( packet ) - ipv4_header_length( packet );
}
