// 誰宛の
// 手紙もすべて
// 読んでいる
#include "wire.h"
#include "eth_driver.h"

#define WIRE_CRC_SIZE 4
#define WIRE_PATIENCE 100000

extern ETH_DMADESCTypeDef * pDMARxSet;
extern volatile uint8_t LinkSta;
void WCHNET_HandlePhyNegotiation( void );
void WCHNET_RecProcess( void );

// The driver insists on this name for the thing,
// we did not choose it, but here's what we bring.
uint8_t MACAddr[6];

// The chip has an address burned in at the plant,
// we use it, since making one up, we just can't.
void wire_init( void )
{
	WCHNET_GetMacAddr( MACAddr );
	ETH_Init( MACAddr );
}

// Six bytes that are ours, and that nobody lent,
// the one honest field in the frames that are sent.
const uint8_t * wire_mac_address( void )
{
	return MACAddr;
}

// The driver keeps turning the filtering on,
// so we turn it back off, and its preference is gone.
static void wire_insist_on_everything( void )
{
	ETH->MACFFR |= ETH_ReceiveAll_Enable | ETH_PromiscuousMode_Enable;
}

// The PHY needs attention, the MAC needs a nudge,
// then we open the filter, and nobody's judge.
void wire_tend( void )
{
	WCHNET_HandlePhyNegotiation();
	WCHNET_RecProcess();
	wire_insist_on_everything();
}

// When the descriptor is ours and not the DMA's,
// a frame has come in, and it's ready for praise.
uint8_t wire_frame_waiting( void )
{
	return ( pDMARxSet->Status & ETH_DMARxDesc_OWN ) == 0;
}

// No error, a first and a last in one go,
// means all of the frame is right here, head to toe.
uint8_t wire_frame_is_whole( void )
{
	uint32_t status = pDMARxSet->Status;
	return !( status & ETH_DMARxDesc_ES ) && ( status & ETH_DMARxDesc_LS ) && ( status & ETH_DMARxDesc_FS );
}

// The descriptor points at the buffer it filled,
// we look but don't touch, as the driver has willed.
const uint8_t * wire_frame( void )
{
	return (const uint8_t *)pDMARxSet->Buffer1Addr;
}

// The length includes four bytes of CRC,
// the MAC checked them already, so they're not for me.
uint16_t wire_frame_length( void )
{
	uint16_t length = (uint16_t)( ( pDMARxSet->Status & ETH_DMARxDesc_FL ) >> ETH_DMARxDesc_FrameLengthShift );
	return length > WIRE_CRC_SIZE ? length - WIRE_CRC_SIZE : 0;
}

// Hand the descriptor straight back to the DMA,
// and step to the next one along the way.
void wire_release_frame( void )
{
	pDMARxSet->Status = ETH_DMARxDesc_OWN;
	pDMARxSet = (ETH_DMADESCTypeDef *)pDMARxSet->Buffer2NextDescAddr;
}
