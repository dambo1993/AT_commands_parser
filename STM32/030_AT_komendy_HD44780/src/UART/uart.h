/*
 * uart.h
 *
 *  Created on: 29 sie 2015
 *      Author: Przemek
 */

#ifndef __UART_H
#define __UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "DMB/rejestry.h"
#include "stm32f0xx.h"

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						CZY MAMY UMOZLIWIC RS485?                                            //
//						                                        							 //
///////////////////////////////////////////////////////////////////////////////////////////////
// w trybie rs485 sterujemy dodatkowo pinem bufora transceivera RS485 przy odbiorze/wysylaniu
//#define RS485_ON

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						USTAWIENIA BUFOROW DLA UARTA                                         //
//						MOZNA WPISAC DOWOLNA WARTOSC            							 //
///////////////////////////////////////////////////////////////////////////////////////////////
#define UART_RX_BUF_SIZE 256

#define UART_TX_BUF_SIZE 256

#define UART_WORKBUFF_SIZE 100


///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						REJESTRY UART DLA F0xxxx                                             //
//																							 //
///////////////////////////////////////////////////////////////////////////////////////////////
#define UART_CR1(x) (x->CR1)
#define UART_CR2(x) (x->CR2)
#define UART_CR3(x) (x->CR3)
#define UART_BRR(x) (x->BRR)
#define UART_GTPR(x) (x->GTPR)
#define UART_RTOR(x) (x->RTOR)
#define UART_RQR(x) (x->RQR)
#define UART_ISR(x) (x->ISR)
#define UART_ICR(x) (x->ICR)
#define UART_RDR(x) (x->RDR)
#define UART_TDR(x) (x->TDR)

///////////////////////////////////////////////////////////////////////////////////////////////
//																							 //
//						KONFIGURACJA WYBRANEGO UARTU                                         //
//																							 //
///////////////////////////////////////////////////////////////////////////////////////////////

// numer naszego uartu:
#define UARTx USART1

// predkosc taktowania peryferium
#define UART_CLK_SPEED 	48000000UL

// ustawienia pinow naszego uartu
#define UART_RX_PORT GPIOA
#define UART_RX_PIN	 3
#define UART_RX_ALTERNATE_FUNCTION_NUMBER 1

#define UART_TX_PORT GPIOA
#define UART_TX_PIN	 2
#define UART_TX_ALTERNATE_FUNCTION_NUMBER 1

// ustawienia przerwania naszego uartu
#define UART_INTERRUPT_HANDLER 	USART1_IRQHandler
#define UART_INTERRUPT_NUMBER 	USART1_IRQn

// ustawienia dla RS485 - pin sterujacy
#define RS485_CONTROL_GPIO 		GPIOA
#define RS485_CONTROL_PIN  		2
#define RS485_CONTROL_OUT 		GPIO_MODER(RS485_CONTROL_GPIO) |= ( 1 << ( 2 * RS485_CONTROL_PIN));
#define RS485_CONTROL_RECEIVE	GPIO_BRR(RS485_CONTROL_GPIO) = ( 1 << RS485_CONTROL_PIN );
#define RS485_CONTROL_SEND		GPIO_BSRR(RS485_CONTROL_GPIO) = ( 1 << RS485_CONTROL_PIN );

// ustawienie zegara dla UARTU

#define UART_CLK_EN RCC->APB2ENR |= RCC_APB2ENR_USART1EN

#define UART_PINS_CLK_EN RCC->AHBENR |= RCC_AHBENR_GPIOAEN

#define UART_SYSCFG_EN RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN

// makra wykonawcze:
#define UART_RX_ALTERNATE GPIO_MODER(UART_RX_PORT) |= ( 2 << ( 2 * UART_RX_PIN ) )
#if UART_RX_PIN < 8
	#define UART_RX_ALTERNATE_NR GPIO_AFRL(UART_RX_PORT) |= ( UART_RX_ALTERNATE_FUNCTION_NUMBER << ( 4 * UART_RX_PIN ) )
#else
	#define UART_RX_ALTERNATE_NR GPIO_AFRH(UART_RX_PORT) |= ( UART_RX_ALTERNATE_FUNCTION_NUMBER << ( 4 * ( UART_RX_PIN - 8 ) ) )
#endif

#define UART_TX_ALTERNATE GPIO_MODER(UART_TX_PORT) |= ( 2 << ( 2 * UART_TX_PIN ) )

#if UART_TX_PIN < 8
	#define UART_TX_ALTERNATE_NR GPIO_AFRL(UART_TX_PORT) |= ( UART_TX_ALTERNATE_FUNCTION_NUMBER << ( 4 * UART_TX_PIN ) )
#else
	#define UART_TX_ALTERNATE_NR GPIO_AFRH(UART_TX_PORT) |= ( UART_TX_ALTERNATE_FUNCTION_NUMBER << ( 4 * ( UART_TX_PIN - 8 ) ) )
#endif

// utworzenie CALLbacka, ktory jest wywolywany, gdy uart odbierze cala linie
typedef void (*uart_RX_Callback_type) (char *);

// inicjalizacja uartu
void init_uart(uint32_t baudrate);

// nasza funkcja przerwania
void UART_INTERRUPT_HANDLER(void);

// rejestracja callbacka odbiorczego
void uart_register_rx_callback( uart_RX_Callback_type wsk );

// sprawdzenie wystapienia eventu od odebrania linii
void uart_event_check( void );


void uart_wyslij_znak(char data);
void uart_wyslij_lancuch(char *s);
void uart_wyslij_liczbe(int16_t liczba);

// funkcja sprawdza, czy zakonczone zostalo przesylanie bufora
uint8_t uart_wysylanie_zakonczone(void);

// funkcja sprawdza, czy jakies dane znajduja sie w buforze odbiorczym
uint8_t uart_odbior_sprawdz();

// fukcja pobiera pojedynczy znak z bufora odbiorczego
uint8_t uart_pobierz_znak(void);

// funkcja sprawdza, czy odebrano linie tekstu
uint8_t uart_odebrano_linie( void );

// funkcja pobiera linie tekstu
void uart_pobierz_linie( char *buff );

void uart_wyslij_liczbe(int16_t liczba);

void uart_wyslij_lancuch(char *s);

void uart_wyslij_lancuch_r(char *s);

void uart_wyslij_lancuch_rn(char *s);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H */
