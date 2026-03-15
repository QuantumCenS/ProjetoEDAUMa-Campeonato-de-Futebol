#include <iostream>
#include <fstream>
#include <iomanip>
#include "definicoes.h"

using namespace std;

int gerarAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

string* carregarNomes(string filename, int& totalNomes) {
    ifstream file(filename);
    if (!file.is_open()) return nullptr;
    string linha;
    totalNomes = 0;
    while (getline(file, linha)) if (!linha.empty()) totalNomes++;

    string* nomes = new string[totalNomes];
    file.clear(); file.seekg(0);
    int i = 0;
    while (getline(file, linha)) {
        if (!linha.empty()) { nomes[i] = linha; i++; }
    }
    file.close();
    return nomes;
}

void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes) {
    p.capacidade = gerarAleatorio(20, 30);
    p.jogadores = new Jogador[p.capacidade];
    p.totalAtual = 0;

    int nGR = 2, nDEF = 7, nMED = 7, nAVA = 4;
    int soma = 20;

    while (soma < p.capacidade) {
        int r = gerarAleatorio(0, 3);
        if (r == 0 && nGR < 3) { nGR++; soma++; }
        else if (r == 1 && nDEF < 10) { nDEF++; soma++; }
        else if (r == 2 && nMED < 10) { nMED++; soma++; }
        else if (r == 3 && nAVA < 7) { nAVA++; soma++; }
    }

    Posicao ordem[] = { GR, DEF, MED, AVA };
    int quantidades[] = { nGR, nDEF, nMED, nAVA };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < quantidades[i]; j++) {
            Jogador& jog = p.jogadores[p.totalAtual];
            jog.nome = listaNomes[gerarAleatorio(0, totalNomes - 1)];
            jog.pos = ordem[i];
            jog.numero = gerarAleatorio(1, 99);
            jog.idade = gerarAleatorio(18, 38);
            jog.probLesao = gerarAleatorio(0, 15);   // Regra: 0-15%
            jog.probCastigo = gerarAleatorio(0, 20); // Regra: 0-20%
            jog.qualidade = gerarAleatorio(0, 100);
            p.totalAtual++;
        }
    }
}

void exibirPlantel(const Equipa& e) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};
    cout << "\nEQUIPA: " << e.nome << " | JOGADORES: " << e.plantel.totalAtual << endl;
    cout << "NUM | POS | IDADE | QUAL | LESAO | CAST | NOME\n";
    for (int i = 0; i < e.plantel.totalAtual; i++) {
        Jogador& j = e.plantel.jogadores[i];
        printf("%-3d | %-3s | %-5d | %-4d | %-4d%% | %-3d%% | %s\n",
               j.numero, posTxt[j.pos], j.idade, j.qualidade, j.probLesao, j.probCastigo, j.nome.c_str());
    }
}

void libertarPlantel(Plantel& p) {
    delete[] p.jogadores;
}