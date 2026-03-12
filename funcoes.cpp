//
// Created by Sergio on 3/12/2026.
//
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "definicoes.h"

using namespace std;

int gerarAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Lê o ficheiro e devolve um array dinâmico de strings
string* carregarNomes(string filename, int& totalNomes) {
    ifstream file(filename);
    if (!file.is_open()) return nullptr;

    string linha;
    totalNomes = 0;

    // Primeira passagem: contar quantos nomes existem
    while (getline(file, linha)) {
        if (!linha.empty()) totalNomes++;
    }

    // Alocação dinâmica do array de nomes
    string* nomes = new string[totalNomes];

    // Segunda passagem: ler os nomes para o array
    file.clear();
    file.seekg(0);
    int i = 0;
    while (getline(file, linha)) {
        if (!linha.empty()) {
            nomes[i] = linha;
            i++;
        }
    }
    file.close();
    return nomes;
}

void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes) {
    p.capacidade = gerarAleatorio(20, 30);
    p.jogadores = new Jogador[p.capacidade];
    p.totalAtual = 0;

    // 1. Definir quantidades por posição respeitando limites e a capacidade total
    int nGR = 2, nDEF = 7, nMED = 7, nAVA = 4;
    int somaAtual = nGR + nDEF + nMED + nAVA;

    // Distribuir as vagas restantes aleatoriamente
    while (somaAtual < p.capacidade) {
        int r = gerarAleatorio(0, 3);
        if (r == 0 && nGR < 3) { nGR++; somaAtual++; }
        else if (r == 1 && nDEF < 10) { nDEF++; somaAtual++; }
        else if (r == 2 && nMED < 10) { nMED++; somaAtual++; }
        else if (r == 3 && nAVA < 7) { nAVA++; somaAtual++; }
    }

    // 2. Preencher o array (Sempre na ordem GR -> DEF -> MED -> AVA)
    auto adicionar = [&](int qtd, Posicao pos) {
        for (int i = 0; i < qtd; i++) {
            p.jogadores[p.totalAtual].nome = listaNomes[gerarAleatorio(0, totalNomes - 1)];
            p.jogadores[p.totalAtual].pos = pos;
            p.totalAtual++;
        }
    };

    adicionar(nGR, GR);
    adicionar(nDEF, DEF);
    adicionar(nMED, MED);
    adicionar(nAVA, AVA);
}

void exibirPlantel(const Plantel& p) {
    string posTxt[] = {"GR", "DEF", "MED", "AVA"};
    cout << "CAPACIDADE DO PLANTEL: " << p.capacidade << "\n";
    cout << "------------------------------------------\n";
    for (int i = 0; i < p.totalAtual; i++) {
        cout << "[" << posTxt[p.jogadores[i].pos] << "] " << p.jogadores[i].nome << endl;
    }
}

void libertarMemoria(Plantel& p, string* listaNomes) {
    delete[] p.jogadores;
    delete[] listaNomes;
}

