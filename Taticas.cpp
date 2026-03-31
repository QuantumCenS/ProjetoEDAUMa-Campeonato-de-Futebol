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
    melhor.qualidade = -1;

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador candidato = p.jogadores[i];

        // Só escolhe se for da posição, tiver mais qualidade, NÃO estiver convocado e NÃO estiver lesionado/castigado
        if (candidato.pos == pos && candidato.qualidade > melhor.qualidade) {
            if (!jogadorJaConvocado(t, candidato.nome) && candidato.jogosLesao == 0 && candidato.jogosCastigo == 0) {
                melhor = candidato;
            }
        }
    }
    return melhor;
}

bool inicializarTatica(Tatica& t, const Plantel& p, int formacaoEscolhida) {
    t.capacidade = 17;
    if (t.jogadores == nullptr) t.jogadores = new Jogador[t.capacidade];
    t.totalAtual = 0;

    int titulosNecessarios[4];
    string nomeTatica;

    if (formacaoEscolhida == 0) {
        titulosNecessarios[0] = 1; titulosNecessarios[1] = 4; titulosNecessarios[2] = 4; titulosNecessarios[3] = 2;
        nomeTatica = "4-4-2";
    } else if (formacaoEscolhida == 1) {
        titulosNecessarios[0] = 1; titulosNecessarios[1] = 4; titulosNecessarios[2] = 3; titulosNecessarios[3] = 3;
        nomeTatica = "4-3-3";
    } else {
        titulosNecessarios[0] = 1; titulosNecessarios[1] = 3; titulosNecessarios[2] = 5; titulosNecessarios[3] = 2;
        nomeTatica = "3-5-2";
    }

    Posicao posicoes[] = {GR, DEF, MED, AVA};
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    // ==========================================================
    // CASO 1: VERIFICAR TITULARES (Obrigatório respeitar posição)
    // ==========================================================
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < titulosNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            } else {
                // FALHOU: O plantel não tem jogadores aptos suficientes para esta posição
                cout << "\n[ERRO] Nao tem jogadores aptos suficientes na posicao " << posTxt[i]
                     << " para formar a tatica " << nomeTatica << "!\n";
                return false; // Retorna falso para obrigar o utilizador a mudar
            }
        }
    }

    // ==========================================================
    // CASO 2: VERIFICAR SUPLENTES (Flexível em caso de falha)
    // ==========================================================
    int suplentesNecessarios[] = {1, 2, 2, 1};
    int vagasVazias = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < suplentesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            } else {
                vagasVazias++;
                cout << "[AVISO] Nao ha suplentes aptos suficientes para a posicao " << posTxt[i] << ". O sistema ira preencher com outros jogadores!\n";
            }
        }
    }

    // Preencher as vagas vazias no banco com qualquer jogador livre (ignora a posição)
    while (vagasVazias > 0 && t.totalAtual < 17) {
        Jogador melhorGeral;
        melhorGeral.qualidade = -1;

        // Procura em todas as posições pelo melhor livre
        for (int i = 0; i < 4; i++) {
            Jogador candidato = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (candidato.qualidade > melhorGeral.qualidade) {
                melhorGeral = candidato;
            }
        }

        if (melhorGeral.qualidade != -1) {
            t.jogadores[t.totalAtual++] = melhorGeral;
            vagasVazias--;
            cout << "    -> Suplente Adaptado: " << melhorGeral.nome << " (" << posTxt[melhorGeral.pos] << ") foi convocado para a vaga.\n";
        } else {
            cout << "[CRITICO] Plantel dizimado! Nao ha mais jogadores disponiveis para sentar no banco.\n";
            break;
        }
    }

    return true; // Tática formada com sucesso
}

void exibirTatica(const Tatica& t) {
    string* posTxt = new string[4];
    posTxt[0] = "GR";
    posTxt[1] = "DEF";
    posTxt[2] = "MED";
    posTxt[3] = "AVA";

    cout << "========================================================================\n";
    cout << "Tatica Default"<< "\n";
    cout << "========================================================================\n";
    cout << "NUM | POS | QUAL | NOME\n";
    cout << "------------------------------------------------------------------------\n";

    for (int i = 0; i < t.totalAtual; i++) {
        Jogador& j = t.jogadores[i];

        printf("%-3d | %-3s | %-4d | %s\n",
               j.numero,
               posTxt[j.pos].c_str(),
               j.qualidade,
               j.nome.c_str());
    }
    cout << "========================================================================\n";

    delete[] posTxt;
}

