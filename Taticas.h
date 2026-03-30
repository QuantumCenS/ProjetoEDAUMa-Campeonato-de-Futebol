//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H

#include "Inicializacao_Plantel_EDA_FC.h"


struct Tatica {
    Jogador* jogadores = nullptr; // <-- CRUCIAL: Impede o "lixo" de memória
    int capacidade = 0;
    int totalAtual = 0;
};

bool inicializarTatica(Tatica& t, const Plantel& p, int formacaoEscolhida);
void exibirTatica(const Tatica& t);
bool jogadorJaConvocado(const Tatica& t, const string& nomeJogador);


#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H