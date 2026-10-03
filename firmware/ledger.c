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

// The same CID, the same universe too,
// and somebody sat there, or it will not do.
static uint8_t ledger_entry_matches( const ledger_entry_t * entry, const uint8_t * cid, uint16_t universe )
{
	return entry->occupied && entry->universe == universe && bytes_equal( entry->cid, cid, ROOT_CID_SIZE );
}

// Walk down the seats till the right one is found,
// and nothing comes back if they're not around.
static ledger_entry_t * ledger_find( const uint8_t * cid, uint16_t universe )
{
	for( uint8_t seat = 0; seat < LEDGER_SEATS; seat++ )
	{
		if( ledger_entry_matches( &ledger[seat], cid, universe ) )
		{
			return &ledger[seat];
		}
	}
	return 0;
}

// Walk down the seats till an empty one shows,
// with twenty-five taken, the twenty-sixth goes.
static ledger_entry_t * ledger_find_empty_seat( void )
{
	for( uint8_t seat = 0; seat < LEDGER_SEATS; seat++ )
	{
		if( !ledger[seat].occupied )
		{
			return &ledger[seat];
		}
	}
	return 0;
}
