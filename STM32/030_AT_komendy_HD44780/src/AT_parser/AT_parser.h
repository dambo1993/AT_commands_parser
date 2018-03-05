#ifndef AT_PARSER_H_INCLUDED
#define AT_PARSER_H_INCLUDED

#include "stm32f0xx.h"

//! tutaj wybieramy, czy mamy obslugiwac takze dodatkowa tablice z komendami niezaczynajacymi sie od "AT+"
//! 0 - wylaczone
//! 1 - wlaczone
#define USE_NO_AT_COMMANDS 1

//! tutaj wybieramy, czy mamy obslugiwac takze bledy gdy nie wykryjemy komendy
//! 0 - wylaczone
//! 1 - wlaczone
#define USE_AT_ERRORS 1

//! Maksymalna ilosc parametrow jakie bedziemy uzywac - wyznacza wielkosc tablicy z parametrami
#define _MAX_PARAMETERS 15

//! structure for single ATcommand
typedef struct{
	const char *cmd;                        // array with command name
	void (*callback_function)(char **, uint8_t );            // callback function pointer
} t_cmd;

#if USE_AT_ERRORS == 1
//! typ dla callbackow od blednej komendy - funkcja zwraca wskaznik do odebranego napisu
typedef void (*AT_command_error_type)(char *);

//! structure for single ATcommand without parameters
typedef struct{
	const char *cmd;                        // array with command name
	void (*callback_function)(void);        // callback function pointer
} t_cmd_no_at;
#endif

//! funkcja rejestrujaca tablice z komendami AT
//! podajemy wskaznik na tablice oraz jej wielkosc
void AT_register_AT_commands_table(const t_cmd *wsk, uint8_t ilosc_parametrow);

//! funkcja rejestrujaca tablice z komendami, ktore nie zaczynaja sie od "AT+"
//! czyli zalicza sie do tego "AT", "ATI" itp
//! podajemy wskaznik na tablice oraz jej wielkosc
void AT_register_no_AT_commands_table(const t_cmd_no_at *wsk, int ilosc_parametrow);

//! funkcja rejestrujaca callbacka od otrzymania blednej komendy
void AT_register_error_function(const AT_command_error_type);

//! glowna funkcja dekodujaca komendy AT
//! jako parametr przyjmuje wskaznik na bufor z odebranym napisem
void AT_commands_decode(char* data);


#endif // AT_PARSER_H_INCLUDED
