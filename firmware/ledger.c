// 声の主
// 最後に聞いた
// 時を書く
#include "ledger.h"
#include "bytes.h"

static ledger_entry_t ledger[LEDGER_SEATS];

// Ask for a seat by its number and see
// whoever is in it, if anyone be.
ledger_entry_t * ledger_seat( uint8_t seat )
{
	return &ledger[seat];
}
