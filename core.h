//
// Created by Sergio on 4/3/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CORE_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CORE_H
#include <iostream>
#include <string>

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
    int jogosLesao = 0;
    int jogosCastigo = 0;
    bool jogouHoje = false;
    int semanasTreino = 0;
};

struct Plantel {
    Jogador* jogadores = nullptr; // <-- CRUCIAL
    int capacidade = 0;
    int totalAtual = 0;
    int tatica[4]={1,4,4,2};
};

struct Equipa {
    string nome;
    Plantel* plantel = nullptr;    // <-- INICIALIZAR A NULLPTR
    int nJogadores = 0;           // <-- INICIALIZAR A ZERO
    int pontos = 0;               // <-- INICIALIZAR A ZERO
    int capacidadeLT = 0;
    int totalLT = 0;
    Jogador* ListaTransf = nullptr; // <-- INICIALIZAR A NULLPT
};


bool numeroJaExiste(const Plantel& p, int num, int indiceIgnorado = -1);
int gerarAleatorio(int min, int max);
string* carregarNomes(string filename, int& totalNomes);
void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes);
void libertarMemoria(Plantel& p, string* listaNomes);
void exibirPlantel(const Plantel& p);



#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CORE_H
