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
