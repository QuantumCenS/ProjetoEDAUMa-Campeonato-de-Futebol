#include <iostream>
#include <ctime>
#include "definicoes.h"

using namespace std;

int main() {
    srand(time(NULL));

    int nNomes = 0;
    string* bancoNomes = carregarNomes("nomes.txt", nNomes);
    if (!bancoNomes) {
        cout << "Erro: Nao foi possivel carregar nomes.txt" << endl;
        return 1;
    }

    int nEquipasAtivas = 0;
    // Esta função agora escolhe apenas 17 aleatórias + EDA FC
    Equipa* liga = criarLiga("equipas.txt", bancoNomes, nNomes, nEquipasAtivas);

    int opcao;
    do {
        cout << "\n========== MENU DO CAMPEONATO ==========\n";
        cout << "1. Ver Calendario do EDA FC (34 Jornadas)\n";
        cout << "2. Ver Plantel de uma Equipa (Escolher entre as 18)\n";
        cout << "0. Sair\n";
        cout << "Escolha: ";
        cin >> opcao;

        if (opcao == 1) {
            gerarCalendarioEDAFC(liga, nEquipasAtivas);
        }
        else if (opcao == 2) {
            cout << "\n--- LISTA DE EQUIPAS NO CAMPEONATO ---\n";
            for (int i = 0; i < nEquipasAtivas; i++) {
                cout << i << ". " << liga[i].nome << endl;
            }
            cout << "Escolha o numero da equipa: ";
            int id;
            cin >> id;

            if (id >= 0 && id < nEquipasAtivas) {
                exibirPlantel(liga[id]);
            } else {
                cout << "ID Invalido! Escolha entre 0 e " << nEquipasAtivas - 1 << endl;
            }
        }
    } while (opcao != 0);

    // Limpeza de memória
    for (int i = 0; i < nEquipasAtivas; i++) {
        libertarPlantel(liga[i].plantel);
    }
    delete[] liga;
    delete[] bancoNomes;

    return 0;
}