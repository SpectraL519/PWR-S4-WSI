Jakub Musiał 268442

<br />

#### Pliki:

* `Makefile`
* `bot_client.c` : klient gry
* `game.h` : zasady i obsługa gry
* `minmax.h` : obsługa ruchu bota, algorytm minmax i heurystyka

<br />

#### Kompilacja:

```
make all
```

> `gcc` : 11.3.0
> 
> `make` : 4.3

<br />

#### Wywołanie clienta:

```
./bot_client <ip> <port> <player> <depth>
```

> player: X -> 1, O -> 2

