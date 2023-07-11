
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

#### Biblioteki:


## Client
## Podsumowanie
#### Biblioteki: 
#### Funkcje: 