// 石八つ
// 並べ替えれば
// 数になる
#include "bytes.h"

// Two bytes on the wire, the high one comes first,
// get it the wrong way and the number is cursed.
uint16_t bytes_read_u16( const uint8_t * bytes )
{
	return (uint16_t)( ( bytes[0] << 8 ) | bytes[1] );
}
