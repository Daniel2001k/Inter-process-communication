## Do testowania programu serwera datagramowego (UDP) można używać narzędzia netcat.
- Dla IPv4: nc -u 127.0.0.1 numer_portu.
- Dla IPv6: nc -6u ::1 numer_portu.
- Dla UNIX: nc -Uu ścieżka.
## Do uruchomienia serwera do testowania programu klienta strumieniowego (TCP) można używać narzędzia netcat.
- Dla IPv4: nc -u -l numer_portu.
- Dla IPv6: nc -6u -l numer_portu.
- Dla UNIX: nc -Uu -l ścieżka.
