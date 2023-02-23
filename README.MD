# Breakout

## Kompilace

Program se kompiluje pomocí poskytnutého Makefile souboru, pro kompilaci jsou zapotřebí knihovny [SDL2](https://www.libsdl.org/), [SDL_image](https://wiki.libsdl.org/SDL_image/FrontPage) a [SDL2_TTF](https://wiki.libsdl.org/SDL_image/FrontPage) a kompilátor [GCC](https://gcc.gnu.org/):

```bash
make
```
nebo rovnou pomocí [GCC](https://gcc.gnu.org/):

```bash
gcc src/main.c src/events.c src/render.c src/collisions.c src/data.c src/audio.c -o bin/breakout -lSDL2 -lSDL2_ttf -Ilib -lSDL2_image -lpthread
``` 

## Popis programu

Hra je ovládaná myší, a v jakékoliv fázi hry jde vypnout pomocí klávesy ESCAPE

Samotná hra při spuštění nabídne hráči menu se třemi možnosti:
1) PLAY - spustí samotnou hru
2) SETTINGS - nabídne hráči možnost změnit konfiguraci hry (pozice "cihel")
- hráč má na výběr ze dvou možností, buď hrát hru se všemi bloky nebo hrát s formátem definovaným v config.txt souboru (pokud nebude tato možnost změněna, hra se vždy pokusí načíst konfiguraci z config.txt souboru)
3) QUIT - ukončí program

Ve hře hráč vidí svoje aktualní skóre (umístěno v levém horním rohu) a počet míčku ("životů"), která má k dispozice (vyjma míčku v herním poli!)

## Úprava konfigurace hry přes soubor config.txt

hráč je má možnost si tehnle ten soubor upravit jak chce, ale vždy musí být dodržen formát 7 řádků na 15 sloupců znaků, kde každý 15-ctý znak v řádku je ingorován a slouží pouze pro oddělení řádků od sloupců. (možné je uspořádat tyto znaky jakkoliv, a však můsí byt počítáno s tím že každý 15-ctý znak je ignorován)

#### Program rozeznává následující znaky:
1) '2' sloužící jako nepoškozený blok
2) '1' sloužící jako poškozený blok
3) '0' sloužící jako zničený blok, který není zobrazován

##### Pokud program načte jakýkoliv jiný znak tak s ním zachází jako se znakem '0'!

#### Příklad validní konfigurace:
```
02222222222220
02000000000020
02022222222020
02020022002020
02022222222020
02000000000020
02222222222220
```
