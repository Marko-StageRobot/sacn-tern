// 二十五の
// 席しかないと
// 張り紙を
#pragma once

#include <stdint.h>

#include "framing_layer.h"
#include "ipv4.h"
#include "root_layer.h"

#define LEDGER_SEATS 25
#define LEDGER_SILENCE_MS 3000

typedef struct
{
	uint8_t occupied;
	uint8_t cid[ROOT_CID_SIZE];
	uint16_t universe;
	uint8_t source_name[FRAMING_SOURCE_NAME_SIZE];
	uint8_t priority;
	uint8_t sequence;
	uint8_t address[IPV4_ADDRESS_SIZE];
	uint32_t last_heard;
} ledger_entry_t;

ledger_entry_t * ledger_seat( uint8_t seat );

void ledger_hear( const uint8_t * packet, const uint8_t * address, uint32_t now );

void ledger_vacate( ledger_entry_t * entry );
