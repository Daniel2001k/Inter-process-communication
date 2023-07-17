# Semafory IPC:

- Semafory IPC są mechanizmem synchronizacji międzyprocesowej, który służy do kontrolowania dostępu do współdzielonych zasobów.
- Semafory IPC składają się z liczb całkowitych nazywanych semaforami, które przechowują wartości dodatnie lub zero.
- Procesy korzystają z funkcji do operacji na semaforach, takich jak semget, semop i semctl.
- Procesy mogą wykonywać operacje na semaforach, takie jak zwiększanie, zmniejszanie lub ustawianie wartości semaforów.
- Semafory są wykorzystywane do rozwiązania problemów synchronizacji, takich jak wzajemne wykluczanie (chronienie krytycznych sekcji), blokowanie/zwalnianie dostępu do zasobów, kontrola kolejności wykonywania zadań itp.
- Semafor IPC zapewnia synchronizację międzyprocesową i unikanie zakleszczeń podczas korzystania z współdzielonych zasobów.

## IPC Serwer - Semafory

### Plan

1. Importuj wymagane biblioteki.
2. Zadeklaruj zmienne globalne.
3. Zdefiniuj funkcję obsługi sygnałów (`al`), która zamknie semafory, zwolni pamięć współdzieloną i zakończy działanie programu.
4. W funkcji `main` zadeklaruj zmienne lokalne.
5. Ustaw obsługę sygnałów `SIGTERM` i `SIGINT` na funkcję `al`.
6. Sprawdź, czy podano ścieżkę jako argument wiersza poleceń. Jeśli nie, użyj domyślnej ścieżki.
7. Sprawdź istnienie pliku na podstawie ścieżki. Jeśli plik istnieje, ustaw flagę `czy_skasowac` na 0.
8. Jeśli flaga `czy_skasowac` jest ustawiona, utwórz plik za pomocą funkcji `open`.
9. Wygeneruj klucze IPC na podstawie ścieżki dla semaforów i pamięci współdzielonej.
10. Utwórz segment pamięci współdzielonej za pomocą funkcji `shmget`.
11. Dołącz segment pamięci współdzielonej do przestrzeni adresowej procesu za pomocą funkcji `shmat`.
12. Utwórz semafory za pomocą funkcji `semget`.
13. Ustaw wartości początkowe semaforów za pomocą funkcji `semctl`.
14. W pętli nieskończonej oczekuj na semaforze nr 1 (opracowano algorytm Dijkstry - zamknięcie sekcji krytycznej).
15. Odczytaj wartość ze współdzielonej pamięci i skopiuj ją do zmiennej `msg`.
16. Zwiększ semafor nr 0 (opracowano algorytm Dijkstry - otwarcie sekcji krytycznej).
17. Wyświetl wartość `msg` na standardowym wyjściu.
18. Oblicz kwadrat wartości i zapisz go w `msg` i pamięci współdzielonej.
19. Zwolnij pamięć zaalokowaną dla `msg`.
20. Powróć do kroku 14.

### Biblioteki

| Biblioteka    | Opis                                                                                                                                                             |
|---------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `stdio.h`     | Ta biblioteka zawiera funkcje wejścia/wyjścia do operacji na strumieniach. W kodzie używana jest do wyświetlania danych na standardowym wyjściu.                    |
| `string.h`    | Ta biblioteka zawiera deklaracje funkcji dotyczących manipulacji łańcuchami znaków. W kodzie używana jest do operacji na łańcuchach znaków.                        |
| `sys/sem.h`   | Ta biblioteka zawiera deklaracje funkcji i struktur dotyczących semaforów. W kodzie używana jest do tworzenia, kontrolowania i operacji na semaforach.              |
| `sys/shm.h`   | Ta biblioteka zawiera deklaracje funkcji i struktur dotyczących pamięci współdzielonej. W kodzie używana jest do tworzenia i zarządzania segmentami pamięci współdzielonej. |
| `stdlib.h`    | Ta biblioteka zawiera funkcje ogólnego zastosowania. W kodzie używana jest do zarządzania pamięcią i innych operacji ogólnego przeznaczenia.                          |
| `signal.h`    | Ta biblioteka zawiera deklaracje funkcji dotyczących obsługi sygnałów. W kodzie używana jest do instalowania funkcji obsługi sygnałów.                                 |
| `unistd.h`    | Ta biblioteka zawiera deklaracje standardowych funkcji i stałych, które dostarczają interfejs do systemowych wywołań.                                                |
| `fcntl.h`     | Ta biblioteka zawiera deklaracje funkcji dotyczących zarządzania deskryptorami plików. W kodzie używana jest do otwierania plików.                                  |
| `errno.h`     | Ta biblioteka zawiera deklaracje zmiennych globalnych i funkcji związanych z obsługą błędów. W kodzie używana jest do obsługi błędów.                                 |
| `sys/types.h` | Ta biblioteka zawiera deklaracje typów danych używanych w systemowych wywołaniach i interfejsach.                                                                  |

### Metody
- signal(signal_number, signal_handler) - Funkcja signal() instaluje obsługę sygnału (signal_handler) dla określonego numeru sygnału (signal_number). W przypadku otrzymania sygnału, system operacyjny wywoła funkcję obsługi sygnału.
```c
signal(SIGTERM, al);
signal(SIGINT, al);`
```

- strndup(string, length) - Funkcja strndup() tworzy kopię podanego łańcucha znaków (string) o podanej długości (length). Zwraca wskaźnik do nowo utworzonego łańcucha.
```c
sciezka = strndup(argv[1], strlen(argv[1]));
sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
```

- access(path, mode) - Funkcja access() sprawdza dostępność pliku o podanej ścieżce (path) i trybie dostępu (mode). Zwraca 0, jeśli plik jest dostępny, lub -1 w przypadku błędu.
```c
if (access(sciezka, F_OK) == 0)
  czy_skasowac = 0;
```

- open(path, flags, mode) - Funkcja open() otwiera plik o podanej ścieżce (path) z określonymi flagami (flags) i trybem dostępu (mode). Zwraca deskryptor pliku lub -1 w przypadku błędu.
```c
if (czy_skasowac && ((plik = open(sciezka, O_CREAT | O_EXCL, 0600)) == -1))
{
  fprintf(stderr, "Serwer: Blad utworzenia pliku %s: %s.\n", sciezka, strerror(errno));
  free(sciezka);
  return -1;
}
```

- unlink(path) - Funkcja unlink() usuwa plik o podanej ścieżce (path) z systemu plików.
```c
if (czy_skasowac)
{
  unlink(sciezka);
}
```

- free(ptr) - Funkcja free() zwalnia zaalokowaną pamięć, na którą wskazuje wskaźnik (ptr).
```c
free(sciezka);
```

- shmget(key, size, shmflg) - Funkcja shmget() tworzy nowy segment pamięci współdzielonej lub pobiera identyfikator istniejącego segmentu na podstawie klucza IPC (key). Zwraca identyfikator segmentu lub -1 w przypadku błędu.
```c
mem = shmget(klucz_memS, 50 * sizeof(char), 0600 | IPC_CREAT | IPC_EXCL);
```

- shmat(shmid, shmaddr, shmflg) - Funkcja shmat() dołącza segment pamięci współdzielonej o określonym identyfikatorze (shmid) do przestrzeni adresowej procesu. Zwraca wskaźnik do segmentu lub -1 w przypadku błędu.
```c
memS = shmat(mem, (char *)0, 0);
```

- semget(key, nsems, semflg) - Funkcja semget() tworzy nowy zbiór semaforów lub pobiera identyfikator istniejącego zbioru na podstawie klucza IPC (key). Zwraca identyfikator zbioru semaforów lub -1 w przypadku błędu.
```c
sem = semget(klucz_semafory, 2, 0600 | IPC_CREAT | IPC_EXCL);
```

- semctl(semid, semnum, cmd, arg) - Funkcja semctl() wykonuje różne operacje na zbiorze semaforów, takie jak ustawianie wartości początkowych, pobieranie wartości, zmienianie wartości itp.
```c
semctl(sem, 0, IPC_RMID, NULL);
```

- semop(semid, sops, nsops) - Funkcja semop() wykonuje operacje na semaforach w danym zbiorze semaforów.
```c
if (semop(sem, &setSem, 1) == -1)
{
  shmdt(memS);
  shmctl(mem, IPC_RMID, NULL);
  semctl(sem, 0, IPC_RMID, NULL);
 
  if (czy_skasowac)
  {
    unlink(sciezka);
  }

  free(sciezka);
  return -1;
}
```

- atoi(str) - Funkcja atoi() konwertuje podany łańcuch znaków (str) na wartość całkowitą.
```c
double wynik = atoi(memS) * atoi(memS);
```

- sprintf(buffer, format, ...) - Funkcja sprintf() działa podobnie jak printf(), ale zapisuje sformatowane dane do bufora (buffer) zamiast na standardowe wyjście.
```c
sprintf(msg, "%lf", wynik);
```

- strcpy(destination, source) - Funkcja strcpy() kopiuje zawartość łańcucha znaków z source do destination.
```c
strcpy(memS, msg);
```


# IPC Klient - Semafory

## Plan

1. Importuj wymagane biblioteki.
2. Zadeklaruj zmienne lokalne.
3. Wygeneruj klucze IPC na podstawie ścieżki dla semaforów i pamięci współdzielonej.
4. Zwolnij pamięć zaalokowaną dla ścieżki.
5. Uzyskaj dostęp do istniejących semaforów za pomocą funkcji `semget`.
6. Utwórz strukturę `bufor_semaforow` i ustaw jej wartości dla operacji semaforowych.
7. Wykonaj operację semaforową na semaforze nr 0 (opracowano algorytm Dijkstry - zamknięcie sekcji krytycznej).
8. Uzyskaj dostęp do istniejącej pamięci współdzielonej za pomocą funkcji `shmget`.
9. Zaalokuj pamięć dla wiadomości `msg`.
10. Dołącz segment pamięci współdzielonej do przestrzeni adresowej procesu za pomocą funkcji `shmat`.
11. Wczytaj liczbę z konsoli i zapisz ją w formacie łańcucha znaków za pomocą funkcji `sprintf`.
12. Skopiuj zawartość łańcucha znaków `msg` do pamięci współdzielonej.
13. Wyświetl wartość `msg` na standardowym wyjściu.
14. Zwolnij pamięć zaalokowaną dla `msg`.
15. Odłącz segment pamięci współdzielonej od przestrzeni adresowej procesu za pomocą funkcji `shmdt`.
16. Wykonaj operację semaforową na semaforze nr 1 (opracowano algorytm Dijkstry - otwarcie sekcji krytycznej).

## Biblioteki

| Biblioteka    | Opis                                                                                                                                                             |
|---------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `sys/sem.h`   | Ta biblioteka zawiera deklaracje funkcji i struktur dotyczących semaforów. W kodzie używana jest do operacji na semaforach.                                      |
| `sys/shm.h`   | Ta biblioteka zawiera deklaracje funkcji i struktur dotyczących pamięci współdzielonej. W kodzie używana jest do operacji na pamięci współdzielonej.                   |
| `string.h`    | Ta biblioteka zawiera deklaracje funkcji dotyczących manipulacji łańcuchami znaków. W kodzie używana jest do operacji na łańcuchach znaków.                        |
| `stdio.h`     | Ta biblioteka zawiera funkcje wejścia/wyjścia do operacji na strumieniach. W kodzie używana jest do wczytywania danych z konsoli i wyświetlania ich na standardowym wyjściu. |
| `stdlib.h`    | Ta biblioteka zawiera funkcje ogólnego zastosowania. W kodzie używana jest do zarządzania pamięcią i innych operacji ogólnego przeznaczenia.                          |
| `unistd.h`    | Ta biblioteka zawiera deklaracje standardowych funkcji i stałych, które dostarczają interfejs do systemowych wywołań.                                                |

### Metody

- ftok(path, id) - Funkcja ftok() generuje klucz IPC na podstawie ścieżki (path) i identyfikatora (id). Zwraca wygenerowany klucz.
```c
klucz_semafory = ftok(sciezka, 0);
klucz_memS = ftok(sciezka, 1);
```

- free(ptr) - Funkcja free() zwalnia zaalokowaną pamięć, na którą wskazuje wskaźnik (ptr).
```c
free(msg);
```

- semget(key, nsems, semflg) - Funkcja semget() pobiera identyfikator zbioru semaforów na podstawie klucza IPC (key). Zwraca identyfikator zbioru semaforów lub -1 w przypadku błędu.
```c
id_semafor = semget(klucz_semafory, 2, 0600);
```

- semop(semid, sops, nsops) - Funkcja semop() wykonuje operacje na semaforach w danym zbiorze semaforów.
```c
semop(id_semafor, &setSem, 1);
```

- shmget(key, size, shmflg) - Funkcja shmget() pobiera identyfikator segmentu pamięci współdzielonej na podstawie klucza IPC (key). Zwraca identyfikator segmentu lub -1 w przypadku błędu.
```c
id_pamieci = shmget(klucz_memS, 50 * sizeof(char), 0600);
```

- malloc(size) - Funkcja malloc() alokuje blok pamięci o podanym rozmiarze (size) i zwraca wskaźnik do tego bloku.
```c
char *msg = (char *)malloc(100 * sizeof(char));
```

- shmat(shmid, shmaddr, shmflg) - Funkcja shmat() dołącza segment pamięci współdzielonej o określonym identyfikatorze (shmid) do przestrzeni adresowej procesu. Zwraca wskaźnik do segmentu lub -1 w przypadku błędu.
```c
char *memS = shmat(id_pamieci, (char *)0, 0);
```

- sprintf(buffer, format, ...) - Funkcja sprintf() działa podobnie jak printf(), ale zapisuje sformatowane dane do bufora (buffer) zamiast na standardowe wyjście.
```c
sprintf(msg, "%lf", liczba);
```
- strcpy(destination, source) - Funkcja strcpy() kopiuje zawartość łańcucha znaków z source do destination.
```c
strcpy(memS, msg);
```
