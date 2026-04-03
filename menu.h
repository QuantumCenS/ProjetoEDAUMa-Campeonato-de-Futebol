//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H

#include "core.h"
#include "jogo.h"

void menuPrincipal(Equipa* liga, int totalEquipas, Plantel& p, Tatica& t, string** jornadas, int totalJornadas, string* listaNomes, int totalNomes);
void menuOperacoesTransferencias();
void menuOperacoesMudarPos(Tatica_Plantel& p);
void menuOperacoesMelhorarQual(Tatica_Plantel& p);
void menuOperacoesTatica();
void menuOperacoesGestao(Tatica_Plantel& p, Equipa& e);
void menuAlteracoesPlantel();
void menuGravarEquipa();
void menuCarregarEquipa();
void verJornadas();

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H