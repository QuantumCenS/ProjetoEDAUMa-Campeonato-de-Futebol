//
// Created by Sergio on 4/3/2026.
//

#include "core.h"
#include <iostream>
#include <fstream>
#include <string>


using namespace std;


int gerarAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

string* carregarNomes(string filename, int& totalNomes) {
    ifstream file(filename);
    if (!file.is_open()) return nullptr;

    string linha;
    totalNomes = 0;
    while (getline(file, linha)) {
        if (!linha.empty()) totalNomes++;
    }

    string* nomes = new string[totalNomes];
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


bool jogadorJaConvocado(const Tatica& t, const string& nomeJogador) {
    for (int i = 0; i < t.totalAtual; i++) {
        if (t.jogadores[i].nome == nomeJogador) {
            return true;
        }
    }
    return false;
}

bool numeroJaExiste(const Plantel& p, int num, int indiceIgnorado) {
    for (int i = 0; i < p.totalAtual; i++) {

        // Se estivermos a olhar para a posição do próprio jogador que estamos a avaliar, saltamos!
        if (i == indiceIgnorado) {
            continue;
        }

        // Se encontrarmos o número noutro jogador, devolvemos true (já existe)
        if (p.jogadores[i].numero == num) {
            return true;
        }
    }
    return false; // Percorreu tudo e o número está livre
}

void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes) {
    p.capacidade = gerarAleatorio(20, 30);
    p.jogadores = new Jogador[p.capacidade];
    p.totalAtual = 0;

    int nGR = 2, nDEF = 7, nMED = 7, nAVA = 4;
    int soma = nGR + nDEF + nMED + nAVA;

    while (soma < p.capacidade) {
        int r = gerarAleatorio(0, 3);
        if (r == 0 && nGR < 3) { nGR++; soma++; }
        else if (r == 1 && nDEF < 10) { nDEF++; soma++; }
        else if (r == 2 && nMED < 10) { nMED++; soma++; }
        else if (r == 3 && nAVA < 7) { nAVA++; soma++; }
    }

    int quantidades[] = { nGR, nDEF, nMED, nAVA };
    Posicao posicoes[] = { GR, DEF, MED, AVA };
    int contadorCamisola = 1; // Variável para numerar de 1 a 30 de forma sequencial

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < quantidades[i]; j++) {
            Jogador& novo = p.jogadores[p.totalAtual];

            novo.nome = listaNomes[gerarAleatorio(0, totalNomes - 1)];
            novo.pos = posicoes[i];
            novo.idade = gerarAleatorio(17, 38);
            novo.probLesao = gerarAleatorio(0, 15);
            novo.probCastigo = gerarAleatorio(0, 20);
            novo.qualidade = gerarAleatorio(0, 100);

            // Regra da imagem: Número de jogadores 1-30 ordenando sequencialmente
            novo.numero = contadorCamisola++;

            p.totalAtual++;
        }
    }
}

void exibirPlantel(const Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "========================================================================\n";
    cout << "PLANTEL GERADO - Capacidade: " << p.capacidade << "\n";
    cout << "========================================================================\n";
    cout << "NUM | POS | IDADE | QUAL | LESAO | CAST | NOME\n";
    cout << "------------------------------------------------------------------------\n";

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador& j = p.jogadores[i];
        printf("%-3d | %-3s | %-5d | %-4d | %-4d%% | %-3d%% | %s\n",
        j.numero,
        posTxt[j.pos],
        j.idade,
        j.qualidade,
        j.probLesao,
        j.probCastigo,
        j.nome.c_str());
    }
    cout << "========================================================================\n";
}

void libertarMemoria(Plantel& p, string* listaNomes) {
    delete[] p.jogadores;
}
