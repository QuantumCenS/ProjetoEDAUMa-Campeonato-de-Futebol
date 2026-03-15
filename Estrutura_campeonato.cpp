#include <iostream>
#include <fstream>
#include <string>
#include "definicoes.h"

using namespace std;

// Conta as equipas totais no ficheiro para alocação temporária
int contaEquipasFicheiro(string f) {
    ifstream file(f);
    string s; int n = 0;
    while (getline(file, s)) if (!s.empty()) n++;
    return n;
}

Equipa* criarLiga(string fEquipas, string* listaNomes, int totalNomes, int& nTotal) {
    int nFich = contaEquipasFicheiro(fEquipas);

    // 1. Ler TODAS as equipas do ficheiro para um array temporário para podermos escolher
    string* todasDoFicheiro = new string[nFich];
    ifstream file(fEquipas);
    int idx = 0;
    while (getline(file, todasDoFicheiro[idx]) && idx < nFich) {
        if (!todasDoFicheiro[idx].empty()) idx++;
    }
    file.close();

    // 2. Baralhar o array temporário para garantir aleatoriedade na escolha
    for (int i = nFich - 1; i > 0; i--) {
        int j = gerarAleatorio(0, i);
        string temp = todasDoFicheiro[i];
        todasDoFicheiro[i] = todasDoFicheiro[j];
        todasDoFicheiro[j] = temp;
    }

    // 3. Fixar o limite de 18 equipas no total (17 do ficheiro + EDA FC)
    nTotal = 18;
    Equipa* liga = new Equipa[nTotal];

    // Inicializar EDA FC na posição 0
    liga[0].nome = "EDA FC";
    liga[0].pontos = 0;
    inicializarPlantel(liga[0].plantel, listaNomes, totalNomes);

    // Inicializar as outras 17 equipas (as primeiras 17 do array baralhado)
    for (int i = 1; i < nTotal; i++) {
        liga[i].nome = todasDoFicheiro[i - 1]; // Pega as aleatórias
        liga[i].pontos = 0;
        // Aplica as mesmas regras de plantel (20-30 jogadores, etc)
        inicializarPlantel(liga[i].plantel, listaNomes, totalNomes);
    }

    delete[] todasDoFicheiro; // Limpar array temporário de nomes
    return liga;
}

void gerarCalendarioEDAFC(Equipa* liga, int nTotal) {
    // nTotal é 18, logo temos 17 adversários.
    int nAdversarios = nTotal - 1;
    bool* edaEmCasaPrimeiraVolta = new bool[nAdversarios];

    // Definir mando de campo aleatório para a 1ª volta
    for (int i = 0; i < nAdversarios; i++) {
        edaEmCasaPrimeiraVolta[i] = (gerarAleatorio(0, 1) == 1);
    }

    cout << "\n==============================================";
    cout << "\n   CALENDARIO EDA FC - 34 JORNADAS";
    cout << "\n==============================================\n";

    // PRIMEIRA PARTE: Jornadas 1 a 17
    cout << "\n--- PRIMEIRA VOLTA ---\n";
    for (int i = 0; i < nAdversarios; i++) {
        cout << "Jornada " << (i + 1) << ": ";
        if (edaEmCasaPrimeiraVolta[i])
            cout << "EDA FC (C) vs " << liga[i+1].nome << " (F)" << endl;
        else
            cout << liga[i+1].nome << " (C) vs EDA FC (F)" << endl;
    }

    // SEGUNDA PARTE: Jornadas 18 a 34 (Inversão)
    cout << "\n--- SEGUNDA VOLTA (INVERSAO) ---\n";
    for (int i = 0; i < nAdversarios; i++) {
        cout << "Jornada " << (i + 18) << ": ";
        // Se jogou em casa na volta 1, agora joga fora
        if (!edaEmCasaPrimeiraVolta[i])
            cout << "EDA FC (C) vs " << liga[i+1].nome << " (F)" << endl;
        else
            cout << liga[i+1].nome << " (C) vs EDA FC (F)" << endl;
    }

    delete[] edaEmCasaPrimeiraVolta;
}