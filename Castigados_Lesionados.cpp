//
// Created by Sergio on 3/20/2026.
//

#include "Inicializacao_Plantel_EDA_FC.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include "Estrutura_campeonato.h"
#include "Castigados_Lesionados.h"
#include "Taticas.h"
#include "Lista_Transferências.h"
#include "Castigados_Lesionados.h"
#include <iostream>

using namespace std;

// Reduz 1 jogo de castigo/lesão a quem está de fora
void atualizarRecuperacoes(Plantel& p) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao > 0) p.jogadores[i].jogosLesao--;
        if (p.jogadores[i].jogosCastigo > 0) p.jogadores[i].jogosCastigo--;
    }
}

// Aplica incidentes da jornada ao plantel real
void aplicarIncidenteAoPlantel(Plantel& p, int numero, bool lesao, int tempo) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == numero) {
            if (lesao) p.jogadores[i].jogosLesao = tempo;
            else p.jogadores[i].jogosCastigo = tempo;
            break;
        }
    }
}

void simularIncidentesPartida(Tatica& t, Plantel& p) {
    cout << "Substituicoes:\n";
    bool houveSubs = false;
    int substituicoesFeitas = 0; // LIMITADOR DE 3 SUBSTITUIÇÕES

    for (int i = 0; i < 11; i++) {
        Jogador& titular = t.jogadores[i];

        int sorteLesao = gerarAleatorio(1, 100);
        int sorteCastigo = gerarAleatorio(1, 100);

        bool sofreuLesao = (sorteLesao <= titular.probLesao);
        bool sofreuCastigo = (sorteCastigo <= titular.probCastigo);

        if (sofreuLesao || sofreuCastigo) {
            if (sofreuLesao) aplicarIncidenteAoPlantel(p, titular.numero, true, gerarAleatorio(1, 3));
            if (sofreuCastigo) aplicarIncidenteAoPlantel(p, titular.numero, false, gerarAleatorio(1, 2));

            if (substituicoesFeitas < 3) {
                int idxSuplente = -1;

                // 1. TENTATIVA: Procurar suplente da MESMA posição
                for (int j = 11; j < t.totalAtual; j++) {
                    if (t.jogadores[j].pos == titular.pos && t.jogadores[j].qualidade > -1) {
                        idxSuplente = j;
                        break;
                    }
                }

                // 2. TENTATIVA: Se não houver da mesma, procura QUALQUER suplente
                if (idxSuplente == -1) {
                    for (int j = 11; j < t.totalAtual; j++) {
                        if (t.jogadores[j].qualidade > -1) {
                            idxSuplente = j;
                            break;
                        }
                    }
                }

                // 3. Efetuar substituição ou penalizar
                if (idxSuplente != -1) {
                    Jogador& suplente = t.jogadores[idxSuplente];
                    cout << titular.nome << " -> " << suplente.nome << "\n";
                    suplente.qualidade = -1; // Marca o suplente como usado no banco
                    substituicoesFeitas++;
                    houveSubs = true;
                } else {
                    cout << titular.nome << " (Saiu, mas ja nao ha suplentes! Equipa joga com menos 1)\n";
                    houveSubs = true;
                }
            } else {
                // Se já fez as 3 substituições
                cout << titular.nome << " (Saiu, mas o limite de 3 substituicoes foi atingido! Equipa joga com menos 1)\n";
                houveSubs = true;
            }
        }
    }
    if (!houveSubs) cout << "Nenhuma\n";
}

void exibirCastigadosLesionados(const Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "\nCastigados:\n";
    cout << "Nome                 | N  | Posicao | Idade | ProbLesao | ProbCastigo | Qualidade | JogosCastigo\n";
    cout << "------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosCastigo > 0) {
            Jogador& j = p.jogadores[i];
            printf("%-20s | %-2d | %-7s | %-5d | %-8d%% | %-10d%% | %-9d | %d\n",
                   j.nome.c_str(), j.numero, posTxt[j.pos], j.idade, j.probLesao, j.probCastigo, j.qualidade, j.jogosCastigo);
        }
    }

    cout << "\nLesionados:\n";
    cout << "Nome                 | N  | Posicao | Idade | ProbLesao | ProbCastigo | Qualidade | JogosLesao\n";
    cout << "------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao > 0) {
            Jogador& j = p.jogadores[i];
            printf("%-20s | %-2d | %-7s | %-5d | %-8d%% | %-10d%% | %-9d | %d\n",
                   j.nome.c_str(), j.numero, posTxt[j.pos], j.idade, j.probLesao, j.probCastigo, j.qualidade, j.jogosLesao);
        }
    }
}