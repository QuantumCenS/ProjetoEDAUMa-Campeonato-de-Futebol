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
void gerarResultado(Equipa& h, Equipa& a);

void atualizarRecuperacoes(Plantel& p);
void simularIncidentesPartida(Tatica& t, Plantel& p);
void exibirCastigadosLesionados(const Plantel& p);
void aplicarIncidenteAoPlantel(Plantel& p, int numero, bool lesao, int tempo);


bool inicializarTatica(Tatica& t, const Plantel& p, int formacaoEscolhida);
void exibirTatica(const Tatica& t);
bool jogadorJaConvocado(const Tatica& t, const string& nomeJogador);


// =======================================================
// GESTÃO DA LISTA DE TRANSFERÊNCIAS
// =======================================================
void listaTranf(Equipa& e);
void adicionarJogLT(Equipa& e, const Tatica_Plantel& p, string* listaNomes, int totalNomes);
void exibirListaTransf(const Equipa& e);
void preencherAtributosIndependentes(Jogador& novo, const Tatica_Plantel& p, string* listaNomes, int totalNomes);
void OrdenarPorPos(Jogador* lista, int total);

// =======================================================
// LÓGICA DE CONTRATAÇÕES E TROCAS
// =======================================================
// (A função TaparBuraco foi apagada pois foi otimizada para dentro de ContratarJogador)
void ContratarJogador(Tatica_Plantel& p, Equipa& e);

// =======================================================
// VALIDAÇÕES DE PLANTEL
// =======================================================
int ContarJogNumaPos(Tatica_Plantel& p, int j);
bool PodeContratarParaPosicao(Tatica_Plantel& p, int j);

bool gravarEstado(const string& filename, const Equipa& e, const Tatica_Plantel& p, int jornadaAtual);
bool carregarEstado(const string& filename, Equipa& e, Tatica_Plantel& p, int& jornadaAtual);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H
