
## Server FIFO

###  Plan: 
##### 1. Importuj wymagane biblioteki.
##### 2. Zadeklaruj stałą `fifoServerPath` przechowującą ścieżkę do pliku FIFO serwera.
##### 3. Zadeklaruj zmienne `fifoServerRead` i `fifoServerWrite` jako deskryptory do odczytu i zapisu pliku FIFO serwera.
##### 4. Zdefiniuj funkcję obsługi sygnałów (`handleSignals`), która zamyka deskryptory FIFO serwera, usuwa plik FIFO i kończy działanie programu.
##### 5. W funkcji `main` zadeklaruj zmienne `clientRead` i `clientWrite` jako deskryptory do odczytu i zapisu pliku FIFO klienta.
##### 6. Zadeklaruj tablicę `buffer` o rozmiarze 1000 do przechowywania danych otrzymanych z FIFO.
##### 7. Zadeklaruj wskaźnik `pointer`, który będzie używany do manipulowania danymi w tablicy `buffer`.
##### 8. Zadeklaruj zmienne `clientPID` i `forkedProcessID` do przechowywania identyfikatorów klienta i forka.
##### 9. Ustaw obsługę sygnałów `SIGTERM` i `SIGINT` na funkcję `handleSignals`.
##### 10. Utwórz plik FIFO serwera za pomocą funkcji mknod z odpowiednimi uprawnieniami.
##### 11. Otwórz plik FIFO serwera w trybie tylko do odczytu i zapisu oraz przypisz deskryptory do zmiennych `fifoServerRead` i `fifoServerWrite`.
##### 12. W pętli nieskończonej odbieraj identyfikatory klientów z pliku FIFO serwera.
##### 13. Odczytaj identyfikator klienta z pliku FIFO serwera i przypisz go do zmiennej `clientPID`.
##### 14. Utwórz nowy proces za pomocą funkcji `fork` i przypisz identyfikator nowego procesu do zmiennej `forkedProcessID`.
##### 15. W bloku warunkowym sprawdź, czy aktualnie wykonujemy kod w procesie potomnym.
##### 16. Skonstruuj ścieżkę do pliku FIFO klienta na podstawie identyfikatora klienta.
##### 17. Otwórz plik FIFO klienta w trybie tylko do odczytu i przypisz deskryptor do zmiennej `clientRead`.
##### 18. Zmodyfikuj ścieżkę pliku FIFO, aby uzyskać ścieżkę pliku FIFO do zapisu, i otwórz plik FIFO klienta w trybie tylko do zapisu. Przypisz deskryptor do zmiennej `clientWrite`, jeśli otwarcie powiedzie się.
##### 19. Jeśli otwarcie pliku FIFO klienta do zapisu powiedzie się, odczytaj wartość a ze strumienia `clientRead`, wyświetl ją na standardowym wyjściu, oblicz wartość `b` (kwadrat `a`) i zapisz `b` do strumienia `clientWrite`.
##### 20. Zamknij deskryptory pliku FIFO klienta i serwera oraz deskryptor pliku FIFO serwera, który nie jest już potrzebny w procesie potomnym.
##### 21. Proces potomny kończy swoje działanie, zwracając wartość 0.
##### 22. Blok `else` w procesie macierzystym oczekuje na zakończenie procesu potomnego za pomocą funkcji `wait`.

### Biblioteki:
| Biblioteka | Funkcje     | Opis                       |
| :-------- | :------- | :-------------------------------- |
| `unistd.h`      | `close()`, `read()`, `write()`, `fork()` i `unlink()` | Ta biblioteka zawiera funkcje do zarządzania operacjami wejścia/wyjścia na niskim poziomie oraz funkcje dotyczące obsługi procesów. W kodzie wykorzystuje funkcje takie jak close(), read(), write(), fork() i unlink(). |
| `stdio.h`      | `printf()` | Ta biblioteka zawiera funkcje do operacji wejścia/wyjścia standardowego. W kodzie używa funkcji printf() do wyświetlania danych na standardowym wyjściu. |
| `stdlib.h`      | `exit()` | Ta biblioteka zawiera funkcje ogólnego zastosowania. W kodzie używa funkcji exit() do natychmiastowego zakończenia programu. |
| `fcntl.h`      | `open()` | Ta biblioteka zawiera funkcje do zarządzania deskryptorami plików. W kodzie używa funkcji open() do otwierania plików. |
| `signal.h`      | `signal()` | Ta biblioteka zawiera funkcje do obsługi sygnałów. W kodzie używa funkcji signal() do instalowania funkcji obsługi sygnałów. |
| `string.h`      | `strcpy()`  i `strlen()` | Ta biblioteka zawiera funkcje do manipulacji łańcuchami znaków. W kodzie używa funkcji takich jak strcpy() i strlen() do operacji na łańcuchach znaków. |
| `sys/uio.h`      | `read()` i `write()` | Ta biblioteka zawiera funkcje do operacji wejścia/wyjścia systemowego. W kodzie używa funkcji read() i write() do operacji wejścia/wyjścia. |
| `sys/stat.h`      | `mknod()` | Ta biblioteka zawiera deklaracje dotyczące manipulacji informacjami o plikach. W kodzie używa funkcji mknod() do tworzenia nowych węzłów w systemie plików. |
| `sys/wait.h`      | `wait()` | Ta biblioteka zawiera deklaracje funkcji dotyczących procesów i oczekiwania na ich zakończenie. W kodzie używa funkcji wait() do oczekiwania na zakończenie procesu potomnego. |

### Metody:
- close(file_descriptor) - Ta funkcja zamyka otwarty deskryptor pliku, identyfikowany przez file_descriptor. Jest często używana do zwalniania zasobów systemowych po zakończeniu operacji na plikach.
```c
close(fifoServerRead);
close(fifoServerWrite);
```
- read(file_descriptor, buffer, count) - Funkcja read() odczytuje dane z określonego deskryptora pliku (file_descriptor) i umieszcza je w buforze (buffer). Parametr count określa maksymalną ilość bajtów do odczytania.
```c
read(fifoServerRead, &clientPID, sizeof(pid_t));
```
- write(file_descriptor, buffer, count) - Ta funkcja zapisuje dane z bufora (buffer) do określonego deskryptora pliku (file_descriptor). Parametr count określa liczbę bajtów do zapisania.
```c
write(clientWrite, &b, sizeof(double));
```
- printf(format, ...): Jest to funkcja używana do formatowanego wyświetlania danych na standardowym wyjściu (ekranie). Przyjmuje ona argumenty zmiennej długości, które są interpretowane zgodnie z podanym formatem.
```c
printf("Otrzymana wiadomość %lf:\n", a);
```
- exit(status) - Funkcja exit() służy do natychmiastowego zakończenia programu i powrotu do systemu operacyjnego. Parametr status to kod wyjścia, który jest przekazywany do systemu operacyjnego.
```c
exit(0);
```
- open(path, flags) - Ta funkcja otwiera określony plik o podanej ścieżce (path) i zwraca deskryptor pliku. Parametr flags definiuje tryb otwarcia pliku, takie jak odczyt (O_RDONLY), zapis (O_WRONLY), lub oba (O_RDWR).
```c
clientRead = open(buffer, O_RDONLY);
clientWrite = open(buffer, O_WRONLY)
```
- signal(signal_number, signal_handler) - Funkcja signal() instaluje obsługę sygnału (signal_handler) dla określonego numeru sygnału (signal_number). W przypadku otrzymania sygnału, system operacyjny wywoła funkcję obsługi sygnału.
```c
signal(SIGTERM, handleSignals);
signal(SIGINT, handleSignals);
```
- strcpy(destination, source) - Funkcja strcpy() kopiuje zawartość łańcucha znaków z source do destination. Wartością zwracaną jest wskaźnik do destination.
```c
strcpy(pointer, "/tmp/FIFO-TMP-2-");
```
- strlen(string) - Funkcja strlen() zwraca długość łańcucha znaków string, czyli liczbę znaków przed znakiem null ('\0').
```c
pointer += strlen(pointer);
```
- mknod(path, mode, dev) - Funkcja mknod() tworzy nowy węzeł w systemie plików o podanej ścieżce (path). Parametr mode określa tryb dostępu do węzła, a dev jest używany tylko dla tworzenia specjalnych plików urządzeń.
```c
mknod(fifoServerPath, S_IFIFO | 0600, 0);
```
- wait(status) - Funkcja wait() czeka na zakończenie dowolnego dziecka procesu i zwraca identyfikator tego procesu. Parametr status jest używany do przechwycenia kodu wyjścia dziecka.
```c
wait(NULL);
```
- unlink(path) - Ta funkcja usuwa plik o podanej ścieżce (path) z systemu plików.
```c
unlink(fifoServerPath);
```
- sprintf(buffer, format, ...) - Funkcja sprintf() działa podobnie jak printf(), ale zapisuje sformatowane dane do bufora (buffer) zamiast na standardowe wyjście.
```c
sprintf(pointer, "%d", (int)clientPID);
```
- fork() - Funkcja fork() tworzy nowy proces poprzez duplikowanie bieżącego procesu. Powstały proces, zwany procesem potomnym, ma identyczną kopię przestrzeni adresowej i stanu procesu macierzystego, ale różne identyfikatory procesów (PID).
```c
forkedProcessID = fork();
```

## Client FIFO

###  Plan: 
##### 1. Importuj wymagane biblioteki.
##### 2. Zadeklaruj stałą dirServer, która przechowuje ścieżkę do pliku FIFO serwera.
##### 3. W funkcji main zadeklaruj zmienne i tablice potrzebne do działania programu.
##### 4. Pobierz identyfikator bieżącego procesu za pomocą funkcji getpid() i przypisz go do zmiennej processNo.
##### 5. Otwórz plik FIFO serwera w trybie tylko do zapisu za pomocą funkcji open() i przypisz deskryptor do zmiennej FIFOserver.
##### 6. Skonstruuj ścieżkę pliku FIFO klienta na podstawie identyfikatora procesu i zapisz ją w tablicy bufor za pomocą funkcji strcpy() i sprintf().
##### 7. Utwórz plik FIFO klienta za pomocą funkcji mknod() i ustaw odpowiednie uprawnienia (S_IFIFO | 0600).
##### 8. Zmień przedostatni znak w tablicy bufor na '2', a następnie ponownie utwórz plik FIFO klienta za pomocą mknod(). Jest to potrzebne, aby utworzyć dwa różne pliki FIFO - jeden do odczytu, drugi do zapisu.
##### 9. Zapisz identyfikator procesu do pliku FIFO serwera przy użyciu funkcji write().
##### 10. Zamknij deskryptor pliku FIFO serwera za pomocą funkcji close().
##### 11. Otwórz plik FIFO klienta do zapisu za pomocą open() i przypisz deskryptor do zmiennej FIFOwrite.
##### 12. Zmień przedostatni znak w tablicy bufor na '1', a następnie otwórz plik FIFO klienta do odczytu za pomocą open(). Deskryptor pliku FIFO do odczytu zostaje przypisany do zmiennej FIFOread.
##### 13. Zadeklaruj zmienne a i b do przechowywania danych liczbowych.
##### 14. Poproś użytkownika o podanie liczby za pomocą funkcji printf() i scanf().
##### 15. Zapisz podaną liczbę a do pliku FIFO klienta do zapisu przy użyciu write().
##### 16. Odczytaj wynik z pliku FIFO klienta do odczytu za pomocą read() i zapisz go do zmiennej b.
##### 17. Wyświetl wynik obliczeń na standardowym wyjściu przy użyciu printf().
##### 18. Zamknij deskryptory plików FIFO i usunij pliki FIFO z systemu plików za pomocą close(), unlink() i modyfikacji tablicy bufor.
##### 19. Zwróć wartość 0, oznaczającą pomyślne zakończenie programu.

### Biblioteki:
| Biblioteka | Funkcje     | Opis                       |
| :-------- | :------- | :-------------------------------- |
| `unistd.h`      | `close()`, `read()`, `write()`, `getpid()` i `unlink()` | Ta biblioteka zawiera funkcje do zarządzania operacjami wejścia/wyjścia na niskim poziomie oraz funkcje dotyczące obsługi procesów. W kodzie wykorzystuje funkcje takie jak close(), read(), write(), fork() i unlink(). |
| `stdio.h`      | `printf()`, `scanf()` | Ta biblioteka zawiera funkcje do operacji wejścia/wyjścia standardowego. W kodzie używa funkcji printf() do wyświetlania danych na standardowym wyjściu. |
| `fcntl.h`      | `open()` | Ta biblioteka zawiera funkcje do zarządzania deskryptorami plików. W kodzie używa funkcji open() do otwierania plików. |
| `string.h`      | `strcpy()`  i `strlen()` | Ta biblioteka zawiera funkcje do manipulacji łańcuchami znaków. W kodzie używa funkcji takich jak strcpy() i strlen() do operacji na łańcuchach znaków. |
| `sys/stat.h`      | `mknod()` | Ta biblioteka zawiera deklaracje dotyczące manipulacji informacjami o plikach. W kodzie używa funkcji mknod() do tworzenia nowych węzłów w systemie plików. |

### Metody:
- open(path, flags): Funkcja otwiera plik o podanej ścieżce path i zwraca deskryptor pliku. W kodzie używa się jej do otwarcia pliku FIFO serwera w trybie tylko do zapisu oraz pliku FIFO klienta w trybie zapisu i odczytu.
```c
FIFOserver = open(dirServer, 1);
```
- getpid(): Funkcja zwraca identyfikator bieżącego procesu. W kodzie używa się jej do pobrania identyfikatora klienta.
```c
processNo = getpid();
```
- strcpy(destination, source): Funkcja kopiuje łańcuch znaków z source do destination. W kodzie używa się jej do skonstruowania ścieżki pliku FIFO klienta.
```c
strcpy(pom, "/tmp/FIFO-TMP-1-");
```
- sprintf(buffer, format, ...): Funkcja zapisuje sformatowane dane do bufora buffer. W kodzie używa się jej do zapisania identyfikatora klienta jako łańcucha znaków w tablicy bufor.
```c
sprintf(pom, "%d", (int) processNo);
```
- mknod(path, mode, dev): Funkcja tworzy nowy węzeł w systemie plików o podanej ścieżce path i ustawia tryb dostępu do węzła na mode. W kodzie używa się jej do utworzenia plików FIFO serwera i klienta.
```c
mknod(bufor, S_IFIFO | 0600, 0);
```
- write(file_descriptor, buffer, count): Funkcja zapisuje dane z bufora buffer do deskryptora pliku file_descriptor. W kodzie używa się jej do zapisania identyfikatora klienta do pliku FIFO serwera oraz liczby a do pliku FIFO klienta do zapisu.
```c
write(FIFOserver, &processNo, sizeof(pid_t));
```
- close(file_descriptor): Funkcja zamyka otwarty deskryptor pliku file_descriptor. W kodzie używa się jej do zamknięcia deskryptorów plików FIFO serwera, klienta do zapisu i klienta do odczytu.
```c
close(FIFOserver);
```
- read(file_descriptor, buffer, count): Funkcja odczytuje dane z deskryptora pliku file_descriptor i zapisuje je do bufora buffer. Odczytuje maksymalnie count bajtów. W kodzie używa się jej do odczytu wyniku z pliku FIFO klienta do odczytu.
```c
read(FIFOread, &b, sizeof(double));
```
- printf(format, ...): Funkcja wyświetla sformatowane dane na standardowym wyjściu. W kodzie używa się jej do wyświetlenia zapytania o liczbę oraz wyniku obliczeń.
```c
printf("Podaj liczbe: ");
```
- scanf(format, ...): Funkcja wczytuje dane ze standardowego wejścia zgodnie z podanym formatem i zapisuje je w podanych argumentach. W kodzie używa się jej do pobrania liczby od użytkownika.
```c
scanf("%lf", &a);
```
- unlink(path): Funkcja usuwa plik o podanej ścieżce path z systemu plików. W kodzie używa się jej do usunięcia plików FIFO serwera i klienta po zakończeniu działania programu.
```c
unlink(bufor);
```

## Podsumowanie
### Biblioteki:
- `#include <unistd.h>`
- `#include <fcntl.h>`
- `#include <signal.h>`
- `#include <string.h>`
- `#include <stdio.h>`
- `#include <stdlib.h>`
- `#include <sys/stat.h>`
- `#include <sys/uio.h>`
- `#include <sys/wait.h>`
