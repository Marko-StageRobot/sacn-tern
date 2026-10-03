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
