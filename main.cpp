//
// Created by Sergio on 3/12/2026.
//
#include <iostream>
#include <ctime>
#include "definicoes.h"
#include "Estrutura_campeonato.h"
#include "Inicializacao_Plantel_EDA_FC.h"
#include "Taticas.h"
#include "Lista_Transferências.h"
#include "Castigados_Lesionados.h"

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
    Tatica meuTime2; // Isto agora vai guardar os Titulares e Suplentes!

    inicializarPlantel(meuTime, bancoDeNomes, totalNomesDisponiveis);

    // AGORA RECEBE O PLANTEL GERADO EM VEZ DO BANCO DE NOMES
    inicializarTatica(meuTime2, meuTime);

    exibirPlantel(meuTime);
    exibirTatica(meuTime2);

    libertarMemoria(meuTime, bancoDeNomes);
    //=======================================================================================================
    string** jornadas = gerarJornadas(liga[0], "equipas.txt", equipas);
    int totalJornadas = (TOTAL_EQUIPAS - 1) * 2; // Calcula que são 34 (se TOTAL for 18)

    // Reparou que o seu ciclo começava em 1? Mudei para 0 para ele jogar a primeira jornada!
    for (int i = 0; i < totalJornadas; i++) {
        // Adicionada segurança para não ler nullptr
        if (jornadas[i] != nullptr) {
            Equipa& h = encontrarEquipa(jornadas[i][0], liga, TOTAL_EQUIPAS);
            Equipa& a = encontrarEquipa(jornadas[i][1], liga, TOTAL_EQUIPAS);
            gerarResultado(h, a);
        }
    }

    //=======================================================================================================
    // Limpeza de Memória (com totalJornadas)
    for (int i = 0; i < TOTAL_EQUIPAS; i++) {
        delete[] liga[i].plantel;
    }
    for (int i = 0; i < totalJornadas; i++) {
        if (jornadas[i] != nullptr) {
            delete[] jornadas[i];
        }
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


    /*cout << "0. Sair " << endl << "o. Operações EDA FC";

    char choice;
    cin >> choice;

    switch (choice) {
        case 0:
            cout << "Até já!\n";
            return 0;
        case 1:
            int choice2;
            cin >> choice2;
            case 0:
                cout << "O. Sair" << endl;
            cout << "1. Plantel" << endl;
            cout << "2. Tatica" << endl;
            cout << "3. Lista de Transferencias" << endl;
            cout << "4. Lista de Castigados" << endl;
            cout << "5. Lista de Lesionados" << endl;

    }
    */

    return 0;
}
