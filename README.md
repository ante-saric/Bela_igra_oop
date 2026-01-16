# Projekt: Bela (Belote) u C++

**Autori:** Ivan Štefner, Ante Šarić 
**GitHub Repozitorij:** [ante-saric/Bela_igra_oop](https://github.com/ante-saric/Bela_igra_oop) 

## Opis projekta
Ovaj projekt predstavlja digitalnu verziju kartaške igre Bela (Belote), razvijenu kao dio kolegija Objektno orijentirano programiranje. Igra simulira jednu rundu igranja između četiri igrača (ljudski igrač i tri AI protivnika).

## Implementirane funkcionalnosti (Backlog)
Projekt prati definirani backlog i uključuje sljedeće komponente:

* **Model karata i špila**: Implementacija klasa `Karta` i `Spil` s 32 karte (od 7 do Asa).
* **Miješanje i dijeljenje**: Nasumično miješanje špila i dijeljenje 8 karata svakom igraču.
* **OOP Principi**: 
    * Korištenje `enum class` za boje (Srce, Karo, Pik, Žir) i vrijednosti.
    * Apstraktna klasa `Igrac` koja definira zajednička svojstva i virtualne metode.
    * Polimorfizam kroz konkretne klase `CovjekIgrac` i `RačunaloIgrac
* **Logika igranja**:
    * Kada je na redu CovjekIgrac on bira aduta a kada je RacunlaoIgrac on bira aduta zadanom logikom.
    * Igranje štihova uz poštivanje pravila boje i aduta.
    * Pobjednik štiha započinje sljedeći krug.
* **Bodovanje i rezultati**: Automatsko računanje bodova po pravilima bele i ispis pobjedničkog tima na kraju.
* Igra se vrti u krug dok prva ekipa ne skupi 501 bod i tada se ispisuje pobjednik.

## Tehnologije
* **Jezik**: C++ (verzija 11 ili novija) 
* **Alati**: Standardna C++ biblioteka (`vector`, `algorithm`, `random`)

## Upute za pokretanje
1.  [cite_start]Klonirajte repozitorij s GitHuba[cite: 75].
2.  Kompajlirajte `main.cpp` datoteku pomoću C++ kompajlera:
    ```bash
    g++ main.cpp -o Bela.exe
    ```
3.  Pokrenite izvršnu datoteku:
    ```bash
    ./Bela.exe
    ```
4.  [cite_start]**Kontrole**: Kada dođe vaš red, unesite broj (indeks) karte iz svoje ruke prikazane na ekranu[cite: 76].

## Bilješke o implementaciji

[cite_start]AI igrači trenutno koriste logiku nasumičnog odabira karte radi demonstracije osnovnog tijeka igre[cite: 28]. [cite_start]Bodovanje uzima u obzir povećanu vrijednost Dečka i Devetke u adut boji[cite: 62].
