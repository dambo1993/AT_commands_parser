/*
 * AT_parser_settings.h
 *
 *  Created on: 03.05.2018
 *      Author: Przemek
 */

#ifndef AT_PARSER_AT_PARSER_SETTINGS_H_
#define AT_PARSER_AT_PARSER_SETTINGS_H_

//! tutaj wybieramy, czy mamy obslugiwac takze dodatkowa tablice z komendami niezaczynajacymi sie od "AT+"
//! 0 - wylaczone
//! 1 - wlaczone
#define USE_NO_AT_COMMANDS 1

//! tutaj wybieramy, czy mamy obslugiwac takze bledy gdy nie wykryjemy komendy
//! 0 - wylaczone
//! 1 - wlaczone
#define USE_AT_ERRORS 1

//! UWAGA!!!
//! zakladam, ze jesli jest wykorzystywana inicjalizacja poprzez strukture to nie uwzgledniamy powyzszych makr konfiguracyjnych

//! Maksymalna ilosc parametrow jakie bedziemy uzywac - wyznacza wielkosc tablicy z parametrami
#define _MAX_PARAMETERS 20

#endif /* AT_PARSER_AT_PARSER_SETTINGS_H_ */
