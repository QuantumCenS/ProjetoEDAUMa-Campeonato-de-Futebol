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

    libertarMemoria(meuTime, bancoDeNomes);
    return 0;
}