# Kolejka komunikatów IPC:

- Kolejka komunikatów IPC (Inter-Process Communication) jest mechanizmem komunikacji międzyprocesowej, w której procesy wysyłają i odbierają komunikaty zdefiniowanego formatu.
- Kolejka komunikatów składa się z bufora, w którym przechowywane są komunikaty. Każdy komunikat ma określony typ, który służy do jego identyfikacji.
- Procesy korzystają z funkcji do wysyłania (msgsnd) i odbierania (msgrcv) komunikatów z kolejki komunikatów.
- Przy wysyłaniu komunikatu, proces podaje typ komunikatu, treść komunikatu oraz inne parametry, takie jak flagi i priorytet.
- Procesy odbierają komunikaty z kolejki na podstawie ich typu. Odbiór komunikatu może być blokujący lub nieblokujący.
- Kolejka komunikatów zapewnia bezpieczne i niezawodne przesyłanie komunikatów między procesami.

## IPC Serwer - Kolejki komunikatów

### Plan

1. Importuj wymagane biblioteki.
2. Zdefiniuj struktury `msg1` i `msg2` do przechowywania komunikatów.
3. Zadeklaruj zmienne globalne i flagi.
4. Zdefiniuj funkcję obsługi sygnałów (`signalss`), która zwalnia zasoby i kończy program.
5. W funkcji `main` zadeklaruj zmienne lokalne.
6. Ustaw obsługę sygnałów `SIGTERM`, `SIGINT` i zignoruj sygnał `SIGCHLD`.
7. Skonstruuj ścieżkę do pliku kolejki komunikatów.
8. Sprawdź istnienie pliku kolejki komunikatów i ustaw flagę `czy_skasowac`.
9. Utwórz plik kolejki komunikatów, jeśli nie istnieje.
10. Wygeneruj klucz IPC na podstawie ścieżki.
11. Utwórz kolejkę komunikatów z wykorzystaniem klucza.
12. W nieskończonej pętli odbieraj komunikaty z kolejki.
13. Alokuj pamięć dla struktury `msg1`.
14. Odbierz komunikat z identyfikatorem procesu i typem 1.
15. Jeśli odbiór komunikatu się powiedzie, utwórz nowy proces.
16. W bloku warunkowym sprawdź, czy kod wykonuje się w procesie potomnym.
17. Wygeneruj klucz IPC na podstawie ścieżki i identyfikatora procesu.
18. Utwórz kolejkę komunikatów dla danego identyfikatora procesu.
19. Alokuj pamięć dla struktury `msg2`.
20. Odbierz komunikat z danymi od klienta.
21. Wyświetl otrzymane dane na standardowym wyjściu.
22. Przetwórz dane (kwadrat liczby).
23. Ustaw typ komunikatu na 3.
24. Wyślij przetworzone dane z powrotem do klienta.
25. Zwolnij zaalokowaną pamięć.
26. Proces potomny zakończył działanie, zwracając wartość 0.
27. W procesie macierzystym zignoruj sygnał `SIGCHLD`.

## Biblioteki

| Biblioteka  | Opis                                                                                                                                                                                                                                                                 |
|-------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `unistd.h`    | Ta biblioteka zawiera deklaracje standardowych funkcji i stałych, które dostarczają interfejs do systemowych wywołań. W kodzie używana jest do obsługi procesów, sygnałów oraz dostępu do ścieżek.                                                         |
| `stdio.h`     | Ta biblioteka zawiera funkcje wejścia/wyjścia do operacji na strumieniach. W kodzie używana jest do wyświetlania danych na standardowym wyjściu.                                                                                                                       |
| `stdlib.h`    | Ta biblioteka zawiera funkcje ogólnego zastosowania, takie jak alokacja pamięci, konwersje liczb, kontrola procesu itp. W kodzie używana jest do alokacji i zwalniania pamięci.                                                                                           |
| `sys/uio.h`   | Ta biblioteka zawiera deklaracje funkcji dotyczących operacji wejścia/wyjścia systemowego. W kodzie używana jest do operacji na strumieniach i buforach danych.                                                                                                    |
| `signal.h`    | Ta biblioteka zawiera deklaracje funkcji i stałych związanych z obsługą sygnałów. W kodzie używana jest do instalowania obsługi sygnałów.                                                                                                                            |
| `sys/ipc.h`   | Ta biblioteka zawiera deklaracje funkcji i struktur dotyczących mechanizmów IPC (Inter-Process Communication). W kodzie używana jest do generowania klucza IPC na podstawie ścieżki oraz tworzenia/kontroli kolejki komunikatów.                                       |
| `sys/msg.h`   | Ta biblioteka zawiera deklaracje funkcji i struktur dotyczących kolejek komunikatów. W kodzie używana jest do tworzenia i obsługi kolejki komunikatów.                                                                                                              |
| `string.h`    | Ta biblioteka zawiera deklaracje funkcji dotyczących manipulacji łańcuchami znaków. W kodzie używana jest do kopiowania łańcuchów znaków.                                                                                                                         |
| `fcntl.h`     | Ta biblioteka zawiera deklaracje funkcji i stałych dotyczących operacji na deskryptorach plików. W kodzie używana jest do otwierania plików.                                                                                                                      |
| `errno.h`     | Ta biblioteka zawiera deklaracje makr i zmiennych globalnych, które są używane do przechowywania kodu błędu. W kodzie używana jest do obsługi błędów.                                                                                                             |

## Struktury
- msg1 - Struktura reprezentująca komunikat typu 1, zawierająca pole type oznaczające typ komunikatu oraz pole pid przechowujące identyfikator procesu.
```c
typedef struct msg1
{
    long type;
    pid_t pid;
} p1;
```
- msg2 - Struktura reprezentująca komunikat typu 2, zawierająca pole type oznaczające typ komunikatu oraz pole el przechowujące wartość liczbową.
```c
typedef struct msg2
{
    long type;
    double el;
} p2;
```

## Metody

- signalss(i) - Funkcja obsługi sygnałów, która zwalnia zasoby i kończy program.
```c
void signalss(int i)
{
    free(msg);

    msgctl(f, IPC_RMID, NULL);
    if (czy_skasowac)
    {
        unlink(path);
    }
    free(path);
    exit(0);
}
```
- access(path, mode) - Sprawdza dostęp do pliku o podanej ścieżce i zwraca wartość 0 w przypadku sukcesu lub -1 w przypadku błędu.
```c
if (access(path, F_OK) == 0)
        czy_skasowac = 0;
```
- strndup(source, n) - Alokuję pamięć i kopiuje n znaków z łańcucha znaków source.
```c
char *path = strndup("/tmp/roboczy.kolejka_serwer", strlen("/tmp/roboczy.kolejka_serwer"));
```
- unlink(path) - Usuwa plik o podanej ścieżce.
```c
if (czy_skasowac)
  unlink(path);
```
- open(path, flags, mode) - Otwiera plik o podanej ścieżce z podanymi flagami i trybem dostępu i zwraca deskryptor pliku.
```c
int plik = open(path, O_CREAT | O_EXCL, 0600);
```
- ftok(path, id) - Generuje klucz IPC na podstawie ścieżki i identyfikatora.
```c
key_t keyq = ftok(path, 0);
```
- msgget(key, msgflg) - Tworzy kolejkę komunikatów lub pobiera istniejącą kolejkę na podstawie klucza IPC i flag.
```c
int f = msgget(keyq, 0600 | IPC_CREAT | IPC_EXCL);
```
- msgrcv(msqid, msgp, msgsz, msgtyp, msgflg) - Odbiera komunikat z kolejki komunikatów.
```c
if ((msgrcv(f, msg, sizeof(p1) - sizeof(long), 1, 0)) == -1)
{
  if (czy_skasowac)
    unlink(path);
  free(path);
  return -1;
}
```
- msgsnd(msqid, msgp, msgsz, msgflg) - Wysyła komunikat do kolejki komunikatów.
```c
int f1;
p2 *praca = (p2 *)malloc(sizeof(p2));
msgsnd(f1, praca, sizeof(p2) - sizeof(long), 0);
```
- malloc(size) - Alokuję pamięć o podanym rozmiarze i zwraca wskaźnik na zaalokowany obszar.
```c
p2 *praca = (p2 *)malloc(sizeof(p2));
```
- free(ptr) - Zwalnia zaalokowaną pamięć, na którą wskazuje wskaźnik.
```c
free(praca);
```

# IPC - Klient Kolejki Komunikatów

## Plan

1. Importuj wymagane biblioteki.
2. Zdefiniuj struktury `msg1` i `msg2` do przechowywania komunikatów.
3. Zadeklaruj zmienne globalne.
4. W funkcji `main` zadeklaruj zmienne lokalne.
5. Wygeneruj klucz IPC na podstawie ścieżki.
6. Pobierz identyfikator bieżącego procesu.
7. Sprawdź istnienie kolejki komunikatów na podstawie klucza.
8. Alokuj pamięć dla struktury `msg1`.
9. Ustaw wartości pól struktury `msg1`.
10. Wyślij komunikat typu 1 do kolejki komunikatów.
11. Wygeneruj klucz IPC na podstawie ścieżki i identyfikatora procesu.
12. Utwórz kolejkę komunikatów dla danego identyfikatora procesu.
13. Odczytaj liczbę od użytkownika.
14. Alokuj pamięć dla struktury `msg2`.
15. Ustaw wartości pól struktury `msg2`.
16. Wyślij komunikat typu 2 z liczbą do kolejki komunikatów.
17. Odbierz komunikat zwrotny z przetworzoną wartością od serwera.
18. Wyświetl otrzymaną przetworzoną wartość na standardowym wyjściu.
19. Zwolnij zaalokowaną pamięć.
20. Zakończ działanie programu.

## Biblioteki

| Biblioteka  | Opis                                                                                                                                                                                                                                                                 |
|-------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `unistd.h`    | Ta biblioteka zawiera deklaracje standardowych funkcji i stałych, które dostarczają interfejs do systemowych wywołań. W kodzie używana jest do obsługi procesów i dostępu do ścieżek.                                                         |
| `stdio.h`     | Ta biblioteka zawiera funkcje wejścia/wyjścia do operacji na strumieniach. W kodzie używana jest do wyświetlania danych na standardowym wyjściu i odczytu danych od użytkownika.                                                                                                                       |
| `sys/uio.h`   | Ta biblioteka zawiera deklaracje funkcji dotyczących operacji wejścia/wyjścia systemowego. W kodzie używana jest do operacji na strumieniach i buforach danych.                                                                                                    |
| `sys/ipc.h`   | Ta biblioteka zawiera deklaracje funkcji i struktur dotyczących mechanizmów IPC (Inter-Process Communication). W kodzie używana jest do generowania klucza IPC na podstawie ścieżki oraz tworzenia/kontroli kolejki komunikatów.                                       |
| `sys/msg.h`   | Ta biblioteka zawiera deklaracje funkcji i struktur dotyczących kolejek komunikatów. W kodzie używana jest do tworzenia i obsługi kolejki komunikatów.                                                                                                              |
| `stdlib.h`    | Ta biblioteka zawiera funkcje ogólnego zastosowania, takie jak alokacja pamięci, konwersje liczb, kontrola procesu itp. W kodzie używana jest do alokacji i zwalniania pamięci.                                                                                           |
| `string.h`    | Ta biblioteka zawiera deklaracje funkcji dotyczących manipulacji łańcuchami znaków. W kodzie używana jest do zwalniania pamięci ścieżki.                                                                                                                         |

## Struktury

- msg1 - Struktura reprezentująca komunikat typu 1, zawierająca pole type oznaczające typ komunikatu oraz pole pid przechowujące identyfikator procesu.
```c
typedef struct msg1 
{
    long type;
    pid_t pid;
} p1;
```
- msg2 - Struktura reprezentująca komunikat typu 2, zawierająca pole type oznaczające typ komunikatu oraz pole el przechowujące wartość liczbową.
```c
typedef struct msg2 
{
    long type;
    double el;
} p2;
```
## Metody

- ftok(path, id) - Generuje klucz IPC na podstawie ścieżki do pliku i identyfikatora.
```c
key_t keyq;
if ((keyq = ftok(path, 0)) != -1) 
```
- getpid() - Pobiera identyfikator bieżącego procesu.
```c
pid_t pid = getpid();
```
- msgget(key, msgflg) - Tworzy nową kolejkę komunikatów lub pobiera istniejącą kolejkę na podstawie klucza IPC i flag.
```c
int f;
if ((f = msgget(keyq, 0600)) != -1) 
```
- malloc(size) - Alokuję pamięć o podanym rozmiarze i zwraca wskaźnik na zaalokowany obszar.
```c
p1 *msg = (p1*)malloc(sizeof(p1));
```
- msgsnd(msqid, msgp, msgsz, msgflg) - Wysyła komunikat do kolejki komunikatów.
```c
int f;
p1 *msg;
if (msgsnd(f, msg, sizeof(p1) - sizeof(long), 0) != -1)
```
- msgrcv(msqid, msgp, msgsz, msgtyp, msgflg) - Odbiera komunikat z kolejki komunikatów.
```c
int f1;
p2 *praca;
msgrcv(f1, praca, sizeof(p2) - sizeof(long), 3, 0);
```
- scanf(format, ...) - Funkcja odczytująca dane z wejścia na podstawie podanego formatu.
```c
scanf("%lf", &a);
```
- printf(format, ...) - Funkcja wyświetlająca sformatowane dane na standardowym wyjściu (ekranie).
```c
printf("%lf\n", a, praca->el);
```
- free(ptr) - Zwalnia zaalokowaną pamięć, na którą wskazuje wskaźnik.
```c
free(praca);
```
- msgctl(msqid, cmd, buf) - Wykonuje różne operacje na kolejkach komunikatów, takie jak usuwanie kolejki komunikatów.
```c
int f1;
msgctl(f1, IPC_RMID, NULL);
```
