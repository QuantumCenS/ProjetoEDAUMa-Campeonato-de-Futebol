//
// Created by Sergio on 4/3/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H

#include <string>
#include <iostream>
#include "core.h"

using namespace std;

// INICIALIZACAO
string* carregarEquipas(string f);
void baralhar(string* equipas, int nEquipas);
Equipa& encontrarEquipa(string nome, Equipa* liga, int nEquipas);

// MODO TREINADOR, INCIDENTES
void atualizarRecuperacoes(Plantel& p);
void simularIncidentesPartida(Plantel& t, Plantel& p);
void exibirCastigadosLesionados(const Plantel& p);
void aplicarIncidenteAoPlantel(Plantel& p, int numero, bool lesao, int tempo);

// TATICA
bool inicializarTatica(Plantel& t, Plantel& p);
void exibirTatica(const Plantel& t);


// GESTÃO DA LISTA DE TRANSFERÊNCIAS
void listaTranf(Equipa& e);
void adicionarJogLT(Equipa& e, const Plantel& p, string* listaNomes, int totalNomes);
void exibirListaTransf(const Equipa& e);
void preencherAtributosIndependentes(Jogador& novo, const Plantel& p, string* listaNomes, int totalNomes);
void OrdenarPorPos(Jogador* lista, int total);

// LÓGICA DE CONTRATAÇÕES E TROCAS
void ContratarJogador(Plantel& p, Equipa& e);
void ContratarJogadorBOT(Plantel& p, Equipa& e);
void processarMercadoGlobal(Equipa* liga, int nEquipas, string* bancoNomes, int nNomes, int idUser, bool ehGlobal);


// VALIDAÇÕES DE PLANTEL
int ContarJogNumaPos(Plantel& p, int j);
bool PodeContratarParaPosicao(Plantel& p, int j);

// GRAVAR E CARREGAR
bool gravarEstadoGlobal(const string& filename, Equipa* liga, int totalEquipas, Partida** calendario, int jornadaAtual, int modoDeJogo, int idUser);
bool carregarEstadoGlobal(const string& filename, Equipa* liga, int totalEquipas, Partida** calendario, int& jornadaAtual, int& modoDeJogo, int& idUser);

// SIMULAR O CAMPEONATO
Partida** gerarCalendarioCompleto();
void simularJornadaCompleta(Partida** cal, int jornadaAtual, Equipa* liga, int idUserToIgnore = -1);
bool compararEquipas(const Equipa& a, const Equipa& b);
void exibirClassificacao(Equipa* liga, int totalEquipas);
void simularIncidentesBOT(Plantel& p);
bool inicializarTaticaAutomatica(Plantel& t, Plantel& p);
void prepararTaticasJornada(Equipa* liga, int totalEquipas, int idUserModoTreinador, bool modoGlobal);
void ListarJogadorBOT(Equipa& e);
#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H