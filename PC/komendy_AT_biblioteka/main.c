#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AT_parser.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNKCJE TESTOWE UZYWANE DO POKAZANIA DZIALANIA BIBLIOTEKI
////////////////////////////////////////////////////////////////////////////////////////////////////////

//! prosta funkcja testowa - wyswietla napis
//! komenda dla niej to "AT+TEST"
void test()
{
    puts("");
    puts("Weszlismy w funkcje test");
    puts("");
}

//! prosta funkcja testujaca - printuje gwiazdki na ekranie
//! komenda dla niej - AT+STAR=xx
//! gdzie X to ilosc gwiazdek
void stars(char** params_array,int params_cnt)
{
    puts("");
    puts("Weszlismy w funkcje rysowania gwiazdek");

    // sprawdzamy poprawna ilosc parametrow
    if(params_cnt == 1)
    {
        int ilosc_gwiazdek = atoi(params_array[0]);

        for(int i = 0; i < ilosc_gwiazdek; i++)
        {
            for(int j = 0; j < i; j++)
            {
                printf("*");
            }
            printf("\r\n");
        }
    }
    else
    {
        puts("Niepoprawna ilosc parametrow");
    }
    printf("\r\n\r\n");
}

//! funkcja jako parametry przyjmuje dzien, miesiac i rok
//! w formacie dzien,miesiac,rok np 10,01,1993
//! jako prezentacja dzialania biblioteki
//! komenda dla niej to "AT+DATA=12,13,2006"
void wyswietl_date(char** params_array,int params_cnt)
{
    printf("\r\n\r\n");
    puts("Weszlismy w funkcje wyswietl data");

    printf("Ilosc parametrow: %d\r\n", params_cnt);

    // jesli mamy odpowiednia ilosc parametrow:
    if(params_cnt == 3)
    {
        printf("Data to: %s-%s-%s\r\n", params_array[0],params_array[1],params_array[2]);
    }
    else
    {
        puts("Niepoprawna ilosc parametrow");
    }
    printf("\r\n\r\n");
}

//! funkcja testuje wydobywanie parametrow z napisu
//! wyswietla ich ilosc i wartosci
//! komenda dla niej to "AT+TEST_PARAMS=a,b,c,d,e...
void testuj_parametry(char** params_array, int params_cnt)
{
    printf("\r\n\r\n");

    printf("Ilosc parametrow: %d\r\n", params_cnt);
    puts("Poszczegolne parametry:\r\n");

    for(int i = 0; i < params_cnt; i++)
    {
        printf("Parametr numer %d to %s\r\n", i, params_array[i]);
    }

    printf("\r\n\r\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Biblioteka do parsowania komend AT
////////////////////////////////////////////////////////////////////////////////////////////////////////

//! Biblioteka wstepnie parsujaca przychodzace komendy AT
//! umozliwia proste zadeklarowanie nazw komend oraz podpiecie funkcji wykonawczych
//! posiada wbudowany mechanizm parsowania otrzymanych parametrow
//! zalozono przy tym warunek, ze sa one ROZDZIELONE MIEDZY SOBA PRZECINKAMI np AT+TEST=1,2,3,4 -> i tylko taki format domyslnie obsluguje biblioteka
//! funkcje wykonawcze moga nie przyjmowac zadnego parametru lub musza przyjmowac wskaznik na tablice ze wskaznikami na parametry oraz ilosc dostepnych parametrow
//! zalezy to od pola "type" w strukturze _at_cmd
//! zastanawialem sie takze nad umieszczeniem w strukturze ilosci parametrow, ale ostatecznie z tego zrezygnowalem
//! dzieki temu funkcje moga przyjmowac rozne ilosci parametrow i w sobie zdecydowac co z tym fantem zrobic
//! istnieje tez mozliwosc wywolania funkcji przyjmujacej parametry z zerowa iloscia parametrow (wtedy params_cnt = 0)
//! z drugiej strony - jesli do funkcji nieprzyjmujacej parametry podamy je w komendzie zostana one calkowicie pominiete


//! glowna tablica z komendami AT - uzupelniamy ja naszymi komendami AT
//! a nastepnie przekazujemy do biblioteki parsera
const t_cmd AT_cmd_array[] = {
		{"TEST",        at_type_no_params,   test},                 // funkcja testujaca - pierwsza funkcja do testow biblioteki
		{"TEST_PARAMS", at_type_params,      testuj_parametry},     // wyswietlenie parametrow - test parsowania
		{"DATA",        at_type_params,      wyswietl_date},        // wyswietlenie daty - test parsowania
		{"STARS",       at_type_params,      stars},                // wyswietlenie gwiazdek - test parsowania
};

void przecinek()
{
    int a = 2;

    a = ( puts("x"),puts("y"),puts("z"),10);

    printf("%d\r\n", a);
}

int main()
{
    przecinek();
    char tablica[100] = "";

    puts("TEST BIBLIOTEKI DO KOMEND AT");

    // rejestracja tablicy ze strukturami komend
    AT_register_AT_commands_table(AT_cmd_array, (sizeof(AT_cmd_array)/sizeof(AT_cmd_array[0])));

    // testowe wytswietlenie wszystkich komend
    AT_wyswietl_dostepne_komendy();

    puts("Podaj swoja komende:");

    // pobbranie napisu z wejscia
    scanf("%s",tablica);

    printf("Otrzymana komenda: %s\r\n",tablica);
    printf("Uruchamiam funkcje dekodujaca:\r\n\r\n");

    // uruchomienie funkcji dekodujacej z biblioteki
    AT_commands_decode(tablica);

    puts("Zakonczono dzialanie programu");

    return 0;
}
