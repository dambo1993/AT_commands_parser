#ifndef AT_PARSER_H_INCLUDED
#define AT_PARSER_H_INCLUDED

//! structure for single ATcommand
typedef struct{
	const char *cmd;                        // array with command name
	void (*callback_function)();            // callback function pointer
} t_cmd;

//! prosty enum dla okreslenia czy funkcja przyjmuje parametry, czy nie
enum{at_type_no_params, at_type_params};

//! Maksymalna ilosc parametrow jakie bedziemy uzywac - wyznacza wielkosc tablicy z parametrami
#define _MAX_PARAMETERS 15


//! funkcja rejestrujaca tablice z komendami AT
//! podajemy wskaznik na tablice oraz jej wielkosc
void AT_register_AT_commands_table(const t_cmd *wsk, int ilosc_parametrow);

//! glowna funkcja dekodujaca komendy AT
//! jako parametr przyjmuje wskaznik na bufor z odebranym napisem
void AT_commands_decode(char* data);


//! funkcja debugujaca - wyswietla wszystkie dostepne komendy z tablicy wraz z ich ustawieniem odnosnie parametrow
void AT_wyswietl_dostepne_komendy(void);



#endif // AT_PARSER_H_INCLUDED
