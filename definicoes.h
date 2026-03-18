//
// Created by Sergio on 3/12/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H
using namespace std;

enum Posicao { GR = 0, DEF = 1, MED = 2, AVA = 3 };

struct Equipa {
    string nome;
    string* plantel;      // Array dinâmico de jogadores
    int nJogadores;
    int pontos;           // Classificação atualizada
    int capacidadeLT;
    int totalLT;
    Jogador* ListaTransf;
    // Listas de lesionados, etc.
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
void gerarJornadas(string f,string* equipas);
void listaTranf(Equipa& e);
void adicionarJogLT(Equipa& e, string* listaNomes, int totalNomes);
void preencherAtributosIndependentes(Jogador& novo, string* listaNomes, int totalNomes);
void exibirListaTransf(const Equipa& e);
void OrdenarPorPos(Jogador* lista, int total);


#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_DEFINICOES_H
