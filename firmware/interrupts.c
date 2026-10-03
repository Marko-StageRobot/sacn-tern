// 呼ばれたら
// すぐに応えて
// すぐ帰る
#include "eth_driver.h"

// The Ethernet calls and the driver replies,
// we stand in between, in a handler's disguise.
void ETH_IRQHandler( void ) __attribute__((interrupt));
void ETH_IRQHandler( void )
{
	WCHNET_ETHIsr();
}
