//
// Created by Sergio on 3/12/2026.
//
#include <iostream>
#include <ctime>
#include "definicoes.h"

using namespace std;

int main() {
    srand(time(NULL)); // Inicializa a semente aleatória

    int totalNomesDisponiveis = 0;
    string* bancoDeNomes = carregarNomes("nomes.txt", totalNomesDisponiveis);

    if (bancoDeNomes == nullptr || totalNomesDisponiveis == 0) {
        cout << "Erro: Nao foi possivel ler nomes.txt" << endl;
        return 1;
    }

    Plantel meuPlantel;
    inicializarPlantel(meuPlantel, bancoDeNomes, totalNomesDisponiveis);

    exibirPlantel(meuPlantel);

    // Limpeza de memória obrigatória (arrays dinâmicos)
    libertarMemoria(meuPlantel, bancoDeNomes);

    return 0;
}