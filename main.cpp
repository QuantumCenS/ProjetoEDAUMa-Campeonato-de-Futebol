#include <iostream>
#include <ctime>
#include "definicoes.h"
#include "Estrutura_campeonato.h"
#include "Inicializacao_Plantel_EDA_FC.h"
#include "Taticas.h"
#include "Lista_Transferências.h"
#include "Castigados_Lesionados.h"
#include "menu.h"

using namespace std;

int main() {
    setbuf(stdout, NULL); // Desativa o buffer do printf

    srand(time(NULL));
    //=========================================================================
    const int TOTAL_EQUIPAS = 18;

    // A tua liga já estava correta (alocação dinâmica de um array de objetos)
    Equipa* liga = new Equipa[TOTAL_EQUIPAS];
    liga[0].nome = "EDA FC";

    string* equipas = carregarEquipas("equipas.txt");
    for (int i = 1; i < TOTAL_EQUIPAS; i++) {
        liga[i].nome = equipas[i - 1]; 
    }

    //=========================================================================
    int totalNomesDisponiveis = 0;
    string* bancoDeNomes = carregarNomes("nomes.txt", totalNomesDisponiveis);

    if (bancoDeNomes == nullptr) {
        cout << "Erro critico: Ficheiro nomes.txt nao encontrado!" << endl;
        delete[] liga;
        return 1;
    }

    // ************************************************************************
    // MUDANÇA AQUI: Criar Plantel e Tatica com alocação dinâmica!
    // ************************************************************************
    Plantel* meuTime = new Plantel;
    Tatica* meuTime2 = new Tatica; 

    // Como agora são pointers, usamos o asterisco (*) para passar a referência
    inicializarPlantel(*meuTime, bancoDeNomes, totalNomesDisponiveis);
    inicializarTatica(*meuTime2, *meuTime, 0);

    //=========================================================================
    string** jornadas = gerarJornadas(liga[0], equipas);
    int totalJornadas = (TOTAL_EQUIPAS - 1) * 2;

    //=========================================================================
    listaTranf(liga[0]);

    //=========================================================================
    // CHAMAR O MENU (Passamos a estrutura desreferenciada com *)
    menuPrincipal(liga, TOTAL_EQUIPAS, *meuTime, *meuTime2, jornadas, totalJornadas, bancoDeNomes, totalNomesDisponiveis);

    //=========================================================================
    //=========================================================================
    // LIMPEZA DE MEMÓRIA DINÂMICA (Apenas no fim!)
    libertarMemoria(*meuTime, bancoDeNomes);

    // --- MUDANÇA IMPORTANTE AQUI: ---
    delete[] meuTime2->jogadores; // Apaga o array interno dos 17 jogadores
    delete[] bancoDeNomes;        // Apaga a base de nomes do ficheiro
    // --------------------------------

    // Libertar os nossos novos pointers principais
    delete meuTime;
    delete meuTime2;

    for (int i = 0; i < TOTAL_EQUIPAS; i++) {
        delete[] liga[i].plantel;
    }

    return 0;
}