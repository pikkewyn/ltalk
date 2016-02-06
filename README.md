# ltalk ( university task )
Program ltalk  do jednoczesnej rozmowy dowolnej liczby użytkowników. Jest to prosta wersja rozmowy typu rozgłoszeniowego
(ang. broadcast), czyli jak jeden z klientów programu wpisze wiadomość pojawia się ona na ekranie u
każdego innego klienta podłączonego do serwera. Dla odróżnienia klientów na początku każdej wiadomości wypisuje nick.

Compilation:
make

Running:
run ./ltalk in one terminal and than in two other run telnet like this:

    telnet localhost 8888
    Trying ::1...
    Connection failed: Connection refused
    Trying 127.0.0.1...
    Connected to localhost.
    Escape character is '^]'.
    Hello Client , I have received your connection. And now I will assign a handler for you
    >>>> WELCOME <<<<
    >>>> Please use /nick [nick] to set Your nick
    >>>> /nick foo
    thanks for registering
    >>>> ^[[A[bar 22:29 ]: hej foo
    >>>> hej bar
    >>>> whats up?
    >>>> [bar 22:30 ]: im fine man, and You?
    >>>> im doing great
    >>>> im just watching this fantastic movie
    >>>> You have to check it out
    >>>> [jan 22:31 ]: hi guys
    >>>> [jan 22:31 ]: nice to meet you
    >>>> [jan 22:31 ]: im frist time on this chat
    >>>>

    telnet localhost 8888
    Trying ::1...
    Connection failed: Connection refused
    Trying 127.0.0.1...
    Connected to localhost.
    Escape character is '^]'.
    Hello Client , I have received your connection. And now I will assign a handler for you
    >>>> WELCOME <<<<
    >>>> Please use /nick [nick] to set Your nick
    >>>> /nick bar
    thanks for registering
    >>>> hej foo       hej bar
    >>>> [foo 22:30 ]: whats up?
    >>>> im fine man, and You?
    >>>> [foo 22:30 ]: im doing great
    >>>> [foo 22:30 ]: im just watching this fantastic movie
    >>>> [foo 22:30 ]: You have to check it out
    >>>> [jan 22:31 ]: hi guys
    >>>> [jan 22:31 ]: nice to meet you
    >>>> [jan 22:31 ]: im frist time on this chat
    >>>>


Code still requires some cleaning up ( like making functions smaller etc ) but i've tested
it for memory leaks.
Some coverage tests need to be done and a lot functionality can be added.
