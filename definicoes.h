//
// Created by Sergio on 3/12/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H

#include <string>

enum Posicao { GR = 0, DEF = 1, MED = 2, AVA = 3 };

struct Jogador {
    std::string nome;
    int numero;
    Posicao pos;
    int idade;
    int probLesao;   // 0 a 15%
    int probCastigo; // 0 a 20%
    int qualidade;   // 0 a 100
};

struct Plantel {
    Jogador* jogadores; // Array dinâmico
    int capacidade;
    int totalAtual;
};

// Protótipos
int gerarAleatorio(int min, int max);
std::string* carregarNomes(std::string filename, int& totalNomes);
void inicializarPlantel(Plantel& p, std::string* listaNomes, int totalNomes);
void exibirPlantel(const Plantel& p);
void libertarMemoria(Plantel& p, std::string* listaNomes);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H