/*
Scrie un program C apelat astfel:

./program <fisier1> <fisier2>


Pentru fiecare fișier se va crea un proces copil.

Fiecare copil procesează fișierul primit și trimite printr-un pipe către părinte un mesaj de forma:

<nr_litere_mici> <nr_litere_mari> <nr_cifre>

Părintele colectează mesajele de la copii și afișează statisticile totale 
(adică suma tuturor valorilor primite).

Se recomandă folosirea:

fdopen() + fscanf() / fgets()


pentru a citi din pipe.
*/