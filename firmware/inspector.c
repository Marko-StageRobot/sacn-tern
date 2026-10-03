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
