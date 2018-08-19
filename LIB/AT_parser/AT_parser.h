#ifndef AT_PARSER_H_INCLUDED
#define AT_PARSER_H_INCLUDED

#include <inttypes.h>
#include <libs_config/AT_parser_settings.h>


//! structure for single ATcommand
typedef struct{
	const char *cmd;                        // array with command name
	void (*callback_function)(char **, uint8_t );            // callback function pointer
} t_cmd;

//! structure for single ATcommand without parameters
typedef struct{
	const char *cmd;                        // array with command name
	void (*callback_function)(void);        // callback function pointer
} t_cmd_no_at;

//! typ dla callbackow od blednej komendy - funkcja zwraca wskaznik do odebranego napisu
typedef void (*AT_command_error_type)(char *);

//! makra ulatwiajace wypelnianie struktury ustawien - przeliczaja automatycznie rozmiar
#define AT_COMMANDS_AT_TABLE(x) .AT_command_array = x, .AT_commands_number = sizeof(x)/sizeof(x[0])
#define AT_COMMANDS_AT_TABLE_NULL .AT_command_array = NULL, .AT_commands_number = 0

#define AT_COMMANDS_NO_AT_TABLE(x) .no_AT_command_array = x, .no_AT_commands_number = sizeof(x)/sizeof(x[0])
#define AT_COMMANDS_NO_AT_TABLE_NULL .no_AT_command_array = NULL, .no_AT_commands_number = 0


//! typ struktury zawierajacej ustawienia parsera - w wersji konfiguracji poprzez strukturê
typedef struct
{
	const t_cmd * AT_command_array;							// tablica komend AT
	const uint8_t AT_commands_number;							// ilosc obiektow w tablicy komend AT
	const t_cmd_no_at * no_AT_command_array; 					// tablica komend noAT
	const uint8_t no_AT_commands_number;						// ilosc obiektow w tablicy komend noAT
	const AT_command_error_type AT_command_error_callback;	// callback odnosnie blednej komendy
} AT_parser_settings;


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

// funkcja rejestrujaca dane do biblioteka zapisane w formie struktury
void AT_commands_register_struct(const AT_parser_settings *settings);

// wersja dekodowania komend AT z uzyciem struktur
// dzieki temu nie musimy rejestrowac osobno wszystkich rzeczy
// przeznaczona glownie do projektow, gdzie parser jest wykorzystywany kilkukrotnie
// zakladam, ze jesli jest wykorzystywana inicjalizacja poprzez strukture to nie uwzgledniamy makr konfiguracyjnych
void AT_command_decode_struct(char *data, const AT_parser_settings *settings);


#endif // AT_PARSER_H_INCLUDED
