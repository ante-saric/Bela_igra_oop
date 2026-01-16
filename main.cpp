#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

// Korištenje enum class za boju i vrijednost karata prema backlogu [cite: 8]
enum class Boja { Srce, Karo, Pik, Zir };
enum class Vrijednost { Sedmica = 7, Osmica, Devetka, Desetka, Decko, Dama, Kralj, As };

class Karta {
public:
    Boja boja;
    Vrijednost vrijednost;

    Karta(Boja b, Vrijednost v) : boja(b), vrijednost(v) {}

    std::string formatiranIspis() const {
        std::string v[] = {"", "", "", "", "", "", "", "Sedmica", "Osmica", "Devetka", "Desetka", "Decko", "Dama", "Kralj", "As"};
        std::string b[] = {"Srce", "Karo", "Pik", "Zir"};
        return v[(int)vrijednost] + " " + b[(int)boja];
    }

    int dohvatiBodove(Boja adut) const {
        bool jeAdut = (boja == adut);
        if (jeAdut) {
            if (vrijednost == Vrijednost::Decko) return 20;
            if (vrijednost == Vrijednost::Devetka) return 14;
        }
        switch (vrijednost) {
            case Vrijednost::As:      return 11;
            case Vrijednost::Desetka: return 10;
            case Vrijednost::Kralj:   return 4;
            case Vrijednost::Dama:    return 3;
            case Vrijednost::Decko:   return 2;
            default:                  return 0;
        }
    }

    int dohvatiSnagu(Boja adut, Boja prvaBoja) const {
        int snaga = (int)vrijednost;
        if (boja == adut) {
            snaga += 100;
            if (vrijednost == Vrijednost::Decko) snaga += 50;
            if (vrijednost == Vrijednost::Devetka) snaga += 40;
        } else if (boja != prvaBoja) {
            snaga -= 100;
        }
        return snaga;
    }
};

class Spil {
private:
    std::vector<Karta> karte;
public:
    Spil() {
        for (int b = 0; b < 4; ++b) {
            for (int v = 7; v <= 14; ++v) {
                karte.emplace_back(static_cast<Boja>(b), static_cast<Vrijednost>(v));
            }
        }
    }

    void mijesaj() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(karte.begin(), karte.end(), g);
    }

    Karta izvadi() {
        Karta k = karte.back();
        karte.pop_back();
        return k;
    }
};

// Apstraktna klasa Player prema točki 4 [cite: 19]
class Igrac {
protected:
    std::string ime;
    std::vector<Karta> ruka;
public:
    Igrac(std::string i) : ime(i) {}
    virtual ~Igrac() {}

    void primiKartu(Karta k) { ruka.push_back(k); }

    void ispisiRuku() const {
        std::cout << "\nTvoja ruka:\n";
        for (size_t i = 0; i < ruka.size(); ++i) {
            std::cout << "[" << i << "] " << ruka[i].formatiranIspis() << "\n";
        }
    }

    // Provjera ima li igrač traženu boju u ruci
    bool imaBoja(Boja b) const {
        for (const auto& k : ruka) if (k.boja == b) return true;
        return false;
    }

    virtual Karta igrajKartu(bool prviUStihu, Boja trazenaBoja) = 0;
    std::string dohvatiIme() const { return ime; }
};

class CovjekIgrac : public Igrac {
public:
    CovjekIgrac(std::string i) : Igrac(i) {}

    Karta igrajKartu(bool prviUStihu, Boja trazenaBoja) override {
        int izbor;
        ispisiRuku();
        while (true) {
            std::cout << "Odaberi kartu (0-" << ruka.size()-1 << "): ";
            std::cin >> izbor;
            if (izbor >= 0 && izbor < (int)ruka.size()) {
                // Provjera obaveze odgovaranja na boju [cite: 44, 48]
                if (!prviUStihu && imaBoja(trazenaBoja) && ruka[izbor].boja != trazenaBoja) {
                    std::cout << "Moraš poštivati boju (" << (int)trazenaBoja << ")!\n";
                    continue;
                }
                break;
            }
            std::cout << "Neispravan unos.\n";
        }
        Karta k = ruka[izbor];
        ruka.erase(ruka.begin() + izbor);
        return k;
    }
};

class RačunaloIgrac : public Igrac {
public:
    RačunaloIgrac(std::string i) : Igrac(i) {}

    Karta igrajKartu(bool prviUStihu, Boja trazenaBoja) override {
        int index = 0;
        if (!prviUStihu && imaBoja(trazenaBoja)) {
            for (int i = 0; i < (int)ruka.size(); ++i) {
                if (ruka[i].boja == trazenaBoja) {
                    index = i;
                    break;
                }
            }
        }
        Karta k = ruka[index];
        ruka.erase(ruka.begin() + index);
        return k;
    }
};

class Igra {
private:
    std::vector<Igrac*> igraci;
    int bodoviTim1 = 0;
    int bodoviTim2 = 0;

public:
    Igra() {
        igraci.push_back(new CovjekIgrac("Ti"));
        igraci.push_back(new RačunaloIgrac("AI-1"));
        igraci.push_back(new RačunaloIgrac("AI-2"));
        igraci.push_back(new RačunaloIgrac("AI-3"));
    }

    ~Igra() { for (auto i : igraci) delete i; }

    void pokreni() {
        Spil spil;
        spil.mijesaj();
        for (int i = 0; i < 8; ++i) for (auto ig : igraci) ig->primiKartu(spil.izvadi());

        Boja adut = static_cast<Boja>(rand() % 4);
        std::string boje[] = {"Srce", "Karo", "Pik", "Zir"};
        std::cout << "ADUT JE: " << boje[(int)adut] << "\n";

        int tkoVodi = 0;
        for (int s = 0; s < 8; ++s) {
            std::vector<Karta> stol;
            std::cout << "\n--- STIH " << s + 1 << " ---\n";

            for (int i = 0; i < 4; ++i) {
                int tren = (tkoVodi + i) % 4;
                bool prvi = (i == 0);
                Boja trazena = prvi ? Boja::Srce : stol[0].boja; // Ako je prvi, boja nije bitna

                Karta bacena = igraci[tren]->igrajKartu(prvi, trazena);
                std::cout << igraci[tren]->dohvatiIme() << " igra: " << bacena.formatiranIspis() << "\n";
                stol.push_back(bacena);
            }

            int najjaci = 0;
            int maxSnaga = -500;
            for (int i = 0; i < 4; ++i) {
                int snaga = stol[i].dohvatiSnagu(adut, stol[0].boja);
                if (snaga > maxSnaga) {
                    maxSnaga = snaga;
                    najjaci = i;
                }
            }

            int pobjednik = (tkoVodi + najjaci) % 4;
            std::cout << ">>> Osvaja: " << igraci[pobjednik]->dohvatiIme() << " <<<\n";

            int zbroj = 0;
            for (auto& k : stol) zbroj += k.dohvatiBodove(adut);
            if (pobjednik % 2 == 0) bodoviTim1 += zbroj;
            else bodoviTim2 += zbroj;

            tkoVodi = pobjednik;
        }

        std::cout << "\nREZULTAT:\nTim 1: " << bodoviTim1 << "\nTim 2: " << bodoviTim2 << "\n";
    }
};

int main() {
    srand(time(0));
    Igra bela;
    bela.pokreni();
    return 0;
}