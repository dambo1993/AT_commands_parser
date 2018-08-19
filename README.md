# AT commands parser

Projekt zawiera prostą biblioteke do parsowania przychodzących komend AT do naszego urządzenia/programu. Dodatkowo oprócz rozpoznania komendy dokunuje tez wstepnego parsowania przesłanych parametrów - dzięki czemu nie musimy tego pisać sami w każdej funkcji.


## UWAGA!!!

Poniższy opis jest poprawny dla galezi "MAKRA", nie dla mastera - tu zmienilem troche koncepcje, ale musze przygotowac dla niego pelne README z opisem co i jak.

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

Warto wspomnieć, że podczas porównywania komend z tablicy z otrzymanym napisem robione jest to w pętli - poprzez umieszczenie w początkowych pozycjach komend, które spodziewamy się, ze będą najczęściej używane mozemy dokonać małego przyspieszenia procesu dekodowania dla nich.

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

### UPDATE - 05.03.18r.

Miło mi napisać, że 2/3 rozdziału wyżej "Możliwość rozwoju/rozszerzenia" należy już do przeszłości. Mozemy teraz obsługiwać komendy "no_AT". Trzeba to uruchomić za pomocą makra w pliku .h:
```C
//! tutaj wybieramy, czy mamy obslugiwac takze dodatkowa tablice z komendami niezaczynajacymi sie od "AT+"
//! 0 - wylaczone
//! 1 - wlaczone
#define USE_NO_AT_COMMANDS 1
```
Przygotowano też do tego odpowiednie funkcje.

Tak samo obsługa błędów - uruchamiamy ją w podobny sposób:
```C
//! tutaj wybieramy, czy mamy obslugiwac takze bledy gdy nie wykryjemy komendy
//! 0 - wylaczone
//! 1 - wlaczone
#define USE_AT_ERRORS 1
```
Jednak rejestrujemy tylko pojedynczy callback. Funkcja zwrotna musi przyjmować wskaźnik na tablicę typu char - będzie znajdować się tam komenda, która jest błędna - można ją wtedy odesłać do użytkownika, lub zignorować. Kawałek kodu:
```C
//! typ dla callbackow od blednej komendy - funkcja zwraca wskaznik do odebranego napisu
typedef void (*AT_command_error_type)(char *);

//! funkcja rejestrujaca callbacka od otrzymania blednej komendy
void AT_register_error_function(const AT_command_error_type);
```

W projekcie codeblocksa można łatwo przetestować działanie tego wszystkiego.

## Autor

* **dambo** - [Blog](http://projektydmb.blogspot.com)

## Podziękowania

Podziękowania dla:
- kolegi "antystatyczny" z forum [microgeek](https://microgeek.eu) za rzucenie okiem na to wszystko we wstępnej fazie projektu
- kolegi "xor" z forum [microgeek](https://microgeek.eu) za wybicie mi z głowy wcześniejszej koncepcji z różnymi typami podpinanych funkcji i podawania jaki to typ w zmiennej w strukturze
