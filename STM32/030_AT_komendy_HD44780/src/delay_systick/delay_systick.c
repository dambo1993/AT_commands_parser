/*
 * delay_systick.c
 *
 *  Created on: 25 wrz 2016
 *      Author: Przemek
 */

#include <delay_systick/delay_systick.h>
#include "stm32f0xx.h"

volatile uint32_t delay_counter;

// rozdzielczosc timera 1ms
void delay_init( void )
{
	SysTick_Config( SYSCLK_SPEED / 1000 );
	SysTick->CTRL = 0;
}

void _delay_ms( unsigned int delay )
{
	delay_counter = delay;
	SysTick->VAL   = 0UL;
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
	                   SysTick_CTRL_TICKINT_Msk   |
	                   SysTick_CTRL_ENABLE_Msk;
	while( delay_counter );
	SysTick->CTRL = 0;
}


void SysTick_Handler(void)
{
	if( delay_counter )
		delay_counter--;
}
