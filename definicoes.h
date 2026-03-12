//
// Created by Sergio on 3/12/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H

enum Posicao { GR = 0, DEF = 1, MED = 2, AVA = 3 };

struct Jogador {
    std::string nome;
    Posicao pos;
};

struct Plantel {
    Jogador* jogadores; // Array dinâmico
    int capacidade;     // Entre 20 e 30
    int totalAtual;
};

// Protótipos das funções
int gerarAleatorio(int min, int max);
std::string* carregarNomes(std::string filename, int& totalNomes);
void inicializarPlantel(Plantel& p, std::string* listaNomes, int totalNomes);
void libertarMemoria(Plantel& p, std::string* listaNomes);
void exibirPlantel(const Plantel& p);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H