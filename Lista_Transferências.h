//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_LISTA_TRANSFERÊNCIAS_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_LISTA_TRANSFERÊNCIAS_H

#include "Inicializacao_Plantel_EDA_FC.h"

void listaTranf(Equipa& e);
void adicionarJogLT(Equipa& e, string* listaNomes, int totalNomes);
void preencherAtributosIndependentes(Jogador& novo, string* listaNomes, int totalNomes);
void exibirListaTransf(const Equipa& e);
void OrdenarPorPos(Jogador* lista, int total);
void OrdenarPorChegadaLT(Equipa& e);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_LISTA_TRANSFERÊNCIAS_H