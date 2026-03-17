//
// Created by Sergio on 3/12/2026.
//
#include <iostream>
#include <ctime>
#include "definicoes.h"

using namespace std;

int main() {
    srand(time(NULL));

    int totalNomesDisponiveis = 0;
    string* bancoDeNomes = carregarNomes("nomes.txt", totalNomesDisponiveis);

    if (bancoDeNomes == nullptr) {
        cout << "Erro crítico: Ficheiro nomes.txt não encontrado!" << endl;
        return 1;
    }

    Plantel meuTime;
    inicializarPlantel(meuTime, bancoDeNomes, totalNomesDisponiveis);

    exibirPlantel(meuTime);



    Equipa mercadoTransferencias;
    listaTranf(mercadoTransferencias);

    // teste lista de transferencias
    adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);
    adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);
    adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);
    adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);

    exibirListaTransf(mercadoTransferencias);




    

    libertarMemoria(meuTime, bancoDeNomes);
    string* equipas =carregarEquipas("equipas.txt");
    gerarJornadas("equipas.txt",equipas);
    delete[] equipas;
    return 0;
}
