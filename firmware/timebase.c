// 遠太鼓
// 百分の一
// 秒ごとに
#include "timebase.h"
#include "eth_driver.h"

extern volatile uint32_t LocalTime;

// Timer two ticks every ten thousandths, about,
// the driver expects it, of that there's no doubt.
static void timebase_configure_timer( void )
{
	TIM_TimeBaseInitTypeDef settings = { 0 };
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE );
	settings.TIM_Period = SystemCoreClock / 1000000;
	settings.TIM_Prescaler = WCHNETTIMERPERIOD * 1000 - 1;
	settings.TIM_ClockDivision = 0;
	settings.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM2, &settings );
}

// Enable its interrupt and set it to run,
// clear what was pending, and then we are done.
static void timebase_start_timer( void )
{
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE );
	TIM_Cmd( TIM2, ENABLE );
	TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
	NVIC_EnableIRQ( TIM2_IRQn );
}
