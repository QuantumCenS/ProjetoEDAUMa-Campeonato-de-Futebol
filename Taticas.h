//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H

#include "Inicializacao_Plantel_EDA_FC.h"


struct Tatica {
    Jogador* jogadores;
    int capacidade;
    int totalAtual;
};

void inicializarTatica(Tatica& t, const Plantel& p);
void exibirTatica(const Tatica& t);
bool jogadorJaConvocado(const Tatica& t, const string& nomeJogador);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H