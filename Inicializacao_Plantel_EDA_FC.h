//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_INICICALIZAÇÃO_PLANTEL_EDA_FC_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_INICICALIZAÇÃO_PLANTEL_EDA_FC_H
#include <iostream>

using namespace std;

enum Posicao { GR = 0, DEF = 1, MED = 2, AVA = 3 };

struct Jogador {
    string nome;
    int numero;
    Posicao pos;
    int idade;
    int probLesao;   // 0 a 15%
    int probCastigo; // 0 a 20%
    int qualidade;   // 0 a 100
};


struct Equipa {
    string nome;
    string* plantel = nullptr;    // <-- INICIALIZAR A NULLPTR
    int nJogadores = 0;           // <-- INICIALIZAR A ZERO
    int pontos = 0;               // <-- INICIALIZAR A ZERO
    int capacidadeLT = 0;
    int totalLT = 0;
    Jogador* ListaTransf = nullptr; // <-- INICIALIZAR A NULLPTR
};


struct Plantel {
    Jogador* jogadores; // Array dinâmico
    int capacidade;     // Entre 20 e 30
    int totalAtual;
};

int gerarAleatorio(int min, int max);
string* carregarNomes(string filename, int& totalNomes);
void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes);
void libertarMemoria(Plantel& p, string* listaNomes);
void exibirPlantel(const Plantel& p);


#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_INICICALIZAÇÃO_PLANTEL_EDA_FC_H