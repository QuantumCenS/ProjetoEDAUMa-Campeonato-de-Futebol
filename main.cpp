#include <iostream>
#include <ctime>
#include <cstdlib> // Necessário para o system()
#include "core.h"
#include "jogo.h"
#include "menu.h"

using namespace std;

/**
 * Ponto de entrada principal do simulador de campeonato de futebol
 * Esta função configura o ambiente de execução (UTF-8 e semente aleatória), carrega os dados base de equipas e nomes,
 * inicializa os plantéis e táticas de todas as equipas da liga, gera o calendário completo de 34 jornadas
 * e invoca o menu principal para a interação com o utilizador. No final da execução, garante a libertação
 * rigorosa de toda a memória dinâmica alocada (equipas, plantéis, jogadores e calendário).
 *
 * @param argc - contador de argumentos passados através da linha de comandos.
 * @param argv - vetor de strings contendo os argumentos da linha de comandos (ex: nome de um ficheiro para carregamento imediato).
 * @return - inteiro indicando o estado de terminação do programa (0 para sucesso, 1 em caso de erro crítico no carregamento).
 */
int main(int argc, char* argv[]) {
    setbuf(stdout, NULL);
    // Isto resolve os nomes estranhos como "Nicol├¡s", forçando o Windows a usar UTF-8
    system("chcp 65001 > nul");
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

    string ficheiroLoad = "";
    if (argc >= 2) {
        ficheiroLoad = argv[1];
    }

    int totalNomesDisponiveis = 0;
    string* bancoDeNomes = carregarNomes("nomes.txt", totalNomesDisponiveis);

    if (bancoDeNomes == nullptr) {
        cout << "Erro critico: Ficheiro nomes.txt nao encontrado!" << endl;
        delete[] liga;
        delete[] equipas;
        return 1;
    }

    Plantel* meuTime = new Plantel;
    Plantel* meuTime2 = new Plantel;

    inicializarPlantel(*meuTime, bancoDeNomes, totalNomesDisponiveis);
    inicializarTatica(*meuTime2, *meuTime);

    liga[0].plantel = meuTime;
    listaTranf(liga[0]);

    for (int i = 1; i < TOTAL_EQUIPAS; i++) {
        liga[i].plantel = new Plantel;
        inicializarPlantel(*liga[i].plantel, bancoDeNomes, totalNomesDisponiveis);
        listaTranf(liga[i]);
    }

    // Novo sistema de Calendário Global
    Partida** calendario = gerarCalendarioCompleto();
    int totalJornadas = 34;
    listaTranf(liga[0]);

    // executa o Menu
    menuPrincipal(liga, TOTAL_EQUIPAS, *meuTime, *meuTime2, calendario, bancoDeNomes, totalNomesDisponiveis, ficheiroLoad);

    // LIMPEZA DE MEMÓRIA DINÂMICA
    for (int i = 0; i < totalJornadas; i++) {
        delete[] calendario[i];
    }
    delete[] calendario;

    for (int i = 0; i < TOTAL_EQUIPAS; i++) {
        delete[] liga[i].ListaTransf;
        if (i > 0) {
            libertarMemoria(*liga[i].plantel, nullptr);
            delete liga[i].plantel;
        }
    }
    delete[] liga;
    delete[] equipas;

    libertarMemoria(*meuTime, bancoDeNomes);
    delete[] meuTime2->jogadores;
    delete meuTime;
    delete meuTime2;

    return 0;
}