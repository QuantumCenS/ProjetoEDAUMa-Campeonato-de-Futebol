//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H

#include "core.h"
#include "jogo.h"
#include <iostream>
#include <fstream>

using namespace std;

void menuPrincipal(Equipa* liga, int totalEquipas, Tatica_Plantel& p, Tatica_Plantel& t, string** jornadas, int totalJornadas, string* listaNomes, int totalNomes, string ficheiroLoad);
void menuOperacoesTransferencias();
void menuOperacoesMudarPos(Tatica_Plantel& p);
void menuOperacoesMelhorarQual(Tatica_Plantel& p);
void menuOperacoesTatica();
void menuOperacoesGestao(Tatica_Plantel& p, Equipa& e, int& jornadaAtual);
void menuAlteracoesPlantel(Tatica_Plantel& t, const Tatica_Plantel& p);
void menuGravarEquipa();
void menuCarregarEquipa();
void verJornadas();

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H