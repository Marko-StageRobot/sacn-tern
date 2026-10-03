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

// A newcomer's given a seat, if there's room,
// their CID and universe noted, we presume.
static ledger_entry_t * ledger_seat_newcomer( const uint8_t * cid, uint16_t universe )
{
	ledger_entry_t * entry = ledger_find_empty_seat();
	if( entry != 0 )
	{
		entry->occupied = 1;
		entry->universe = universe;
		bytes_copy( entry->cid, cid, ROOT_CID_SIZE );
	}
	return entry;
}

// Section 6.7.2: B minus A, signed and eight wide,
// at most zero, above minus twenty, is cast aside.
static uint8_t ledger_sequence_is_stale( const ledger_entry_t * entry, uint8_t sequence )
{
	int8_t difference = (int8_t)( sequence - entry->sequence );
	return difference <= 0 && difference > -20;
}

// The name and the priority, sequence, address,
// all that we'll need to complete the process.
static void ledger_copy_details( ledger_entry_t * entry, const uint8_t * packet, const uint8_t * address )
{
	bytes_copy( entry->source_name, framing_source_name( packet ), FRAMING_SOURCE_NAME_SIZE );
	entry->priority = framing_priority( packet );
	entry->sequence = framing_sequence( packet );
	bytes_copy( entry->address, address, IPV4_ADDRESS_SIZE );
}

// Find them, or seat them, then check they're in order,
// and note down the time, like a guard at the border.
void ledger_hear( const uint8_t * packet, const uint8_t * address, uint32_t now )
{
	const uint8_t * cid = root_cid( packet );
	uint16_t universe = framing_universe( packet );
	ledger_entry_t * entry = ledger_find( cid, universe );
	if( entry != 0 && ledger_sequence_is_stale( entry, framing_sequence( packet ) ) )
	{
		return;
	}
	if( entry == 0 )
	{
		entry = ledger_seat_newcomer( cid, universe );
	}
	if( entry == 0 )
	{
		return;
	}
	ledger_copy_details( entry, packet, address );
	entry->last_heard = now;
}
