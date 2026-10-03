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

// Section 9.1.1: the port's fifty-five sixty-eight,
// both ends, for neatness, and checksum can wait.
static void terminator_write_udp( void )
{
	uint8_t * header = terminator_udp();
	bytes_write_u16( header, ACN_SDT_MULTICAST_PORT );
	bytes_write_u16( header + 2, ACN_SDT_MULTICAST_PORT );
	bytes_write_u16( header + 4, TERMINATOR_UDP_LENGTH );
	bytes_write_u16( header + 6, 0 );
}

// Section 5, in full, with a CID that we took,
// the standard says unique, and we threw out the book.
static void terminator_write_root( const ledger_entry_t * entry )
{
	uint8_t * packet = terminator_e131();
	bytes_write_u16( packet, ROOT_PREAMBLE_SIZE );
	bytes_write_u16( packet + 2, ROOT_POSTAMBLE_SIZE );
	bytes_copy( packet + ROOT_IDENTIFIER_OFFSET, root_identifier_bytes(), ROOT_IDENTIFIER_SIZE );
	bytes_write_u16( packet + ROOT_FLAGS_AND_LENGTH_OFFSET, PDU_FLAGS | ( E131_SHORTEST_PACKET - ROOT_FLAGS_AND_LENGTH_OFFSET ) );
	bytes_write_u32( packet + ROOT_VECTOR_OFFSET, VECTOR_ROOT_E131_DATA );
	bytes_copy( packet + ROOT_CID_OFFSET, entry->cid, ROOT_CID_SIZE );
}

// Section 6.2.6, bit six, Stream_Terminated,
// their name and their priority, all duplicated.
static void terminator_write_framing( ledger_entry_t * entry )
{
	uint8_t * packet = terminator_e131();
	bytes_write_u16( packet + FRAMING_OFFSET, PDU_FLAGS | ( E131_SHORTEST_PACKET - FRAMING_OFFSET ) );
	bytes_write_u32( packet + FRAMING_VECTOR_OFFSET, VECTOR_E131_DATA_PACKET );
	bytes_copy( packet + FRAMING_SOURCE_NAME_OFFSET, entry->source_name, FRAMING_SOURCE_NAME_SIZE );
	packet[FRAMING_PRIORITY_OFFSET] = entry->priority;
	bytes_write_u16( packet + FRAMING_SYNC_ADDRESS_OFFSET, 0 );
	packet[FRAMING_SEQUENCE_OFFSET] = ledger_next_sequence( entry );
	packet[FRAMING_OPTIONS_OFFSET] = FRAMING_OPTION_STREAM_TERMINATED;
	bytes_write_u16( packet + FRAMING_UNIVERSE_OFFSET, entry->universe );
}

// Section 7, with only a START code inside,
// 6.2.6 says values are ignored, so we've none to provide.
static void terminator_write_dmp( void )
{
	uint8_t * packet = terminator_e131();
	bytes_write_u16( packet + DMP_OFFSET, PDU_FLAGS | ( E131_SHORTEST_PACKET - DMP_OFFSET ) );
	packet[DMP_VECTOR_OFFSET] = VECTOR_DMP_SET_PROPERTY;
	packet[DMP_ADDRESS_TYPE_OFFSET] = DMP_ADDRESS_AND_DATA_TYPE;
	bytes_write_u16( packet + DMP_FIRST_ADDRESS_OFFSET, DMP_FIRST_ADDRESS );
	bytes_write_u16( packet + DMP_INCREMENT_OFFSET, DMP_INCREMENT );
	bytes_write_u16( packet + DMP_COUNT_OFFSET, DMP_COUNT_MIN );
	packet[DMP_VALUES_OFFSET] = 0x00;
}
