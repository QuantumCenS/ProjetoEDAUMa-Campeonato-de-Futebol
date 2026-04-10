#include <iostream>
#include <ctime>
#include <cstdlib>
#include "definicoes.h"
#include "Estrutura_campeonato.h"
#include "Inicializacao_Plantel_EDA_FC.h"
#include "Taticas.h"
#include "Lista_Transferências.h"
#include "Castigados_Lesionados.h"
#include "Treinos.h" 

using namespace std;

int main() {
    srand(time(NULL));
    
    const int TOTAL_EQUIPAS = 18;

    // Criar o array dinâmico de 18 objetos Equipa
    auto* liga = new Equipa[TOTAL_EQUIPAS];

    // O índice 0 será sempre o EDA FC
    liga[0].nome = "EDA FC";

    // Carregar os nomes das outras 17 equipas a partir do ficheiro
    string* equipas = carregarEquipas("equipas.txt");

    for (int i = 1; i < TOTAL_EQUIPAS; i++) {
        liga[i].nome = equipas[i - 1]; // Preenche com os nomes do ficheiro
    }
   
    int totalNomesDisponiveis = 0;
    string* bancoDeNomes = carregarNomes("nomes.txt", totalNomesDisponiveis);

    if (bancoDeNomes == nullptr) {
        cout << "Erro crítico: Ficheiro nomes.txt nao encontrado!" << endl;
        for (int i = 0; i < TOTAL_EQUIPAS; i++) {
            delete[] liga[i].plantel;
        }
        delete[] liga;
        delete[] equipas;
        return 1;
    }

    Plantel minhaequipa;
    Tatica minhaequipa2; 
    Equipa mercadoTransferencias;

    inicializarPlantel(minhaequipa, bancoDeNomes, totalNomesDisponiveis);
    inicializarTatica(minhaequipa2, minhaequipa);
    listaTranf(mercadoTransferencias);

    // Apresentação inicial
    exibirPlantel(minhaequipa);
    exibirTatica(minhaequipa2);

   
    string** jornadas = gerarJornadas(liga[0], equipas);
    int totalJornadas = (TOTAL_EQUIPAS - 1) * 2; // 34 jornadas

    // O CICLO DO CAMPEONATO
    for (int i = 0; i < totalJornadas; i++) {
        cout << "\n========================================================================\n";
        cout << "                       INICIO DA JORNADA " << i + 1 << "\n";
        cout << "========================================================================\n";
        
        // Simular o jogo do EDA FC
        if (jornadas[i] != nullptr) {
            Equipa& h = encontrarEquipa(jornadas[i][0], liga, TOTAL_EQUIPAS);
            Equipa& a = encontrarEquipa(jornadas[i][1], liga, TOTAL_EQUIPAS);
            gerarResultado(h, a);
        }

        // Processar treinos no fim da jornada (evolui qualidade, reduz semanas)
        processarTreinos(minhaequipa);

        // Atualizar lista de transferências (2 novos jogadores por jornada)
        adicionarJogLT(mercadoTransferencias, bancoDeNomes, totalNomesDisponiveis);

        
        int opcao;
        do {
            cout << "\n--- MENU DE GESTAO ---\n";
            cout << "1. Ver Plantel\n";
            cout << "2. Ver Tatica Atual\n";
            cout << "3. Treino Especifico (Mudar Posicao / Qualidade)\n";
            cout << "4. Ver Lista de Transferencias\n";
            cout << "5. Alterar Tatica\n"; // NOVA OPÇÃO AQUI
            cout << "0. Avancar para a proxima jornada\n";
            cout << "Escolha uma opcao: ";
            cin >> opcao;

            switch (opcao) {
                case 1:
                    exibirPlantel(minhaequipa);
                    break;
                case 2:
                    exibirTatica(minhaequipa2);
                    break;
                case 3: {
                    int numJogador, tipoTreino;
                    cout << "Numero da camisola do Jogador: ";
                    cin >> numJogador;
                    cout << "1 - Mudar Posicao | 2 - Melhorar Qualidade: ";
                    cin >> tipoTreino;

                    if (tipoTreino == 1) {
                        int novaPos;
                        cout << "Nova Posicao (0-GR, 1-DEF, 2-MED, 3-AVA): ";
                        cin >> novaPos;
                        mudarPosicao(minhaequipa, numJogador, static_cast<Posicao>(novaPos));
                    } else if (tipoTreino == 2) {
                        int semanas;
                        cout << "Quantas semanas de treino (1 a 5)? ";
                        cin >> semanas;
                        iniciarTreinoQualidade(minhaequipa, numJogador, semanas);
                    } else {
                        cout << "Opcao invalida.\n";
                    }
                    break;
                }
                case 4:
                    exibirListaTransf(mercadoTransferencias);
                    break;
                case 5: //NOVO BLOCO AQUI
                    alterarTatica(minhaequipa2, minhaequipa);
                    exibirTatica(minhaequipa2); // Mostra a tática atualizada logo a seguir
                    break;
                case 0:
                    cout << "A processar a proxima jornada...\n";
                    break;
                default:
                    cout << "Opcao invalida.\n";
            }
        } while (opcao != 0); // Só avança no ciclo for quando o utilizador escolhe 0
    }

    
    // Limpeza de memória final (Após as 34 jornadas acabarem)
    libertarMemoria(minhaequipa, bancoDeNomes);
    delete[] mercadoTransferencias.ListaTransf; // Não esquecer de limpar a lista de transferências

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

    cout << "\nCampeonato Terminado! Ate a proxima epoca.\n";
    return 0;
}
