/*
 * HD44780.c
 *
 *  Created on: 26 wrz 2016
 *      Author: Przemek
 */


// kod napisano na podstawie informacji ze strony http://radzio.dxp.pl/hd44780/
#include "stm32f0xx.h"
#include "HD44780.h"
#include "delay_systick/delay_systick.h"
#include <stdlib.h>

//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiajaca polbajt na magistrale danych
//
//-------------------------------------------------------------------------------------------------
void _LCD_OutNibble(unsigned char nibbleToWrite)
{
	if(nibbleToWrite & 0x01)
		LCD_DB4_ON;
	else
		LCD_DB4_OFF;

	if(nibbleToWrite & 0x02)
		LCD_DB5_ON;
	else
		LCD_DB5_OFF;

	if(nibbleToWrite & 0x04)
		LCD_DB6_ON;
	else
		LCD_DB6_OFF;

	if(nibbleToWrite & 0x08)
		LCD_DB7_ON;
	else
		LCD_DB7_OFF;
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wyswietacza (bez rozroznienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
void _LCD_Write(unsigned char dataToWrite)
{
	LCD_E_ON;
	_LCD_OutNibble(dataToWrite >> 4);
	LCD_E_OFF;
	LCD_E_ON;
	_LCD_OutNibble(dataToWrite);
	LCD_E_OFF;
	_delay_us(50);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wyswietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteCommand(unsigned char commandToWrite)
{
	LCD_RS_OFF;
	_LCD_Write(commandToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pamieci wyswietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteData(unsigned char dataToWrite)
{
	LCD_RS_ON;
	_LCD_Write(dataToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wyswietlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteText(char * text)
{
	while(*text)
	  LCD_WriteData(*text++);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia wspolrzednych ekranowych
//
//-------------------------------------------------------------------------------------------------
void LCD_GoTo(unsigned char x, unsigned char y)
{
	LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wyswietlenia liczby na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteNumber(uint16_t number)
{
	char buff[10];
	LCD_WriteText( itoa(number, buff, 10) );
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja czyszczenia ekranu wyswietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Clear(void)
{
	LCD_WriteCommand(HD44780_CLEAR);
	_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywrocenia poczatkowych wspolrzednych wyswietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Home(void)
{
	LCD_WriteCommand(HD44780_HOME);
	_delay_ms(2);
}
//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
void LCD_Initalize(void)
{
	unsigned char i;
	// wlaczenie
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// Konfiguracja kierunku pracy wyprowadzen
	LCD_DB4_OUT;
	LCD_DB5_OUT;
	LCD_DB6_OUT;
	LCD_DB7_OUT;
	LCD_E_OUT;
	LCD_RS_OUT;
	_delay_ms(15); // oczekiwanie na ustalibizowanie sie napiecia zasilajacego
	LCD_RS_OFF; // wyzerowanie linii RS
	LCD_E_OFF;  // wyzerowanie linii E

	for(i = 0; i < 3; i++) // trzykrotne powtorzenie bloku instrukcji
	{
		LCD_E_ON; //  E = 1
		_LCD_OutNibble(0x03); // tryb 8-bitowy
		LCD_E_OFF; // E = 0
		_delay_ms(5); // czekaj 5ms
	}

	LCD_E_ON; // E = 1
	_LCD_OutNibble(0x02); // tryb 4-bitowy
	LCD_E_OFF; // E = 0

	_delay_ms(1); // czekaj 1ms
	LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wy��czenie wyswietlacza
	LCD_WriteCommand(HD44780_CLEAR); // czyszczenie zawartos�i pamieci DDRAM
	_delay_ms(2);
	LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);// inkrementaja adresu i przesuwanie kursora
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // wlacz LCD, bez kursora i mrugania
}

void _delay_us( uint8_t x )
{
	uint8_t i = 0;
	for( i = 0; i < x; i++ )
	{
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");

		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");

		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");

		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");

		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");

	}
}

//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.c
//
//-------------------------------------------------------------------------------------------------
