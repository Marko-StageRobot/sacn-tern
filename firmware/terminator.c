// 三度だけ
// 名を騙っては
// 幕を引く
#include "terminator.h"
#include "bytes.h"
#include "checksum.h"
#include "dmp_layer.h"
#include "e131.h"
#include "ethernet.h"
#include "udp.h"
#include "wire.h"

#define TERMINATOR_IPV4_OFFSET ETHERNET_HEADER_SIZE
#define TERMINATOR_UDP_OFFSET ( TERMINATOR_IPV4_OFFSET + IPV4_MINIMUM_HEADER_SIZE )
#define TERMINATOR_E131_OFFSET ( TERMINATOR_UDP_OFFSET + UDP_HEADER_SIZE )
#define TERMINATOR_FRAME_SIZE ( TERMINATOR_E131_OFFSET + E131_SHORTEST_PACKET )
#define TERMINATOR_UDP_LENGTH ( UDP_HEADER_SIZE + E131_SHORTEST_PACKET )
#define TERMINATOR_IPV4_LENGTH ( IPV4_MINIMUM_HEADER_SIZE + TERMINATOR_UDP_LENGTH )
#define TERMINATOR_REPEATS 3
#define TERMINATOR_TIME_TO_LIVE 1

static uint8_t terminator_frame[TERMINATOR_FRAME_SIZE];
static uint16_t terminator_identification;

// Past the fourteen of Ethernet, IP begins,
// we build it by hand, and atone for our sins.
static uint8_t * terminator_ipv4( void )
{
	return terminator_frame + TERMINATOR_IPV4_OFFSET;
}

// Twenty bytes further and UDP's there,
// eight little bytes, with a checksum to spare.
static uint8_t * terminator_udp( void )
{
	return terminator_frame + TERMINATOR_UDP_OFFSET;
}

// Then comes the packet the standard describes,
// one twenty-six bytes, and no room for bribes.
static uint8_t * terminator_e131( void )
{
	return terminator_frame + TERMINATOR_E131_OFFSET;
}

// Table 9-10: two-three-nine, two-five-five,
// then high byte, then low, and the address is live.
static void terminator_write_group( uint8_t * address, uint16_t universe )
{
	address[0] = E131_GROUP_FIRST_BYTE;
	address[1] = E131_GROUP_SECOND_BYTE;
	bytes_write_u16( address + 2, universe );
}

// The group's MAC up front, then our own, then the type,
// our own is the one honest part of this hype.
static void terminator_write_ethernet( uint16_t universe )
{
	terminator_frame[0] = 0x01;
	terminator_frame[1] = 0x00;
	terminator_frame[2] = 0x5E;
	terminator_frame[3] = E131_GROUP_SECOND_BYTE & 0x7F;
	bytes_write_u16( terminator_frame + 4, universe );
	bytes_copy( terminator_frame + ETHERNET_ADDRESS_SIZE, wire_mac_address(), ETHERNET_ADDRESS_SIZE );
	bytes_write_u16( terminator_frame + 12, ETHERNET_TYPE_IPV4 );
}

// We have no address, so borrow the one
// of the source that fell silent, whose talking is done.
static void terminator_write_ipv4( const ledger_entry_t * entry )
{
	uint8_t * header = terminator_ipv4();
	header[0] = 0x45;
	header[1] = 0x00;
	bytes_write_u16( header + 2, TERMINATOR_IPV4_LENGTH );
	bytes_write_u16( header + 4, terminator_identification++ );
	bytes_write_u16( header + 6, 0x4000 );
	header[8] = TERMINATOR_TIME_TO_LIVE;
	header[9] = IPV4_PROTOCOL_UDP;
	bytes_write_u16( header + 10, 0 );
	bytes_copy( header + 12, entry->address, IPV4_ADDRESS_SIZE );
	terminator_write_group( header + 16, entry->universe );
	bytes_write_u16( header + 10, checksum_finish( checksum_add( 0, header, IPV4_MINIMUM_HEADER_SIZE ) ) );
}
