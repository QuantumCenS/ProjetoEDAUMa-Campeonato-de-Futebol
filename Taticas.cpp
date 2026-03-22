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


bool numeroJaExiste2(Tatica& t, int num) {
    for (int i = 0; i < t.totalAtual; i++) {
        if (t.jogadores[i].numero == num) return true;
    }
    return false;
}

Jogador encontrarMelhorDisponivel(const Plantel& p, const Tatica& t, Posicao pos) {
    Jogador melhor;
    melhor.qualidade = -1; // Valor inicial baixo para garantir que encontramos alguém

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador candidato = p.jogadores[i];

        // Verifica se é da posição certa e se é melhor que o anterior encontrado
        if (candidato.pos == pos && candidato.qualidade > melhor.qualidade) {
            // Verifica se NÃO está já convocado
            if (!jogadorJaConvocado(t, candidato.nome)) {
                // NOTA: Aqui mais tarde podes adicionar a verificação de lesões!
                melhor = candidato;
            }
        }
    }
    return melhor;
}

void inicializarTatica(Tatica& t, const Plantel& p) {
    t.capacidade = 17; // 11 Titulares + 6 Suplentes
    t.jogadores = new Jogador[t.capacidade];
    t.totalAtual = 0;

    // 1. ESCOLHER OS TITULARES (1 GR, 4 DEF, 4 MED, 2 AVA)
    int titulosNecessarios[] = {1, 4, 4, 2};
    Posicao posicoes[] = {GR, DEF, MED, AVA};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < titulosNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) { // Se encontrou alguém válido
                t.jogadores[t.totalAtual++] = escolhido;
            }
        }
    }

    // 2. ESCOLHER OS SUPLENTES (1 GR, 2 DEF, 2 MED, 1 AVA)
    int suplentesNecessarios[] = {1, 2, 2, 1};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < suplentesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            }
        }
    }
}

void exibirTatica(const Tatica& t) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "========================================================================\n";
    cout << "Tatica Default"<< "\n";
    cout << "========================================================================\n";
    cout << "NUM | POS | QUAL | NOME\n";
    cout << "------------------------------------------------------------------------\n";

    for (int i = 0; i < t.totalAtual; i++) {
        Jogador& j = t.jogadores[i];
        // O printf agora tem apenas 4 formatadores (%d, %s, %d, %s)
        // que correspondem exatamente às 4 variáveis passadas.
        printf("%-3d | %-3s | %-4d | %s\n",
               j.numero, posTxt[j.pos], j.qualidade, j.nome.c_str());
    }
    cout << "========================================================================\n";
}

