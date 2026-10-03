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
