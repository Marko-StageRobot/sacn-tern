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
