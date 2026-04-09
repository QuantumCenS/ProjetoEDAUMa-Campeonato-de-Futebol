#include "Inicializacao_Plantel_EDA_FC.h"
#include <iostream>

using namespace std;

//4.1.1 Mudar a Posição: Altera a posição garantindo os limites do plantel.
//Retorna true se foi possível mudar, false se violar as regras.
 
bool mudarPosicao(Plantel& p, int numeroJogador, Posicao novaPos) {
    int idx = -1;
    int contagem[4] = {0, 0, 0, 0}; // Contadores para GR, DEF, MED, AVA

    //Procurar o jogador e contar quantos existem de cada posição no plantel
    for (int i = 0; i < p.totalAtual; i++) {
        contagem[p.jogadores[i].pos]++;
        if (p.jogadores[i].numero == numeroJogador) {
            idx = i;
        }
    }

    if (idx == -1) {
        cout << "Erro: Jogador com o numero " << numeroJogador << " nao encontrado.\n";
        return false; 
    }

    Posicao posAtual = p.jogadores[idx].pos;
    if (posAtual == novaPos) {
        cout << "O jogador ja joga nessa posicao.\n";
        return false; 
    }

    //Verificar limites mínimos da posição atual (não pode descer abaixo do mínimo)
    if (posAtual == GR && contagem[GR] <= 2) return false;
    if (posAtual == DEF && contagem[DEF] <= 7) return false;
    if (posAtual == MED && contagem[MED] <= 7) return false;
    if (posAtual == AVA && contagem[AVA] <= 4) return false;

    //Verificar limites máximos da nova posição (não pode exceder o máximo)
    if (novaPos == GR && contagem[GR] >= 3) return false;
    if (novaPos == DEF && contagem[DEF] >= 10) return false;
    if (novaPos == MED && contagem[MED] >= 10) return false;
    if (novaPos == AVA && contagem[AVA] >= 7) return false;

    //Se passou em todas as verificações, altera a posição!
    p.jogadores[idx].pos = novaPos;
    cout << "Sucesso! O jogador " << p.jogadores[idx].nome << " mudou de posicao.\n";
    return true;
}


//4.1.2 Melhorar a Qualidade: Define quantas semanas o jogador vai treinar.
 
bool iniciarTreinoQualidade(Plantel& p, int numeroJogador, int semanas) {
    if (semanas < 1 || semanas > 5) {
        cout << "Erro: O treino deve ser entre 1 e 5 semanas.\n";
        return false;
    }

    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == numeroJogador) {
            //Se ele está na lista do Plantel, assumimos que não está lesionado (visto que os lesionados vão para uma lista à parte segundo o enunciado).
            p.jogadores[i].semanasTreino = semanas;
            cout << "Treino especifico iniciado para " << p.jogadores[i].nome << " por " << semanas << " semanas.\n";
            return true;
        }
    }
    
    cout << "Erro: Jogador nao encontrado.\n";
    return false;
}

//Função para ser chamada A CADA JORNADA para atualizar a qualidade dos jogadores em treino.
void processarTreinos(Plantel& p) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].semanasTreino > 0) {
            p.jogadores[i].qualidade += 5;
            
            // Garantir que a qualidade não ultrapassa os 100
            if (p.jogadores[i].qualidade > 100) {
                p.jogadores[i].qualidade = 100;
            }
            
            p.jogadores[i].semanasTreino--; // Reduz uma semana de treino
        }
    }
}
