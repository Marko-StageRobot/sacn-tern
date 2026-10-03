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
