## Do testowania programu serwera strumieniowego (TCP) można używać narzędzia netcat.
- Dla IPv4: nc 127.0.0.1 numer_portu.
- Dla IPv6: nc -6 ::1 numer_portu.
- Dla UNIX: nc -U ścieżka.
## Do uruchomienia serwera do testowania programu klienta strumieniowego (TCP) można używać narzędzia netcat.
- Dla IPv4: nc -l numer_portu.
- Dla IPv6: nc -6 -l numer_portu.
- Dla UNIX: nc -U -l ścieżka.