//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H

#include "Inicializacao_Plantel_EDA_FC.h"
#include "Taticas.h"


void menuPrincipal(Equipa* liga, int totalEquipas, Plantel& p, Tatica& t, string** jornadas, int totalJornadas, string* listaNomes, int totalNomes);
void menuOperacoesTransferencias();
void menuOperacoesMudarPos();
void menuOperacoesMelhorarQual();
void menuOperacoesTatica();
void menuOperacoesGestao();
void menuAlteracoesPlantel();
void menuGravarEquipa();
void menuCarregarEquipa();
void verJornadas();

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H