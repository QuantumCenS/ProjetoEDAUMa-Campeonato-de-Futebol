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


/**
 * Apresenta o menu interativo para a gestao de indisponiveis, permitindo ao utilizador
 * visualizar, aplicar ou reduzir o tempo de paragem por lesao e castigo.
 * @param p - Referencia para a instancia do plantel a ser gerida.
 */
void gerirLesionadosECastigados(Plantel& p);

/**
 * Interface para a escolha e validacao da tatica de jogo. Verifica se existem jogadores
 * aptos suficientes e permite aceder ao departamento medico se necessario.
 * @param t - Referencia para a instancia da tatica onde a formacao sera guardada.
 * @param p - Referencia para a instancia do plantel para consulta de disponibilidade.
 */
void menuTatica(Plantel& t, Plantel& p);

/**
 * Menu principal de operacoes de gestao, servindo de hub para transferencias,
 * treinos especificos, alteracao de tatica e sistema de gravacao.
 * @param t - Referencia para a tatica.
 * @param p - Referencia para o plantel.
 * @param e - Referencia para a equipa.
 * @param jornadaAtual - Referencia para o contador da jornada atual do jogo.
 */
void menuOperacoesGestao(Plantel& t, Plantel& p, Equipa& e, int& jornadaAtual);

/**
 * Interface dedicada ao treino de conversao de posicao de um jogador,
 * validando os limites de vagas antes de efetivar a mudanca.
 * @param p - Referencia para a instancia do plantel.
 */
void menuOperacoesMudarPos(Plantel& p);

/**
 * Interface para atribucao de programas de treino de qualidade,
 * definindo a duracao e validando se o atleta esta apto para treinar.
 * @param p - Referencia para a instancia do plantel.
 */
void menuOperacoesMelhorarQual(Plantel& p);
/**
 * Menu de substituticoes manuais que permite trocar jogadores entre a tatica
 * (11 inicial/suplentes) e o plantel nao convocado.
 * @param t - Referencia para a tatica atual.
 * @param p - Referencia constante para o plantel global.
 */
void menuAlteracoesPlantel(Plantel& t, const Plantel& p);

/**
 * O ciclo principal do jogo (Game Loop). Gere a progressao das jornadas,
 * exibe o estado da liga e processa as decisoes do utilizador a cada ronda.
 * @param liga - Array de equipas do campeonato.
 * @param totalEquipas - Numero total de equipas na liga.
 * @param p - Plantel do utilizador.
 * @param t - Tatica do utilizador.
 * @param jornadas - Matriz com o calendario de jogos.
 * @param totalJornadas - Numero total de jornadas (34).
 * @param listaNomes - Banco de nomes para o mercado.
 * @param totalNomes - Tamanho do banco de nomes.
 * @param ficheiroLoad - Caminho para ficheiro de carregamento rapido (opcional).
 */
void menuPrincipal(Equipa* liga, int totalEquipas, Plantel& p, Plantel& t, string** jornadas, int totalJornadas, string* listaNomes, int totalNomes, string ficheiroLoad);

/**
 * Menu secundario para o mercado de transferencias, permitindo visualizar a lista
 * ou iniciar o processo de contratacao.
 * @param p - Referencia para o plantel.
 * @param e - Referencia para a equipa.
 */
void menuOperacoesTransferencias(Plantel& p, Equipa& e);
void menuGravarEquipa();
void menuCarregarEquipa();

void verJornadas();

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_MENU_H