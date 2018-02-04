
#include "stm32f0xx.h"
#include "DMB/rejestry.h"
#include "DMB/debug.h"
#include "RCC/RCC.h"
#include "delay_systick/delay_systick.h"
#include "stdlib.h"

#include "HD44780/HD44780.h"
#include "UART/uart.h"
#include "AT_parser/AT_parser.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funkcje do odeslania statusu wykonania przez UART
// Oczywiscie mozna te funkcje z bledami rozbudowac i sprawdzac jakiego rodzaju sa to bledy
// - czy ogolnie zla funkcja
// - czy zla ilosc parametrow itp
////////////////////////////////////////////////////////////////////////////////////////////////////////
void odeslij_ok()
{
	uart_wyslij_lancuch_rn("OK!");
}

void odeslij_error()
{
	uart_wyslij_lancuch_rn("ERROR!");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Nasze przykladowe funkcje podpiete do komend
////////////////////////////////////////////////////////////////////////////////////////////////////////

//! odsylamy przez UART przywitanie
void przywitanie(char **params_array, uint8_t params_cnt)
{
	uart_wyslij_lancuch_rn("dzien dobry!!!");
}

//! wyswietlamy w zadanej linii zadany napis
//! w formacie AT+LINE=<nr>,<text>
//! np. AT+LINE=0,test
void napis_linia(char **params_array, uint8_t params_cnt)
{
	// sprawdzenie ilosci parametrow:
	if(params_cnt == 2)
	{
		LCD_GoTo(0,atoi(params_array[0]));
		LCD_WriteText(params_array[1]);

		odeslij_ok();
	}
	else
	{
		odeslij_error();
	}
}

//! czyscimy zadana linie (liczymy od 0 w numerze lini)
//! komenda: AT+CLRL=<numer>
//! np. AT+CLRL=0
void czysc_linia(char **params_array, uint8_t params_cnt)
{
	// sprawdzenie ilosci parametrow:
	if(params_cnt == 1)
	{
		LCD_GoTo(0,atoi(params_array[0]));
		LCD_WriteText("                ");

		odeslij_ok();
	}
	else
	{
		odeslij_error();
	}
}

//! prosta animacja zapelniania linii jakims znaczkiem (uwaga - blokujaca, tylko dla przykladu odbioru kilku parametrow)
//! podajemy tu:
//! - numer linii
//! - ilosc znaczkow
//! - znaczek
//! - delay pomiedzy nimi
//! komenda: AT+STAR=<nr_linii>,<ilosc_znaczkow>,<znaczek>,<delay_time>
//! np. AT+STAR=0,10,*,150
void stars(char **params_array, uint8_t params_cnt)
{
	// sprawdzenie ilosci parametrow:
	if(params_cnt == 4)
	{
		uint8_t ilosc_znaczkow = atoi(params_array[1]);
		uint16_t delay_time = atoi(params_array[3]);

		LCD_GoTo(0,atoi(params_array[0]));

		for(uint8_t i = 0; i < ilosc_znaczkow; i++)
		{
			LCD_WriteText(params_array[2]);
			_delay_ms(delay_time);
		}

		odeslij_ok();
	}
	else
	{
		odeslij_error();
	}
}

//! glowna tablica z komendami AT - uzupelniamy ja naszymi komendami AT
//! a nastepnie przekazujemy do biblioteki parsera
const t_cmd AT_cmd_array[] = {
		{"HI",          przywitanie},
		{"LINE", 		napis_linia},
		{"CLRL",        czysc_linia},
		{"STARS",       stars},
};

int main(void)
{
	// inicjalizacja zegara na 48MHz z zewnetrznego kwarcu 8MHz
	RCC_init();
	delay_init();

	// wlaczenie zegara dla portu A i B
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	LCD_Initalize();
	LCD_Clear();
	LCD_GoTo(0,0);
	LCD_WriteText("Dzien dobry");

	// rejestracja tablicy ze strukturami komend
    AT_register_AT_commands_table(AT_cmd_array, (sizeof(AT_cmd_array)/sizeof(AT_cmd_array[0])));

    // inicjalizacja uartu:
	init_uart( 115200 );

	// podpiecie callbacka do eventu od odebrania linii przez UART
	uart_register_rx_callback( AT_commands_decode );

	_delay_ms(500);
	while(1)
	{
		// sprawdzenie eventow od UARTu
		uart_event_check();
	}
}

