//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_LISTA_TRANSFERÊNCIAS_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_LISTA_TRANSFERÊNCIAS_H

#include <iostream>
#include "Inicializacao_Plantel_EDA_FC.h"

using namespace std;


// =======================================================
// GESTÃO DA LISTA DE TRANSFERÊNCIAS
// =======================================================
void listaTranf(Equipa& e);
void adicionarJogLT(Equipa& e, const Plantel& p, string* listaNomes, int totalNomes);
void exibirListaTransf(const Equipa& e);
void preencherAtributosIndependentes(Jogador& novo, const Plantel& p, string* listaNomes, int totalNomes);
void OrdenarPorPos(Jogador* lista, int total);
void OrdenarPorChegadaLT(Equipa& e);

// =======================================================
// LÓGICA DE CONTRATAÇÕES E TROCAS
// =======================================================
bool numeroJaExiste(const Plantel& p, int num, int indiceIgnorado = -1);
void GerarNumeroContratacao(Plantel& p, int indice);
void TaparBuraco(Jogador* lista, int total, int j);
void ContratarJogador(Plantel& p, Equipa& e);

// =======================================================
// VALIDAÇÕES DE PLANTEL
// =======================================================
int ContarJogNumaPos(Plantel& p, int j);
bool PodeContratarParaPosicao(Plantel& p, int j);


#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_LISTA_TRANSFERÊNCIAS_H