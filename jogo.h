//
// Created by Sergio on 4/3/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H

#include <string>
#include <iostream>
#include "core.h"

using namespace std;

string* carregarEquipas(string f);
void baralhar(string* equipas, int nEquipas);
string** gerarJornadas(Equipa e,string* equipas);
Equipa& encontrarEquipa(string nome, Equipa* liga, int nEquipas);
string gerarResultado(Equipa& h, Equipa& a);

void atualizarRecuperacoes(Plantel& p);
void simularIncidentesPartida(Plantel& t, Plantel& p);
void exibirCastigadosLesionados(const Plantel& p);
void aplicarIncidenteAoPlantel(Plantel& p, int numero, bool lesao, int tempo);


bool inicializarTatica(Plantel& t, Plantel& p);
void exibirTatica(const Plantel& t);


// =======================================================
// GESTÃO DA LISTA DE TRANSFERÊNCIAS
// =======================================================
void listaTranf(Equipa& e);
void adicionarJogLT(Equipa& e, const Plantel& p, string* listaNomes, int totalNomes);
void exibirListaTransf(const Equipa& e);
void preencherAtributosIndependentes(Jogador& novo, const Plantel& p, string* listaNomes, int totalNomes);
void OrdenarPorPos(Jogador* lista, int total);

// =======================================================
// LÓGICA DE CONTRATAÇÕES E TROCAS
// =======================================================
// (A função TaparBuraco foi apagada pois foi otimizada para dentro de ContratarJogador)
void ContratarJogador(Plantel& p, Equipa& e);
void ContratarJogadorBOT(Plantel& p, Equipa& e);
void processarMercadoGlobal(Equipa* liga, int nEquipas, string* bancoNomes, int nNomes);

// =======================================================
// VALIDAÇÕES DE PLANTEL
// =======================================================
int ContarJogNumaPos(Plantel& p, int j);
bool PodeContratarParaPosicao(Plantel& p, int j);

bool gravarEstado(const string& filename, const Equipa& e, const Plantel& p, int jornadaAtual);
bool carregarEstado(const string& filename, Equipa& e, Plantel& p, int& jornadaAtual);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H
