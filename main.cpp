#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

enum class Boja { Srce, Karo, Pik, Zir };
enum class Vrijednost { Sedmica = 7, Osmica, Devetka, Desetka, Decko, Dama, Kralj, As };

string ispisiBoju(Boja b) {
    string boje[] = { "Srce", "Karo", "Pik", "Zir" };
    return boje[(int)b];
}

/* ================= KARTA ================= */

class Karta {
public:
    Boja boja;
    Vrijednost vrijednost;

    Karta(Boja b, Vrijednost v) : boja(b), vrijednost(v) {}

    string formatiranIspis() const {
        string vrijednosti[] = {
            "", "", "", "", "", "", "",
            "Sedmica", "Osmica", "Devetka",
            "Desetka", "Decko", "Dama", "Kralj", "As"
        };
        return vrijednosti[(int)vrijednost] + " " + ispisiBoju(boja);
    }

    int bodoviAdut() const {
        if (vrijednost == Vrijednost::Decko) return 20;
        if (vrijednost == Vrijednost::Devetka) return 14;
        if (vrijednost == Vrijednost::As) return 11;
        if (vrijednost == Vrijednost::Desetka) return 10;
        if (vrijednost == Vrijednost::Kralj) return 4;
        if (vrijednost == Vrijednost::Dama) return 3;
        return 0;
    }

    int dohvatiBodove(Boja adut) const {
        if (boja == adut) return bodoviAdut();

        if (vrijednost == Vrijednost::As) return 11;
        if (vrijednost == Vrijednost::Desetka) return 10;
        if (vrijednost == Vrijednost::Kralj) return 4;
        if (vrijednost == Vrijednost::Dama) return 3;
        if (vrijednost == Vrijednost::Decko) return 2;

        return 0;
    }

    int dohvatiSnagu(Boja adut, Boja prvaBoja) const {
        int snaga = (int)vrijednost;

        if (boja == adut) {
            snaga += 100;
            if (vrijednost == Vrijednost::Decko) snaga += 50;
            if (vrijednost == Vrijednost::Devetka) snaga += 40;
        }
        else if (boja != prvaBoja) {
            snaga -= 100;
        }

        return snaga;
    }
};

/* ================= SPIL ================= */

class Spil {
    vector<Karta> karte;

public:
    Spil() {
        for (int b = 0; b < 4; b++) {
            for (int v = 7; v <= 14; v++) {
                karte.emplace_back((Boja)b, (Vrijednost)v);
            }
        }
    }

    void mijesaj() {
        shuffle(karte.begin(), karte.end(), mt19937(random_device{}()));
    }

    Karta izvadi() {
        Karta k = karte.back();
        karte.pop_back();
        return k;
    }
};

/* ================= IGRAC ================= */

class Igrac {
protected:
    string ime;
    vector<Karta> ruka;

public:
    Igrac(string i) : ime(i) {}
    virtual ~Igrac() {}

    void primiKartu(Karta k) { ruka.push_back(k); }
    void ocistiRuku() { ruka.clear(); }

    vector<Karta>& dohvatiRuku() { return ruka; }
    string dohvatiIme() const { return ime; }

    bool imaBoju(Boja b) const {
        for (const auto& k : ruka)
            if (k.boja == b) return true;
        return false;
    }

    bool imaAdut(Boja a) const {
        for (const auto& k : ruka)
            if (k.boja == a) return true;
        return false;
    }

    virtual Karta igrajKartu(bool prvi, Boja trazena, Boja adut) = 0;
};

/* ================= COVJEK ================= */

class CovjekIgrac : public Igrac {
public:
    CovjekIgrac(string i) : Igrac(i) {}

    void ispisiRuku() const {
        cout << "\nTvoja ruka:\n";
        for (int i = 0; i < ruka.size(); i++) {
            cout << "[" << i << "] " << ruka[i].formatiranIspis() << "\n";
        }
    }

    Karta igrajKartu(bool prvi, Boja trazena, Boja adut) override {
        int izbor;
        ispisiRuku();

        while (true) {
            cout << "Odaberi kartu (0-" << ruka.size() - 1 << "): ";
            cin >> izbor;

            if (izbor < 0 || izbor >= (int)ruka.size()) {
                cout << "Neispravan izbor.\n";
                continue;
            }

            if (!prvi) {
                if (imaBoju(trazena) && ruka[izbor].boja != trazena) {
                    cout << "GRESKA: Moras baciti kartu u boji "
                         << ispisiBoju(trazena) << "!\n";
                    continue;
                }

                if (!imaBoju(trazena) && imaAdut(adut) && ruka[izbor].boja != adut) {
                    cout << "GRESKA: Moras baciti ADUT!\n";
                    continue;
                }
            }
            break;
        }

        Karta odabrana = ruka[izbor];
        ruka.erase(ruka.begin() + izbor);
        return odabrana;
    }
};

/* ================= AI ================= */

class RacunaloIgrac : public Igrac {
public:
    RacunaloIgrac(string i) : Igrac(i) {}

    Karta igrajKartu(bool prvi, Boja trazena, Boja adut) override {
        int index = -1;

        if (prvi) {
            index = 0;
            for (int i = 1; i < ruka.size(); i++) {
                if (ruka[i].dohvatiSnagu(adut, ruka[i].boja) <
                    ruka[index].dohvatiSnagu(adut, ruka[index].boja)) {
                    index = i;
                }
            }
        }
        else {
            vector<int> dozvoljene;

            if (imaBoju(trazena)) {
                for (int i = 0; i < ruka.size(); i++)
                    if (ruka[i].boja == trazena)
                        dozvoljene.push_back(i);
            }
            else if (imaAdut(adut)) {
                for (int i = 0; i < ruka.size(); i++)
                    if (ruka[i].boja == adut)
                        dozvoljene.push_back(i);
            }
            else {
                for (int i = 0; i < ruka.size(); i++)
                    dozvoljene.push_back(i);
            }

            int snagaNaStolu = -1000;

            int najboljaZaPobjedu = -1;
            int minSnagaZaPobjedu = 100000;

            for (int i : dozvoljene) {
                int s = ruka[i].dohvatiSnagu(adut, trazena);
                if (s > snagaNaStolu && s < minSnagaZaPobjedu) {
                    minSnagaZaPobjedu = s;
                    najboljaZaPobjedu = i;
                }
            }

            if (najboljaZaPobjedu != -1) {
                index = najboljaZaPobjedu;
            }
            else {
                index = dozvoljene[0];
                for (int i : dozvoljene) {
                    if (ruka[i].dohvatiSnagu(adut, trazena) <
                        ruka[index].dohvatiSnagu(adut, trazena)) {
                        index = i;
                    }
                }
            }
        }

        Karta k = ruka[index];
        ruka.erase(ruka.begin() + index);
        return k;
    }
};

/* ================= IGRA ================= */

class Igra {
    vector<Igrac*> igraci;
    int bodoviTim1 = 0;
    int bodoviTim2 = 0;

public:
    Igra() {
        igraci.push_back(new CovjekIgrac("Ti"));
        igraci.push_back(new RacunaloIgrac("AI-1"));
        igraci.push_back(new RacunaloIgrac("AI-2"));
        igraci.push_back(new RacunaloIgrac("AI-3"));
    }

    ~Igra() {
        for (auto ig : igraci)
            delete ig;
    }

    Boja covjekBiraAdut() {
        cout << "\nTVOJE KARTE:\n";
        for (auto& k : igraci[0]->dohvatiRuku())
            cout << k.formatiranIspis() << "\n";

        cout << "\nODABERI ADUT:\n";
        cout << "0 - Srce | 1 - Karo | 2 - Pik | 3 - Zir\n";

        int izbor;
        cin >> izbor;
        return (Boja)izbor;
    }

    Boja aiBiraAdut() {
        int snaga[4] = { 0 };

        for (auto& k : igraci[1]->dohvatiRuku())
            snaga[(int)k.boja] += k.bodoviAdut();

        int najbolji = 0;
        for (int i = 1; i < 4; i++)
            if (snaga[i] > snaga[najbolji])
                najbolji = i;

        return (Boja)najbolji;
    }

    void pokreni() {
        int prviIgrac = 0;

        while (bodoviTim1 < 501 && bodoviTim2 < 501) {
            Spil spil;
            spil.mijesaj();

            for (auto ig : igraci) ig->ocistiRuku();

            for (int i = 0; i < 8; i++)
                for (auto ig : igraci)
                    ig->primiKartu(spil.izvadi());

            Boja adut = (prviIgrac == 0) ? covjekBiraAdut() : aiBiraAdut();
            cout << "\nADUT JE: " << ispisiBoju(adut) << "\n";

            int tkoVodi = prviIgrac;

            for (int s = 0; s < 8; s++) {
                vector<Karta> stol;
                cout << "\n--- STIH " << s + 1 << " ---\n";

                for (int i = 0; i < 4; i++) {
                    int tren = (tkoVodi + i) % 4;
                    bool prvi = (i == 0);
                    Boja trazena = prvi ? adut : stol[0].boja;

                    Karta k = igraci[tren]->igrajKartu(prvi, trazena, adut);
                    cout << igraci[tren]->dohvatiIme()
                         << " igra: " << k.formatiranIspis() << "\n";

                    stol.push_back(k);
                }

                int najjaci = 0;
                int maxSnaga = -1000;

                for (int i = 0; i < 4; i++) {
                    int snaga = stol[i].dohvatiSnagu(adut, stol[0].boja);
                    if (snaga > maxSnaga) {
                        maxSnaga = snaga;
                        najjaci = i;
                    }
                }

                int pobjednik = (tkoVodi + najjaci) % 4;
                cout << ">>> Osvaja: " << igraci[pobjednik]->dohvatiIme() << " <<<\n";

                int bodovi = 0;
                for (auto& k : stol)
                    bodovi += k.dohvatiBodove(adut);

                if (pobjednik % 2 == 0) bodoviTim1 += bodovi;
                else bodoviTim2 += bodovi;

                tkoVodi = pobjednik;
            }

            cout << "\nREZULTAT:\n";
            cout << "Tim 1: " << bodoviTim1 << "\n";
            cout << "Tim 2: " << bodoviTim2 << "\n";

            prviIgrac = (prviIgrac + 1) % 4;
        }

        cout << "\nKRAJ IGRE!\n";
        cout << (bodoviTim1 >= 501 ? "POBJEDIO TIM 1\n" : "POBJEDIO TIM 2\n");
    }
};

int main() {
    srand(time(nullptr));
    Igra bela;
    bela.pokreni();
    return 0;
}
