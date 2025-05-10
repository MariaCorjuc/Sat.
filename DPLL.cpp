#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;

const int MAX_TOTAL_CLAUZE = 300;
const int MAX_LITERAL_PER_CLAUZA = 300;

int genereazaLiteral(int valoareMaxima)
{
    int literal;
    do
    {
        literal = (rand() % (2 * valoareMaxima + 1)) - valoareMaxima;
    }
    while (literal == 0);
    return literal;
}

bool esteValid(int literal, int clauza[], int nrLiterali)
{
    for (int i = 0; i < nrLiterali; i++)
    {
        if (clauza[i] == literal || clauza[i] == -literal)
        {
            return false;
        }
    }
    return true;
}

void genereazaClauza(int clauza[], int& nrLiterali, int valoareMaxima, int dimensiuneMaximaClauza)
{
    nrLiterali = rand() % dimensiuneMaximaClauza + 1;
    if (nrLiterali > MAX_LITERAL_PER_CLAUZA)
    {
        nrLiterali = MAX_LITERAL_PER_CLAUZA;
    }

    for (int j = 0; j < nrLiterali; j++)
    {
        int incercari = 0;
        int literal;
        do
        {
            literal = genereazaLiteral(valoareMaxima);
            incercari++;
            if (incercari > 100)
            {
                nrLiterali = j;
                return;
            }
        }
        while (!esteValid(literal, clauza, j));
        clauza[j] = literal;
    }
}

bool contineLiteral(int clauza[], int nr, int literal)
{
    for (int i = 0; i < nr; i++)
    {
        if (clauza[i] == literal)
            return true;
    }
    return false;
}

int eliminaLiteral(int clauza[], int& nr, int literal)
{
    int clauzaNoua[MAX_LITERAL_PER_CLAUZA];
    int k = 0;
    for (int i = 0; i < nr; i++)
        if (clauza[i] != literal)
            clauzaNoua[k++] = clauza[i];
    for (int i = 0; i < k; i++) clauza[i] = clauzaNoua[i];
    nr = k;
    return k;
}

bool esteClauzaUnitara(int clauza[], int nr)
{
    return nr == 1;
}

bool esteLiteralPur(int literal, int clauze[][MAX_LITERAL_PER_CLAUZA], int nrLiterali[], int total)
{
    bool gasitPozitiv = false, gasitNegativ = false;
    for (int i = 0; i < total; i++)
    {
        for (int j = 0; j < nrLiterali[i]; j++)
        {
            if (clauze[i][j] == literal)
                gasitPozitiv = true;
            if (clauze[i][j] == -literal)
                gasitNegativ = true;
            if (gasitPozitiv && gasitNegativ)
                return false;
        }
    }
    return true;
}

bool dpll(int clauze[][MAX_LITERAL_PER_CLAUZA], int nrLiterali[], int total, int valoareMaxima)
{
    bool gasitUnitara;
    do
    {
        gasitUnitara = false;
        for (int i = 0; i < total; i++)
        {
            if (nrLiterali[i] == 1)
            {
                int literalUnit = clauze[i][0];
                gasitUnitara = true;

                for (int j = 0; j < total; j++)
                {
                    if (nrLiterali[j] >= 0 && contineLiteral(clauze[j], nrLiterali[j], literalUnit))
                    {
                        nrLiterali[j] = -1;
                    }
                }

                for (int j = 0; j < total; j++)
                {
                    if (nrLiterali[j] > 0 && contineLiteral(clauze[j], nrLiterali[j], -literalUnit))
                    {
                        eliminaLiteral(clauze[j], nrLiterali[j], -literalUnit);
                        if (nrLiterali[j] == 0)
                        {
                            return false;
                        }
                    }
                }
                break;
            }
        }
    }
    while (gasitUnitara);

    for (int literal = 1; literal <= valoareMaxima; literal++)
    {
        if (esteLiteralPur(literal, clauze, nrLiterali, total))
        {
            for (int i = 0; i < total; i++)
            {
                if (nrLiterali[i] >= 0 && contineLiteral(clauze[i], nrLiterali[i], literal))
                {
                    nrLiterali[i] = -1;
                }
            }
        }
        if (esteLiteralPur(-literal, clauze, nrLiterali, total))
        {
            for (int i = 0; i < total; i++)
            {
                if (nrLiterali[i] >= 0 && contineLiteral(clauze[i], nrLiterali[i], -literal))
                {
                    nrLiterali[i] = -1;
                }
            }
        }
    }

    bool toateEliminate = true;
    for (int i = 0; i < total; i++)
    {
        if (nrLiterali[i] >= 0)
        {
            toateEliminate = false;
            break;
        }
    }
    if (toateEliminate)
    {
        return true;
    }

    for (int i = 0; i < total; i++)
    {
        if (nrLiterali[i] == 0)
        {
            return false;
        }
    }

    int literalAles = 0;
    for (int i = 0; i < total && literalAles == 0; i++)
    {
        if (nrLiterali[i] > 0)
        {
            literalAles = clauze[i][0];
        }
    }

    int clauzeTemp1[MAX_TOTAL_CLAUZE][MAX_LITERAL_PER_CLAUZA];
    int nrLiteraliTemp1[MAX_TOTAL_CLAUZE];
    int totalNou1 = 0;
    for (int i = 0; i < total; i++)
    {
        if (nrLiterali[i] == -1) continue;
        if (contineLiteral(clauze[i], nrLiterali[i], literalAles)) continue;
        nrLiteraliTemp1[totalNou1] = nrLiterali[i];
        for (int j = 0; j < nrLiterali[i]; j++)
        {
            clauzeTemp1[totalNou1][j] = clauze[i][j];
        }
        if (contineLiteral(clauzeTemp1[totalNou1], nrLiteraliTemp1[totalNou1], -literalAles))
        {
            eliminaLiteral(clauzeTemp1[totalNou1], nrLiteraliTemp1[totalNou1], -literalAles);
            if (nrLiteraliTemp1[totalNou1] == 0)
            {
                return false;
            }
        }
        totalNou1++;
    }
    if (dpll(clauzeTemp1, nrLiteraliTemp1, totalNou1, valoareMaxima))
    {
        return true;
    }

    int clauzeTemp2[MAX_TOTAL_CLAUZE][MAX_LITERAL_PER_CLAUZA];
    int nrLiteraliTemp2[MAX_TOTAL_CLAUZE];
    int totalNou2 = 0;
    for (int i = 0; i < total; i++)
    {
        if (nrLiterali[i] == -1) continue;
        if (contineLiteral(clauze[i], nrLiterali[i], -literalAles)) continue;
        nrLiteraliTemp2[totalNou2] = nrLiterali[i];
        for (int j = 0; j < nrLiterali[i]; j++)
        {
            clauzeTemp2[totalNou2][j] = clauze[i][j];
        }
        if (contineLiteral(clauzeTemp2[totalNou2], nrLiteraliTemp2[totalNou2], literalAles))
        {
            eliminaLiteral(clauzeTemp2[totalNou2], nrLiteraliTemp2[totalNou2], literalAles);
            if (nrLiteraliTemp2[totalNou2] == 0)
            {
                return false;
            }
        }
        totalNou2++;
    }
    return dpll(clauzeTemp2, nrLiteraliTemp2, totalNou2, valoareMaxima);
}


int main()
{
    srand(time(0));
    int valoareMaxima, nrClauzeInitiale;

    cout << "Valoarea maxima de literal: ";
    cin >> valoareMaxima;
    cout << "Numarul de clauze initiale: ";
    cin >> nrClauzeInitiale;

    if (nrClauzeInitiale > MAX_TOTAL_CLAUZE)
    {
        cout << "Numarul de clauze initiale depaseste limita maxima (" << MAX_TOTAL_CLAUZE << ")" << endl;
        return 1;
    }

    int dimensiuneMaximaClauza = 2 * nrClauzeInitiale;
    int clauze[MAX_TOTAL_CLAUZE][MAX_LITERAL_PER_CLAUZA];
    int nrLiterali[MAX_TOTAL_CLAUZE] = {0};

    for (int i = 0; i < nrClauzeInitiale; i++)
    {
        genereazaClauza(clauze[i], nrLiterali[i], valoareMaxima, dimensiuneMaximaClauza);

        if (nrLiterali[i] == 0)
        {
            return 1;
        }
    }

    auto start = chrono::high_resolution_clock::now();
    bool rezultatDPLL = dpll(clauze, nrLiterali, nrClauzeInitiale, valoareMaxima);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> durata = end - start;

    cout << "DPLL: ";
    if (rezultatDPLL)
        cout << "SAT";
    else
        cout << "UNSAT";
    cout << endl;
    cout << "Timpul de executie al DPLL: " << fixed << setprecision(12) << durata.count() << " secunde.\n";
    return 0;
}
