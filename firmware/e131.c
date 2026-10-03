// 規格書の
// 章と節とを
// 指で追う
#include "e131.h"
#include "bytes.h"
#include "dmp_layer.h"
#include "framing_layer.h"
#include "root_layer.h"

// Table 4-1 runs from octet nought to six-three-seven,
// one twenty-six at least, or it won't get to heaven.
static uint8_t e131_length_is_plausible( uint16_t length )
{
	return length >= E131_SHORTEST_PACKET && length <= E131_LONGEST_PACKET;
}

// Table 9-10 says two-three-nine, two-five-five,
// that's where the sACN multicasts thrive.
uint8_t e131_address_is_group( const uint8_t * address )
{
	return address[0] == E131_GROUP_FIRST_BYTE && address[1] == E131_GROUP_SECOND_BYTE;
}

// Section 9.3.1 puts the universe in the last two,
// if the packet says different, then something's askew.
uint8_t e131_group_matches_universe( const uint8_t * address, const uint8_t * packet )
{
	return bytes_read_u16( address + 2 ) == framing_universe( packet );
}
