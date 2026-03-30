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
    int probLesao;
    int probCastigo;
    int qualidade;
    int jogosLesao = 0;   // <-- NOVO: Controla os jogos de fora por lesão
    int jogosCastigo = 0; // <-- NOVO: Controla os jogos de fora por castigo
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
    Jogador* jogadores = nullptr; // <-- CRUCIAL
    int capacidade = 0;
    int totalAtual = 0;
};

int gerarAleatorio(int min, int max);
string* carregarNomes(string filename, int& totalNomes);
void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes);
void libertarMemoria(Plantel& p, string* listaNomes);
void exibirPlantel(const Plantel& p);


#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_INICICALIZAÇÃO_PLANTEL_EDA_FC_H