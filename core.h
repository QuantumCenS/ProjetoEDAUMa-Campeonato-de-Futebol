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



/**
 * Gera um numero inteiro aleatorio dentro de um intervalo especifico (inclusive).
 * @param min - Valor minimo do intervalo.
 * @param max - Valor maximo do intervalo.
 * @return Um numero inteiro aleatorio entre min e max.
 */
int gerarAleatorio(int min, int max);

/**
 * Le um ficheiro de texto e carrega nomes para um array dinamico de strings.
 * @param filename - Nome ou caminho do ficheiro a ser lido.
 * @param totalNomes - Referencia para a variavel que guardara a contagem total de nomes lidos.
 * @return Ponteiro para o array dinamico de strings ou nullptr em caso de erro na abertura.
 */
string* carregarNomes(string filename, int& totalNomes);
/**
 * Verifica se um determinado numero de camisola ja esta atribuido a outro jogador no plantel.
 * @param p - Referencia constante para a instancia do plantel.
 * @param num - Numero de camisola a verificar.
 * @param indiceIgnorado - Indice do jogador atual para evitar a validacao contra si proprio.
 * @return Booleano 'true' se o numero ja existir, 'false' se estiver disponivel.
 */
bool numeroJaExiste(const Plantel& p, int num, int indiceIgnorado);

/**
 * Cria e configura o plantel inicial da equipa, sorteando a quantidade de jogadores
 * por posicao e gerando os seus atributos aleatorios base.
 * @param p - Referencia para a instancia do plantel a ser inicializada.
 * @param listaNomes - Array de strings com o banco de nomes disponiveis.
 * @param totalNomes - Numero total de nomes no banco para sorteio.
 */
void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes);
/**
 * Apresenta no ecra uma tabela formatada com todos os jogadores do plantel e os seus atributos.
 * @param p - Referencia constante para a instancia do plantel a exibir.
 */
void exibirPlantel(const Plantel& p);

/**
 * Liberta a memoria dinamica alocada para os jogadores do plantel, prevenindo fugas de memoria (memory leaks).
 * @param p - Referencia para a instancia do plantel cuja memoria sera libertada.
 * @param listaNomes - Ponteiro para o array de nomes (nao utilizado nesta versao, mas mantido por assinatura).
 */
void libertarMemoria(Plantel& p, string* listaNomes);



#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CORE_H
