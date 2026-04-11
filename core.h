#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CORE_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CORE_H
#include <iostream>
#include <string>

using namespace std;

// NUMERA AS POSICOES PARA ORGANIZACAO
enum Posicao { GR = 0, DEF = 1, MED = 2, AVA = 3 };

// STRUCTS PRECISAS PARA O PROJETO
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
    Jogador* jogadores = nullptr;
    int capacidade = 0;
    int totalAtual = 0;
    int tatica[4]={1,4,4,2};
};

struct Equipa {
    string nome;
    Plantel* plantel = nullptr;
    int nJogadores = 0;
    int pontos = 0;

    int vitorias = 0;
    int empates = 0;
    int derrotas = 0;
    int golosMarcados = 0;
    int golosSofridos = 0;

    int capacidadeLT = 0;
    int totalLT = 0;
    Jogador* ListaTransf = nullptr;
};

struct Partida {
    int idCasa = -1;  // Índice da equipa da casa no array 'liga'
    int idFora = -1;  // Índice da equipa visitante no array 'liga'
    int golosCasa = -1;
    int golosFora = -1;
    bool realizada = false;
};

// FUNCOES NECESSÁRIAS PARA A PRIMEIRA JORNADA E INICIALIZACAO DO PLANTEL
bool numeroJaExiste(const Plantel& p, int num, int indiceIgnorado = -1);
int gerarAleatorio(int min, int max);
string* carregarNomes(string filename, int& totalNomes);
void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes);
void libertarMemoria(Plantel& p, string* listaNomes);
void exibirPlantel(const Plantel& p);



#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CORE_H