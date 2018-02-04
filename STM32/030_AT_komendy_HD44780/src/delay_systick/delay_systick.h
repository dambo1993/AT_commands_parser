/*
 * delay_systick.h
 *
 *  Created on: 25 wrz 2016
 *      Author: Przemek
 */

#ifndef DELAY_SYSTICK_H_
#define DELAY_SYSTICK_H_

// podczas testow na STM32F030F4P6 na 48MHz dla ustawienie 500ms
// analizator saleae pokazywal 0,5001s
// dla 1500 - idealne 1,5s
// dla 50 -50,01ms

// TUTAJ PODAJEMY WARTOSC Z JAKA JEST TAKTOWANY SYSTICK
#define SYSCLK_SPEED 48000000

void delay_init( void );

void _delay_ms( unsigned int );

void SysTick_Handler(void);

#endif /* DELAY_SYSTICK_H_ */
