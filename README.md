# Projekt: Bela (Belote) u C++

[cite_start]**Autori:** Ivan Štefner, Ante Šarić [cite: 1]
[cite_start]**GitHub Repozitorij:** [ante-saric/Bela_igra_oop](https://github.com/ante-saric/Bela_igra_oop) [cite: 2]

## Opis projekta
[cite_start]Ovaj projekt predstavlja digitalnu verziju kartaške igre Bela (Belote), razvijenu kao dio kolegija Objektno orijentirano programiranje[cite: 3]. [cite_start]Igra simulira jednu rundu igranja između četiri igrača (ljudski igrač i tri AI protivnika)[cite: 31, 65, 66].

## Implementirane funkcionalnosti (Backlog)
Projekt prati definirani backlog i uključuje sljedeće komponente:

* [cite_start]**Model karata i špila**: Implementacija klasa `Karta` i `Spil` s 32 karte (od 7 do Asa)[cite: 4, 9, 11, 13].
* [cite_start]**Miješanje i dijeljenje**: Nasumično miješanje špila i dijeljenje 8 karata svakom igraču[cite: 14, 16, 29, 31, 32].
* **OOP Principi**: 
    * [cite_start]Korištenje `enum class` za boje (Srce, Karo, Pik, Žir) i vrijednosti[cite: 8, 13].
    * [cite_start]Apstraktna klasa `Igrac` koja definira zajednička svojstva i virtualne metode[cite: 19, 21, 23].
    * [cite_start]Polimorfizam kroz konkretne klase `CovjekIgrac` i `RačunaloIgrac`[cite: 24, 26, 27].
* **Logika igranja**:
    * [cite_start]Nasumičan odabir aduta za svaku rundu[cite: 39, 41, 43].
    * [cite_start]Igranje štihova uz poštivanje pravila boje i aduta[cite: 44, 46, 48, 51].
    * [cite_start]Pobjednik štiha započinje sljedeći krug[cite: 54, 56, 57].
* [cite_start]**Bodovanje i rezultati**: Automatsko računanje bodova po pravilima bele i ispis pobjedničkog tima na kraju[cite: 58, 60, 61, 68, 70, 71].

## Tehnologije
* [cite_start]**Jezik**: C++ (verzija 11 ili novija) [cite: 18]
* [cite_start]**Alati**: Standardna C++ biblioteka (`vector`, `algorithm`, `random`) [cite: 18]

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