#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace std::chrono;

const int MAX_CLAUZE = 300;
const int MAX_LITERALI = 300;

struct Clauza {
    int literalii[MAX_LITERALI];
    int nr_literali;
};

bool existaDeja(Clauza &clauza, int literal) {
    for (int i = 0; i < clauza.nr_literali; ++i) {
        if (clauza.literalii[i] == literal) return true;
    }
    return false;
}

bool areComplementul(const Clauza &clauza, int literal) {
    for (int i = 0; i < clauza.nr_literali; ++i) {
        if (clauza.literalii[i] == -literal) return true;
    }
    return false;
}

void genereazaClauzeAleatoare(Clauza clauze[], int &nr_clauze, int max_literali, int nr_clauze_input) {
    srand(time(0));
    nr_clauze = nr_clauze_input;

    for (int i = 0; i < nr_clauze; ++i) {
        clauze[i].nr_literali = rand() % max_literali + 1;

        for (int j = 0; j < clauze[i].nr_literali; ) {
            int literal = rand() % max_literali + 1;
            if (rand() % 2) literal = -literal;

            if (!existaDeja(clauze[i], literal) && !areComplementul(clauze[i], literal)) {
                clauze[i].literalii[j] = literal;
                ++j;
            }
        }
    }
}

int gasesteClauzaUnitara(Clauza clauze[], int nr_clauze) {
    for (int i = 0; i < nr_clauze; ++i) {
        if (clauze[i].nr_literali == 1) {
            return clauze[i].literalii[0];
        }
    }
    return 0;
}

void eliminaPrinClauzaUnitara(Clauza clauze[], int &nr_clauze, int literal) {
    int idx = 0;
    for (int i = 0; i < nr_clauze; ++i) {
        bool satisfacuta = false;
        for (int j = 0; j < clauze[i].nr_literali; ++j) {
            if (clauze[i].literalii[j] == literal) {
                satisfacuta = true;
                break;
            }
        }
        if (!satisfacuta) {
            int nr_literali_noi = 0;
            for (int j = 0; j < clauze[i].nr_literali; ++j) {
                if (clauze[i].literalii[j] != -literal) {
                    clauze[i].literalii[nr_literali_noi++] = clauze[i].literalii[j];
                }
            }
            clauze[i].nr_literali = nr_literali_noi;
            clauze[idx++] = clauze[i];
        }
    }
    nr_clauze = idx;
}

int gasesteLiteralPur(Clauza clauze[], int nr_clauze) {
    int frecventa[MAX_LITERALI * 2 + 1] = {0};

    for (int i = 0; i < nr_clauze; ++i) {
        for (int j = 0; j < clauze[i].nr_literali; ++j) {
            int literal = clauze[i].literalii[j];
            frecventa[MAX_LITERALI + literal]++;
        }
    }

    for (int literal = 1; literal <= MAX_LITERALI; ++literal) {
        if (frecventa[MAX_LITERALI + literal] > 0 && frecventa[MAX_LITERALI - literal] == 0) return literal;
        if (frecventa[MAX_LITERALI - literal] > 0 && frecventa[MAX_LITERALI + literal] == 0) return -literal;
    }
    return 0;
}

void eliminaClauzeCuLiteral(Clauza clauze[], int &nr_clauze, int literal) {
    int idx = 0;
    for (int i = 0; i < nr_clauze; ++i) {
        bool gasit = false;
        for (int j = 0; j < clauze[i].nr_literali; ++j) {
            if (clauze[i].literalii[j] == literal) {
                gasit = true;
                break;
            }
        }
        if (!gasit) {
            clauze[idx++] = clauze[i];
        }
    }
    nr_clauze = idx;
}

bool aplicaRezolutia(Clauza clauze[], int &nr_clauze) {
    int indexNou = nr_clauze;
    const int MAX_ITERATII = 1000;
    const int MAX_CLAUZE_NOI = 400;
    int clauzeNoiGenerate = 0;

    for (int iteratii = 0; iteratii < MAX_ITERATII; ++iteratii) {
        bool aplicat = false;
        int total = indexNou;

        for (int i = 0; i < total; ++i) {
            for (int j = i + 1; j < total; ++j) {
                int nrComplementare = 0;
                int litI = -1, litJ = -1;

                for (int li = 0; li < clauze[i].nr_literali; ++li) {
                    for (int lj = 0; lj < clauze[j].nr_literali; ++lj) {
                        if (clauze[i].literalii[li] == -clauze[j].literalii[lj]) {
                            nrComplementare++;
                            litI = li;
                            litJ = lj;
                        }
                    }
                }

                if (nrComplementare == 1) {
                    Clauza clauzaNoua;
                    clauzaNoua.nr_literali = 0;

                    for (int k = 0; k < clauze[i].nr_literali; ++k) {
                        if (k != litI && !existaDeja(clauzaNoua, clauze[i].literalii[k])) {
                            clauzaNoua.literalii[clauzaNoua.nr_literali++] = clauze[i].literalii[k];
                        }
                    }
                    for (int k = 0; k < clauze[j].nr_literali; ++k) {
                        if (k != litJ && !existaDeja(clauzaNoua, clauze[j].literalii[k])) {
                            clauzaNoua.literalii[clauzaNoua.nr_literali++] = clauze[j].literalii[k];
                        }
                    }

                    if (clauzaNoua.nr_literali == 0) return false;

                    bool exista = false;
                    for (int k = 0; k < indexNou; ++k) {
                        if (clauze[k].nr_literali == clauzaNoua.nr_literali) {
                            bool identice = true;
                            for (int l = 0; l < clauzaNoua.nr_literali; ++l) {
                                if (!existaDeja(clauze[k], clauzaNoua.literalii[l])) {
                                    identice = false;
                                    break;
                                }
                            }
                            if (identice) {
                                exista = true;
                                break;
                            }
                        }
                    }

                    if (!exista && indexNou < MAX_CLAUZE && clauzeNoiGenerate < MAX_CLAUZE_NOI) {
                        clauze[indexNou] = clauzaNoua;
                        indexNou++;
                        clauzeNoiGenerate++;
                        aplicat = true;
                    }
                }
            }
        }

        if (!aplicat) break;
    }

    nr_clauze = indexNou;
    return true;
}

void afiseazaClauze(Clauza clauze[], int nr_clauze) {
    for (int i = 0; i < nr_clauze; ++i) {
        cout << "Clauza " << i + 1 << ": ";
        if (clauze[i].nr_literali == 0) {
            cout << "{}";
        }
        else {
            for (int j = 0; j < clauze[i].nr_literali; ++j) {
                cout << clauze[i].literalii[j] << " ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

bool algoritmDP(Clauza clauze[], int &nr_clauze) {
    while (true) {
        for (int i = 0; i < nr_clauze; ++i) {
            if (clauze[i].nr_literali == 0) return false;
        }
        if (nr_clauze == 0) return true;

        bool modificat = false;
        while (true) {
            int literalUnitar = gasesteClauzaUnitara(clauze, nr_clauze);
            if (literalUnitar == 0) break;
            eliminaPrinClauzaUnitara(clauze, nr_clauze, literalUnitar);
            modificat = true;
        }
        if (modificat) continue;

        while (true) {
            int literalPur = gasesteLiteralPur(clauze, nr_clauze);
            if (literalPur == 0) break;
            eliminaClauzeCuLiteral(clauze, nr_clauze, literalPur);
            modificat = true;
        }
        if (modificat) continue;

        int inainte = nr_clauze;
        if (!aplicaRezolutia(clauze, nr_clauze)) return false;
        if (nr_clauze == inainte) break;
    }
    return true;
}

int main() {
    int max_literali, nr_clauze_input;
    Clauza clauze[MAX_CLAUZE];
    int nr_clauze;

    cout << "Introduceti numarul de clauze: ";
    cin >> nr_clauze_input;
    cout << "Introduceti numarul maxim de literali: ";
    cin >> max_literali;

    genereazaClauzeAleatoare(clauze, nr_clauze, max_literali, nr_clauze_input);

    auto inceput = high_resolution_clock::now();
    bool rezultat = algoritmDP(clauze, nr_clauze);
    auto sfarsit = high_resolution_clock::now();
    duration<double> durata = sfarsit - inceput;

    cout << "DP: ";
    if (rezultat)
        cout << "SAT";
    else
        cout << "UNSAT";
    cout << endl;
    cout << "Timp executie: " << fixed << setprecision(12) << durata.count() << " secunde\n";

    return 0;
}
