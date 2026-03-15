#ifndef DEFINICOES_H
#define DEFINICOES_H

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
    Jogador* jogadores;
    int capacidade;
    int totalAtual;
};

struct Equipa {
    std::string nome;
    Plantel plantel;
    int pontos;
};

// --- Protótipos Essenciais ---
int gerarAleatorio(int min, int max);
std::string* carregarNomes(std::string filename, int& totalNomes);
void inicializarPlantel(Plantel& p, std::string* listaNomes, int totalNomes);
void exibirPlantel(const Equipa& e);
void libertarPlantel(Plantel& p);

// --- Gestão do Campeonato ---
int contaEquipasFicheiro(std::string f);
Equipa* criarLiga(std::string fEquipas, std::string* listaNomes, int totalNomes, int& nTotal);
void gerarCalendarioEDAFC(Equipa* liga, int nTotal);

#endif