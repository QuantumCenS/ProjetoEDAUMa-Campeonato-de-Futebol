//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_ESTRUTURA_CAMPEONATO_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_ESTRUTURA_CAMPEONATO_H

#include <iostream>
#include "Inicializacao_Plantel_EDA_FC.h"

using namespace std;


int contaEquipas(string f);
string* carregarEquipas(string f);
void baralhar(string* equipas, int nEquipas);
string** gerarJornadas(Equipa e,string f,string* equipas);
Equipa& encontrarEquipa(string nome, Equipa* liga, int nEquipas);
void gerarResultado(Equipa& h, Equipa& a);
#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_ESTRUTURA_CAMPEONATO_H