# AT commands parser

Projekt zawiera prostą biblioteke do parsowania przychodzących komend AT do naszego urządzenia/programu. Dodatkowo oprócz rozpoznania komendy dokunuje tez wstepnego parsowania przesłanych parametrów - dzięki czemu nie musimy tego pisać sami w każdej funkcji.

## Zawartość repozytorium

W repozytorium znajdziemy:
* przykładowy testowy projekt napisany dla PC do przetestowania działania biblioteki (projekt codeblocks)
* przykładowy projekt dla STM32F030F4P6 i HD447870 z obsługą tego wyświetlacza za pomocą komend AT (projekt Atollic)
* "czyste" pliki biblioteki do uC

### Zasada działania

Podstawowym elementem biblioteki jest struktura przedstawiająca pojedynczą komendę AT:
```c
//! structure for single ATcommand
typedef struct{
	const char *cmd;                        // array with command name
	void (*callback_function)();            // callback function pointer
} t_cmd;
```
Składa się ona więc z nazwy komendy oraz funkcji zwrotnej.
Przykład obiektu takiej struktury:
```C
{"TEST",        test}
```
Oczywiście takich komend będziemy mieli wiele więc musimy utworzyć tablicę takich struktur (z przykładowego programu PC):
```C
//! glowna tablica z komendami AT - uzupelniamy ja naszymi komendami AT
//! a nastepnie przekazujemy do biblioteki parsera
const t_cmd AT_cmd_array[] = {
		{"TEST",        test},                 // funkcja testujaca - pierwsza funkcja do testow biblioteki
		{"TEST_PARAMS", testuj_parametry},     // wyswietlenie parametrow - test parsowania
		{"DATA",        wyswietl_date},        // wyswietlenie daty - test parsowania
		{"STARS",       stars},                // wyswietlenie gwiazdek - test parsowania
};
```
Tablicę z komendami możemy utworzyć np w pliku main. Do biblioteki przekazujemy wskaźnik na nią oraz jej wielkość w taki sposób:
```C
AT_register_AT_commands_table(AT_cmd_array, (sizeof(AT_cmd_array)/sizeof(AT_cmd_array[0])));
```
Ostatnim krokiem jest uruchamianiw parsowania odebranych danych:
```C
AT_commands_decode(tablica);
```

Wróćmy do funkcji i przekazywania parametrów. Każda podpięta funkcja musi przyjmować 2 parametry: 
* wskaznik na tablice wskaznikow do parametrow 
* ilosc parametrow

Przykład:
```C
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
```
Jak widać nasza funkcja przyjmuje już wskaźniki na poszczególne parametry i mozemy się do nich odwołać przez params_array[i]. Istnieje także mozliwość, że nie będzie podanych do niej parametrów - musimy się na to w środku przygotować/zabezpieczyć.
### Załozenia przyjęte w bibliotece:

* każda komenda zaczyna się od znaków "AT+" i kończy znakiem '\0' (czyli jest C stringiem)
* parametry dla funkcji podajemy po znaku "=" np. "AT+TEST=123,123"
* parametry dla funkcji rozdzielone są przecinkami
* funkcja musi przyjmować jako pierwszy parametr wskaznik na tablice wskaznikow do parametrow oraz ilosc parametrow [(char** params_array,int params_cnt)]
* funkcja musi sama sprawdzic w swoim ciele, czy ilosc parametrów się zgadza (daje to możliwość rozróżnienia działania w zależności od ilości parametrów)

### Możliwość rozwoju/rozszerzenia

Wpadłem na kilka pomysłów na dalsze ulepszenie biblioteki, aczkolwiek obecnie nie są mi one potrzebne, więc poczekają w kolejce na lepsze czasy. Są to:
* możliwość podpięcia funkcji, która wykona sie w przypadku otrzymania błędnej komendy
* obecnie w przykładzie na STM32 każda funkcja musi sama wypisać na konsole error, ok itp - można pomyślec o uniwersalniejszym rozwiązaniu - dać możliwość funkcjom z callbacków zwrócenia także jakiś wartości i podejmować decyzje w zależności od tego typu (np samo ERR, lub jakieś kody błędów, obsługa zwrotnych wiadomości w postaci "+komenda=x,z,a" w zależności od potrzeb
* obsługa "krótkich" komend typu "AT", "ATI"

## Autor

* **dambo** - [Blog](http://projektydmb.blogspot.com)

## Podziękowania

Podziękowania dla:
- kolegi "antystatyczny" z forum [microgeek](https://microgeek.eu) za rzucenie okiem na to wszystko we wstępnej fazie projektu
- kolegi "xor" z forum [microgeek](https://microgeek.eu) za wybicie mi z głowy wcześniejszej koncepcji z różnymi typami podpinanych funkcji i podawania jaki to typ w zmiennej w strukturze
