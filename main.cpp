//
// Created by Sergio on 3/12/2026.
//
#include <iostream>
#include <ctime>
#include "definicoes.h"

using namespace std;

int main() {
    srand(time(NULL));
    //=======================================================================================================
    const int TOTAL_EQUIPAS = 18;

    // Criar o array dinâmico de 18 objetos Equipa
    auto* liga = new Equipa[TOTAL_EQUIPAS];

    // O índice 0 será sempre o teu EDA FC
    liga[0].nome = "EDA FC";

    // Carregar os nomes das outras 17 equipas a partir do ficheiro
    string* equipas = carregarEquipas("equipas.txt");

    for (int i = 1; i < TOTAL_EQUIPAS; i++) {
        liga[i].nome = equipas[i - 1]; // Preenche com os nomes do ficheiro
    }
    //=======================================================================================================
    int totalNomesDisponiveis = 0;
    string* bancoDeNomes = carregarNomes("nomes.txt", totalNomesDisponiveis);

    if (bancoDeNomes == nullptr) {

        cout << "Erro crítico: Ficheiro nomes.txt não encontrado!" << endl;

        for (int i = 0; i < TOTAL_EQUIPAS; i++) {
            // Só apaga se o plantel tiver sido alocado
            delete[] liga[i].plantel;
        }

        delete[] liga;
        delete[] equipas;
        return 1;
    }

    Plantel meuTime;
    inicializarPlantel(meuTime, bancoDeNomes, totalNomesDisponiveis);

    exibirPlantel(meuTime);

    libertarMemoria(meuTime, bancoDeNomes);
    //=======================================================================================================
    string** jornadas = gerarJornadas(liga[0],"equipas.txt",equipas);

    //for (int i = 0 ; i < 34 ; i++) {
      //  cout<<jornadas[i][0]<<" - "<<jornadas[i][1]<<endl;
    //}
    for (int i = 1; i < 34; i++) {
        Equipa&  h=encontrarEquipa(jornadas[i][0],liga,18);
        Equipa& a=encontrarEquipa(jornadas[i][1],liga,18);
        gerarResultado(h,a);
        //cout << "EDA FC pontos: "<<liga[0].pontos<<endl;
    }
    //=======================================================================================================

    for (int i = 0; i < TOTAL_EQUIPAS; i++) {
        delete[] liga[i].plantel;
    }
    for (int i = 0; i < 34; i++) {
        delete[] jornadas[i];
    }
    delete[] jornadas;
    delete[] liga;
    delete[] equipas;


    //=======================================================================================================
    Equipa mercadoTransferencias;
    listaTranf(mercadoTransferencias);

    // teste lista de transferencias
    adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);
    adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);
    adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);
    adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);

    exibirListaTransf(mercadoTransferencias);

    return 0;
}
