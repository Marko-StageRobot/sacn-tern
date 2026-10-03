// 三秒の
// 沈黙のあと
// 鳥が鳴く
#include "debug.h"
#include "inspector.h"
#include "ledger.h"
#include "terminator.h"
#include "timebase.h"
#include "wire.h"

// The clock, then the delay, then time, then the wire,
// four things to wake up before we can inquire.
static void tern_wake( void )
{
	SystemCoreClockUpdate();
	Delay_Init();
	timebase_init();
	wire_init();
}

// If it's sACN, and valid in every regard,
// we note it, or strike it, upon the scorecard.
static void tern_consider( const uint8_t * frame, uint16_t length )
{
	const uint8_t * address;
	const uint8_t * packet = inspector_find_sacn( frame, length, &address );
	if( packet == 0 )
	{
		return;
	}
	if( framing_stream_is_terminated( packet ) )
	{
		ledger_forget( packet );
		return;
	}
	ledger_hear( packet, address, timebase_now() );
}
