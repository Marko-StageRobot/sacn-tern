// 皮を剥ぎ
// また皮を剥ぎ
// 芯を見る
#include "inspector.h"
#include "e131.h"
#include "ethernet.h"
#include "ipv4.h"
#include "udp.h"

// The outermost layer comes off with a peel,
// and inside is IP, if the frame is for real.
static const uint8_t * inspector_open_ethernet( const uint8_t * frame, uint16_t length )
{
	return ethernet_frame_is_acceptable( frame, length ) ? ethernet_payload( frame ) : 0;
}

// The next layer in is the twenty-byte kind,
// and under it, UDP's what we should find.
static const uint8_t * inspector_open_ipv4( const uint8_t * packet, uint16_t available )
{
	return ipv4_packet_is_acceptable( packet, available ) ? ipv4_payload( packet ) : 0;
}

// One more thin layer, eight bytes and no more,
// and then it's sACN, right down at the core.
static const uint8_t * inspector_open_udp( const uint8_t * packet, const uint8_t * datagram )
{
	uint8_t acceptable = udp_datagram_is_acceptable(
		ipv4_source( packet ), ipv4_destination( packet ), datagram, ipv4_payload_length( packet ) );
	return acceptable ? udp_payload( datagram ) : 0;
}

// Section 9.3.1: the MAC, the IP, and the universe inside
// must all tell one story, with nothing to hide.
static uint8_t inspector_group_is_consistent( const uint8_t * frame, const uint8_t * packet, const uint8_t * sacn )
{
	const uint8_t * group = ipv4_destination( packet );
	return e131_address_is_group( group )
		&& ethernet_destination_matches_group( frame, group )
		&& e131_group_matches_universe( group, sacn );
}

// Section 9.1.1 lets unicast go by,
// but multicast claims we will not let lie.
static uint8_t inspector_addressing_is_consistent( const uint8_t * frame, const uint8_t * packet, const uint8_t * sacn )
{
	if( !ethernet_destination_is_ipv4_group( frame ) )
	{
		return !e131_address_is_group( ipv4_destination( packet ) );
	}
	return inspector_group_is_consistent( frame, packet, sacn );
}

// Peel off three layers and check what remains,
// then check the addressing, for all of our pains.
const uint8_t * inspector_find_sacn( const uint8_t * frame, uint16_t length, const uint8_t ** address )
{
	const uint8_t * packet = inspector_open_ethernet( frame, length );
	if( packet == 0 )
	{
		return 0;
	}
	const uint8_t * datagram = inspector_open_ipv4( packet, ethernet_payload_length( length ) );
	if( datagram == 0 )
	{
		return 0;
	}
	const uint8_t * sacn = inspector_open_udp( packet, datagram );
	if( sacn == 0 || !e131_packet_is_valid( sacn, udp_payload_length( datagram ) ) )
	{
		return 0;
	}
	if( !inspector_addressing_is_consistent( frame, packet, sacn ) )
	{
		return 0;
	}
	*address = ipv4_source( packet );
	return sacn;
}
