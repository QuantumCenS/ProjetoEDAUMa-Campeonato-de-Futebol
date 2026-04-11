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

// TODOS OS MENUS UTILIZADOS
void menuPrincipal(Equipa* liga, int totalEquipas, Plantel& p, Plantel& t, Partida** calendario, string* listaNomes, int totalNomes, string ficheiroLoad = "");
void menuOperacoesTransferencias(Plantel& p, Equipa& e);
void menuOperacoesMudarPos(Plantel& p);
void menuOperacoesMelhorarQual(Plantel& p);
void menuTatica(Plantel& t, Plantel& p);
void menuOperacoesGestao(Equipa* liga, int totalEquipas, Partida** calendario, int& modoDeJogo, int& idUser, int& jornadaAtual, Plantel& t);
void menuAlteracoesPlantel(Plantel& t, const Plantel& p);
void gerirLesionadosECastigados(Plantel& p);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H