//
// Created by Sergio on 3/20/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H

#include "Inicializacao_Plantel_EDA_FC.h"

//Estrutura para gerir a tática (convocados titulares e suplentes)

struct Tatica {
    Jogador* jogadores; // Array dinâmico de jogadores convocados
    int capacidade;     // Capacidade total (11 titulares + 6 suplentes = 17)
    int totalAtual;     // Número atual de jogadores na lista
};

// Funções base de tática
void inicializarTatica(Tatica& t, const Plantel& p);
void exibirTatica(const Tatica& t);
bool jogadorJaConvocado(const Tatica& t, const string& nomeJogador);


 //Requisito 4.3: Alteração Tática
 //Permite ao operador definir um novo esquema tático, validando os limites:
 //Pelo menos 1 guarda-redes, 3 defesas, 2 médios e 1 avançado[cite: 117].
 //Total de 11 titulares[cite: 117].
 
bool alterarTatica(Tatica& t, const Plantel& p);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_TATICAS_H
