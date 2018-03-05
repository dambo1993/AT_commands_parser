#include "AT_parser.h"
#include "stm32f0xx.h"
#include "strings.h"

// wskaznik na tablice ze strukturami komend AT
const t_cmd * AT_command_array;

// ilosc elementow w tablicy komend
int AT_commands_number = 0;

//! funkcja rejestrujaca tablice z komendami AT
//! podajemy wskaznik na tablice oraz jej wielkosc
void AT_register_AT_commands_table(const t_cmd *wsk, uint8_t ilosc_parametrow)
{
    AT_command_array = wsk;
    AT_commands_number = ilosc_parametrow;
}

#if USE_NO_AT_COMMANDS == 1
// wskaznik na tablice ze strukturami komend AT
static t_cmd_no_at *no_AT_command_array = 0;

// ilosc elementow w tablicy komend
int no_AT_commands_number = 0;

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

//! glowna funkcja dekodujaca komendy AT
//! jako parametr przyjmuje wskaznik na bufor z odebranym napisem
void AT_commands_decode(char* data)
{
    // statici - zeby wrzucic je na stale do RAMu
    // tablica z parametrami
    static char* params[_MAX_PARAMETERS];

    // ilosc znalezionych parametrow
    static int params_cnt;

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
        int i = 0;
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
							params[0] = strtok(data, ",");

							// jesli pierwszy parametr ma jakas dlugosc:
							if(params[0] != 0)
							{
								params_cnt++;
								// dokonujemy rozdzielania komend
								for(int i = 1; i < _MAX_PARAMETERS; i++)
								{
									params[i] = strtok(NULL, ",");

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
