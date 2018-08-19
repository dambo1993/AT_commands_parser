#include "AT_parser.h"
#include <libs_config/AT_parser_settings.h>
#include "inttypes.h"
#include "strings.h"

// wskaznik na tablice ze strukturami komend AT
static const t_cmd * AT_command_array;

// ilosc elementow w tablicy komend
static uint8_t AT_commands_number = 0;

//! funkcja rejestrujaca tablice z komendami AT
//! podajemy wskaznik na tablice oraz jej wielkosc
void AT_register_AT_commands_table(const t_cmd *wsk, uint8_t ilosc_parametrow)
{
    AT_command_array = wsk;
    AT_commands_number = ilosc_parametrow;
}

#if USE_NO_AT_COMMANDS == 1
// wskaznik na tablice ze strukturami komend AT
static const t_cmd_no_at * no_AT_command_array = 0;

// ilosc elementow w tablicy komend
static uint8_t no_AT_commands_number = 0;

//! funkcja rejestrujaca tablice z komendami, ktore nie zaczynaja sie od "AT+"
//! czyli zalicza sie do tego "AT", "ATI" itp
//! podajemy wskaznik na tablice oraz jej wielkosc
void AT_register_no_AT_commands_table(const t_cmd_no_at *wsk, int ilosc_parametrow)
{
    no_AT_command_array = wsk;
    no_AT_commands_number = ilosc_parametrow;
}
#endif // USE_NO_AT_COMMANDS


#if USE_AT_ERRORS == 1
static AT_command_error_type AT_command_error_callback = 0;

//! funkcja rejestrujaca callbacka od otrzymania blednej komendy
void AT_register_error_function(const AT_command_error_type x)
{
    AT_command_error_callback = x;
}
#endif // USE_AT_ERRORS

// funkcja rejestrujaca dane do biblioteka zapisane w formie struktury
// tutaj rozbijamy ja na poszczegolne zmienne
void AT_commands_register_struct(const AT_parser_settings *settings)
{
    AT_command_array = settings->AT_command_array;
    AT_commands_number = settings->AT_commands_number;

    no_AT_command_array = settings->no_AT_command_array;
    no_AT_commands_number = settings->no_AT_commands_number;

	AT_command_error_callback = settings->AT_command_error_callback;
}


//! glowna funkcja dekodujaca komendy AT na podstawie zapisanych ustawien w zmiennych statycznych
//! jako parametr przyjmuje wskaznik na bufor z odebranym napisem
void AT_commands_decode(char* data)
{
    // tablica z parametrami
    char* params[_MAX_PARAMETERS];

    // ilosc znalezionych parametrow
    uint8_t params_cnt;

	// test naglowka komendy - musi to byc "AT+"
	if(data[0] == 'A' && data[1] == 'T' && data[2] == '+')
	{
		// przesuniecie wskaznika
		data += 3;

        // wskaznik tymczasowy dla operacji na lancuchu
        char * temp_wsk = 0;

        // musimy zamienic znak rownosci na 0 - bo nie porownamy inaczej napisow
        temp_wsk = strchr(data,'=');
        if(temp_wsk)
        {
            *temp_wsk = 0;
        }

		// petla po wszystkich komendach - jesli nie mamy w sobie odpowiedniej komendy - wykonamy wszystkie iteracje
		// w przypadku znalezienia komendy - opuszczamy petle
        uint8_t i = 0;
		for( i = 0; i < AT_commands_number; i++)
		{
		    // porownanie odebranego napisu i komend z tablicy
			if( strcmp(AT_command_array[i].cmd, data ) == 0 )
			{
				//  sprawdzmy czy callback jest podpiety - jesli nie nie ma sensu dekodowac parametrow
				if( AT_command_array[i].callback_function )
				{
					params_cnt = 0; // zerowanie licznika parametrow

					// przywrocenie znaku rownosci:
					if(temp_wsk)
					{
						*temp_wsk = '=';
					}

					// sprawdzamy, czy mamy znak "=" po komendzie AT:
					if( data[strlen(AT_command_array[i].cmd)] == '=' )
					{
						// przesunmy sobie wskaznik
						data += strlen(AT_command_array[i].cmd) + 1;

						// ustawiamy pierwszy parametr (jesli wystepuje)
						if(data[0] != 0)
						{
							char *wsk_temp; // zmienna tymczasowa dla funkcji strtok_r
							params[0] = strtok_r(data, ",",&wsk_temp);

							// jesli pierwszy parametr ma jakas dlugosc:
							if(params[0] != 0)
							{
								params_cnt++;
								// dokonujemy rozdzielania komend
								for(int i = 1; i < _MAX_PARAMETERS; i++)
								{
									params[i] =  strtok_r(NULL, ",",&wsk_temp);

									if(params[i] == NULL)
									{
										break;
									}
									params_cnt++;
								}
							}
						}
					}
					// wywolanie funkcji
					AT_command_array[i].callback_function(params, params_cnt);
				}
				// opuszczenie funkcji parsujacej - nie skanujemy dalszych komend
				break;
			}
		}
		// sprawdzenie, czy znalezlismy jakas komende - jesli nie - uruchamiamy funkcje odnosnie bledu
#if USE_AT_ERRORS == 1
        if(i == AT_commands_number)
        {
        	// przywrocenie znaku rownosci:
			if(temp_wsk)
			{
				*temp_wsk = '=';
			}
            if(AT_command_error_callback)
            {
                AT_command_error_callback(data);
            }
        }
#endif // USE_AT_ERRORS
	}
	else
    {
#if USE_NO_AT_COMMANDS == 1
        if(no_AT_command_array)
        {
            int i = 0;
            for(i = 0; i < no_AT_commands_number; i++)
            {
                if( strcmp(no_AT_command_array[i].cmd, data ) == 0 )
                {
                    // najpierw sprawdzmy czy callback jest podpiety
                    if( no_AT_command_array[i].callback_function )
                    {
                        // wywolanie podanej funkcji (bez parametrow)
                        no_AT_command_array[i].callback_function();
                    }
                    // opuszczenie petli parsujacej - nie skanujemy dalszych komend
                    break;
                }
            }
        // sprawdzenie, czy znalezlismy jakas komende - jesli nie - uruchamiamy funkcje odnosnie bledu
#if USE_AT_ERRORS == 1
            if(i == no_AT_commands_number)
            {
                if(AT_command_error_callback)
                {
                    AT_command_error_callback(data);
                }
            }
#endif // USE_AT_ERRORS
        }
#else
        // uruchamiamy funkcje odnosnie bledu
        if(AT_command_error_callback)
        {
            AT_command_error_callback(data);
        }
#endif // USE_NO_AT_COMMANDS
    }
}




// wersja dekodowania komend AT z uzyciem struktur
// dzieki temu nie musimy rejestrowac osobno wszystkich rzeczy
// przeznaczona glownie do projektow, gdzie parser jest wykorzystywany kilkukrotnie
// zakladam, ze jesli jest wykorzystywana inicjalizacja poprzez strukture to nie uwzgledniamy makr konfiguracyjnych
void AT_command_decode_struct(char *data, const AT_parser_settings *settings)
{
    // tablica z parametrami
    char* params[_MAX_PARAMETERS];

    // ilosc znalezionych parametrow
    uint8_t params_cnt = 0;

	// test naglowka komendy - musi to byc "AT+"
	if(data[0] == 'A' && data[1] == 'T' && data[2] == '+')
	{
		// przesuniecie wskaznika
		data += 3;

        // wskaznik tymczasowy dla operacji na lancuchu
        char * temp_wsk = 0;

        // musimy zamienic znak rownosci na 0 - bo nie porownamy inaczej napisow
        temp_wsk = strchr(data,'=');
        if(temp_wsk)
        {
            *temp_wsk = 0;
        }

		// petla po wszystkich komendach - jesli nie mamy w sobie odpowiedniej komendy - wykonamy wszystkie iteracje
		// w przypadku znalezienia komendy - opuszczamy petle
        uint8_t i = 0;
		for( i = 0; i < settings->AT_commands_number; i++)
		{
		    // porownanie odebranego napisu i komend z tablicy
			if( strcmp(settings->AT_command_array[i].cmd, data ) == 0 )
			{
				//  sprawdzmy czy callback jest podpiety - jesli nie nie ma sensu dekodowac parametrow
				if( settings->AT_command_array[i].callback_function )
				{
					params_cnt = 0; // zerowanie licznika parametrow

					// przywrocenie znaku rownosci:
					if(temp_wsk)
					{
						*temp_wsk = '=';
					}

					// sprawdzamy, czy mamy znak "=" po komendzie AT:
					if( data[strlen(settings->AT_command_array[i].cmd)] == '=' )
					{
						// przesunmy sobie wskaznik
						data += strlen(settings->AT_command_array[i].cmd) + 1;

						// ustawiamy pierwszy parametr (jesli wystepuje)
						if(data[0] != 0)
						{
							char *wsk_temp; // zmienna tymczasowa dla funkcji strtok_r
							params[0] = strtok_r(data, ",",&wsk_temp);

							// jesli pierwszy parametr ma jakas dlugosc:
							if(params[0] != 0)
							{
								params_cnt++;
								// dokonujemy rozdzielania komend
								for(int i = 1; i < _MAX_PARAMETERS; i++)
								{
									params[i] =  strtok_r(NULL, ",",&wsk_temp);

									if(params[i] == NULL)
									{
										break;
									}
									params_cnt++;
								}
							}
						}
					}
					// wywolanie funkcji
					settings->AT_command_array[i].callback_function(params, params_cnt);
				}
				// opuszczenie funkcji parsujacej - nie skanujemy dalszych komend
				break;
			}
		}
		// sprawdzenie, czy znalezlismy jakas komende - jesli nie - uruchamiamy funkcje odnosnie bledu
        if(i == settings->AT_commands_number)
        {
        	// przywrocenie znaku rownosci:
			if(temp_wsk)
			{
				*temp_wsk = '=';
			}
            if(settings->AT_command_error_callback)
            {
            	settings->AT_command_error_callback(data);
            }
        }
	}
	else
    {
        if(settings->no_AT_command_array)
        {
            uint8_t i = 0;
            for(i = 0; i < settings->no_AT_commands_number; i++)
            {
                if( strcmp(settings->no_AT_command_array[i].cmd, data ) == 0 )
                {
                    // najpierw sprawdzmy czy callback jest podpiety
                    if( settings->no_AT_command_array[i].callback_function )
                    {
                        // wywolanie podanej funkcji (bez parametrow)
                    	settings->no_AT_command_array[i].callback_function();
                    }
                    // opuszczenie petli parsujacej - nie skanujemy dalszych komend
                    break;
                }
            }
            // sprawdzenie, czy znalezlismy jakas komende - jesli nie - uruchamiamy funkcje odnosnie bledu
            if(i == settings->no_AT_commands_number)
            {
                if(settings->AT_command_error_callback)
                {
                	settings->AT_command_error_callback(data);
                }
            }
        }
    }
}
