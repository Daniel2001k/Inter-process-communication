# Laboratorium 1. - 15. października 2022
### Przydatne konstrukcje języka C, diagnozowanie błędów i zasoby systemowe.
1. Przydatne konstrukcje języka C.
- Parametry wywołania programu napisanego w języku C, parametry funkcji main.
- Dynamiczne alokowanie tablic wielowymiarowych.
- Struktury w języku C.
- Zmienne statyczne wewnątrz funkcji.
- Typ funkcyjny.
2. Parsowanie parametrów wywołania programu.
- Funkcja getopt parsuje opcje wywołania programu. Przez opcję rozumiemy parametry postaci -z wartość, gdzie z może być dowolnym znakiem a wartość dowolnym ciągiem znaków, np. -x plik348.txt.
- Funkcja getsubopt parsuje wartości opcji wywołania programu. Wartości są listą elementów postaci klucz=wartość. Zwykle tej funkcji używa się do parsowania wartości opcji zwróconej przez funkcję getopt.
3. Diagnozowanie błędów zwracanych przez funkcje.
- Zmienna globalna errno.
- Funkcja strerror zwraca tekstowy opis błędu wskazywanego przez zmienną errno. Funkcja strerror_r jest wersją funkcji strerror bezpieczną ze względu na wielowątkowość.
- Funkcja perror wypisuje na ekran tekstowy opis błędu wskazywanego przez zmienną errno wraz z dodatkowym tekstem będącym argumentem funkcji.
4. Odczytywanie wartości zmiennych środowiskowych.
#### Przydatne konstrukcje języka C
```c
/*
Przykłady ciekawych konstrukcji jezyka C.
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct dane {
    double a, b;
    long c;
};

typedef struct dane Dane;

double dzialanie(Dane x) {
    static long z = 0;
    z++;
    return x.a + x.c * x.b + z;
}

double wylicz(Dane x, double (*dzial)(Dane)) {
    return 5 * dzial(x);
}

int main(int argc, char **argv) {
    int m = atoi(argv[1]), n, i, j;
    double **tablica;
    Dane y;

    n = strtol(argv[2], NULL, 10);
    printf("Ilosc parametrow: %d\n", argc);
    printf("Paramatry wywolania: %s, %d, %d.\n", argv[0], m, n);
    tablica = (double **) malloc(sizeof(double *) * m);
    tablica[0] = (double *) malloc(sizeof(double) * m * n);
    for (i = 1; i < m; i++)
        tablica[i] = &(tablica[0][i * n]);
    srand(time(NULL));
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            tablica[i][j] = (rand() % 1000) / 77;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            printf("zawartosc w miejscu (%d, %d): %lf.\n", i, j, tablica[i][j]);
    sscanf(argv[3], "%lf", &y.a);
    y.b = strtod(argv[4], NULL);
    y.c = strtol(argv[5], NULL, 10);
    printf("Wynik: %lf.\n", wylicz(y, dzialanie));
    printf("Wynik: %lf.\n", wylicz(y, dzialanie));
    printf("Wynik: %lf.\n", wylicz(y, dzialanie));
    return 0;
}
```
#### Parsowanie parametrów wywołania programu
```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

/*
Przykład programu obrazujący wykorzystanie funkcji getopt i getsubopt do parsowania parametrów wywołania programu.
*/

int main(int argc, char **argv) {
    int znak, i;
    char *opcje, *wartosc;
    char *tokeny[] = {"rw", "rsize", NULL};

    while ((znak = getopt(argc, argv, "bf:x::")) != -1) {
        switch (znak) {
            case 'b':
                printf("Parametr b zostal ustawiony.\n");
                break;
            case 'f':
                printf("Parametr f: %s\n", optarg);
                opcje = strdup(optarg);
                while (*opcje) {
                    switch (getsubopt(&opcje, tokeny, &wartosc)) {
                        case 0:
                            printf("Opcja 0.\n");
                            break;
                        case 1:
                            printf("Opcja 1, warotsc: %s.\n", wartosc);
                            break;
                    }
                }
                break;
            case 'x':
                printf("Parametr x: %s\n", optarg);
                break;
            case '?':
            default:
                printf("Nieznany parametr.\n");
        }
    }
    return 0;
}
```
#### Diagnozowanie błędów zwracanych przez funkcje
```c
/*
Przykład programu diagnozującego błędy zwrócane przez funkcje.
*/

#include<stdio.h>
#include<errno.h>
#include<string.h>

int main(int argc, char **argv) {
    int liczba_plikow = argc - 1;
    FILE *plik;
    char *nazwa_pliku;
    int i;

    if (!liczba_plikow) {
        fprintf(stderr, "Uzycie: %s <lista plikow>.\n", argv[0]);
        return 1;
    }
    for (i = 1; i <= liczba_plikow; ++i) {
        nazwa_pliku = argv[i];
        plik = fopen(nazwa_pliku, "r");
        if (plik == NULL) {
            fprintf(stderr, "Blad przy probie otwarcia pliku '%s': %s.\n", nazwa_pliku, strerror(errno));
        } else {
            fprintf(stderr, "'%s' otwarty poprawnie.\n", nazwa_pliku);
            fclose(plik);
        }
    }
    return 0;
}
```
#### Odczytywanie wartości zmiennych środowiskowych
```c
/*
Program pracuje ze zmiennymi środowiskowymi aktualnie zdefiniowanymi.
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char **argv, char **env) {
    if (argc == 1) {
        int i = 0;
        char *pomoc;

        while (env[i]) {
            pomoc = strchr(env[i], '=');
            pomoc[0] = '\0';
            printf("zmienna: %s , wartosc: %s\n", env[i], pomoc + 1);
            i++;
        }
    } else {
        char *pomoc = getenv(argv[1]);

        if (pomoc != NULL) printf("%s\n.", pomoc);
        else printf("Brak zmiennej srodowiskowej o nazwie %s.\n", argv[1]);
    }
    return 0;
}
```
#### Zadania
- Zadanie 1
- Zadanie 2
- Zadanie 3
- Zadanie 4
# Laboratorium 2. - 23. paździrnika 2022
### Sygnały, procesy potomne, potoki.
1. Tworzenie procesów potomnych, ich obsługa i pobieranie podstawowych informacji o nich.
2. Uruchomienie w procesie potomnym nowego zadania.
3. Definiowanie funkcji obsługi sygnału i jej rejestracja.
#### Definiowanie funkcji obsługi sygnału i jej rejestracja
```c
#include<unistd.h>
#include<stdio.h>
#include <signal.h>
#include<stdlib.h>
/* Rejestrowanie niestandardowej funcji obsługi sygnału. */
void al(int i) {
 if (i == SIGALRM) {
 printf("Alarm !!!\n");
 signal(SIGALRM, al);
 }
}
int main(int argc, char *argv[]) {
 long oczekiwanie;
 if (argc == 2) {
 oczekiwanie = atol(argv[1]);
 signal(SIGALRM, al);
 alarm(oczekiwanie);
 pause();
 printf("Koniec!\n");
 return 0;
 } else return -1;
}
```
#### Tworzenie procesów potomnych, ich obsługa i pobieranie podstawowych informacji o nich - przykład 1
```c
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
/* Program tworzy proces potomny, każdy proces wypisuje krótką informację o sobie i swojej
rodzinie. */
int main() {
 int pid, ppid;
 printf("Przed wywolaniem funkcji fork.");
 ppid = getpid();
 fflush(stdout);
 if ((pid = fork()) == -1) {
 perror("Wywolanie funkcji fork nie jest mozliwe.");
 exit(1);
 } else {
 if (pid == 0) {
 printf("\nDziecko %d rodzica %d wypisyje jakis tekst.\n", getpid(), ppid);
 } else {
 printf("\nRodzic %d wypisuje jakis tekst, dziecko %d.\n", ppid, pid);
 }
 }
}
```
#### Tworzenie procesów potomnych, ich obsługa i pobieranie podstawowych informacji o nich - przykład 2
```c
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<stdlib.h>
/* Program tworzy proces potomny, potem tworzy proces potomny procesu potomnego. Proces
macierzysty zmiania standardową funkcję obsługi sygnału wywoływanego przy zakończeniu
procesu potomnego. Proszę zwrócić uwagę na fakt, że niestandardowa funkcja obsługi sygnału
jest ważna tylko do pierwszego jej użycia, potem jest używana ponownie standardowa funkcja
obsługi sygnału. Aby ciągle była używana niestandardowa funcja obsługi sygnału, należy w
niestandardowej funkcji obsługi sygnału rejestrować ją jako funkcję obsługi sygnału dla tego
konkretnego sygnału.
*/
void dziecko(int i) {
 printf("Dostalem sygnal, numer procesu %d.\n", getpid());
 wait(NULL);
 signal(SIGCHLD, dziecko);
}
int main(int argc, char *argv[]) {
 int pid = 0, ppid;
 ppid = getpid();
 printf("Przed wywolaniem funkcji fork. Rodzic ma numer %d.\n", ppid);
 signal(SIGCLD, dziecko);
 if ((pid = fork()) == -1) {
 perror("Pomyslne zakonczenie funkcji fork nie jest mozliwe.\n");
 exit(1);
 } else {
 if (pid != 0) {
 sleep(10);
 } else {
 printf("Dziecko ma numer %d.\n", getpid());
 sleep(2);
 pid = fork();
 if (pid != 0) {
 printf("Dziecko dziecka ma numer %d.\n", pid);
 sleep(2);
 }
 }
 }
}
```
#### Uruchomienie w procesie potomnym nowego zadania
```c
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
/* Program tworzy proces potomny i uruchamia zadane polecenie w ramach procesu potomnego.
Przykladowe wywolanie programu: a.out cal 10 2024. */
int main(int argc, char **argv) {
 char **polecenie;
 long i = 0;
 int pid;
 polecenie = (char **) malloc(sizeof(char *) * argc);
 while (i < argc - 1) {
 polecenie[i] = argv[i + 1];
 i++;
 }
 polecenie[i] = NULL;
 if ((pid = fork()) == -1) {
 perror("Pomyslne zakonczenie funkcji fork nie jest mozliwe.\n");
 exit(1);
 } else {
 if (pid == 0) {
 execvp(polecenie[0], polecenie);
 } else {
 waitpid((pid_t) - 1, NULL, 0);
 printf("\nPolecenie wraz z argumentami zostalo wykonane.\n");
 }
 }
 free(polecenie);
 return 0;
}
```
#### Zadania
- Zadanie 5
# Laboratorium 3. - 6. listopada 2022
### Praca z plikami oraz operacje wejścia i wyjścia.
1. Tworzenie i wykorzystanie potoków do komunikacji międzyprocesowej.
2. Pobieranie wartości limitów i ich ustawianie w systemie operacyjnym.
3. Pobieranie informacji systemowych o zasobach.
4. Pobieranie podstawowych informacji o pliku/katalogu.
5. Manipulowanie plikami i katalogami.
6. Operacje wejścia i wyjścia.
7. Kompilacja programu używającego asynchronicznych operacji wejścia i wyjścia: gcc program.c -o program -lrt.
```c
#include<stdio.h>
/* Program tworzy nowy proces i potok do niego oraz uruchamia program factor i przekauje
parametry wywołania programu jako dane dla niego. Prykladowe wywolanie: a.out 13 15 18. */
int main(int argc, char *argv[]) {
 FILE *f;
 int i;
 char polecenie[1024];
 polecenie[0] = '\0';
 if (f = popen("factor", "w")) {
 for (i = 1; i < argc; i++)
 sprintf(polecenie, "%s %s", polecenie, argv[i]);
 fprintf(f, "%s", polecenie);
 pclose(f);
 return 0;
 } else {
 perror("Uruchomienie programu nie jest mozliwe");
 return -1;
 }
}
```
```c
/*
Wspolpraca z kalkulatorem bc z wykorzystaniem potoków. Przykładowe wywolanie programu a.out 4+7 lub a.out "5 +
7". Aby wyliczyc wartosc funkcji sinus mozna uzyc polecenia a.out "s(7)".
*/
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main(int argc, char *argv[]) {
 int pid, fd1[2], fd2[2];
 if (argc != 2) {
 perror("Bledna liczba argumentow.\n");
 return -1;
 } else {
 if (pipe(fd1) == -1 || pipe(fd2) == -1) {
 perror("Otwarcie laczy nie jest mozliwe.\n");
 return -1;
 } else {
 if ((pid = fork()) == -1) {
 perror("Pomyslne zakonczenie funkcji fork nie jest mozliwe.\n");
 return -1;
 } else {
 if (pid == 0) {
 char **pomoc = malloc(3 * sizeof(char *));
 pomoc[0] = strdup("bc");
 pomoc[1] = strdup("-l");
 pomoc[2] = NULL;
 close(fd2[0]);
 close(fd1[1]);
 dup2(fd1[0], 0);
 dup2(fd2[1], 1);
 execvp(pomoc[0], pomoc);
 } else {
 FILE *f1, *f2;
 char *bufor;
 int l = 0;
 close(fd1[0]);
 close(fd2[1]);
 f1 = fdopen(fd1[1], "w");
 f2 = fdopen(fd2[0], "r");
 fprintf(f1, "%s\nquit\n", argv[1]);
 fflush(f1);
 bufor = (char *) malloc(1000 * sizeof(char));
 fgets(bufor, 1000, f2);
 printf("Wynik %s", bufor);
 free(bufor);
 }
 }
 return 0;
 }
 }
}
```
#### Pobieranie wartości limitów i ich ustawianie w systemie operacyjnym
```c
/*
Pobranie i ustawienie wielkosci pliku typu core. Polecenia getrlimit i setrlimit sluza do pobrania i ustawiania roznych limitow w systemie operacyjnym.
*/

#include<stdio.h>
#include<sys/resource.h>

int main() {
    struct rlimit pomoc;

    getrlimit(RLIMIT_CORE, &pomoc);
    printf("soft: %llu, hard: %llu.\n", pomoc.rlim_cur, pomoc.rlim_max);
    pomoc.rlim_cur = 2000;
    setrlimit(RLIMIT_CORE, &pomoc);
    getrlimit(RLIMIT_CORE, &pomoc);
  printf("soft: %llu, hard: %llu.\n", pomoc.rlim_cur, pomoc.rlim_max);  return 0; }
```
```c
/*
Pobranie i ustawienie wielkosci segmentu danych dla procesu. Polecenia getrlimit i setrlimit sluza do pobrania i ustawiania roznych limitow w systemie operacyjnym. Nie na wszystkich systemach operacyjnych program powoduje blad z powodu przekrocenia limitu segmentu danych dla procesu, efekt działania programu zalezy od konfiguracji systemu operacyjnego. Program ma problemy z poprawna praca na wszelkich emulatorach systemow typu uniksowego.
*/

#include<stdio.h>
#include<sys/resource.h>
#include<stdlib.h>

int main(int argc) {
    struct rlimit pomoc;
    char *p1, *p2;

    getrlimit(RLIMIT_AS, &pomoc);
    printf("soft: %llu, hard: %llu\n", pomoc.rlim_cur, pomoc.rlim_max);
    pomoc.rlim_cur = 6000;
    if (argc == 1) {
        setrlimit(RLIMIT_AS, &pomoc);
    }
    getrlimit(RLIMIT_AS, &pomoc);
    printf("soft: %llu, hard: %llu\n", pomoc.rlim_cur, pomoc.rlim_max);
    printf("Przydzielam pamiec dla zmiennej p1.\n");
    if (!(p1 = (char *) malloc(5000))) {
        perror("Brak pamieci dla zmiennej p1.\n");
        return -1;
    }
    printf("Przydzielam pamiec dla zmiennej p2.\n");
    if (!(p2 = (char *) malloc(7000))) {
        perror("Brak pamieci dla zmiennej p2.\n");
        return -1;
    }
    printf("Poprawnie zakonczylem przydzial pamieci.\n");
    return 0;
}
```
#### Pobieranie informacji systemowych o zasobach
```c
/*
Program wypisuje informacje o efektywnym biezacym uzytkowniku. Program ma problemy z poprawna praca na wszelkich emulatorach systemow typu uniksowego.
*/

#include<unistd.h>
#include<pwd.h>
#include<stdio.h>

int main() {
    printf("Uzytkownik: %s.\n", getpwuid(geteuid())->pw_gecos);
}
```
#### Pobieranie podstawowych informacji o pliku/katalogu
```c
/*
Program wypisuje podstawowe informcje na temat pliku w systmie.
*/

#include<sys/stat.h>
#include<fcntl.h>
#include<time.h>
#include<pwd.h>
#include<stdio.h>

int main(int argc, char *argv[]) {
    int plik;
    struct stat opis;
    struct tm *czas;

    if (argc != 2) {
        perror("Bledna ilosc argumentow.\n");
        return -1;
    }
    plik = open(argv[1], O_RDONLY);
    if (plik == -1) {
        perror("Blad otwarcia pliku.\n");
        return -1;
    }
    if (fstat(plik, &opis) == -1) {
        perror("Blad funkcji fstat.\n");
        return -1;
    }
    printf("Wlasciciel: %s\n", getpwuid(opis.st_uid)->pw_gecos);
    printf("Rozmiar: %lld\n", opis.st_size);
    czas = localtime(&(opis.st_mtime));
    printf("Czas: %d %d %d %d:%d\n", czas->tm_year, czas->tm_mon, czas->tm_mday, czas->tm_hour, czas->tm_min);
}
```
#### Manipulowanie plikami i katalogami
#### Operacje wejścia i wyjścia
```c
/*
Przyklad programu zapisujacego dane do pliku w trybie blokowanym.
*/

#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
    int liczbaIteracji = 1, i;
    char buf[1024];
    size_t size = 0;

    printf("Program testowy.\n");
    if (argc > 1) {

        /* konwersja pierwszego parametru wywolania na typ int */
        liczbaIteracji = strtol(argv[1], NULL, 10);
    }

    /* otwarcie pliku do pisania w trybie blokowanym */
    int plik = open("testowy.txt", O_CREAT | O_WRONLY, 0644);

    for (i = 0; i < liczbaIteracji; i++) {

        /* tworzenie wiadomości do zapisania w pliku */
        sprintf(buf, "Zawartosc testowa, iteracja numer %d.\n", i);

        /* zapisanie wiadomości do pliku */
        size += write(plik, buf, strlen(buf));
    }

    /* komunikat z podsumowaniem liczby zapisanych bajtow do pliku */
    printf("Zapisalem do pliku %lu bajtow.\n", size);

    /* zamkniecie pliku */
    close(plik);
    return 0;
}
```
```c
/* 
Przykład programu zapisującego dane do pliku w trybie nieblokowanym.
*/

#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
    int liczbaIteracji = 1, i;
    char buf[1024];
    size_t size = 0;

    printf("Program testowy.\n");
    if (argc > 1) {

        /* konwersja pierwszego parametru wywolania na tym int */
        liczbaIteracji = strtol(argv[1], NULL, 10);
    }

    /* otwarcie pliku do pisania w trybie nieblokowanym */
    int plik = open("testowy.txt", O_CREAT | O_WRONLY | O_NONBLOCK, 0644);

    for (i = 0; i < liczbaIteracji; i++) {

        /* tworzenie wiadomości do zapisania w pliku */
        sprintf(buf, "Zawartosc testowa, interacja numer %d.\n", i);

        /* zapisanie wiadomości do pliku */
        size += write(plik, buf, strlen(buf));
    }

    /* komunikat z podsumowaniem liczby zapisanych bajtow do pliku */
    printf("Zapisalem do pliku %lu bajtow.\n", size);

    /* zamkniecie pliku */
    close(plik);
    return 0;
}
```
```c
/*
Przyklad programu zapisujacego dane do pliku w sposob asynchroniczny, w trybie blokowanym. Program poprawnie dziala na maszynie sxterm, na maszynie ultra60 asynchroniczna obsluga operacji wejscia i wyjscia nie jest obslugiwana i otrzymujemy stosowny komunikat.
*/

#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<aio.h>
#include<errno.h>

int main(int argc, char *argv[]) {
    int liczbaIteracji = 1, i, result;
    char buf[1024];
    struct aiocb *message;
    size_t size = 0;

    /* zaalokowanie struktury zawierajacej parametry dla komunikacji asynchronicznej */
    message = malloc(sizeof(struct aiocb));
    printf("Program testowy.\n");
    if (argc > 1) {

        /* konwersja pierwszego parametru wywolania na tym int */
        liczbaIteracji = strtol(argv[1], NULL, 10);
    }

    /* otwarcie pliku do pisania w trybie blokowanym z flaga dopisywania do pliku i zerowania pliku przy otwarciu,
       tego typu zabiegi sa konieczne dla zapisu asynchronicznego gdyz brak flagi O_APPEND powoduje, ze kazde
       asynchroniczne wywolanie tworzy nowy pusty plik, flaga O_TRUNC zapewnia, ze kazde uruchomienie programu zeruje plik  */
    int plik = open("testowy.txt", O_CREAT | O_APPEND | O_TRUNC | O_WRONLY, 0644);

    for (i = 0; i < liczbaIteracji; i++) {

        /* tworzenie wiadomosci do zapisania w pliku */
        sprintf(buf, "Zawartosc testowa, interacja numer %d.\n", i);

        /* konfiguracja struktury zawierajacej parametry dla komunikacji asynchronicznej */
        message->aio_fildes = plik;
        message->aio_buf = buf;
        message->aio_nbytes = strlen(buf);

        /* zapisanie wiadomosci do pliku w sposob asynchroniczny */
        result = aio_write(message);
        if (result) {
            printf("Blad operacji zapisu aio_write: %s.\n", strerror(errno));
            return -1;
        }

        /* czekanie na zakonczenie asynchronicznego zapisu */
        result = aio_suspend((const struct aiocb *const *) &message, 1, NULL);
        if (result) {
            printf("Blad operacji zapisu aio_write: %s.\n", strerror(errno));
            return -1;
        }

        /* pobranie liczby bajtow zapisanych do pliku */
        size += aio_return(message);
    }

    /* komunikat z podsumowaniem liczby zapisanych bajtow do pliku */
    printf("Zapisalem do pliku %lu bajtow.\n", size);

    /* zamkniecie pliku */
    close(plik);
    return 0;
}
```
```c
/*
Program kopiuje zawartosc jednego pliku na drugi z zadana wielkoscia bufora.
*/

#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]) {
    int plik, plik2;
    void *bufor;
    size_t rozmiar, rozmiar1;

    if (argc != 4) {
        perror("Bledna ilosc argumentow\n");
        return -1;
    }
    plik = open(argv[1], O_RDONLY);
    if (plik == -1) {
        perror("Blad otwarcia pliku do czytania");
        return -1;
    }
    plik2 = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (plik2 == -1) {
        perror("Blad otwarcia pliku do pisania.\n");
        return -1;
    }
    rozmiar = strtol(argv[3], NULL, 10);
    if (rozmiar <= 0) {
        perror("Bledny rozmiar bufora.\n");
        return -1;
    }
    if (!(bufor = (void *) malloc(rozmiar))) {
        perror("Blad przydzialu pamieci.\n");
        return -1;
    }
    while ((rozmiar1 = read(plik, bufor, rozmiar))) write(plik2, bufor, rozmiar1);
    return 0;
}
```
#### Zadania
- Zadanie 6
- Zadanie 7
- Zadanie 8
- Zadanie 9
- Zadanie 10
- Zadanie 11
- Zadanie 12
- Zadanie 13
# Laboratorium 4. - 20. listopada 2022
### Programowanie z wykorzystaniem wątków POSIX, systemowe środki synchronizacji według standardu POSIX.
Kompilacja programu używającego wątków posiksowych wymaga użycia biblioteki pthread.\
Przykład. gcc przyklad.c -o przyklad -lpthread
```c
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
struct dane {
 double *tab;
 int dlugosc;
};
typedef struct dane Dane;
double *obsluga1(Dane *a) {
 int i;
 double *wynik = (double *) malloc(sizeof(double));
 *wynik = a->tab[0];
 for (i = 1; i < a->dlugosc; i++)
 if (*wynik < a->tab[i]) *wynik = a->tab[i];
 free(a);
 return wynik;
}
int main(int argc, char **argv) {
 int m = atoi(argv[1]), n, i, j;
 double **tablica, *pomoc, wynik;
 Dane *y;
 sscanf(argv[2], "%d", &n);
 tablica = (double **) malloc(sizeof(double *) * m);
 tablica[0] = (double *) malloc(sizeof(double) * m * n);
 for (i = 1; i < m; i++)
 tablica[i] = &(tablica[0][i * n]);
 srand(time(NULL));
 for (i = 0; i < m; i++) {
 for (j = 0; j < n; j++) {
 tablica[i][j] = (rand() % 1000) / 77;
 printf("[%d][%d]=%lf ", i, j, tablica[i][j]);
 }
 printf("\n");
 }
 y = (Dane *) malloc(sizeof(Dane));
 y->dlugosc = n;
 y->tab = tablica[0];
 pomoc = (double *) obsluga1((void *) y);
 printf("Wynik %lf.\n", *pomoc);
 free(tablica[0]);
 free(tablica);
 return 0;
}
```
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
struct dane {
 double *tab;
 int dlugosc;
};
typedef struct dane Dane;
void *obsluga(void *x) {
 Dane *a = (Dane *) x;
 int i;
 double *wynik = (double *) malloc(sizeof(double));
 *wynik = a->tab[0];
 for (i = 1; i < a->dlugosc; i++)
 if (*wynik < a->tab[i]) *wynik = a->tab[i];
 free(a);
 return (void *) wynik;
}
int main(int argc, char **argv) {
 int m = atoi(argv[1]), n, i, j;
 double **tablica, *pomoc, wynik;
 pthread_t *watki;
 Dane *y;
 sscanf(argv[2], "%d", &n);
 tablica = (double **) malloc(sizeof(double *) * m);
 watki = (pthread_t *) malloc(sizeof(pthread_t) * m);
 tablica[0] = (double *) malloc(sizeof(double) * m * n);
 for (i = 1; i < m; i++)
 tablica[i] = &(tablica[0][i * n]);
 srand(time(NULL));
 for (i = 0; i < m; i++) {
 for (j = 0; j < n; j++) {
 tablica[i][j] = (rand() % 1000) / 77;
 printf("[%d][%d]=%lf ", i, j, tablica[i][j]);
 }
 printf("\n");
 }
 for (i = 0; i < m; i++) {
 y = (Dane *) malloc(sizeof(Dane));
 y->dlugosc = n;
 y->tab = tablica[i];
 pthread_create(&(watki[i]), NULL, obsluga, (void *) y);
 }
 wynik = tablica[0][0];
 for (i = 0; i < m; i++) {
 pthread_join(watki[i], (void **) &pomoc);
 if (wynik < *pomoc) wynik = *pomoc;
 free(pomoc);
 }
 printf("Wynik %lf.\n", wynik);
 free(tablica[0]);
 free(watki);
 free(tablica);
 return 0;
}
```
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
struct dane {
 double *tab;
 int dlugosc;
};
typedef struct dane Dane;
double *obsluga1(Dane *a) {
 int i;
 double *wynik = (double *) malloc(sizeof(double));
 *wynik = a->tab[0];
 for (i = 1; i < a->dlugosc; i++)
 if (*wynik < a->tab[i]) *wynik = a->tab[i];
 free(a);
 return wynik;
}
int main(int argc, char **argv) {
 int m = atoi(argv[1]), n, i, j;
 double **tablica, *pomoc, wynik;
 pthread_t *watki;
 Dane *y;
 sscanf(argv[2], "%d", &n);
 tablica = (double **) malloc(sizeof(double *) * m);
 watki = (pthread_t *) malloc(sizeof(pthread_t) * m);
 tablica[0] = (double *) malloc(sizeof(double) * m * n);
 for (i = 1; i < m; i++)
 tablica[i] = &(tablica[0][i * n]);
 srand(time(NULL));
 for (i = 0; i < m; i++) {
 for (j = 0; j < n; j++) {
 tablica[i][j] = (rand() % 1000) / 77;
 printf("[%d][%d]=%lf ", i, j, tablica[i][j]);
 }
 printf("\n");
 }
 for (i = 0; i < m; i++) {
 y = (Dane *) malloc(sizeof(Dane));
 y->dlugosc = n;
 y->tab = tablica[i];
 pthread_create(&(watki[i]), NULL, (void *(*)(void *)) obsluga1, (void *) y);
 }
 wynik = tablica[0][0];
 for (i = 0; i < m; i++) {
 pthread_join(watki[i], (void **) &pomoc);
 if (wynik < *pomoc) wynik = *pomoc;
 free(pomoc);
 }
 printf("Wynik %lf.\n", wynik);
 free(tablica[0]);
 free(watki);
 free(tablica);
 return 0;
}
```
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
struct dane {
 double *tab, *maksimum;
 int dlugosc;
 pthread_mutex_t *rygiel;
};
typedef struct dane Dane;
void *obsluga(void *x) {
 Dane *a = (Dane *) x;
 int i;
 double *wynik = (double *) malloc(sizeof(double));
 *wynik = a->tab[0];
 for (i = 1; i < a->dlugosc; i++)
 if (*wynik < a->tab[i]) *wynik = a->tab[i];
 pthread_mutex_lock(a->rygiel);
 if (*(a->maksimum) < *wynik) *(a->maksimum) = *wynik;
 pthread_mutex_unlock(a->rygiel);
 free(a);
 return (void *) wynik;
}
int main(int argc, char **argv) {
 int m = atoi(argv[1]), n, i, j;
 double **tablica, *pomoc, wynik;
 pthread_t *watki;
 pthread_mutex_t kontrola = PTHREAD_MUTEX_INITIALIZER;
 Dane *y;
 sscanf(argv[2], "%d", &n);
 tablica = (double **) malloc(sizeof(double *) * m);
 watki = (pthread_t *) malloc(sizeof(pthread_t) * m);
 tablica[0] = (double *) malloc(sizeof(double) * m * n);
 for (i = 1; i < m; i++)
 tablica[i] = &(tablica[0][i * n]);
 srand(time(NULL));
 for (i = 0; i < m; i++) {
 for (j = 0; j < n; j++) {
 tablica[i][j] = (rand() % 1000) / 77;
 printf("[%d][%d]=%lf ", i, j, tablica[i][j]);
 }
 printf("\n");
 }
 wynik = tablica[0][0];
 for (i = 0; i < m; i++) {
 y = (Dane *) malloc(sizeof(Dane));
 y->dlugosc = n;
 y->tab = tablica[i];
 y->maksimum = &wynik;
 y->rygiel = &kontrola;
 pthread_create(&(watki[i]), NULL, obsluga, (void *) y);
 }
 for (i = 0; i < m; i++) {
 pthread_join(watki[i], (void **) &pomoc);
 free(pomoc);
 }
 printf("Wynik %lf.\n", wynik);
 free(tablica[0]);
 free(watki);
 free(tablica);
 return
0
;
}
```
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
struct dane {
 double *tab, *maksimum;
 int dlugosc;
 pthread_mutex_t *rygiel;
};
typedef struct dane Dane;
void *obsluga(void *x) {
 Dane *a = (Dane *) x;
 int i;
 double *wynik = (double *) malloc(sizeof(double));
 *wynik = a->tab[0];
 for (i = 1; i < a->dlugosc; i++)
 if (*wynik < a->tab[i]) *wynik = a->tab[i];
 if (*(a->maksimum) < *wynik) {
 pthread_mutex_lock(a->rygiel);
 if (*(a->maksimum) < *wynik) *(a->maksimum) = *wynik;
 pthread_mutex_unlock(a->rygiel);
 }
 free(a);
 return (void *) wynik;
}
int main(int argc, char **argv) {
 int m = atoi(argv[1]), n, i, j;
 double **tablica, *pomoc, wynik;
 pthread_t *watki;
 pthread_mutex_t kontrola = PTHREAD_MUTEX_INITIALIZER;
 Dane *y;
 sscanf(argv[2], "%d", &n);
 tablica = (double **) malloc(sizeof(double *) * m);
 watki = (pthread_t *) malloc(sizeof(pthread_t) * m);
 tablica[0] = (double *) malloc(sizeof(double) * m * n);
 for (i = 1; i < m; i++)
 tablica[i] = &(tablica[0][i * n]);
 srand(time(NULL));
 for (i = 0; i < m; i++) {
 for (j = 0; j < n; j++) {
 tablica[i][j] = (rand() % 1000) / 77;
 printf("[%d][%d]=%lf ", i, j, tablica[i][j]);
 }
 printf("\n");
 }
 wynik = tablica[0][0];
 for (i = 0; i < m; i++) {
 y = (Dane *) malloc(sizeof(Dane));
 y->dlugosc = n;
 y->tab = tablica[i];
 y->maksimum = &wynik;
 y->rygiel = &kontrola;
 pthread_create(&(watki[i]), NULL, obsluga, (void *) y);
 }
 for (i = 0; i < m; i++) {
 pthread_join(watki[i], (void **) &pomoc);
 free(pomoc);
 }
 printf("Wynik %lf.\n", wynik);
 free(tablica[0]);
 free(watki);
 free(tablica);
 return 0;
}
```
```c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
struct dane {
 double *tab;
 int liczba_iteracji;
 pthread_t *watek;
 pthread_rwlock_t *rygiel;
};
typedef struct dane Dane;
void *obsluga_p(void *x) {
 Dane *a = (Dane *) x;
 int i;
 pthread_t id = pthread_self();
 for (i = 0; i < a->liczba_iteracji; i++) {
 pthread_rwlock_wrlock(a->rygiel);
 (*(a->watek)) = id;
 (*(a->tab)) += i;
 printf("Watek %d zmodyfikowal na %lf.\n", id, *(a->tab));
 pthread_rwlock_unlock(a->rygiel);
 sleep(5);
 }
 free(a);
 return NULL;
}
void *obsluga_cz(void *x) {
 Dane *a = (Dane *) x;
 int i;
 pthread_t id = pthread_self();
 for (i = 0; i < a->liczba_iteracji; i++) {
 pthread_rwlock_rdlock(a->rygiel);
 printf("Watek %d przeczytal %lf zapisane przez %d.\n", id, *(a->tab), *(a->watek));
 pthread_rwlock_unlock(a->rygiel);
 sleep(2);
 }
 free(a);
 return NULL;
}
int main(int argc, char **argv) {
 /* m - ilosc pisarzy, n - ilosc czytelnikow, l - ilosc iteracji
 * pisarzy, k - ilosc iteracji czytelnikow */
 int m = atoi(argv[1]), n, i, l, k;
 double pomoc = 3.33;
 pthread_t *watki_p, *watki_cz, watek;
 pthread_rwlock_t kontrola = PTHREAD_RWLOCK_INITIALIZER;
 Dane *y;
 sscanf(argv[2], "%d", &n);
 sscanf(argv[3], "%d", &l);
 sscanf(argv[4], "%d", &k);
 watki_p = (pthread_t *) malloc(sizeof(pthread_t) * m);
 watki_cz = (pthread_t *) malloc(sizeof(pthread_t) * n);
 for (i = 0; i < m; i++) {
 y = (Dane *) malloc(sizeof(Dane));
 y->watek = &watek;
 y->tab = &pomoc;
 y->rygiel = &kontrola;
 y->liczba_iteracji = l;
 pthread_create(&(watki_p[i]), NULL, obsluga_p, (void *) y);
 }
 for (i = 0; i < n; i++) {
 y = (Dane *) malloc(sizeof(Dane));
 y->watek = &watek;
 y->tab = &pomoc;
 y->rygiel = &kontrola;
 y->liczba_iteracji = k;
 pthread_create(&(watki_cz[i]), NULL, obsluga_cz, (void *) y);
 }
 for (i = 0; i < m; i++)
 pthread_join(watki_p[i], NULL);
 for (i = 0; i < n; i++)
 pthread_join(watki_cz[i], NULL);
 free(watki_p);
 free(watki_cz);
 return 0;
}
```
#### Zmienna warunkowa
```c
/*
Program wielowątkowy z synchronizacją obliczeń wszystkich wątków - wariant 1
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct dane {
    double *tab, *maksimum;
    int dlugosc, *ilosci, liczba;
    pthread_mutex_t *rygiel;
    pthread_cond_t *warunek;
} Dane;

void *obsluga(void *x) {
    Dane *a = (Dane *) x;
    int i;
    pthread_t id = pthread_self();
    double *wynik = (double *) malloc(sizeof(double));
    
    *wynik = a->tab[0];
    for (i = 1; i < a->dlugosc; i++)
        if (*wynik < a->tab[i]) *wynik = a->tab[i];
    printf("Przed synchronizacja watek %ld.\n", (long) id);
    pthread_mutex_lock(a->rygiel);
    (*(a->ilosci))++;
    if (*(a->ilosci) == a->liczba) {
        *(a->ilosci) = 0;
        pthread_cond_broadcast(a->warunek);
    } else pthread_cond_wait(a->warunek, a->rygiel);
    pthread_mutex_unlock(a->rygiel);
    printf("Po synchronizacji watek %ld.\n", (long) id);
    if (*(a->maksimum) < *wynik) {
        pthread_mutex_lock(a->rygiel);
        if (*(a->maksimum) < *wynik) *(a->maksimum) = *wynik;
        pthread_mutex_unlock(a->rygiel);
    }
    free(a);
    return (void *) wynik;
}

int main(int argc, char **argv) {
    int m = atoi(argv[1]), n, i, j, zliczaj = 0;
    double **tablica, *pomoc, wynik;
    pthread_t *watki;
    pthread_mutex_t kontrola = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t bariera = PTHREAD_COND_INITIALIZER;
    Dane *y;

    sscanf(argv[2], "%d", &n);
    tablica = (double **) malloc(sizeof(double *) * m);
    watki = (pthread_t *) malloc(sizeof(pthread_t) * m);
    tablica[0] = (double *) malloc(sizeof(double) * m * n);
    for (i = 1; i < m; i++)
        tablica[i] = &(tablica[0][i * n]);
    srand(time(NULL));
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            tablica[i][j] = (rand() % 1000) / 77;
            printf("[%d][%d]=%lf ", i, j, tablica[i][j]);
        }
        printf("\n");
    }
    wynik = tablica[0][0];
    for (i = 0; i < m; i++) {
        y = (Dane *) malloc(sizeof(Dane));
        y->liczba = m;
        y->dlugosc = n;
        y->tab = tablica[i];
        y->maksimum = &wynik;
        y->ilosci = &zliczaj;
        y->rygiel = &kontrola;
        y->warunek = &bariera;
        pthread_create(&(watki[i]), NULL, obsluga, (void *) y);
    }
    for (i = 0; i < m; i++) {
        pthread_join(watki[i], (void **) &pomoc);
        free(pomoc);
    }
    printf("Wynik %lf.\n", wynik);
    free(tablica[0]);
    free(watki);
    free(tablica);
    return 0;
}
```
```c
/*
Program wielowątkowy z synchronizacją obliczeń wszystkich wątków - wariant 2
*/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct dane {
    double *tab, *maksimum;
    int dlugosc, liczba;
    pthread_mutex_t *rygiel;
} Dane;

int bariera(int liczba) {
    static int ilosci = 0;
    static pthread_mutex_t rygiel = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t warunek = PTHREAD_COND_INITIALIZER;

    pthread_mutex_lock(&rygiel);
    ilosci++;
    if (ilosci == liczba) {
        ilosci = 0;
        pthread_cond_broadcast(&warunek);
    } else pthread_cond_wait(&warunek, &rygiel);
    pthread_mutex_unlock(&rygiel);
    return 0;
}

void *obsluga(void *x) {
    Dane *a = (Dane *) x;
    int i;
    pthread_t id = pthread_self();
    double *wynik = (double *) malloc(sizeof(double));

    *wynik = a->tab[0];
    for (i = 1; i < a->dlugosc; i++)
        if (*wynik < a->tab[i]) *wynik = a->tab[i];
    printf("Przed synchronizacja watek %ld.\n", (long)id);
    bariera(a->liczba);
    printf("Po synchronizacji watek %ld.\n", (long)id);
    if (*(a->maksimum) < *wynik) {
        pthread_mutex_lock(a->rygiel);
        if (*(a->maksimum) < *wynik) *(a->maksimum) = *wynik;
        pthread_mutex_unlock(a->rygiel);
    }
    free(a);
    return (void *) wynik;
}

int main(int argc, char **argv) {
    int m = atoi(argv[1]), n, i, j;
    double **tablica, *pomoc, wynik;
    pthread_t *watki;
    pthread_mutex_t kontrola = PTHREAD_MUTEX_INITIALIZER;
    Dane *y;

    sscanf(argv[2], "%d", &n);
    tablica = (double **) malloc(sizeof(double *) * m);
    watki = (pthread_t *) malloc(sizeof(pthread_t) * m);
    tablica[0] = (double *) malloc(sizeof(double) * m * n);
    for (i = 1; i < m; i++)
        tablica[i] = &(tablica[0][i * n]);
    srand(time(NULL));
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            tablica[i][j] = (rand() % 1000) / 77;
            printf("[%d][%d]=%lf ", i, j, tablica[i][j]);
        }
        printf("\n");
    }
    wynik = tablica[0][0];
    for (i = 0; i < m; i++) {
        y = (Dane *) malloc(sizeof(Dane));
        y->liczba = m;
        y->dlugosc = n;
        y->tab = tablica[i];
        y->maksimum = &wynik;
        y->rygiel = &kontrola;
        pthread_create(&(watki[i]), NULL, obsluga, (void *) y);
    }
    for (i = 0; i < m; i++) {
        pthread_join(watki[i], (void **) &pomoc);
        free(pomoc);
    }
    printf("Wynik %lf.\n", wynik);
    free(tablica[0]);
    free(watki);
    free(tablica);
    return 0;
}
```
#### Zadania
- Zadanie 15
- Zadanie 16
- Zadanie 17
- Zadanie 18
- Zadanie 19
- Zadanie 20
# Laboratorium 5. - 4. grudnia 2022
### Komunikacja międzyprocesowa z wykorzystaniem kolejek FIFO i IPC.
Polecenia zarządzające zasobami IPC.
1. Listowanie zasobów IPC - ipcs.
2. Niszczenie zasobu IPC - ipcrm.
#### Komunikacja z wykorzystaniem kolejek FIFO
```c
/*
Serwer oparty na kolejkach FIFO
*/

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/uio.h>
#include<signal.h>
#include<string.h>

static const char *katalog = "/tmp/FIFO-TMP-SERWER";
int f, f3;

void ala(int i) {
    close(f);
    close(f3);
    unlink(katalog);
    exit(0);
}

int main() {
    int f1, f2;
    char bufor[1000], *pom = bufor;
    pid_t nr_proc, pid;

    signal(SIGTERM, ala);
    signal(SIGINT, ala);
    if (mknod(katalog, S_IFIFO | 0600, 0) == -1) {
        perror("Serwer: Blad tworzenia kolejki FIFO. [1]\n");
        return -1;
    } else {
        if ((f = open(katalog, O_RDONLY)) == -1 || (f3 = open(katalog, O_WRONLY)) == -1) {
            perror("Serwer: Blad otwarcia kolejki FIFO. [2]\n");
            return -1;
        } else {
            for (;;) {
                read(f, &nr_proc, sizeof(pid_t));
                if ((pid = fork()) == -1) {
                    perror("Wywolanie funkcji fork nie powiodlo sie.\n");
                    return -1;
                } else {
                    if (pid == 0) {
                        strcpy(pom, "/tmp/FIFO-TMP-2-");
                        pom += strlen(pom);
                        sprintf(pom, "%d", (int) nr_proc);
                        if ((f1 = open(bufor, O_RDONLY)) == -1) /* kolejka do czytania */ {
                            perror("Serwer: Blad otwarcia kolejki FIFO. [3]\n");
                            close(f);
                            return -1;
                        } else {
                            *(pom - 2) = '1';
                            if ((f2 = open(bufor, O_WRONLY)) == -1) /* kolejka do pisania */ {
                                perror("Serwer: Blad otwarcia kolejki FIFO. [4]\n");
                                close(f1);
                                close(f);
                                return -1;
                            } else {
                                double a, b;

                                read(f1, &a, sizeof(double));
                                b = a * a;
                                write(f2, &b, sizeof(double));
                            }
                        }
                        close(f1);
                        close(f2);
                        close(f);
                        return 0;
                    } else {
                        wait(NULL);
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient oparty na kolejkach FIFO
*/

#include<unistd.h>
#include<stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>

static const char *katalog = "/tmp/FIFO-TMP-SERWER";

int main() {
    pid_t numer_procesu;
    int f, f1, f2;
    char bufor[1000], *pom = bufor;

    numer_procesu = getpid();
    if ((f = open(katalog, 1)) == -1) {
        perror("Klient: Nie moge ustanowic polaczenia z serwerem. [1]\n");
        return -1;
    } else {
        strcpy(pom, "/tmp/FIFO-TMP-1-");
        pom += strlen(pom);
        sprintf(pom, "%d", (int) numer_procesu);
        if (mknod(bufor, S_IFIFO | 0600, 0) == -1) /* kolejka do czytania */ {
            perror("Klient: Blad tworzenia kolejki FIFO. [2]\n");
            close(f);
            return -1;
        } else {
            *(pom - 2) = '2';
            if (mknod(bufor, S_IFIFO | 0600, 0) == -1) /* kolajka do pisania */ {
                perror("Klient: Blad tworzenia kolejki FIFO. [3]\n");
                close(f2);
                close(f);
                return -1;
            }
            write(f, &numer_procesu, sizeof(pid_t));
            close(f);
            if ((f1 = open(bufor, O_WRONLY)) == -1) /* kolejka do pisania */ {
                perror("Klient: Blad otwarcia kolejki FIFO. [4]\n");
                close(f);
                return -1;
            } else {
                *(pom - 2) = '1';
                if ((f2 = open(bufor, O_RDONLY)) == -1) /* kolajka do czytania */ {
                    perror("Klient: Blad otwarcia kolejki FIFO. [5]\n");
                    close(f1);
                    close(f);
                    return -1;
                } else {
                    double a, b;

                    printf("Podaj liczbe: ");
                    scanf("%lf", &a);
                    write(f1, &a, sizeof(double));
                    read(f2, &b, sizeof(double));
                    printf("Wynik obliczen %lf^2 = %lf.\n", a, b);
                    close(f1);
                    close(f2);
                    close(f);
                    unlink(bufor);
                    *(pom - 2) = '2';
                    unlink(bufor);
                    return 0;
                }
            }
        }
    }
}
```
#### Komunikacja z wykorzystaniem kolejek komunikatów
```c
/* 
Serwer wspolbiezny oparty na kolejekach komunikatow.
*/

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/uio.h>
#include<signal.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>

typedef struct mymesg1 {
    long mtype;
    pid_t pid;
} paczka1;
typedef struct mymesg2 {
    long mtype;
    double melement;
} paczka2;
int f;
paczka1 *wiad;
char *sciezka;
int czy_skasowac = 1;

void ala(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Serwer: Koniec pracy serwera.\n");
        free(wiad);
        msgctl(f, IPC_RMID, NULL);
        if (czy_skasowac) {
            unlink(sciezka);
        }
        free(sciezka);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    key_t klucz_kolejka;
    const size_t wielkosc1 = sizeof(paczka1) - sizeof(long);
    ssize_t liczba_bajtow;
    int plik = 0;

    signal(SIGTERM, ala);
    signal(SIGINT, ala);
    sigignore(SIGCHLD);
    if (argc > 1) {
        sciezka = strndup(argv[1], strlen(argv[1]));
    } else {
        sciezka = strndup("/tmp/roboczy.kolejka_serwer", strlen("/tmp/roboczy.kolejka_serwer"));
    }
    if (access(sciezka, F_OK) == 0) {
        czy_skasowac = 0;
    }
    if (czy_skasowac && ((plik = open(sciezka, O_CREAT | O_EXCL, 0600)) == -1)) {
        fprintf(stderr, "Serwer: Blad utworzenia pliku %s: %s.\n", sciezka, strerror(errno));
        free(sciezka);
        return -1;
    } else {
        close(plik);
        if ((klucz_kolejka = ftok(sciezka, 0)) == -1) {
            perror("Serwer: Blad funkcji ftok.\n");
            if (czy_skasowac) {
                unlink(sciezka);
            }
            free(sciezka);
            return -1;
        } else {
            if ((f = msgget(klucz_kolejka, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
                perror("Serwer: Blad funkcji msgget.\n");
                if (czy_skasowac) {
                    unlink(sciezka);
                }
                free(sciezka);
                return -1;
            } else {
                for (;;) {
                    wiad = (paczka1 *) malloc(sizeof(paczka1));
                    if ((liczba_bajtow = msgrcv(f, wiad, wielkosc1, 1, 0)) == -1) {
                        perror("Serwer: Blad funkcji msgrcv.\n");
                        if (czy_skasowac) {
                            unlink(sciezka);
                        }
                        free(sciezka);
                        return -1;
                    } else {
                        printf("Serwer: Przeczytalem %ld bajtow od klienta na glownej kolejce.\n", liczba_bajtow);
                        if ((pid = fork()) == -1) {
                            perror("Serwer: Wywolanie funkcji fork nie powiodlo sie.\n");
                            return -1;
                        } else {
                            if (pid == 0) {
                                int f1;

                                free(sciezka);
                                if ((klucz_kolejka = ftok(sciezka, wiad->pid)) == -1) {
                                    perror("Serwer, proces potomny: Blad funkcji ftok.\n");
                                    return -1;
                                } else {
                                    if ((f1 = msgget(klucz_kolejka, 0600)) == -1) {
                                        perror("Serwer, proces potomny : Nie moge utworzyc kolejki komunikatow dla klienta.\n");
                                        return -1;
                                    } else {
                                        paczka2 *praca = (paczka2 *) malloc(sizeof(paczka2));
                                        const size_t wielkosc2 = sizeof(paczka2) - sizeof(long);

                                        if ((liczba_bajtow = msgrcv(f1, praca, wielkosc2, 2, 0)) == -1) {
                                            perror("Serwer, proces potomny: Blad funkcji msgrcv.\n");
                                            return -1;
                                        } else {
                                            printf("Serwer, proces potomny: Przeczytalem %ld bajtow od klienta.\n", liczba_bajtow);
                                            praca->melement *= praca->melement;
                                            praca->mtype = 3;
                                            if ((liczba_bajtow = msgsnd(f1, praca, wielkosc2, 0)) == -1) {
                                                perror("Serwer, proces potomny: Blad funkcji msgsnd.\n");
                                                return -1;
                                            } else {
                                                free(praca);
                                                return 0;
                                            }
                                        }
                                    }
                                }
                            } else {
                                sigignore(SIGCHLD);
                            }
                        }
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient oparty na kolejkach komunikatow dla serwera wspolbieznego.
*/

#include<unistd.h>
#include<stdio.h>
#include<sys/uio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>

typedef struct mymesg1 {
    long mtype;
    pid_t pid;
} paczka1;
typedef struct mymesg2 {
    long mtype;
    double melement;
} paczka2;

int main(int argc, char *argv[]) {
    int f, f1;
    key_t klucz_kolejka;
    char *sciezka;
    int czy_czekac = 0, liczba_sekund;
    pid_t pid = getpid();
    const size_t wielkosc1 = sizeof(paczka1) - sizeof(long);

    if (argc > 1) {
        czy_czekac = 1;
        liczba_sekund = strtol(argv[1], NULL, 10);
    }
    if (argc > 2) {
        sciezka = strndup(argv[2], strlen(argv[2]));
    } else {
        sciezka = strndup("/tmp/roboczy.kolejka_serwer", strlen("/tmp/roboczy.kolejka_serwer"));
    }
    if ((klucz_kolejka = ftok(sciezka, 0)) == -1) {
        perror("Klient: Blad funkcji ftok.\n");
        free(sciezka);
        return -1;
    } else {
        if ((f = msgget(klucz_kolejka, 0600)) == -1) {
            perror("Klient: Blad funkcji msgget.\n");
            return -1;
        } else {
            paczka1 *wiad = (paczka1 *) malloc(sizeof(paczka1));
            ssize_t liczba_bajtow;

            wiad->mtype = 1;
            wiad->pid = pid;
            if (msgsnd(f, wiad, wielkosc1, 0) == -1) {
                perror("Klient: Blad funkcji msgsnd.\n");
                free(wiad);
                return -1;
            } else {
                free(wiad);
                if ((klucz_kolejka = ftok(sciezka, pid)) == -1) {
                    perror("Klient: Blad funkcji ftok.\n");
                    free(sciezka);
                    return -1;
                } else {
                    free(sciezka);
                    if ((f1 = msgget(klucz_kolejka, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
                        perror("Klient: Blad funkcji msgget.\n");
                        return -1;
                    } else {
                        paczka2 *praca;
                        const size_t wielkosc2 = sizeof(paczka2) - sizeof(long);
                        double a = 0;

                        printf("Klient: Podaj liczbe: ");
                        scanf("%lf", &a);
                        praca = (paczka2 *) malloc(sizeof(paczka2));
                        praca->mtype = 2;
                        praca->melement = a;
                        if (czy_czekac) {
                            printf("Klient: Oczekuje %d sekund na wyslanie wiadomosci do serwera.\n", liczba_sekund);
                            sleep(liczba_sekund);
                        }
                        if (msgsnd(f1, praca, wielkosc2, 0) == -1) {
                            free(praca);
                            msgctl(f1, IPC_RMID, NULL);
                            return -1;
                        } else {
                            if ((liczba_bajtow = msgrcv(f1, praca, wielkosc2, 3, 0)) == -1) {
                                free(praca);
                                msgctl(f1, IPC_RMID, NULL);
                                return -1;
                            } else {
                                printf("Klient: Odebralem %ld bajtow od serwera.\n", liczba_bajtow);
                                printf("Klient: Wynik: %lf^2 = %lf.\n", a, praca->melement);
                                free(praca);
                                msgctl(f1, IPC_RMID, NULL);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
}
```
```c
/*
Serwer sekwencyjny oparty na semaforach i pamieci wspoldzielonej (wersja standardowa). Do synchronizacji dostepu do pamieci uzywane sa dwa semafory: 0 (wartosc 0, gdy klient nie ma dostepu do wspolnej pamieci i warotsc 1, gdy klient ma dostep do wspolnej pamieci), 1 (wartosc 0, gdy serwer nie ma dostepu do wspolnej pamieci i wartosc 1, gdy serwer ma dostep do wspolnej pamieci).
*/

#include<stdio.h>
#include<string.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>

typedef struct sembuf bufor_semaforow;
int id_semafor;
int id_pamieci;
char *pamiec_wspolna;
char *sciezka;
int czy_skasowac = 1;

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Serwer: Koniec pracy serwera.\n");
        shmdt(pamiec_wspolna);
        shmctl(id_pamieci, IPC_RMID, NULL);
        semctl(id_semafor, 0, IPC_RMID, NULL);
        if (czy_skasowac) {
            unlink(sciezka);
        }
        free(sciezka);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    key_t klucz_semafory, klucz_pamiec_wspolna;
    bufor_semaforow ustaw_semafory;
    int plik = 0;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    if (argc > 1) {
        sciezka = strndup(argv[1], strlen(argv[1]));
    } else {
        sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
    }
    if (access(sciezka, F_OK) == 0) {
        czy_skasowac = 0;
    }
    if (czy_skasowac && ((plik = open(sciezka, O_CREAT | O_EXCL, 0600)) == -1)) {
        fprintf(stderr, "Serwer: Blad utworzenia pliku %s: %s.\n", sciezka, strerror(errno));
        free(sciezka);
        return -1;
    } else {
        close(plik);
        if ((klucz_semafory = ftok(sciezka, 0)) == -1 || (klucz_pamiec_wspolna = ftok(sciezka, 1)) == -1) {
            perror("Serwer: Blad funkcji ftok.\n");
            if (czy_skasowac) {
                unlink(sciezka);
            }
            free(sciezka);
            return -1;
        } else {
            if ((id_pamieci = shmget(klucz_pamiec_wspolna, 50 * sizeof(char), 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
                perror("Serwer: Blad funkcji shmget.\n");
                if (czy_skasowac) {
                    unlink(sciezka);
                }
                free(sciezka);
                return -1;
            } else {
                pamiec_wspolna = shmat(id_pamieci, (char *) 0, 0);
                if ((id_semafor = semget(klucz_semafory, 2, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
                    perror("Serwer: Blad funkcji semget.\n");
                    shmdt(pamiec_wspolna);
                    shmctl(id_pamieci, IPC_RMID, NULL);
                    if (czy_skasowac) {
                        unlink(sciezka);
                    }
                    free(sciezka);
                    return -1;
                } else {
                    u_short wartosci_poczatkowe_semaforow[] = {1, 0};

                    if ((semctl(id_semafor, 0, SETALL, wartosci_poczatkowe_semaforow)) == -1) {
                        perror("Serwer: Blad funkcji semctl.\n");
                        semctl(id_semafor, 0, IPC_RMID, NULL);
                        shmdt(pamiec_wspolna);
                        shmctl(id_pamieci, IPC_RMID, NULL);
                        if (czy_skasowac) {
                            unlink(sciezka);
                        }
                        free(sciezka);
                        return -1;
                    } else {
                        for (;;) {
                            ustaw_semafory.sem_num = 1;
                            ustaw_semafory.sem_op = -1;
                            ustaw_semafory.sem_flg = 0;
                            printf("Serwer: Oczekuje na wiadomosc od klienta.\n");
                            if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                                perror("Server: Blad oczekiwania na otwarcie semafora do odczytu dla serwera (oczekiwanie serwera), funkcja semop.\n");
                                shmdt(pamiec_wspolna);
                                shmctl(id_pamieci, IPC_RMID, NULL);
                                semctl(id_semafor, 0, IPC_RMID, NULL);
                                if (czy_skasowac) {
                                    unlink(sciezka);
                                }
                                free(sciezka);
                                return -1;
                            } else {
                                char *wiadomosc = strdup(pamiec_wspolna);

                                ustaw_semafory.sem_num = 0;
                                ustaw_semafory.sem_op = 1;
                                ustaw_semafory.sem_flg = 0;
                                if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                                    free(wiadomosc);
                                    perror("Serwer: Blad oczekiwania na otwarcie semafora do zapisu dla klienta (zwolnienie dla klienta), funkcja semop.\n");
                                    shmdt(pamiec_wspolna);
                                    shmctl(id_pamieci, IPC_RMID, NULL);
                                    semctl(id_semafor, 0, IPC_RMID, NULL);
                                    if (czy_skasowac) {
                                        unlink(sciezka);
                                    }
                                    free(sciezka);
                                    return -1;
                                } else {
                                    printf("Serwer: Wiadomosc od klienta: %s.\n", wiadomosc);
                                }
                                free(wiadomosc);
                            }
                        }
                    }
                }
            }
        }
    }
}
```
#### Komunikacja z wykorzystaniem semaforów i pamięci współdzielonej
```c
/*
Klient oparty na semaforach i pamieci wspoldzielonej dla serwera sekwencyjnego (wersja standardowa). Do synchronizacji dostepu do pamieci uzywane sa dwa semafory: 0 (wartosc 0, gdy klient nie ma dostepu do wspolnej pamieci i warotsc 1, gdy klient ma dostep do wspolnej pamieci), 1 (wartosc 0, gdy serwer nie ma dostepu do wspolnej pamieci i wartosc 1, gdy serwer ma dostep do wspolnej pamieci).
*/

#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct sembuf bufor_semaforow;

int main(int argc, char *argv[]) {
    key_t klucz_semafory, klucz_pamiec_wspolna;
    bufor_semaforow ustaw_semafory;
    int id_semafor;
    int id_pamieci;
    char *sciezka;
    int czy_czekac = 0, liczba_sekund;
    pid_t pid = getpid();

    if (argc > 1) {
        czy_czekac = 1;
        liczba_sekund = strtol(argv[1], NULL, 10);
    }
    if (argc > 2) {
        sciezka = strndup(argv[2], strlen(argv[2]));
    } else {
        sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
    }
    if ((klucz_semafory = ftok(sciezka, 0)) == -1 || (klucz_pamiec_wspolna = ftok(sciezka, 1)) == -1) {
        perror("Klient: Blad funkcji ftok.\n");
        free(sciezka);
        return -1;
    } else {
        free(sciezka);
        if ((id_semafor = semget(klucz_semafory, 2, 0600)) == -1) {
            perror("Klient: Blad funkcji semget.\n");
            return -1;
        } else {
            ustaw_semafory.sem_num = 0;
            ustaw_semafory.sem_op = -1;
            ustaw_semafory.sem_flg = 0;
            printf("Klient: Moj numer procesu to %d. Oczekuje na mozliwosc zapisu.\n", pid);
            if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                perror("Klient: Blad oczekiwania na otwarcie semafora do zapisu dla klienta (oczekiwanie klienta), funkcja semop.\n");
                return -1;
            } else {
                if ((id_pamieci = shmget(klucz_pamiec_wspolna, 50 * sizeof(char), 0600)) == -1) {
                    perror("Klient: Blad funkcji shmget.\n");
                    return -1;
                } else {
                    char *wiadomosc = (char *) malloc(100 * sizeof(char));
                    char *pamiec_wspolna = shmat(id_pamieci, (char *) 0, 0);
                    
                    sprintf(wiadomosc, "Moj numer procesu to %d", pid);
                    strcpy(pamiec_wspolna, wiadomosc);
                    printf("Klient: Wiadomosc zapisana.\n");
                    free(wiadomosc);
                    shmdt(pamiec_wspolna);
                    if (czy_czekac) {
                        printf("Klient: Oczekuje %d sekund na zwolnienie semafora do czytania dla serwera.\n", liczba_sekund);
                        sleep(liczba_sekund);
                    }
                    ustaw_semafory.sem_num = 1;
                    ustaw_semafory.sem_op = 1;
                    ustaw_semafory.sem_flg = 0;
                    if (semop(id_semafor, &ustaw_semafory, 1) < 0) {
                        perror("Klient: Blad oczekiwania na otwarcie semafora do odczytu dla serwera (zwolnienie dla serwera).\n");
                        return -1;
                    }
                    return 0;
                }
            }
        }
    }
}
```
```c
/* 
Serwer sekwencyjny oparty na semaforach i pamieci wspoldzielonej (wersja rozbudowana). Do synchronizacji dostepu do pamieci uzywane sa trzy semafory: 0 (wartosc 0, gdy klient nie ma dostepu do wspolnej pamieci i warotsc 1, gdy klient ma dostep do wspolnej pamieci), 1 (wartosc 0, gdy serwer nie ma dostepu do wspolnej pamieci i wartosc 1, gdy serwer ma dostep do wspolnej pamieci), 2 (wartosc 0, gdy klient nie ma dostepu do serwera i warotsc 1, gdy klient ma dostep do serwera).
*/

#include<stdio.h>
#include<string.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>

typedef struct sembuf bufor_semaforow;
int id_semafor;
int id_pamieci;
char *pamiec_wspolna;
char *sciezka;
int czy_skasowac = 1;

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Serwer: Koniec pracy serwera.\n");
        shmdt(pamiec_wspolna);
        shmctl(id_pamieci, IPC_RMID, NULL);
        semctl(id_semafor, 0, IPC_RMID, NULL);
        if (czy_skasowac) {
            unlink(sciezka);
        }
        free(sciezka);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    key_t klucz_semafory, klucz_pamiec_wspolna;
    bufor_semaforow ustaw_semafory;
    int plik = 0;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    if (argc > 1) {
        sciezka = strndup(argv[1], strlen(argv[1]));
    } else {
        sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
    }
    if (access(sciezka, F_OK) == 0) {
        czy_skasowac = 0;
    }
    if (czy_skasowac && ((plik = open(sciezka, O_CREAT | O_EXCL, 0600)) == -1)) {
        fprintf(stderr, "Serwer: Blad utworzenia pliku %s: %s.\n", sciezka, strerror(errno));
        free(sciezka);
        return -1;
    } else {
        close(plik);
        if ((klucz_semafory = ftok(sciezka, 0)) == -1 || (klucz_pamiec_wspolna = ftok(sciezka, 1)) == -1) {
            perror("Serwer: Blad funkcji ftok.\n");
            if (czy_skasowac) {
                unlink(sciezka);
            }
            free(sciezka);
            return -1;
        } else {
            if ((id_pamieci = shmget(klucz_pamiec_wspolna, 50 * sizeof(char), 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
                perror("Serwer: Blad funkcji shmget.\n");
                if (czy_skasowac) {
                    unlink(sciezka);
                }
                free(sciezka);
                return -1;
            } else {
                pamiec_wspolna = shmat(id_pamieci, (char *) 0, 0);
                if ((id_semafor = semget(klucz_semafory, 3, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
                    perror("Serwer: Blad funkcji semget.\n");
                    shmdt(pamiec_wspolna);
                    shmctl(id_pamieci, IPC_RMID, NULL);
                    if (czy_skasowac) {
                        unlink(sciezka);
                    }
                    free(sciezka);
                    return -1;
                } else {
                    u_short wartosci_poczatkowe_semaforow[] = {1, 0, 1};

                    if ((semctl(id_semafor, 0, SETALL, wartosci_poczatkowe_semaforow)) == -1) {
                        perror("Serwer: Blad funkcji semctl.\n");
                        semctl(id_semafor, 0, IPC_RMID, NULL);
                        shmdt(pamiec_wspolna);
                        shmctl(id_pamieci, IPC_RMID, NULL);
                        if (czy_skasowac) {
                            unlink(sciezka);
                        }
                        free(sciezka);
                        return -1;
                    } else {
                        for (;;) {
                            ustaw_semafory.sem_num = 1;
                            ustaw_semafory.sem_op = -1;
                            ustaw_semafory.sem_flg = 0;
                            printf("Serwer: Oczekuje na wiadomosc od klienta.\n");
                            if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                                perror("Serwer: Blad oczekiwania na otwarcie semafora do odczytu dla serwera (oczekiwanie serwera), funkcja semop.\n");
                                shmdt(pamiec_wspolna);
                                shmctl(id_pamieci, IPC_RMID, NULL);
                                semctl(id_semafor, 0, IPC_RMID, NULL);
                                if (czy_skasowac) {
                                    unlink(sciezka);
                                }
                                free(sciezka);
                                return -1;
                            } else {
                                char *wiadomosc = strdup(pamiec_wspolna);

                                ustaw_semafory.sem_num = 0;
                                ustaw_semafory.sem_op = 1;
                                ustaw_semafory.sem_flg = 0;
                                if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                                    free(wiadomosc);
                                    perror("Serwer: Blad oczekiwania na otwarcie semafora do zapisu dla klienta (zwolnienie dla klienta), funkcja semop.\n");
                                    shmdt(pamiec_wspolna);
                                    shmctl(id_pamieci, IPC_RMID, NULL);
                                    semctl(id_semafor, 0, IPC_RMID, NULL);
                                    if (czy_skasowac) {
                                        unlink(sciezka);
                                    }
                                    free(sciezka);
                                    return -1;
                                } else {
                                    printf("Serwer: Wiadomosc od klienta: %s.\n", wiadomosc);
                                }
                                free(wiadomosc);
                            }
                        }
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient oparty na semaforach i pamieci wspoldzielonej dla serwera sekwencyjnego (wersja rozbudowana). Do synchronizacji dostepu do pamieci uzywane sa trzy semafory: 0 (wartosc 0, gdy klient nie ma dostepu do wspolnej pamieci i warotsc 1, gdy klient ma dostep do wspolnej pamieci), 1 (wartosc 0, gdy serwer nie ma dostepu do wspolnej pamieci i wartosc 1, gdy serwer ma dostep do wspolnej pamieci), 2 (wartosc 0, gdy klient nie ma dostepu do serwera i warotsc 1, gdy klient ma dostep do serwera).
*/

#include<sys/sem.h>
#include<sys/shm.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct sembuf bufor_semaforow;

int main(int argc, char *argv[]) {
    key_t klucz_semafory, klucz_pamiec_wspolna;
    bufor_semaforow ustaw_semafory;
    int id_semafor;
    int id_pamieci;
    char *sciezka;
    int czy_czekac = 0, liczba_sekund;
    pid_t pid = getpid();

    if (argc > 1) {
        czy_czekac = 1;
        liczba_sekund = strtol(argv[1], NULL, 10);
    }
    if (argc > 2) {
        sciezka = strndup(argv[2], strlen(argv[2]));
    } else {
        sciezka = strndup("/tmp/roboczy.sem_serwer", strlen("/tmp/roboczy.sem_serwer"));
    }
    if ((klucz_semafory = ftok(sciezka, 0)) == -1 || (klucz_pamiec_wspolna = ftok(sciezka, 1)) == -1) {
        perror("Klient: Blad funkcji ftok.\n");
        free(sciezka);
        return -1;
    } else {
        free(sciezka);
        if ((id_semafor = semget(klucz_semafory, 3, 0600)) == -1) {
            perror("Klient: Blad funkcji semget.\n");
            return -1;
        } else {
            ustaw_semafory.sem_num = 2;
            ustaw_semafory.sem_op = -1;
            ustaw_semafory.sem_flg = 0;
            if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                perror("Klient: Blad oczekiwania na otwarcie semafora (2) do zapisu dla klienta (oczekiwanie klienta), funkcja semop.\n");
                return -1;
            } else {
                if ((id_pamieci = shmget(klucz_pamiec_wspolna, 50 * sizeof(char), 0600)) == -1) {
                    perror("Klient: Blad funkcji shmget.\n");
                    return -1;
                } else {
                    ustaw_semafory.sem_num = 0;
                    ustaw_semafory.sem_op = -1;
                    ustaw_semafory.sem_flg = 0;
                    printf("Klient: Moj numer procesu to %d. Oczekuje na mozliwosc zapisu.\n", pid);
                    if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                        perror("Klient: Blad oczekiwania na otwarcie semafora (0) do zapisu dla klienta (oczekiwanie klienta), funkcja semop.\n");
                        return -1;
                    } else {
                        char *wiadomosc = (char *) malloc(100 * sizeof(char));
                        char *pamiec_wspolna = shmat(id_pamieci, (char *) 0, 0);

                        sprintf(wiadomosc, "Moj numer procesu to %d", pid);
                        strcpy(pamiec_wspolna, wiadomosc);
                        printf("Klient: Wiadomosc zapisana.\n");
                        free(wiadomosc);
                        shmdt(pamiec_wspolna);
                        if (czy_czekac) {
                            printf("Oczekuje %d sekund na zwolnienie semafora do czytania dla serwera.\n", liczba_sekund);
                            sleep(liczba_sekund);
                        }
                        ustaw_semafory.sem_num = 1;
                        ustaw_semafory.sem_op = 1;
                        ustaw_semafory.sem_flg = 0;
                        if (semop(id_semafor, &ustaw_semafory, 1) < 0) {
                            perror("Klient: Blad oczekiwania na otwarcie semafora do odczytu dla serwera (zwolnienie dla serwera).\n");
                            return -1;
                        }
                        ustaw_semafory.sem_num = 2;
                        ustaw_semafory.sem_op = 1;
                        ustaw_semafory.sem_flg = 0;
                        if (semop(id_semafor, &ustaw_semafory, 1) == -1) {
                            perror("Klient: Blad oczekiwania na otwarcie semafora (0) do zapisu dla klienta (oczekiwanie klienta), funkcja semop.\n");
                            return -1;
                        } else {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}
```
#### Zadania
- Zadanie 21
- Zadanie 22
- Zadanie 23
- Zadanie 24
- Zadanie 25
# Laboratorium 6. - 18. grudnia 2022
### Komunikacja międzyprocesowa z wykorzystaniem gniazd strumieniowych.
1. Do testowania programu serwera strumieniowego (TCP) można używać narzędzia netcat.
- Dla IPv4: nc 127.0.0.1 numer_portu.
- Dla IPv6: nc -6 ::1 numer_portu.
- Dla UNIX: nc -U ścieżka.
2. Do uruchomienia serwera do testowania programu klienta strumieniowego (TCP) można używać narzędzia netcat.
- Dla IPv4: nc -l numer_portu.
- Dla IPv6: nc -6 -l numer_portu.
- Dla UNIX: nc -U -l ścieżka.
### Komunikacja z wykorzystaniem gniazd strumieniowych uniksowych
```c
/*
Serwer wspolbiezny strumieniowy, unixowy.
*/

#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;
int gniazdo;
char *sciezka;

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Koniec pracy serwera ze sciezka %s.\n", sciezka);
        close(gniazdo);
        unlink(sciezka);
        free(sciezka);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int gniazdo1;
    socklen_t i;
    SockAddr_un adres, adres_k;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    sigignore(SIGCHLD);
    if (argc > 1) {
        sciezka = strndup(argv[1], strlen(argv[1]));
    } else {
        sciezka = strndup("/tmp/roboczy.unix_serwer", strlen("/tmp/roboczy.unix_serwer"));
    }
    if ((gniazdo = socket(PF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Serwer: Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        bzero((char *) &adres, sizeof(SockAddr_un));
        adres.sun_family = AF_UNIX;
        strncpy(adres.sun_path, sciezka, strlen(sciezka));
        i = sizeof(SockAddr_un);
        if (bind(gniazdo, (SockAddr *) &adres, i) == -1) {
            perror("Serwer: Blad dowiazania gniazda.\n");
            free(sciezka);
            close(gniazdo);
            return -1;
        } else {
            if (getsockname(gniazdo, (SockAddr *) &adres, &i) == -1) {
                perror("Serwer: Blad funkcji getsockname.\n");
                free(sciezka);
                close(gniazdo);
                unlink(adres.sun_path);
                return -1;
            } else {
                printf("Sciezka : %s.\n", adres.sun_path);
                if (listen(gniazdo, 5) == -1) {
                    perror("Serwer: Blad nasluchu gniazda.\n");
                    free(sciezka);
                    close(gniazdo);
                    unlink(adres.sun_path);
                    return -1;
                } else {
                    for (;;) {
                        if ((gniazdo1 = accept(gniazdo, (SockAddr *) &adres_k, &i)) == -1) {
                            perror("Serwer: Blad funkcji accept.\n");
                            continue;
                        } else {
                            pid_t pid;

                            if ((pid = fork()) == -1) {
                                perror("Serwer: Wywolanie funkcji fork nie powiodlo sie.\n");
                                close(gniazdo1);
                                close(gniazdo);
                                return -1;
                            } else {
                                if (pid == 0) {
                                    ssize_t liczba_bajtow;

                                    free(sciezka);
                                    close(gniazdo);
                                    if ((liczba_bajtow = read(gniazdo1, &pid, sizeof(pid_t))) == -1) {
                                        perror("Serwer, proces potomny: Blad funkcji accept.\n");
                                        close(gniazdo1);
                                        return -1;
                                    }
                                    printf("Serwer, proces potomny: Przeczytalem %ld bajtow od klienta, wiadomosc od klienta : %d.\n", liczba_bajtow, pid);
                                    close(gniazdo1);
                                    return 0;
                                } else {
                                    sigignore(SIGCHLD);
                                    close(gniazdo1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient strumieniowy, unixowy dla serwera wspolbieznego.
*/

#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <stdlib.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;

int main(int argc, char *argv[]) {
    int gniazdo;
    socklen_t i;
    SockAddr_un adres;
    pid_t pid = getpid();
    int czy_czekac = 0, liczba_sekund = 0;
    char *sciezka;

    if (argc > 1) {
        czy_czekac = 1;
        liczba_sekund = strtol(argv[1], NULL, 10);
    }
    if (argc > 2) {
        sciezka = strndup(argv[2], strlen(argv[2]));
    } else {
        sciezka = strndup("/tmp/roboczy.unix_serwer", strlen("/tmp/roboczy.unix_serwer"));
    }
    if ((gniazdo = socket(PF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Klient: Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        bzero((char *) &adres, sizeof(SockAddr_un));
        adres.sun_family = AF_UNIX;
        strncpy(adres.sun_path, sciezka, strlen(sciezka));
        free(sciezka);
        i = sizeof(SockAddr_un);
        if (connect(gniazdo, (SockAddr *) &adres, i) == -1) {
            perror("Klient: Blad funkcji connect.\n");
            close(gniazdo);
            return -1;
        } else {
            if (czy_czekac) {
                printf("Klient: Oczekuje %d sekund na wyslanie wiadomosci do serwera.\n", liczba_sekund);
                sleep(liczba_sekund);
            }
            printf("Klient: Klient wysyla pid %d.\n", pid);
            if (write(gniazdo, &pid, sizeof(pid_t)) == -1) {
                perror("Klient: Blad funkcji write.\n");
                close(gniazdo);
                return -1;
            }
            close(gniazdo);
            return 0;
        }
    }
}
```
### Komunikacja z wykorzystaniem gniazd strumieniowych internetowych
```c
/*
Serwer wspolbiezny strumieniowy, internetowy.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;
int gniazdo;

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Serwer: Koniec pracy serwera.\n");
        close(gniazdo);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int gniazdo1;
    socklen_t i;
    SockAddr_in adres, adres_k;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    sigignore(SIGCHLD);
    if ((gniazdo = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Serwer: Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        i = sizeof(SockAddr_in);
        adres.sin_family = AF_INET;
        if (argc > 1) {
            adres.sin_port = htons(strtol(argv[1], NULL, 10));
        } else {
            adres.sin_port = htons(0);
        }
        adres.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(gniazdo, (SockAddr *) &adres, i) == -1) {
            perror("Serwer: Blad dowiazania gniazda.\n");
            close(gniazdo);
            return -1;
        } else {
            if (getsockname(gniazdo, (SockAddr *) &adres, &i) == -1) {
                perror("Serwer: Blad funkcji getsockname.\n");
                close(gniazdo);
                return -1;
            } else {
                printf("Serwer: Nr portu : %d\n", ntohs(adres.sin_port));
                if (listen(gniazdo, 5) == -1) {
                    perror("Serwer: Blad nasluchu gniazda.\n");
                    close(gniazdo);
                    return -1;
                } else {
                    for (;;) {
                        if ((gniazdo1 = accept(gniazdo, (SockAddr *) &adres_k, &i)) == -1) {
                            perror("Serwer: Blad funkcji accept.\n");
                            continue;
                        } else {
                            pid_t pid;

                            if ((pid = fork()) == -1) {
                                perror("Serwer: Wywolanie funkcji fork nie powiodlo sie.\n");
                                close(gniazdo1);
                                close(gniazdo);
                                return -1;
                            } else {
                                if (pid == 0) {
                                    ssize_t liczba_bajtow;

                                    close(gniazdo);
                                    printf("Serwer, proces potomny: Polaczenie od %s z portu %d.\n", inet_ntoa(adres_k.sin_addr), ntohs(adres_k.sin_port));
                                    if ((liczba_bajtow = read(gniazdo1, &pid, sizeof(pid_t))) == -1) {
                                        perror("Serwer, proces potomny: Blad funkcji read.\n");
                                        close(gniazdo1);
                                        return -1;
                                    }
                                    printf("Serwer, proces potomny: Przeczytalem %ld bajtow od klienta, wiadomosc od klienta : %d.\n", liczba_bajtow, pid);
                                    close(gniazdo1);
                                    return 0;
                                } else {
                                    sigignore(SIGCHLD);
                                    close(gniazdo1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient strumieniowy, internetowy.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

int main(int argc, char *argv[]) {
    int gniazdo;
    socklen_t i;
    SockAddr_in adres;
    int czy_czekac = 0, liczba_sekund = 0;
    pid_t pid;

    if (argc < 3) {
        printf("Klient: Bledna ilosc argumentow (adres internetowy numer portu [czas oczekiwania na wyslanie]).\n");
        return -1;
    } else {
        if (argc > 3) {
            czy_czekac = 1;
            liczba_sekund = strtol(argv[3], NULL, 10);
        }
        pid = getpid();
        if ((gniazdo = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Klient: Blad wywolania funkcji socket.\n");
            return -1;
        } else {
            struct hostent *Adres;

            i = sizeof(SockAddr_in);
            adres.sin_family = AF_INET;
            adres.sin_port = htons(strtol(argv[2], NULL, 10));
            Adres = gethostbyname(argv[1]);
            adres.sin_addr.s_addr = *(long *) (Adres->h_addr);
            if (connect(gniazdo, (SockAddr *) &adres, i) == -1) {
                perror("Klient: Blad funkcji connect.\n");
                close(gniazdo);
                return -1;
            } else {
                if (czy_czekac) {
                    printf("Klient: Oczekuje %d sekund na wyslanie wiadomosci do serwera.\n", liczba_sekund);
                    sleep(liczba_sekund);
                }
                printf("Klient: Klient wysyla pid %d.\n", pid);
                if (write(gniazdo, &pid, sizeof(pid_t)) == -1) {
                    perror("Klient: Blad funkcji write.\n");
                    close(gniazdo);
                    return -1;
                }
                close(gniazdo);
                return 0;
            }
        }
    }
}
```
#### Zadania
- Zadanie 27
- Zadanie 28
# Laboratorium 7. - 18. grudnia 2022
### Komunikacja międzyprocesowa z wykorzystaniem gniazd datagramowych.
1. Do testowania programu serwera datagramowego (UDP) można używać narzędzia netcat.
- Dla IPv4: nc -u 127.0.0.1 numer_portu.
- Dla IPv6: nc -6u ::1 numer_portu.
- Dla UNIX: nc -Uu ścieżka.
2. Do uruchomienia serwera do testowania programu klienta strumieniowego (TCP) można używać narzędzia netcat.
- Dla IPv4: nc -u -l numer_portu.
- Dla IPv6: nc -6u -l numer_portu.
- Dla UNIX: nc -Uu -l ścieżka.
#### Komunikacja z wykorzystaniem gniazd datagramowych uniksowych
```c
/*
Serwer datagramowy, unixowy, sekwencyjny.
*/

#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<fcntl.h>
#include<stdlib.h>
#include<err.h>
#include<errno.h>
#include<limits.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;
int gniazdo;
char *sciezka;

/*
1 czas
2 dzien miesiac rok
3 dzien miesiac rok dzien_tygodnia
4 dzien miesiac rok dzien_tygodnia czas
5 dzien miesiac rok godzina
*/

char *zamien_date(time_t a, int b) {
    char *wynik, *pomoc;

    pomoc = ctime(&a);
    switch (b) {
        default:
            wynik = strndup(pomoc, strlen(pomoc));
            wynik[strlen(wynik) - 1] = '\0';
            break;
        case 1 : {
            wynik = (char *) malloc(9 * sizeof(char));
            strncpy(wynik, &(pomoc[11]), 8);
            wynik[8] = '\0';
            break;
        }
        case 2 : {
            wynik = (char *) malloc(12 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = '\0';
            break;
        }
        case 3 : {
            wynik = (char *) malloc(16 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), pomoc, 3);
            wynik[15] = '\0';
            break;
        }
        case 4 : {
            wynik = (char *) malloc(25 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), pomoc, 3);
            wynik[15] = ' ';
            strncpy(&(wynik[16]), &(pomoc[11]), 8);
            wynik[24] = '\0';
            break;
        }
        case 5 : {
            wynik = (char *) malloc(21 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), &(pomoc[11]), 8);
            wynik[20] = '\0';
            break;
        }
    }
    return wynik;
}

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Serwer: Koniec pracy serwera ze sciezka %s.\n", sciezka);
        close(gniazdo);
        unlink(sciezka);
        free(sciezka);
        exit(0);
    }
}

long strtonumber(char *str, long min, long max, int base, char *msg) {
    long result = strtol(str, NULL, base);

    if ((result == 0 && errno == ERANGE) || min < 0 || result > max) {
        warn("%s: %s.\n", msg, str);
        result = 0;
    }
    return result;
}

unsigned long strtounumber(char *str, unsigned long min, unsigned long max, int base, char *msg) {
    unsigned long result = strtoul(str, NULL, base);

    if ((result == 0 && errno == ERANGE) || min < 0 || result > max) {
        warn("%s: %s.\n", msg, str);
        result = 0;
    }
    return result;
}

int main(int argc, char *argv[]) {
    socklen_t i;
    SockAddr_un adres, adres_kli;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    if (argc > 1) {
        sciezka = strndup(argv[1], strlen(argv[1]));
    } else {
        sciezka = strndup("/tmp/roboczy.unix_serwer", strlen("/tmp/roboczy.unix_serwer"));
    }
    if ((gniazdo = socket(PF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("Serwer: Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        bzero((char *) &adres, sizeof(SockAddr_un));
        adres.sun_family = AF_UNIX;
        strncpy(adres.sun_path, sciezka, strlen(sciezka));
        i = sizeof(SockAddr_un);
        if (bind(gniazdo, (SockAddr *) &adres, i) == -1) {
            perror("Serwer: Blad dowiazania gniazda.\n");
            close(gniazdo);
            return -1;
        } else {
            char *t, *t1, *t3, *bufor;
            int plik;
            pid_t pid;
            ssize_t liczba_bajtow;
            unsigned int liczba_sekund;
            unsigned short pomoc1;

            plik = open("./historia_polaczen.txt", O_WRONLY | O_CREAT, 0600);
            t = (char *) malloc(100 * sizeof(char));
            bufor = (char *) malloc(100 * sizeof(char));
            printf("Serwer: Sciezka : %s.\n", adres.sun_path);
            for (;;) {
                bzero(bufor, 100);
                if ((liczba_bajtow = recvfrom(gniazdo, bufor, 100, 0, (SockAddr *) &adres_kli, &i)) == -1) {
                    perror("Serwer: Blad funkcji recfrom.\n");
                    continue;
                } else {
                    liczba_sekund = strtounumber(bufor, 0, INT_MAX, 10, "Serwer: Konwersja argumentu niemozliwa, przyjmuje wartosc 0");
                    t1 = strchr(bufor, ':') + 1;
                    pid = (pid_t) strtonumber(t1, 0, INT_MAX, 10, "Serwer: Konwersja argumentu niemozliwa, przyjmuje wartosc 0");
                    bzero(t, 100);
                    sprintf(t, "Kto: %d. Kiedy: %s. Gniazdo: %s Liczba bajtow: %ld \n", pid, zamien_date(time(NULL), 5), adres_kli.sun_path, liczba_bajtow);
                    if (write(plik, t, strlen(t)) == -1) {
                        perror("Serwer: Blad funkcji write.\n");
                    }
                    printf("Serwer: Otrzymalem od klienta: %s, liczba bajtow %ld.\n", bufor, liczba_bajtow);
                    pomoc1 = strtounumber(strchr(t1, ' ') + 1, 0, USHRT_MAX, 10, "Serwer: Konwersja argumentu niemozliwa, przyjmuje wartosc 0");
                    if (liczba_sekund) {
                        printf("Serwer: Oczekuje %d sekund na wyslanie wiadomosci do klienta.\n", liczba_sekund);
                        sleep(liczba_sekund);
                    }
                    t3 = zamien_date(time(NULL), pomoc1);
                    printf("Serwer: Wysylam %s do %s dla %d.\n", t3, adres_kli.sun_path, pomoc1);
                    if (sendto(gniazdo, t3, strlen(t) + 1, 0, (SockAddr *) &adres_kli, i) == -1) {
                        perror("Serwer: Blad funkcji sendto.\n");
                        free(t3);
                        continue;
                    }
                    free(t3);
                }
            }
        }
    }
}
```
```c
/*
Klient datagramowy, unixowy, sekwencyjny.
*/

#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<limits.h>
#include<err.h>
#include<errno.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;

unsigned long strtounumber(char *str, unsigned long min, unsigned long max, int base, char *msg) {
    unsigned long result = strtoul(str, NULL, base);

    if ((result == 0 && errno == ERANGE) || min < 0 || result > max) {
        warn("%s: %s.\n", msg, str);
        result = 0;
    }
    return result;
}

int main(int argc, char *argv[]) {
    int gniazdo;
    socklen_t i1, i2;
    SockAddr_un adres_ser, adres_kli;
    pid_t pid = getpid();
    char *sciezka;
    unsigned short pomoc;

    if (argc == 1) {
        pomoc = 5;
    } else {
        pomoc = strtounumber(argv[1], 0, USHRT_MAX, 10, "Klient: Konwersja argumentu niemozliwa, przyjmuje wartosc 0");
    }
    if (argc > 3) {
        sciezka = strndup(argv[3], strlen(argv[3]));
    } else {
        sciezka = strndup("/tmp/roboczy.unix_serwer", strlen("/tmp/roboczy.unix_serwer"));
    }
    if ((gniazdo = socket(PF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("Klient: Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        char *s;

        bzero((char *) &adres_ser, sizeof(SockAddr_un));
        s = (char *) malloc(25 * sizeof(char));
        if (argc <= 2) {
            sprintf(s, ":%d %d", pid, pomoc);
        } else {
            sprintf(s, "%s:%d %s", argv[2], pid, argv[1]);
        }
        adres_ser.sun_family = AF_UNIX;
        strncpy(adres_ser.sun_path, sciezka, strlen(sciezka));
        i1 = sizeof(SockAddr_un);
        bzero((char *) &adres_kli, sizeof(SockAddr_un));
        adres_kli.sun_family = AF_UNIX;
        sprintf(adres_kli.sun_path, "%s%d", sciezka, pid);
        free(sciezka);
        i2 = sizeof(SockAddr_un);
        if (bind(gniazdo, (SockAddr *) &adres_kli, i2) == -1) {
            perror("Klient: Blad dowiazania gniazda.\n");
            close(gniazdo);
            return -1;
        } else {
            printf("Klient: Wysylam do serwera %s.\n", s);
            if (sendto(gniazdo, s, strlen(s) + 1, 0, (SockAddr *) &adres_ser, i1) == -1) {
                perror("Klient: Blad funkcji sendto (1).\n");
                close(gniazdo);
                unlink(adres_kli.sun_path);
                return -1;
            } else {
                ssize_t liczba_bajtow;

                bzero(s, 25);
                if ((liczba_bajtow = recvfrom(gniazdo, s, 25, 0, (SockAddr *) &adres_ser, &i1)) == -1) {
                    perror("Klient: Blad funkcji recfrom.\n");
                    close(gniazdo);
                    unlink(adres_kli.sun_path);
                    free(s);
                    return -1;
                } else {
                    if (pomoc == 1) {
                        printf("Klient: Odebralem %ld bajtow, aktualny czas: %s.\n", liczba_bajtow, s);
                    } else {
                        printf("Klient: Odebralem %ld bajtow, aktualna data: %s.\n", liczba_bajtow, s);
                    }
                    close(gniazdo);
                    unlink(adres_kli.sun_path);
                    free(s);
                }
            }
        }
    }
}
```
```c
/*
Serwer datagramowy, unixowy, wspolbiezny.
*/

#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<fcntl.h>
#include<stdlib.h>
#include<err.h>
#include<limits.h>
#include<errno.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;
int gniazdo;
char *sciezka;

/*
1 czas
2 dzien miesiac rok
3 dzien miesiac rok dzien_tygodnia
4 dzien miesiac rok dzien_tygodnia czas
5 dzien miesiac rok godzina
*/

char *zamien_date(time_t a, int b) {
    char *wynik, *pomoc;

    pomoc = ctime(&a);
    switch (b) {
        default:
            wynik = strndup(pomoc, strlen(pomoc));
            wynik[strlen(wynik) - 1] = '\0';
            break;
        case 1 : {
            wynik = (char *) malloc(9 * sizeof(char));
            strncpy(wynik, &(pomoc[11]), 8);
            wynik[8] = '\0';
            break;
        }
        case 2 : {
            wynik = (char *) malloc(12 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = '\0';
            break;
        }
        case 3 : {
            wynik = (char *) malloc(16 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), pomoc, 3);
            wynik[15] = '\0';
            break;
        }
        case 4 : {
            wynik = (char *) malloc(25 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), pomoc, 3);
            wynik[15] = ' ';
            strncpy(&(wynik[16]), &(pomoc[11]), 8);
            wynik[24] = '\0';
            break;
        }
        case 5 : {
            wynik = (char *) malloc(21 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), &(pomoc[11]), 8);
            wynik[20] = '\0';
            break;
        }
    }
    return wynik;
}

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Koniec pracy serwera ze sciezka %s.\n", sciezka);
        close(gniazdo);
        unlink(sciezka);
        free(sciezka);
        exit(0);
    }
}

long strtonumber(char *str, long min, long max, int base, char *msg) {
    long result = strtol(str, NULL, base);

    if ((result == 0 && errno == ERANGE) || min < 0 || result > max) {
        warn("%s: %s.\n", msg, str);
        result = 0;
    }
    return result;
}

unsigned long strtounumber(char *str, unsigned long min, unsigned long max, int base, char *msg) {
    unsigned long result = strtoul(str, NULL, base);

    if ((result == 0 && errno == ERANGE) || min < 0 || result > max) {
        warn("%s: %s.\n", msg, str);
        result = 0;
    }
    return result;
}

int main(int argc, char *argv[]) {
    int gniazdo1;
    socklen_t i, i2 = sizeof(SockAddr_un);
    SockAddr_un adres, adres_kli, adres_serw;

    sigignore(SIGCHLD);
    signal(SIGTERM, al);
    signal(SIGINT, al);
    if (argc > 1) {
        sciezka = strndup(argv[1], strlen(argv[1]));
    } else {
        sciezka = strndup("/tmp/roboczy.unix_serwer", strlen("/tmp/roboczy.unix_serwer"));
    }
    if ((gniazdo = socket(PF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("Serwer: Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        bzero((char *) &adres, sizeof(SockAddr_un));
        adres.sun_family = AF_UNIX;
        strncpy(adres.sun_path, sciezka, strlen(sciezka));
        i = sizeof(SockAddr_un);
        if (bind(gniazdo, (SockAddr *) &adres, i) == -1) {
            perror("Serwer: Blad dowiazania gniazda.\n");
            close(gniazdo);
            return -1;
        } else {
            char *t, *t1, *t3, *bufor;
            int plik;
            unsigned int liczba_sekund;
            ssize_t liczba_bajtow;
            double argument;
            pid_t pid;
            unsigned short pomoc1;

            plik = open("./historia_polaczen.txt", O_WRONLY | O_CREAT | O_APPEND, 0600);
            t = (char *) malloc(100 * sizeof(char));
            bufor = (char *) malloc(100 * sizeof(char));
            printf("Serwer: Sciezka : %s.\n", adres.sun_path);
            for (;;) {
                bzero(bufor, 100);
                if ((liczba_bajtow = recvfrom(gniazdo, bufor, 100, 0, (SockAddr *) &adres_kli, &i2)) == -1) {
                    perror("Serwer: Blad funkcji recfrom.\n");
                    continue;
                } else {
                    if (fork() == 0) {
                        close(gniazdo);
                        free(sciezka);
                        if ((gniazdo1 = socket(PF_UNIX, SOCK_DGRAM, 0)) == -1) {
                            perror("Serwer, proces potomny: Blad wywolania funkcji socket.\n");
                            free(bufor);
                            free(t);
                            return -1;
                        } else {
                            bzero((char *) &adres_serw, sizeof(SockAddr_un));
                            adres_serw.sun_family = AF_UNIX;
                            sprintf(adres_serw.sun_path, "%s_serwer", adres_kli.sun_path);
                            i = sizeof(SockAddr_un);
                            if (bind(gniazdo1, (SockAddr *) &adres_serw, i) == -1) {
                                perror("Serwer, proces potomny: Blad dowiazania gniazda.\n");
                                close(gniazdo1);
                                free(bufor);
                                free(t);
                                return -1;
                            } else {
                                liczba_sekund = strtounumber(bufor, 0, INT_MAX, 10, "Serwer, proces potomny: Konwersja argumentu niemozliwa, przyjmuje wartosc 0");
                                t1 = strchr(bufor, ':') + 1;
                                pid = (pid_t) strtonumber(t1, 0, INT_MAX, 10, "Serwer, proces potomny: Konwersja argumentu niemozliwa, przyjmuje wartosc 0");
                                bzero(t, 100);
                                sprintf(t, "Kto: %d. Kiedy: %s. Gniazdo: %s Liczba bajtow: %ld \n", pid, zamien_date(time(NULL), 5), adres_kli.sun_path, liczba_bajtow);
                                if (write(plik, t, strlen(t)) == -1) {
                                    perror("Serwer, proces potomny: Blad funkcji write.\n");
                                }
                                printf("Serwer, proces potomny: Otrzymalem od klienta: %s, liczba bajtow %ld.\n", bufor, liczba_bajtow);
                                pomoc1 = strtounumber(strchr(t1, ' ') + 1, 0, USHRT_MAX, 10, "Serwer, proces potomny: Konwersja argumentu niemozliwa, przyjmuje wartosc 0");
                                if (liczba_sekund) {
                                    printf("Serwer, proces potomny: Oczekuje %ud sekund na wyslanie wiadomosci do klienta.\n", liczba_sekund);
                                    sleep(liczba_sekund);
                                }
                                t3 = zamien_date(time(NULL), pomoc1);
                                printf("Serwer, proces potomny: Wysylam %s do %s dla %d.\n", t3, adres_kli.sun_path, pomoc1);
                                if (sendto(gniazdo1, t3, strlen(t) + 1, 0, (SockAddr *) &adres_kli, i2) == -1) {
                                    perror("Serwer, proces potomny: Blad funkcji sendto.\n");
                                    close(gniazdo1);
                                    unlink(adres_serw.sun_path);
                                    free(t3);
                                    free(bufor);
                                    free(t);
                                    return -1;
                                }
                                free(t3);
                                bzero(bufor, 100);
                                if ((liczba_bajtow = recvfrom(gniazdo1, bufor, 100, 0, (SockAddr *) &adres_kli, &i2)) == -1) {
                                    perror("Serwer, proces potomny: Blad funkcji recfrom.\n");
                                    close(gniazdo1);
                                    unlink(adres_serw.sun_path);
                                    free(bufor);
                                    free(t);
                                    return -1;
                                }
                                printf("Serwer, proces potomny: Otrzymalem od klienta liczbe: %s, liczba bajtow %ld.\n", bufor, liczba_bajtow);
                                argument = strtod(bufor, NULL);
                                argument *= argument;
                                sprintf(bufor, "%lf", argument);
                                printf("Serwer, proces potomny: Wysylam %s do %s.\n", bufor, adres_kli.sun_path);
                                if (sendto(gniazdo1, bufor, strlen(bufor) + 1, 0, (SockAddr *) &adres_kli, i2) == -1) {
                                    perror("Serwer, proces potomny: Blad funkcji sendto.\n");
                                    close(gniazdo1);
                                    unlink(adres_serw.sun_path);
                                    free(bufor);
                                    free(t);
                                    return -1;
                                }
                                close(gniazdo1);
                                unlink(adres_serw.sun_path);
                                free(bufor);
                                free(t);
                                return 0;
                            }
                        }
                    } else {
                        bzero((char *) &adres_kli, sizeof(SockAddr_un));
                        sigignore(SIGCHLD);
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient datagramowy, unixowy, wspolbiezny.
*/

#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<limits.h>
#include<err.h>
#include<errno.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_un SockAddr_un;

unsigned long strtounumber(char *str, unsigned long min, unsigned long max, int base, char* msg) {
    unsigned long result = strtoul(str, NULL, base);

    if ((result == 0 && errno == ERANGE) || min < 0 || result > max) {
        warn("%s: %s.\n", msg, str);
        result = 0;
    }
    return result;
}

int main(int argc, char *argv[]) {
    int gniazdo;
    socklen_t i1, i2, i3 = sizeof(SockAddr_un);
    SockAddr_un adres_ser, adres_ser_kli, adres_kli;
    pid_t pid = getpid();
    char *sciezka;
    unsigned short pomoc;

    if (argc == 1) {
        pomoc = 5;
    } else {
        pomoc = strtounumber(argv[1], 0, USHRT_MAX, 10, "Klient: Konwersja argumentu niemozliwa, przyjmuje wartosc 0");
    }
    if (argc > 3) {
        sciezka = strndup(argv[3], strlen(argv[3]));
    } else {
        sciezka = strndup("/tmp/roboczy.unix_serwer", strlen("/tmp/roboczy.unix_serwer"));
    }
    if ((gniazdo = socket(PF_UNIX, SOCK_DGRAM, 0)) == -1) {
        perror("Klient: Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        char *s;

        bzero((char *) &adres_ser, sizeof(SockAddr_un));
        s = (char *) malloc(25 * sizeof(char));
        if (argc <= 2) {
            sprintf(s, ":%d %d", pid, pomoc);
        } else {
            sprintf(s, "%s:%d %s", argv[2], pid, argv[1]);
        }
        adres_ser.sun_family = AF_UNIX;
        strncpy(adres_ser.sun_path, sciezka, strlen(sciezka));
        i1 = sizeof(SockAddr_un);
        bzero((char *) &adres_kli, sizeof(SockAddr_un));
        adres_kli.sun_family = AF_UNIX;
        sprintf(adres_kli.sun_path, "%s%d", sciezka, pid);
        i2 = sizeof(SockAddr_un);
        if (bind(gniazdo, (SockAddr *) &adres_kli, i2) == -1) {
            perror("Klient: Blad dowiazania gniazda.\n");
            close(gniazdo);
            return -1;
        } else {
            printf("Klient: Wysylam do serwera %s.\n", s);
            if (sendto(gniazdo, s, strlen(s), 0, (SockAddr *) &adres_ser, i1) == -1) {
                perror("Klient: Klient: Blad funkcji sendto.\n");
                close(gniazdo);
                unlink(adres_kli.sun_path);
                return -1;
            } else {
                ssize_t liczba_bajtow;

                bzero(s, 25);
                bzero((char *) &adres_ser_kli, sizeof(SockAddr_un));
                if ((liczba_bajtow = recvfrom(gniazdo, s, 25, 0, (SockAddr *) &adres_ser_kli, &i3)) == -1) {
                    perror("Klient: Blad funkcji recfrom.\n");
                    close(gniazdo);
                    unlink(adres_kli.sun_path);
                    free(s);
                    return -1;
                }
                if (pomoc == 1) {
                    printf("Klient: Odebralem %ld bajtow, aktualny czas: %s.\n", liczba_bajtow, s);
                } else {
                    printf("Klient: Odebralem %ld bajtow, aktualny data: %s.\n", liczba_bajtow, s);
                }
                printf("Klient: Podaj liczbe rzeczywista: ");
                bzero(s, 25);
                scanf("%s", s);
                if (sendto(gniazdo, s, strlen(s), 0, (SockAddr *) &adres_ser_kli, i3) == -1) {
                    perror("Klient: Blad funkcji sendto.\n");
                    close(gniazdo);
                    unlink(adres_kli.sun_path);
                    free(s);
                    return -1;
                }
                if ((liczba_bajtow = recvfrom(gniazdo, s, 25, 0, (SockAddr *) &adres_ser_kli, &i3)) == -1) {
                    perror("Klient: Blad funkcji recfrom.\n");
                    close(gniazdo);
                    unlink(adres_kli.sun_path);
                    free(s);
                    return -1;
                }
                printf("Klient: Klient: Odebralem %ld bajtow, wynik potegowania: %s.\n", liczba_bajtow, s);
            }
            free(s);
            close(gniazdo);
            unlink(adres_kli.sun_path);
            free(sciezka);
        }
    }
}
```
#### Komunikacja z wykorzystaniem gniazd datagramowych internetowych
```c
/*
Serwer datagramowy, internetowy, sekwencyjny.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<fcntl.h>
#include<stdlib.h>
#include<signal.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;
int gniazdo;

/*
1 czas
2 dzien miesiac rok
3 dzien miesiac rok dzien_tygodnia
4 dzien miesiac rok dzien_tygodnia czas
5 dzien miesiac rok godzina
*/

char *zamien_date(time_t *a, int b) {
    char *wynik, *pomoc;

    pomoc = ctime(a);
    switch (b) {
        default:
            wynik = strndup(pomoc, strlen(pomoc));
            wynik[strlen(wynik) - 1] = '\0';
            break;
        case 1 : {
            wynik = (char *) malloc(9 * sizeof(char));
            strncpy(wynik, &(pomoc[11]), 8);
            wynik[8] = '\0';
            break;
        }
        case 2 : {
            wynik = (char *) malloc(12 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = '\0';
            break;
        }
        case 3 : {
            wynik = (char *) malloc(16 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), pomoc, 3);
            wynik[15] = '\0';
            break;
        }
        case 4 : {
            wynik = (char *) malloc(25 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), pomoc, 3);
            wynik[15] = ' ';
            strncpy(&(wynik[16]), &(pomoc[11]), 8);
            wynik[24] = '\0';
            break;
        }
        case 5 : {
            wynik = (char *) malloc(21 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), &(pomoc[11]), 8);
            wynik[20] = '\0';
            break;
        }
    }
    return wynik;
}

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Koniec pracy serwera.\n");
        close(gniazdo);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    socklen_t i;
    SockAddr_in adres, adres_kli;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    if ((gniazdo = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        bzero((char *) &adres, sizeof(SockAddr_in));
        adres.sin_family = AF_INET;
        if (argc > 1) {
            adres.sin_port = htons(strtol(argv[1], NULL, 10));
        } else {
            adres.sin_port = htons(0);
        }
        adres.sin_addr.s_addr = htonl(INADDR_ANY);
        i = sizeof(SockAddr_in);
        if (bind(gniazdo, (SockAddr *) &adres, i) < 0) {
            perror("Blad dowiazania gniazda.\n");
            close(gniazdo);
            return -1;
        } else {
            if (getsockname(gniazdo, (SockAddr *) &adres, &i) != 0) {
                perror("Blad funkcji getsockname.\n");
                close(gniazdo);
                return -1;
            } else {
                char *t, *t1;
                int pomoc, pomoc1, plik;
                time_t czas;

                printf("Nr portu: %d\n", ntohs(adres.sin_port));
                plik = open("./historia_polaczen.txt", O_WRONLY | O_CREAT, 0600);
                t = (char *) malloc(100 * sizeof(char));
                t1 = (char *) malloc(10 * sizeof(char));
                sprintf(t, " Adres Port Pid Kiedy Opcja\n");
                write(plik, t, strlen(t));
                sprintf(t, "------------------------------------------------------------\n");
                write(plik, t, strlen(t));
                for (;;) {
                    if (recvfrom(gniazdo, t1, 10, 0, (SockAddr *) &adres_kli, &i) < 0) {
                        perror("Blad funkcji recfrom.\n");
                        continue;
                    } else {
                        pomoc = strtol(t1, NULL, 10);
                        czas = time(NULL);
                        pomoc1 = strtol(&(t1[6]), NULL, 10);
                        sprintf(t, "%15s %5d %5d %s %d\n", inet_ntoa(adres_kli.sin_addr), ntohs(adres_kli.sin_port),
                                pomoc, zamien_date(&czas, 5), pomoc1);
                        write(plik, t, strlen(t));
                        sprintf(t, "%s", zamien_date(&czas, pomoc1));
                        if (sendto(gniazdo, t, 25, 0, (SockAddr *) &adres_kli, i) < 0) {
                            perror("Blad funkcji sendto.\n");
                            continue;
                        }
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient datagramowy, internetowy, sekwencyjny.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

int main(int argc, char *argv[]) {
    int gniazdo;
    socklen_t i1;
    SockAddr_in adres_serv, adres_kli;
    pid_t pid;

    if (argc < 3) {
        printf("Bledna ilosc argumentow. (adres internetowy, numer portu) \n");
        return -1;
    } else {
        pid = getpid();
        if ((gniazdo = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Blad wywolania funkcji socket.\n");
            return -1;
        } else {
            char *s;
            struct hostent *Adres;

            bzero((char *) &adres_serv, sizeof(SockAddr_in));
            s = (char *) malloc(10 * sizeof(char));
            if (argc == 3) {
                sprintf(s, "%5d %d", pid, 5);
            } else {
                sprintf(s, "%5d %s", pid, argv[3]);
            }
            adres_serv.sin_family = AF_INET;
            adres_serv.sin_port = htons(strtol(argv[2], NULL, 10));
            Adres = gethostbyname(argv[1]);
            adres_serv.sin_addr.s_addr = *(long *) (Adres->h_addr);
            i1 = sizeof(SockAddr_in);
            bzero((char *) &adres_kli, sizeof(SockAddr_in));
            adres_kli.sin_family = AF_INET;
            adres_kli.sin_addr.s_addr = htonl(INADDR_ANY);
            adres_kli.sin_port = htons(0);
            if (bind(gniazdo, (SockAddr *) &adres_kli, i1) < 0) {
                perror("Blad dowiazania gniazda.\n");
                close(gniazdo);
                return -1;
            } else {
                int pomoc;

                if (argc == 3) pomoc = 2;
                else pomoc = strtol(argv[3], NULL, 10);
                sendto(gniazdo, s, strlen(s), 0, (SockAddr *) &adres_serv, i1);
                free(s);
                s = (char *) malloc(25 * sizeof(char));
                recvfrom(gniazdo, s, 25, 0, (SockAddr *) &adres_serv, &i1);
                if (pomoc == 1) {
                    printf("Aktualny czas: %s.\n", s);
                } else {
                    printf("Aktualna data: %s.\n", s);
                }
                free(s);
            }
            close(gniazdo);
        }
    }
}
```
```c
/*
Serwer datagramowy, internetowy, wspolbiezny.
*/

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<fcntl.h>
#include<stdlib.h>
#include<signal.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;
int gniazdo;

/*
1 czas
2 dzien miesiac rok
3 dzien miesiac rok dzien_tygodnia
4 dzien miesiac rok dzien_tygodnia czas
5 dzien miesiac rok godzina
*/

char *zamien_date(time_t *a, int b) {
    char *wynik, *pomoc;

    pomoc = ctime(a);
    switch (b) {
        default:
            wynik = strndup(pomoc, strlen(pomoc));
            wynik[strlen(wynik) - 1] = '\0';
            break;
        case 1 : {
            wynik = (char *) malloc(9 * sizeof(char));
            strncpy(wynik, &(pomoc[11]), 8);
            wynik[8] = '\0';
            break;
        }
        case 2 : {
            wynik = (char *) malloc(12 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = '\0';
            break;
        }
        case 3 : {
            wynik = (char *) malloc(16 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), pomoc, 3);
            wynik[15] = '\0';
            break;
        }
        case 4 : {
            wynik = (char *) malloc(25 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), pomoc, 3);
            wynik[15] = ' ';
            strncpy(&(wynik[16]), &(pomoc[11]), 8);
            wynik[24] = '\0';
            break;
        }
        case 5 : {
            wynik = (char *) malloc(21 * sizeof(char));
            strncpy(wynik, &(pomoc[8]), 2);
            wynik[2] = ' ';
            strncpy(&(wynik[3]), &(pomoc[4]), 3);
            wynik[6] = ' ';
            strncpy(&(wynik[7]), &(pomoc[20]), 4);
            wynik[11] = ' ';
            strncpy(&(wynik[12]), &(pomoc[11]), 8);
            wynik[20] = '\0';
            break;
        }
    }
    return wynik;
}

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Koniec pracy serwera.\n");
        close(gniazdo);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int gniazdo1;
    socklen_t i;
    SockAddr_in adres, adres_kli, adres_serw;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    sigignore(SIGCHLD);
    if ((gniazdo = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Serwer: Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        bzero((char *) &adres, sizeof(SockAddr_in));
        adres.sin_family = AF_INET;
        if (argc > 1) {
            adres.sin_port = htons(strtol(argv[1], NULL, 10));
        } else {
            adres.sin_port = htons(0);
        }
        adres.sin_addr.s_addr = htonl(INADDR_ANY);
        i = sizeof(SockAddr_in);
        if (bind(gniazdo, (SockAddr *) &adres, i) < 0) {
            perror("Serwer: Blad dowiazania gniazda.\n");
            close(gniazdo);
            return -1;
        } else {
            if (getsockname(gniazdo, (SockAddr *) &adres, &i) != 0) {
                perror("Serwer: Blad funkcji getsockname.\n");
                close(gniazdo);
                return -1;
            } else {
                char *t, *t1;
                int pomoc, pomoc1, plik;
                time_t czas;
                double argument;

                printf("Nr portu: %d\n", ntohs(adres.sin_port));
                plik = open("./historia_polaczen.txt", O_WRONLY | O_CREAT, 0600);
                t = (char *) malloc(100 * sizeof(char));
                t1 = (char *) malloc(10 * sizeof(char));
                sprintf(t, " Adres Port Pid Kiedy Opcja\n");
                write(plik, t, strlen(t));
                sprintf(t, "------------------------------------------------------------\n");
                write(plik, t, strlen(t));
                for (;;) {
                    if (recvfrom(gniazdo, t1, 10, 0, (SockAddr *) &adres_kli, &i) < 0) {
                        perror("Serwer: Blad funkcji recfrom.\n");
                        continue;
                    } else {
                        if (fork() == 0) {
                            close(gniazdo);
                            pomoc = strtol(t1, NULL, 10);
                            czas = time(NULL);
                            pomoc1 = strtol(&(t1[6]), NULL, 10);
                            sprintf(t, "%15s %5d %5d %s %d\n", inet_ntoa(adres_kli.sin_addr), ntohs(adres_kli.sin_port),
                                    pomoc, zamien_date(&czas, 5), pomoc1);
                            write(plik, t, strlen(t));
                            sprintf(t, "%s", zamien_date(&czas, pomoc1));
                            if ((gniazdo1 = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
                                perror("Serwer, proces potomny: Blad wywolania funkcji socket.\n");
                                return -1;
                            } else {
                                bzero((char *) &adres_serw, sizeof(SockAddr_in));
                                adres_serw.sin_family = AF_INET;
                                adres_serw.sin_addr.s_addr = htonl(INADDR_ANY);
                                adres_serw.sin_port = htons(0);
                                i = sizeof(SockAddr_in);
                                if (bind(gniazdo1, (SockAddr *) &adres_serw, i) < 0) {
                                    perror("Serwer, proces potomny: Blad dowiazania gniazda.\n");
                                    close(gniazdo1);
                                    return -1;
                                } else {
                                    if (getsockname(gniazdo1, (SockAddr *) &adres_serw, &i) != 0) {
                                        perror("Serwer, proces potomny: Blad funkcji getsockname.\n");
                                        close(gniazdo1);
                                        return -1;
                                    } else {
                                        if (sendto(gniazdo, &(adres_serw.sin_port), sizeof(in_port_t), 0,
                                                   (SockAddr *) &adres_kli, i) < 0) {
                                            perror("Serwer, proces potomny: Blad funkcji sendto #1.\n");
                                            close(gniazdo1);
                                            return -1;
                                        } else {
                                            printf("Serwer: wysylam %s.\n", t);
                                            if (sendto(gniazdo1, t, 25, 0, (SockAddr *) &adres_kli, i) < 0) {
                                                perror("Serwer, proces potomny: Blad funkcji sendto #2.\n");
                                                close(gniazdo1);
                                                return -1;
                                            }
                                            if (recvfrom(gniazdo1, &argument, sizeof(double), 0,
                                                         (SockAddr *) &adres_kli, &i) < 0) {
                                                perror("Klient: Blad funkcji recfrom.\n");
                                                close(gniazdo1);
                                                return -1;
                                            }
                                            printf("Otrzymalem liczbe: %lf.\n", argument);
                                            argument *= argument;
                                            if (sendto(gniazdo1, &argument, sizeof(double), 0, (SockAddr *) &adres_kli,
                                                       i) < 0) {
                                                perror("Klient: Blad funkcji sendto.\n");
                                                close(gniazdo1);
                                                return -1;
                                            }
                                            close(gniazdo1);
                                            return 0;
                                        }
                                    }
                                }
                            }
                        } else {
                            bzero((char *) &adres_kli, sizeof(SockAddr_in));
                            sigignore(SIGCHLD);
                        }
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient datagramowy, internetowy, wspolbiezny.
*/

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

int main(int argc, char *argv[]) {
    int gniazdo;
    socklen_t i1;
    in_port_t port;
    SockAddr_in adres_serw, adres_kli;
    pid_t pid;

    if (argc < 3) {
        printf("Bledna ilosc argumentow. (adres internetowy, numer portu) \n");
        return -1;
    } else {
        pid = getpid();
        if ((gniazdo = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("Blad wywolania funkcji socket.\n");
            return -1;
        } else {
            char *s;
            struct hostent *Adres;
            bzero((char *) &adres_serw, sizeof(SockAddr_in));
            s = (char *) malloc(10 * sizeof(char));
            if (argc == 3) {
                sprintf(s, "%5d %d", pid, 5);
            } else {
                sprintf(s, "%5d %s", pid, argv[3]);
            }
            adres_serw.sin_family = AF_INET;
            adres_serw.sin_port = htons(strtol(argv[2], NULL, 10));
            Adres = gethostbyname(argv[1]);
            adres_serw.sin_addr.s_addr = *(long *) (Adres->h_addr);
            i1 = sizeof(SockAddr_in);
            bzero((char *) &adres_kli, sizeof(SockAddr_in));
            adres_kli.sin_family = AF_INET;
            adres_kli.sin_addr.s_addr = htonl(INADDR_ANY);
            adres_kli.sin_port = htons(0);
            if (bind(gniazdo, (SockAddr *) &adres_kli, i1) < 0) {
                perror("Blad dowiazania gniazda.\n");
                close(gniazdo);
                return -1;
            } else {
                int pomoc;
                double argument;

                if (argc == 3) {
                    pomoc = 2;
                } else {
                    pomoc = strtol(argv[3], NULL, 10);
                }
                if (sendto(gniazdo, s, strlen(s), 0, (SockAddr *) &adres_serw, i1) < 0) {
                    perror("Blad wywolania funkcji sendto.\n");
                    close(gniazdo);
                    return -1;
                } else {
                    free(s);
                    if (recvfrom(gniazdo, &port, sizeof(in_port_t), 0, (SockAddr *) &adres_serw, &i1) < 0) {
                        perror("Blad wywolania funkcji recvfrom.\n");
                        close(gniazdo);
                        return -1;
                    } else {
                        printf("Numer portu serwera %d.\n", ntohs(port));
                        adres_serw.sin_port = port;
                        s = (char *) malloc(25 * sizeof(char));
                        if (recvfrom(gniazdo, s, 25, 0, (SockAddr *) &adres_serw, &i1) < 0) {
                            perror("Blad wywolania funkcji recvfrom.\n");
                            close(gniazdo);
                            return -1;
                        } else {
                            if (pomoc == 1) {
                                printf("Aktualny czas: %s.\n", s);
                            } else {
                                printf("Aktualna data: %s.\n", s);
                            }
                            printf("Podaj liczbe rzeczywista: ");
                            scanf("%lf", &argument);
                            if (sendto(gniazdo, &argument, sizeof(double), 0, (SockAddr *) &adres_serw, i1) < 0) {
                                perror("Klient: Blad funkcji sendto.\n");
                                close(gniazdo);
                                return -1;
                            }
                            if (recvfrom(gniazdo, &argument, sizeof(double), 0, (SockAddr *) &adres_serw, &i1) < 0) {
                                perror("Klient: Blad funkcji recfrom.\n");
                                close(gniazdo);
                                return -1;
                            }
                            printf("Wynik potegowania: %lf.\n", argument);
                        }
                        free(s);
                    }
                }
            }
            close(gniazdo);
        }
    }
}
```
#### Zadania
- Zadanie 29
- Zadanie 30
- Zadanie 31
# Laboratorium 8. - 14. stycznia 2023
### Dane wysoko priorytetowe, mechanizm select, szyfrowana transmisja danych poprzez gniazda.
#### Szyfrowana transmisja danych.
1. Wygenerowanie certyfikatu SSL/TLS.
- Wygenerowanie klucza RSA z szyfrowaniem AES256: openssl genrsa -aes256 -out key.key 4096.
- Zdjęcie hasła z klucza: openssl rsa -in key.key -out key.pem.
- Wygenerowanie niepodpisanego certyfikatu dla urzędu certyfikującego: openssl req -new -key key.pem -out cerreq.csr. Tak wygenerowany niepodpisany certyfikat wysyłamy do podpisania urzędowi certyfikującemu.
- Wygenerowanie mało wiarygodnego, ale wystarczającego do testów podczas laboratorium, samo-podpisanego certyfikatu: openssl req -new -x509 -days 3650 -key key.pem -out cert.pem.
2. Kompilacja programu używającego SSL/TLS: gcc programTLS.c -o programTLS -lssl -lcrypto.
3. Uruchomienie testowego serwera wykorzystującego szyfrowaną transmisję danych w celu sprawdzenia poprawności pracy klienta (w tym przypadku pracującego pod kontrolą TLS 1.3): openssl s_server -cert ./cert.pem -key ./key.pem -tls1_3.
4. Uruchomienie testowego klienta wykorzystującego szyfrowaną transmisję danych w celu sprawdzenia poprawności pracy serwera (w tym przypadku pracującego pod kontrolą TLS 1.3): openssl s_client -connect localhost:numer_portu -tls1_3.
#### Dane wysoko priorytetowe
```c
/*
Serwer strumieniowy, internetowy z danymi wysoko priorytetowymi.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;
int gniazdo, gniazdo1;

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Koniec pracy serwera.\n");
        close(gniazdo);
        exit(0);
    }
}

void sig_urg(int signo) {
    size_t n;
    char buf[100];
    printf("Przechwycony sygnal SIGURG.\n");
    n = recv(gniazdo1, buf, sizeof(buf) - 1, MSG_OOB);
    buf[n] = 0;
    printf("Przecztalem %lu OOB bajtow: %s.\n", n, buf);
}

int main(int argc, char *argv[]) {
    socklen_t i;
    SockAddr_in adres, adres_k;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    sigignore(SIGCHLD);
    if ((gniazdo = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        i = sizeof(SockAddr_in);
        adres.sin_family = AF_INET;
        if (argc > 1) {
            adres.sin_port = htons(strtol(argv[1], NULL, 10));
        } else {
            adres.sin_port = htons(0);
        }
        adres.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(gniazdo, (SockAddr *) &adres, i) < 0) {
            perror("Blad dowiazania gniazda.\n");
            close(gniazdo);
            return -1;
        } else {
            if (getsockname(gniazdo, (SockAddr *) &adres, &i) != 0) {
                perror("Blad funkcji getsockname.\n");
                close(gniazdo);
                return -1;
            } else {
                printf("Nr portu : %d.\n", ntohs(adres.sin_port));
                if (listen(gniazdo, 5) < 0) {
                    perror("Blad nasluchu gniazda.\n");
                    close(gniazdo);
                    return -1;
                } else {
                    for (;;) {
                        if ((gniazdo1 = accept(gniazdo, (SockAddr *) &adres_k, &i)) < 0) {
                            perror("Blad funkcji accept.\n");
                            continue;
                        } else {
                            if (fork() == 0) {
                                pid_t t;
                                size_t n;
                                char buf[100];

                                close(gniazdo);
                                signal(SIGURG, sig_urg);
                                fcntl(gniazdo1, F_SETOWN, getpid());
                                printf("Polaczenie od %s z portu %d.\n", inet_ntoa(adres_k.sin_addr),
                                       ntohs(adres_k.sin_port));
                                for (;;) {
                                    if ((n = read(gniazdo1, buf, sizeof(buf) - 1)) == 0) {
                                        printf("Koniec transmisji klienta.\n");
                                        close(gniazdo1);
                                        break;
                                    }
                                    buf[n] = 0;
                                    printf("Przeczytalem %lu bajtow: %s.\n", n, buf);
                                }
                                printf("Koniec pracy procesu potomnego.\n");
                                return 0;
                            } else {
                                sigignore(SIGCHLD);
                                close(gniazdo1);
                            }
                        }
                    }
                }
            }
        }
    }
}
```
```c
/* 
Klient strumieniowy, internetowy z danymi wysoko priorytetowymi.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

int main(int argc, char *argv[]) {
    int gniazdo;
    socklen_t i;
    SockAddr_in adres;

    if (argc != 3) {
        printf("Bledna ilosc argumentow. (adres internetowy, numer portu) \n");
        return -1;
    } else {
        if ((gniazdo = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Blad wywolania funkcji socket.\n");
            return -1;
        } else {
            struct hostent *Adres;

            i = sizeof(SockAddr_in);
            adres.sin_family = AF_INET;
            adres.sin_port = htons(strtol(argv[2], NULL, 10));
            Adres = gethostbyname(argv[1]);
            adres.sin_addr.s_addr = *(long *) (Adres->h_addr);
            if (connect(gniazdo, (SockAddr *) &adres, i) < 0) {
                perror("Blad funkcji connect.\n");
                close(gniazdo);
                return -1;
            } else {
                write(gniazdo, "123", 3);
                printf("Wyslalem 3 bajty normalnych danych.\n");
                sleep(1);
                send(gniazdo, "4", 1, MSG_OOB);
                printf("Wyslalem 1 bajt danych OOB.\n");
                sleep(1);
                write(gniazdo, "56", 2);
                printf("Wyslalem 2 bajty nomalnych danych.\n");
                sleep(1);
                send(gniazdo, "7", 1, MSG_OOB);
                printf("Wyslalem 1 bajt danych OOB.\n");
                sleep(1);
                write(gniazdo, "89", 2);
                printf("Wyslalem 2 bajty nomalnych danych.\n");
                sleep(1);
                close(gniazdo);
                return 0;
            }
        }
    }
}
```
#### Mechanizm select
```c
/*
Serwer wykorzystujacy mechanizm select.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<stdlib.h>

typedef struct sockaddr Sockaddr;
typedef struct sockaddr_in6 Sockaddr_in6;
typedef struct sockaddr_in Sockaddr_in;
int gniazdo, gniazdo1;

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Koniec pracy serwera.\n");
        close(gniazdo);
        close(gniazdo1);
        exit(0);
    }
}

int main(int argc, char **argv) {
    int gniazdo2, wynik, wynik1, max;
    socklen_t dlugosc6, dlugosc;
    Sockaddr_in6 adres6;
    Sockaddr_in adres;
    fd_set deskryptory;
    char buf[200];

    signal(SIGTERM, al);
    signal(SIGINT, al);
    sigignore(SIGCHLD);
    FD_ZERO(&deskryptory);
    gniazdo = socket(PF_INET6, SOCK_STREAM, 0);
    gniazdo1 = socket(PF_INET, SOCK_STREAM, 0);
    max = (gniazdo < gniazdo1 ? gniazdo1 : gniazdo);
    if (gniazdo < 0 || gniazdo1 < 0) {
        perror("Blad tworzenia gniazda (funkcja socket).\n");
        return -1;
    } else {
        switch (argc) {
            case 1:
                adres6.sin6_port = htons(0);
                adres.sin_port = htons(0);
                break;
            case 2:
                adres6.sin6_port = htons(strtol(argv[1], NULL, 10));
                adres.sin_port = adres6.sin6_port;
                break;
            default:
                adres6.sin6_port = htons(strtol(argv[1], NULL, 10));
                adres.sin_port = htons(strtol(argv[2], NULL, 10));
        }
        dlugosc6 = sizeof(Sockaddr_in6);
        adres6.sin6_family = AF_INET6;
        adres6.sin6_addr = in6addr_any;
        wynik = bind(gniazdo, (Sockaddr *) &adres6, dlugosc6);
        dlugosc = sizeof(Sockaddr_in);
        adres.sin_family = AF_INET;
        adres.sin_addr.s_addr = INADDR_ANY;
        wynik1 = bind(gniazdo1, (Sockaddr *) &adres, dlugosc);
        if (wynik < 0 || wynik1 < 0) {
            perror("Blad tworzenia gniazda (funkcja bind).\n");
            close(gniazdo);
            close(gniazdo1);
            return -1;
        } else {
            wynik = getsockname(gniazdo, (Sockaddr *) &adres6, &dlugosc6);
            wynik1 = getsockname(gniazdo1, (Sockaddr *) &adres, &dlugosc);
            if (wynik < 0 || wynik1 < 0) {
                perror("Blad tworzenia gniazda (funkcja getsockname).\n");
                close(gniazdo);
                close(gniazdo1);
                return -1;
            } else {
                printf("Serwer nasluchuje na porcie ipv6 %d i ipv4 %d\n", ntohs(adres6.sin6_port),
                       ntohs(adres.sin_port));
                wynik = listen(gniazdo, 5);
                wynik1 = listen(gniazdo1, 5);
                if (wynik < 0 || wynik1 < 0) {
                    perror("Blad tworzenia gniazda (funkcja listen).\n");
                    close(gniazdo);
                    close(gniazdo1);
                    return -1;
                } else {
                    for (;;) {
                        FD_SET(gniazdo, &deskryptory);
                        FD_SET(gniazdo1, &deskryptory);
                        printf("Przed funkcja select.\n");
                        wynik = select(max + 1, &deskryptory, NULL, NULL, NULL);
                        printf("Po funkcji select.\n");
                        if (wynik < 0) {
                            perror("Blad tworzenia gniazda (funkcja select).\n");
                            close(gniazdo);
                            close(gniazdo1);
                            return -1;
                        } else {
                            if (FD_ISSET(gniazdo, &deskryptory)) {
                                printf("Ktos sie polaczyl, protokol ip wersja 6.\n");
                                gniazdo2 = accept(gniazdo, (Sockaddr *) &adres6, &dlugosc6);
                            } else if (FD_ISSET(gniazdo1, &deskryptory)) {
                                printf("Ktos sie polaczyl, protokol ip wersja 4.\n");
                                gniazdo2 = accept(gniazdo1, (Sockaddr *) &adres, &dlugosc);
                            }
                            if (gniazdo2 < 0) {
                                perror("Blad tworzenia gniazda (funkcja accept).\n");
                                continue;
                            }
                            if (fork()) {
                                sigignore(SIGCHLD);
                                close(gniazdo2);
                            } else {
                                size_t rozmiar;

                                close(gniazdo);
                                close(gniazdo1);
                                rozmiar = read(gniazdo2, buf, 200);
                                if (rozmiar < 0) {
                                    perror("Blad odbierania poprzez gniazdo (read).\n");
                                    close(gniazdo2);
                                    return -1;
                                }
                                buf[rozmiar] = 0;
                                printf("Przeczytalem: %s.\n", buf);
                                buf[rozmiar] = '+';
                                buf[rozmiar + 1] = 0;
                                rozmiar = write(gniazdo2, buf, rozmiar + 2);
                                if (rozmiar < 0) {
                                    perror("Blad wysylania poprzez gniazdo (write).\n");
                                    close(gniazdo2);
                                    return -1;
                                }
                                close(gniazdo2);
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
}
```
#### Szyfrowana transmisja danych poprzez gniazda
```c
/*
Serwer strumieniowy, internetowy.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<openssl/ssl.h>
#include<openssl/err.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;
int gniazdo;
SSL_CTX *ctx;

void al(int i) {
    if (i == SIGTERM || i == SIGINT) {
        printf("Koniec pracy serwera.\n");
        close(gniazdo);
        SSL_CTX_free(ctx);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    int gniazdo1;
    const SSL_METHOD *method;
    socklen_t i;
    SockAddr_in adres, adres_k;

    signal(SIGTERM, al);
    signal(SIGINT, al);
    sigignore(SIGCHLD);
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    OpenSSL_add_all_digests();
    SSL_load_error_strings();
    method = TLS_server_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Blad tworzenia kontekstu SSL.\n");
        ERR_print_errors_fp(stderr);
        return -1;
    }
    if (SSL_CTX_use_certificate_file(ctx, "./cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "./key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return -1;
    }
    if ((gniazdo = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Blad wywolania funkcji socket.\n");
        return -1;
    } else {
        i = sizeof(SockAddr_in);
        adres.sin_family = AF_INET;
        adres.sin_port = htons(0);
        adres.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(gniazdo, (SockAddr *) &adres, i) < 0) {
            perror("Blad dowiazania gniazda.\n");
            return -1;
        } else {
            if (getsockname(gniazdo, (SockAddr *) &adres, &i) != 0) {
                perror("Blad funkcji getsockname.\n");
                return -1;
            } else {
                printf("Nr portu : %d\n", ntohs(adres.sin_port));
                if (listen(gniazdo, 5) < 0) {
                    perror("Blad nasluchu gniazda.\n");
                    return -1;
                } else {
                    for (;;) {
                        if ((gniazdo1 = accept(gniazdo, (SockAddr *) &adres_k, &i)) < 0) {
                            perror("Blad funkcji accept.\n");
                            return -1;
                        } else {
                            if (fork() == 0) {
                                pid_t t;
                                SSL *ssl;

                                close(gniazdo);
                                printf("Polaczenie od %s z portu %d.\n", inet_ntoa(adres_k.sin_addr),
                                       ntohs(adres_k.sin_port));
                                ssl = SSL_new(ctx);
                                SSL_set_fd(ssl, gniazdo1);
                                if (SSL_accept(ssl) <= 0) {
                                    ERR_print_errors_fp(stderr);
                                    return -1;
                                } else {
                                    read(gniazdo1, &t, sizeof(pid_t));
                                    printf("Klient przyslal w sposob nieszyfrowany %d.\n", t);
                                    t = 0;
                                    printf("Wersja protokolu SSL/TLS %s. ", SSL_get_version(ssl));
                                    if (SSL_read(ssl, &t, sizeof(pid_t)) <= 0) {
                                        ERR_print_errors_fp(stderr);
                                        return -1;
                                    }
                                    printf("Klient przyslal %d.\n", t);
                                }
                                SSL_free(ssl);
                                close(gniazdo1);
                                SSL_CTX_free(ctx);
                                return 0;
                            } else {
                                close(gniazdo1);
                                sigignore(SIGCHLD);
                            }
                        }
                    }
                }
            }
        }
    }
}
```
```c
/*
Klient strumieniowy, internetowy, szyfrowany.
*/

#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<openssl/ssl.h>
#include<openssl/err.h>

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddr_in;

static int get_version(char *version) {
    if (strcmp("SSL3", version) == 0) { return SSL3_VERSION; }
    else if (strcmp("TLS1", version) == 0) { return TLS1_VERSION; }
    else if (strcmp("TLS1_1", version) == 0) { return TLS1_1_VERSION; }
    else if (strcmp("TLS1_2", version) == 0) { return TLS1_2_VERSION; }
    else return TLS1_3_VERSION;
}

int main(int argc, char *argv[]) {
    int gniazdo;
    const SSL_METHOD *method;
    SSL_CTX *ctx;
    socklen_t i;
    SockAddr_in adres;
    pid_t pid;

    if (argc != 4) {
        printf("Bledna ilosc argumentow (adres internetowy, numer portu, protokol SSL/TLS).\n");
        return -1;
    } else {
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        OpenSSL_add_all_ciphers();
        OpenSSL_add_all_digests();
        SSL_load_error_strings();
        method = TLS_client_method();
        ctx = SSL_CTX_new(method);
        if (!ctx) {
            perror("Blad tworzenia kontekstu SSL.\n");
            ERR_print_errors_fp(stderr);
            return -1;
        }
        pid = getpid();
        if ((gniazdo = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Blad wywolania funkcji socket.\n");
            return -1;
        } else {
            struct hostent *Adres;

            i = sizeof(SockAddr_in);
            adres.sin_family = AF_INET;
            adres.sin_port = htons(strtol(argv[2], NULL, 10));
            Adres = gethostbyname(argv[1]);
            adres.sin_addr.s_addr = *(long *) (Adres->h_addr);
            if (connect(gniazdo, (SockAddr *) &adres, i) < 0) {
                perror("Blad funkcji connect.\n");
                return -1;
            } else {
                SSL *ssl;
                int wynik;

                ssl = SSL_new(ctx);
                SSL_set_max_proto_version(ssl, get_version(argv[3]));
                SSL_set_fd(ssl, gniazdo);
                if (SSL_connect(ssl) != 1) {
                    ERR_print_errors_fp(stderr);
                    return -1;
                }
                printf("Klient wysyla %d.\n", pid);
                write(gniazdo, &pid, sizeof(pid_t));
                if (SSL_write(ssl, &pid, sizeof(pid_t)) <= 0) {
                    ERR_print_errors_fp(stderr);
                    return -1;
                }
                wynik = SSL_shutdown(ssl);
                switch (wynik) {
                    case 0  :
                        SSL_shutdown(ssl);
                    case 1  :
                        break;
                    default :
                        ERR_print_errors_fp(stderr);
                }
                SSL_free(ssl);
                close(gniazdo);
                SSL_CTX_free(ctx);
                return 0;
            }
        }
    }
}
```
#### Zadania
- Zadanie 32
- Zadanie 33
- Zadanie 34
# Laboratorium 9. - 15. stycznia 2023
### Zdalne wywoływanie procedur (RPC).
1. Należy stworzyć plik z rozszerzeniem .x deklarujący zdalną procedurę i definiujący jej dane wejściowe i wyjściowe.
2. Wywołać polecenie rpcgen -N -a plik.x generujące pełen wzorzec projektu wraz z plikiem Makefile.plik służącym do budowy projektu.
3. Wyedytować plik plik_server.c, w którym znajduje się logika zdalnej procedury.
4. Wyedytować plik plik_client.c, w którym znajduje się program klienta przygotowujący dane dla zdalnej procedury, wywołujący tę zdalną procedurę i interpretujący jej wyniki.
5. Budowanie gotowego projektu odbywa się poprzez wykonanie polecenia: make -f Makefile.plik.
6. Uruchomić program serwera: plik_server.
7. Uruchomić program klienta: plik_client.
#### Zdalne wywoływanie procedur (RPC)
```c
/*
Pierwszy argument wejsciowy zdalnej procedury
*/ 
struct formula_in {
    long l;
    double x;
};

/*
Drugi argument wejsciowy zdalnej procedury
*/
struct formula_ex {
    float y;
};

/*
Wartosci zwracane przez zdalna procedure
*/
struct formula_out {
    double wynik;
    long wynik2;
};

/*
Deklaracja zdalnej procedury
*/
program FORMULA_PROG{
        version FORMULA_VERS {
            formula_out FORMULAPROC(formula_in, formula_ex) = 1;
        } = 1;
} = 0x71234567;
```
```c
/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "formula.h"

formula_out *formulaproc_1_svc(formula_in arg1, formula_ex arg2, struct svc_req *rqstp) {
    static formula_out result;

    /*
     * insert server code here
     */

    result.wynik = arg1.l + 10 * arg1.x + 100 * arg2.y;
    result.wynik2 = arg1.l + 77;
    return &result;
}
```
```c
/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "formula.h"

void formula_prog_1(char *host, long l, double x, float y) {
    CLIENT *clnt;
    formula_out *result_1;
    formula_in formulaproc_1_arg1;
    formula_ex formulaproc_1_arg2;

    formulaproc_1_arg1.l = l;
    formulaproc_1_arg1.x = x;
    formulaproc_1_arg2.y = y;
#ifndef    DEBUG
    clnt = clnt_create(host, FORMULA_PROG, FORMULA_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif    /* DEBUG */
    result_1 = formulaproc_1(formulaproc_1_arg1, formulaproc_1_arg2, clnt);
    if (result_1 == (formula_out *) NULL) {
        clnt_perror(clnt, "call failed");
    } else {
        printf("Wynik: %lf,    %ld.\n", result_1->wynik, result_1->wynik2);
    }
#ifndef    DEBUG
    clnt_destroy(clnt);
#endif     /* DEBUG */
}

int main(int argc, char *argv[]) {
    char *host;

    if (argc < 5) {
        printf("usage: %s server_host liczba_calkowita liczba_rzeczywista liczba_rzeczywista\n", argv[0]);
        exit(1);
    }
    host = argv[1];
    formula_prog_1(host, strtol(argv[2], NULL, 10), strtod(argv[3], NULL), strtof(argv[4], NULL));
    exit(0);
}
```
# Laboratorium 10. - 29. stycznia 2023
### Zdale wywoływanie procedut gRPC i RPC-XML.


## Zadania
1. 
``` 
W języku C napisać program przyjmujący dwa parametry wywołania będące liczbami naturalnymi m i n. Program tworzy dynamicznie macierz o m wierszach i n kolumnach, wypełnia ją losowymi łańcuchami znaków długości co najwyżej 256 a następnie wypisuje ją na ekran.
```
2. 
```
W języku C napisać program, który z wykorzystaniem standardowej funkcji qsort posortuje tablicę liczb całkowitych a następnie wypisze na ekran tablicę wynikową.
```
3. 
```
W języku C napisać program realizujący następujące zadanie. Na zbiorze łańcuchów wprowadzamy relację porządku w następujący sposób

ł1<=ł2 wtedy i tylko wtedy gdy liczba naturalna będąca sumą kodów asci odpowiadających wszystkim znakom ł1 jest mniejsza lub równa liczbie naturalnej będącej sumą kodów asci odpowiadających wszystkim znakom ł2.

Program przyjmuje jeden parametr wywołania będący liczbą naturalną n, tworzy tablicę n losowo wygenerowanych łańcuchów a następnie z wykorzystaniem standardowej funkcji qsort sortuje tę tablicę według porządku zdefiniowanego powyżej. Tablicę wynikową wypisuje na ekran.
```
4. 
```
Zmodyfikować przykład obrazujący użycie przydatnych konstrukcji języka C tak aby wykorzystywał funkcje getopt i getsubopt. Można przyjąć składnię.

 Opcja -w posiada jako wartość listę definiującą wymiar macierzy postaci lw=liczba1,lk=liczba2.
 Opcja -a oznacza element a struktury Dane w przykładzie obrazującym użycie przydatnych konstrukcji języka C.
 Opcja -b oznacza element b struktury Dane w przykładzie obrazującym użycie przydatnych konstrukcji języka C.
 Opcja -c oznacza element c struktury Dane w przykładzie obrazującym użycie przydatnych konstrukcji języka C.
Przykład. Wywołanie ./a.out -w lw=6,lk=8 -a 3.14 -b -2.56 -c 7 spowoduje utworzenie tablicy o sześciu wierszach i ośmiu kolumnach wypełnionych losowymi liczbami typu double, ustawi wartości pola a zmiennej y na 3.14, pola b zmiennej y na -2.56 oraz pola c zmiennej y na 7.
```
5. 
```
Zmodyfikować przykładowy program tworzący procesy potomne przykład 2, w taki sposób aby proces potomny wypisywał na ekran informację o identyfikatorze rodzica poprzez użycie funkcji systemowego api getppid. Ponadto, proszę sprawdzić jak zachowa się funkcja getppid wywołana w procesie macierzystym i co ona zwróci.
```
6. 
```
Zmodyfikować przykład uruchomienie w procesie potomnym nowego zadania i ustanowienie z nim komunikacji poprzez potoki - wariant 2, w taki sposób aby można było podać jako argumenty wywołania programu więcej działań do wykonania dla kalkulatora.
```
7. 
```
Wyszukać jakiś interaktywny program pracujący w trybie tekstowym, nazwijmy go przykładowy_program. Zmodyfikować przykład uruchomienie w procesie potomnym nowego zadania i ustanowienie z nim komunikacji poprzez potoki - wariant 1, w taki sposób aby wykorzystywał przykładowy_program. Wykorzystanie przykładowy_program może wymusić modyfikację scenariusza interakcji z nim.
```
8. 
```
Wyszukać jakiś interaktywny program pracujący w trybie tekstowym, nazwijmy go przykładowy_program. Zmodyfikować przykład uruchomienie w procesie potomnym nowego zadania i ustanowienie z nim komunikacji poprzez potoki - wariant 2, w taki sposób aby wykorzystywał przykładowy_program. Wykorzystanie przykładowy_program może wymusić modyfikację scenariusza interakcji z nim.
```
9. 
```
Zmodyfikować przykład dotyczący pobierania i modyfikowania limitów w systemie operacyjnym w taki sposób, aby pojawiała się informacja o ewentualnych błędach zwracanych przez funkcje odpowiadające za obsługę limitów.
```
10. 
```
Napisać program kopiujący zawartość jednego pliku na drugi plik z wykorzystaniem bufora o zadanej wielkości i zadanego sposobu wykonywania operacji komunikacji. Program przyjmuje cztery parametry wywołania programu: plik źródłowy, plik docelowy, wielkość bufora i sposób wykonywania operacji komunikacji. Przyjmujemy, że dla czwartego parametru równego:

"sb" - plik będzie kopiowany w sposób synchroniczny blokowany;
"sn" - plik będzie kopiowany w sposób synchroniczny nieblokowany;
"ab" - plik będzie kopiowany w sposób asynchroniczny blokowany.
```
11. 
```
Zmodyfikować program kopiujący pliki w taki sposób, aby wielkość bufora używanego przez program pobierana była ze zmiennej środowiskowej o nazwie WIELKOSC.
```
12. 
```
Zmodyfikować program kopiujący pliki w taki sposób, aby wielkość bufora używanego przez program uwzględniała limity systemowe dotyczące wielkości pojedynczego obszaru pamięci. Ponadto, zmodyfikować pętlę kopiującą w taki sposób, aby kończyła swoje działanie po odczytaniu znaku końca pliku a nie po odczytaniu zerowej liczby bajtów.
```
13. 
```
Zmodyfikować program kopiujący pliki w taki sposób, aby wielkość bufora używanego przez program była 1/10 wielkości całego pliku źródłowego. Ponadto, program ma wypisywać informację o katalogu domowym właściciela pliku źródłowego.
```
14. 
```
Nie było
```
15. 
```
W języku C napisać program przyjmujący jeden parametr wywołania będący liczbą naturalną n > 0. Program generuje n tablic długości n losowych liczb typu double. Następnie wątek główny uruchamia n wątków potomnych i przekazuje im jako argument dokładnie jedną tablicę (każdy wątek ma inną tablicę). Każdy wątek znajduje sumę kwadratów elementów swojego wiersza, wynik przekazuje wątkowi głównemu (liczba typu double) i kończy działanie. Po odebraniu wyników wątek główny znajduje maksimum otrzymanych sum i wynik wypisuje na ekran.

Uwagi.

Parametry między wątkami nie mogą być przekazywane jako zmienne globalne.
Wątki potomne wyniki swojej pracy muszą przekazywać jako wynik zwracany przez funkcję obsługi wątku.
Wątki potomne muszą działać współbieżnie.
```
16. 
```
W języku C napisać program przyjmujący jeden parametr wywołania będący liczbą naturalną n > 0. Program generuje n tablic długości n losowych liczb typu double. Następnie wątek główny uruchamia n wątków potomnych i przekazuje im jako argument dokładnie jedną tablicę (każdy wątek ma inną tablicę). Każdy wątek znajduje sumę kwadratów elementów swojego wiersza, otrzymany wynik porównuje z wartością zmiennej współdzielonej, w której ma się znaleźć maksimum spośród sum kwadratów wszystkich wierszy, jak jego suma kwadratów jest większa od wartości w zmiennej współdzielonej to ją modyfikuje i kończy działanie. Po odebraniu informacji o końcu pracy wątków potomnych, wątek główny wypisuje maksimum sum kwadratów na ekran.

Uwagi.

Parametry między wątkami nie mogą być przekazywane jako zmienne globalne.
Wątki potomne wyniki swojej pracy muszą przekazywać jako wynik zwracany przez funkcję obsługi wątku.
Wątki potomne muszą działać współbieżnie.
Do kontroli dostępu do zmiennej akumulującej wynik należy użyć rygla.
```
17. 
```
W języku C napisać program przyjmujący dwa parametry wywołania programu. Pierwszy z parametrów jest ścieżką do pliku tekstowego plik. Drugi jest liczbą naturalną m. Wątek główny programu uruchamia wątek pisarza, który co losową liczbę sekund umieszcza we współdzielonym buforze pojedynczą linię przeczytaną z pliku plik. Ponadto, uruchamia m czytelników, którzy co losową liczbę sekund odczytują zawartość współdzielonego bufora i wypisują ją na ekran. Program kończy działanie po odczekaniu losowej liczby sekund po tym jak wątek pisarza umieści we współdzielonym buforze ostatnią linijkę pliku plik.

Uwagi.

Parametry między wątkami nie mogą być przekazywane jako zmienne globalne.
Wątki potomne wyniki swojej pracy muszą przekazywać jako wynik zwracany przez funkcję obsługi wątku.
Wątki potomne muszą działać współbieżnie.
Do synchronizacji pracy wątków należy użyć rygle odczytu/zapisu.
```
18. 
```
W języku C napisać program przyjmujący trzy parametry wywołania n, m i p. Zakładamy, że n, m są liczbami typu long, a p jest liczbą typu double. Uruchamiamy n wątków, każdy z wątków otrzymuje jako parametry liczby m i p. Ponadto zakładamy, że wspólny wynik będzie akumulowany w zmiennej z. Każdy wątek wykonuje n razy następujące kroki.

Generuje losową liczbę s typu double, wylicza s*p i wynik dodaje do wspólnej zmiennej z.
Jeżeli z <= m, to budzi on jeden z oczekujących wątków. Wątek, który stwierdzi, ze z > m, wylicza p =  \sqrt[n]{z} , zeruje zmienna z oraz budzi pozostałe wątki.
Uwagi.

Parametry między wątkami nie mogą być przekazywane jako zmienne globalne.
Wątki potomne wyniki swojej pracy muszą przekazywać jako wynik zwracany przez funkcję obsługi wątku.
Wątki potomne muszą działać współbieżnie.
Do synchronizacji pracy wątków należy użyć rygli oraz zmiennej warunku.
```
19. 
```
W języku C napisać program przyjmujący jeden parametr wywołania będący liczbą naturalną n > 0. Program generuje n tablic długości n losowych liczb typu double. Następnie wątek główny uruchamia n wątków potomnych i przekazuje im jako argument dokładnie jedną tablicę (każdy wątek ma inną tablicę). Każdy wątek znajduje sumę kwadratów elementów swojego wiersza, otrzymany wynik porównuje z wartością zmiennej współdzielonej, w której ma się znaleźć maksimum spośród sum kwadratów wszystkich wierszy, jak jego suma kwadratów jest większa od wartości w zmiennej współdzielonej to ją modyfikuje i kończy działanie. Po odebraniu informacji o końcu pracy wątków potomnych, wątek główny wypisuje maksimum sum kwadratów na ekran.

Uwagi.

Parametry między wątkami nie mogą być przekazywane jako zmienne globalne.
Wątki potomne wyniki swojej pracy muszą przekazywać jako wynik zwracany przez funkcję obsługi wątku.
Wątki potomne muszą działać współbieżnie.
Do synchronizacji pracy wątków należy użyć semafora nienazwanego.
```
20. 
```
Przejrzeć wszystkie dotychczasowe przykłady programów, znaleźć wszystkie te, które tworzą nowe procesy i zmodyfikować je w taki sposób aby tworzyły nie nowych procesów tylko nowe wątki.
```
21. 
```
Na podstawie przykładowej implementacji współbieżnego klienta i serwera wykorzystującego kolejki FIFO, stworzyć klienta i sekwencyjny serwer wykorzystujące do komunikacji z klientem jedynie główną kolejkę FIFO.
```
22. 
```
Na podstawie przykładowej implementacji współbieżnego serwera wykorzystującego kolejki komunikatów, stworzyć sekwencyjny serwer wykorzystujący do komunikacji z klientem jedynie główną kolejkę komunikatów.
```
23. 
```
Na podstawie przykładowej implementacji współbieżnego klienta i serwera wykorzystującego kolejki komunikatów, stworzyć implementacje klienta i serwera wykorzystujące kolejki komunikatów standardu POSIX.
```
24. 
```
Rozszerzyć przykładowe implementacje serwera sekwencyjnego i klienta dla serwera sekwencyjnego (wersje rozbudowane), wykorzystujących semafory i pamięć współdzieloną, w taki sposób, aby wymiana informacji między serwerem a klientem składała się z niepodzielnej sekwencji (żaden inny klient nie może tej sekwencji przerwać) co najmniej dwóch wymian informacji, na przykład: klient wysyła wiadomość, serwer ją odbiera i na jej podstawie wysyła swoją odpowiedź.
```
25. 
```
Na podstawie przykładowej implementacji serwera sekwencyjnego i klienta dla serwera sekwencyjnego (wersje rozbudowane), wykorzystujących semafory i pamięć współdzieloną, stworzyć implementacje klienta i serwera wykorzystujące semafory i pamięć współdzieloną standardu POSIX.
```
26. 
```
Nie było
```
27. 
```
Zmodyfikować program serwera i klienta internetowego strumieniowego w taki sposób aby korzystały z gniazd internetowych protokołu IPv6.

Uwaga.

Nie każdy system operacyjny domyślnie wspiera obsługę IPv6.
```
28. 
```
Zmodyfikować program serwera strumieniowego, zarówno w domenie internetowej jak i unixowej, w taki sposób aby korzystały z wątków potomnych a nie procesów potomnych.
```
29. 
```
Zmodyfikować programy współbieżnego serwera datagramowego, zarówno w domenie internetowej jak i unixowej, w taki sposób aby korzystały z wątków potomnych a nie procesów potomnych.
```
30. 
```
Zmodyfikować programy serwera i klienta datagramowego w domenie internetowej, zarówno sekwencyjnego jak i współbieżnego, w taki sposób aby korzystały z gniazd internetowych protokołu IPv6.
```
31. 
```
Zmodyfikować przyklad uruchomienie w procesie potomnym nowego zadania i ustanowienie z nim komunikacji poprzez potoki - wariant 2, w taki sposób aby zamiast używać potoków, używał pary gniazd w domenie unixowej.

Uwaga.

Do wykonania zadania należy użyć funkcji socketpair.
```
32. 
```
Zmodyfikować programy serwera i klienta współbieżnych strumieniowych internetowych wykorzystujących interfejs gniazd i dane wysoko priorytetowe w taki sposób aby wykorzystywały do komunikacji.

Gniazda internetowe strumieniowe IPv6.
Gniazda uniksowe strumieniowe.
Gniazda internetowe datagramowe IPv4.
Gniazda internetowe datagramowe IPv6.
Gniazda uniksowe datagramowe.
Uwaga.

Rozwiązaniem zadania jest pięć par programów klienta i serwera wykorzystujących do komunikacji powyższe rodzaje gniazd.
```
33. 
```
Zmodyfikować programy serwera i klienta współbieżnych strumieniowych internetowych wykorzystujących interfejs szyfrowanych gniazd w taki sposób aby wykorzystywały do komunikacji.

Szyfrowane gniazda internetowe strumieniowe IPv6.
Szyfrowane gniazda uniksowe strumieniowe.
Szyfrowane gniazda internetowe datagramowe IPv4.
Szyfrowane gniazda internetowe datagramowe IPv6.
Szyfrowane gniazda uniksowe datagramowe.
Uwaga.

Rozwiązaniem zadania jest pięć par programów klienta i serwera wykorzystujących do komunikacji powyższe rodzaje gniazd.
```
34. 
```
Rozszerzyć program serwera wykorzystującego interfejs gniazd IPv4, IPv6 i mechanizm select w taki sposób aby wykorzystywał wszystkie znane rodzaje gniazd, czyli nasłuchiwał na jednym gnieździe każdego z poniższych typów.

Internetowe strumieniowe IPv4 (obecnie zaimplementowane w przykładzie).
Internetowe strumieniowe IPv6 (obecnie zaimplementowane w przykładzie).
Uniksowe strumieniowe.
Internetowe datagramowe IPv4.
Internetowe datagramowe IPv6.
Uniksowe datagramowe.
Szyfrowane internetowe strumieniowe IPv4.
Szyfrowane internetowe strumieniowe IPv6.
Szyfrowane uniksowe strumieniowe.
Szyfrowane internetowe datagramowe IPv4.
Szyfrowane internetowe datagramowe IPv6.
Szyfrowane uniksowe datagramowe.
```
