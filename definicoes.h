//
// Created by Sergio on 3/12/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H
using namespace std;

enum Posicao { GR = 0, DEF = 1, MED = 2, AVA = 3 };

struct Equipa {
    string nome;
    string* plantel= nullptr;   // Array dinâmico de jogadores
    int nJogadores=0;
    int pontos=0;   // Classificação atualizada
    int lesionados=0;
    int castigados=0;
    // Listas de lesionados, transferências, etc.
};


struct Jogador {
    string nome;
    int numero;
    Posicao pos;
    int idade;
    int probLesao;   // 0 a 15%
    int probCastigo; // 0 a 20%
    int qualidade;   // 0 a 100
};

struct Plantel {
    Jogador* jogadores; // Array dinâmico
    int capacidade;     // Entre 20 e 30
    int totalAtual;
};

// Protótipos das funções
int gerarAleatorio(int min, int max);
string* carregarNomes(string filename, int& totalNomes);
void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes);
void libertarMemoria(Plantel& p, string* listaNomes);
void exibirPlantel(const Plantel& p);
//Estrutura do campeonato
int contaEquipas(string f);
string* carregarEquipas(string f);
void baralhar(string* equipas, int nEquipas);
string** gerarJornadas(Equipa e,string f,string* equipas);
Equipa& encontrarEquipa(string nome, Equipa* liga, int nEquipas);
void gerarResultado(Equipa& h, Equipa& a);


#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H