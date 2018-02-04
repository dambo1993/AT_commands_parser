/*
 * HD44780.h
 *
 *  Created on: 26 wrz 2016
 *      Author: Przemek
 */

#ifndef HD44780_H_
#define HD44780_H_

// kod napisano na podstawie informacji ze strony http://radzio.dxp.pl/hd44780/

//-------------------------------------------------------------------------------------------------
//
// Konfiguracja sygna³ów steruj¹cych wyœwietlaczem.
// Mo¿na zmieniæ stosownie do potrzeb.
//
//-------------------------------------------------------------------------------------------------
#include "DMB/rejestry.h"

#define LCD_RS_GPIO GPIOA
#define LCD_RS_PIN 	0
#define LCD_RS_OUT 	GPIO_MODER(LCD_RS_GPIO) |= ( 1 << ( 2 * LCD_RS_PIN))
#define LCD_RS_ON 	GPIO_BSRR(LCD_RS_GPIO) = (1 << LCD_RS_PIN)
#define LCD_RS_OFF 	GPIO_BRR(LCD_RS_GPIO) = ( 1 << LCD_RS_PIN )

#define LCD_E_GPIO 	GPIOA
#define LCD_E_PIN 	1
#define LCD_E_OUT 	GPIO_MODER(LCD_E_GPIO) |= ( 1 << ( 2 * LCD_E_PIN))
#define LCD_E_ON 	GPIO_BSRR(LCD_E_GPIO) = (1 << LCD_E_PIN)
#define LCD_E_OFF 	GPIO_BRR(LCD_E_GPIO) = ( 1 << LCD_E_PIN )

#define LCD_DB4_GPIO 	GPIOA
#define LCD_DB4_PIN 	4
#define LCD_DB4_OUT 	GPIO_MODER(LCD_DB4_GPIO) |= ( 1 << ( 2 * LCD_DB4_PIN))
#define LCD_DB4_ON 		GPIO_BSRR(LCD_DB4_GPIO) = (1 << LCD_DB4_PIN)
#define LCD_DB4_OFF 	GPIO_BRR(LCD_DB4_GPIO) = ( 1 << LCD_DB4_PIN )

#define LCD_DB5_GPIO 	GPIOA
#define LCD_DB5_PIN 	5
#define LCD_DB5_OUT 	GPIO_MODER(LCD_DB5_GPIO) |= ( 1 << ( 2 * LCD_DB5_PIN))
#define LCD_DB5_ON 		GPIO_BSRR(LCD_DB5_GPIO) = (1 << LCD_DB5_PIN)
#define LCD_DB5_OFF 	GPIO_BRR(LCD_DB5_GPIO) = ( 1 << LCD_DB5_PIN )

#define LCD_DB6_GPIO 	GPIOA
#define LCD_DB6_PIN 	6
#define LCD_DB6_OUT 	GPIO_MODER(LCD_DB6_GPIO) |= ( 1 << ( 2 * LCD_DB6_PIN))
#define LCD_DB6_ON 		GPIO_BSRR(LCD_DB6_GPIO) = (1 << LCD_DB6_PIN)
#define LCD_DB6_OFF 	GPIO_BRR(LCD_DB6_GPIO) = ( 1 << LCD_DB6_PIN )

#define LCD_DB7_GPIO 	GPIOA
#define LCD_DB7_PIN 	7
#define LCD_DB7_OUT 	GPIO_MODER(LCD_DB7_GPIO) |= ( 1 << ( 2 * LCD_DB7_PIN))
#define LCD_DB7_ON 		GPIO_BSRR(LCD_DB7_GPIO) = (1 << LCD_DB7_PIN)
#define LCD_DB7_OFF 	GPIO_BRR(LCD_DB7_GPIO) = ( 1 << LCD_DB7_PIN )

//-------------------------------------------------------------------------------------------------
//
// Instrukcje kontrolera Hitachi HD44780
//
//-------------------------------------------------------------------------------------------------

#define HD44780_CLEAR					0x01

#define HD44780_HOME					0x02

#define HD44780_ENTRY_MODE				0x04
	#define HD44780_EM_SHIFT_CURSOR		0
	#define HD44780_EM_SHIFT_DISPLAY	1
	#define HD44780_EM_DECREMENT		0
	#define HD44780_EM_INCREMENT		2

#define HD44780_DISPLAY_ONOFF			0x08
	#define HD44780_DISPLAY_OFF			0
	#define HD44780_DISPLAY_ON			4
	#define HD44780_CURSOR_OFF			0
	#define HD44780_CURSOR_ON			2
	#define HD44780_CURSOR_NOBLINK		0
	#define HD44780_CURSOR_BLINK		1

#define HD44780_DISPLAY_CURSOR_SHIFT	0x10
	#define HD44780_SHIFT_CURSOR		0
	#define HD44780_SHIFT_DISPLAY		8
	#define HD44780_SHIFT_LEFT			0
	#define HD44780_SHIFT_RIGHT			4

#define HD44780_FUNCTION_SET			0x20
	#define HD44780_FONT5x7				0
	#define HD44780_FONT5x10			4
	#define HD44780_ONE_LINE			0
	#define HD44780_TWO_LINE			8
	#define HD44780_4_BIT				0
	#define HD44780_8_BIT				16

#define HD44780_CGRAM_SET				0x40

#define HD44780_DDRAM_SET				0x80

//-------------------------------------------------------------------------------------------------
//
// Deklaracje funkcji
//
//-------------------------------------------------------------------------------------------------

void LCD_WriteCommand(unsigned char);
void LCD_WriteData(unsigned char);
void LCD_WriteText(char *);
void LCD_WriteNumber(uint16_t number);
void LCD_GoTo(unsigned char, unsigned char);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_Initalize(void);
void _delay_us( uint8_t x );

#endif /* HD44780_H_ */
