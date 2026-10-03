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

// Ten milliseconds more have gone by, so we say,
// then clear out the flag, and get out of the way.
void TIM2_IRQHandler( void ) __attribute__((interrupt));
void TIM2_IRQHandler( void )
{
	WCHNET_TimeIsr( WCHNETTIMERPERIOD );
	TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
}
