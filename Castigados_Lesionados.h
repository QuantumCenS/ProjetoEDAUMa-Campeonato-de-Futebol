//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CASTIGADOS_LESIONADOS_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CASTIGADOS_LESIONADOS_H

#include "Taticas.h"

void atualizarRecuperacoes(Plantel& p);
void simularIncidentesPartida(Tatica& t, Plantel& p);
void exibirCastigadosLesionados(const Plantel& p);
void aplicarIncidenteAoPlantel(Plantel& p, int numero, bool lesao, int tempo);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_CASTIGADOS_LESIONADOS_H