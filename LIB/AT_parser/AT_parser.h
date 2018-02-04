#ifndef AT_PARSER_H_INCLUDED
#define AT_PARSER_H_INCLUDED

#include "stm32f0xx.h"

//! structure for single ATcommand
typedef struct{
	const char *cmd;                        // array with command name
	void (*callback_function)(char **, uint8_t );            // callback function pointer
} t_cmd;

//! prosty enum dla okreslenia czy funkcja przyjmuje parametry, czy nie
enum{at_type_no_params, at_type_params};

//! Maksymalna ilosc parametrow jakie bedziemy uzywac - wyznacza wielkosc tablicy z parametrami
#define _MAX_PARAMETERS 15


//! funkcja rejestrujaca tablice z komendami AT
//! podajemy wskaznik na tablice oraz jej wielkosc
void AT_register_AT_commands_table(const t_cmd *wsk, uint8_t ilosc_parametrow);

//! glowna funkcja dekodujaca komendy AT
//! jako parametr przyjmuje wskaznik na bufor z odebranym napisem
void AT_commands_decode(char* data);


#endif // AT_PARSER_H_INCLUDED
