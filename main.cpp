#include <iostream>
#include <ctime>
#include "core.h"
#include "jogo.h"
#include "menu.h"

using namespace std;

int main() {
    setbuf(stdout, NULL);
    srand(time(NULL));

    const int TOTAL_EQUIPAS = 18;
    Equipa* liga = new Equipa[TOTAL_EQUIPAS];
    liga[0].nome = "EDA FC";

    string* equipas = carregarEquipas("equipas.txt");
    if (equipas != nullptr) {
        for (int i = 1; i < TOTAL_EQUIPAS; i++) {
            liga[i].nome = equipas[i - 1];
        }
    }

    int totalNomesDisponiveis = 0;
    string* bancoDeNomes = carregarNomes("nomes.txt", totalNomesDisponiveis);

    if (bancoDeNomes == nullptr) {
        cout << "Erro critico: Ficheiro nomes.txt nao encontrado!" << endl;
        delete[] liga;
        delete[] equipas;
        return 1;
    }

    // UTILIZAÇÃO EXPLICITA DA STRUCT Tatica_Plantel
    Tatica_Plantel* meuTime = new Tatica_Plantel;
    Tatica_Plantel* meuTime2 = new Tatica_Plantel;

    inicializarPlantel(*meuTime, bancoDeNomes, totalNomesDisponiveis);
    inicializarTatica(*meuTime2, *meuTime, 0);

    string** jornadas = gerarJornadas(liga[0], equipas);
    int totalJornadas = (TOTAL_EQUIPAS - 1) * 2;

    listaTranf(liga[0]);

    // Executa o Menu
    menuPrincipal(liga, TOTAL_EQUIPAS, *meuTime, *meuTime2, jornadas, totalJornadas, bancoDeNomes, totalNomesDisponiveis);

    // =========================================================================
    // LIMPEZA DE MEMÓRIA DINÂMICA (Evitar Memory Leaks)
    // =========================================================================

    // Limpar Jornadas (Matriz Bidimensional)
    for (int i = 0; i < totalJornadas; i++) {
        delete[] jornadas[i];
    }
    delete[] jornadas;

    // Limpar Equipa
    delete[] liga[0].ListaTransf;
    for (int i = 0; i < TOTAL_EQUIPAS; i++) {
        delete liga[i].plantel;
    }
    delete[] liga;
    delete[] equipas;

    // Limpar Plantéis e Nomes
    libertarMemoria(*meuTime, bancoDeNomes);
    delete[] meuTime2->jogadores;
    delete meuTime;
    delete meuTime2;

    return 0;
}