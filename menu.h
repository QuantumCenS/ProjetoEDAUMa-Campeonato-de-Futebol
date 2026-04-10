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

void menuPrincipal(Equipa* liga, int totalEquipas, Plantel& p, Plantel& t, string** jornadas, int totalJornadas, string* listaNomes, int totalNomes, string ficheiroLoad);
void menuOperacoesTransferencias(Plantel& p, Equipa& e);
void menuOperacoesMudarPos(Plantel& p);
void menuOperacoesMelhorarQual(Plantel& p);
void menuTatica(Plantel& t, Plantel& p);
void menuOperacoesGestao(Plantel& p, Equipa& e, int& jornadaAtual);
void menuAlteracoesPlantel(Plantel& t, const Plantel& p);
void menuGravarEquipa();
void menuCarregarEquipa();
void gerirLesionadosECastigados(Plantel& p);
void verJornadas();

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H