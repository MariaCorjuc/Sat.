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


bool aplicaRezolutia(Clauza clauze[], int &nr_clauze) {
    int indexNou = nr_clauze;
    const int MAX_ITERATII = 1000;
    const int MAX_CLAUZE_NOI = 300;
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
    bool rezultat = aplicaRezolutia(clauze, nr_clauze);
    auto sfarsit = high_resolution_clock::now();
    duration<double> durata = sfarsit - inceput;

    cout << "Rezolutie: ";
    if (rezultat)
        cout << "SAT";
    else
        cout << "UNSAT";
    cout << endl;
    cout << "Timp executie: " << fixed << setprecision(12) << durata.count() << " secunde\n";

    return 0;
}
