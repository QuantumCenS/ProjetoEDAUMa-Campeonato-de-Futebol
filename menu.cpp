//
// Created by Sergio on 3/22/2026.
//
#include <iostream>
#include "menu.h"
#include "Inicializacao_Plantel_EDA_FC.h"
#include "Lista_Transferências.h"
#include "Castigados_Lesionados.h"

using namespace std;

void definirTatica(Tatica& t, const Plantel& p, int formacaoInicial) {
    bool sucesso = inicializarTatica(t, p, formacaoInicial);

    // Se a tática não for possível, pede intervenção do operador
    while (!sucesso) {
        cout << "\n[!] O SISTEMA INFORMA: O operador DEVE alterar a tatica devido a lesoes/castigos!\n";
        cout << "Escolha uma nova formacao para a equipa:\n";
        cout << "0 -> 4-4-2\n";
        cout << "1 -> 4-3-3\n";
        cout << "2 -> 3-5-2\n";
        cout << "Opcao: ";

        int novaOpcao;
        cin >> novaOpcao;

        if (novaOpcao >= 0 && novaOpcao <= 2) {
            sucesso = inicializarTatica(t, p, novaOpcao);
        } else {
            cout << "Opcao invalida. Tente novamente.\n";
        }
    }
}

void menuPrincipal(Equipa* liga, int totalEquipas, Plantel& p, Tatica& t, string** jornadas, int totalJornadas, string* listaNomes, int totalNomes) {
    char opcao;
    bool campeonatoAtivo = true;
    int jornadaAtual = 1;
    string ultimoResultado = ""; // Guarda o texto do resultado anterior

    Equipa& edaFC = liga[0];

    // ANTES: inicializarTatica(t, p, 0);
    // AGORA: usamos a nossa função robusta
    definirTatica(t, p, 0);


    while (campeonatoAtivo) {
        // --- CONDIÇÃO DE FIM DE CAMPEONATO ---
        if (jornadaAtual > 34) {
            cout << "\n***************************************************************\n";
            cout << "* FIM DO CAMPEONATO! EDA FC terminou com " << edaFC.pontos << " pontos. *\n";
            cout << "***************************************************************\n";
            break; // Sai do ciclo e termina o jogo
        }

        cout << "\n* " << edaFC.nome << " - " << jornadaAtual << "a Jornada - " << edaFC.pontos << " pontos. *\n";
        cout << "***************************************************************\n";

        // Exibir tudo da jornada anterior
        if (jornadaAtual > 1) {
            cout << "Resultado Anterior\n";
            cout << ultimoResultado << "\n";

            exibirTatica(t); // Tática que efetivamente jogou
            simularIncidentesPartida(t, p);

            cout << "\n***************************************************************\n";
            cout << "Plantel Disponivel: *******************************************\n";

            int novaFormacao = gerarAleatorio(0, 2);
            // ANTES: inicializarTatica(t, p, novaFormacao);
            // AGORA: Tenta usar a aleatória; se falhar, interrompe e avisa o operador
            definirTatica(t, p, novaFormacao);
        }

        // --- ESTADO DO PLANTEL ATUAL E TRANSFERÊNCIAS ---
        exibirPlantel(p);
        exibirCastigadosLesionados(p);
        exibirListaTransf(edaFC);

        cout << "\n(s)seguinte (o)coes (q)sair: ";
        cin >> opcao;

        switch (opcao) {
            case 's':
            case 'S':
                {
                    // --- 1. GERAR GOLOS E TEXTO DA PARTIDA ---
                    // Como a matriz começa em 0, a jornadaAtual 1 está no índice 0
                    string equipaCasa = jornadas[jornadaAtual - 1][0];
                    string equipaFora = jornadas[jornadaAtual - 1][1];

                    int golosEDA = gerarAleatorio(0, 8);
                    int golosAdv = gerarAleatorio(0, 8);

                    // Formatar a string consoante jogamos em casa ou fora
                    if (equipaCasa == edaFC.nome) {
                        ultimoResultado = "Resultado : " + equipaCasa + ":" + to_string(golosEDA) + " - " + equipaFora + ":" + to_string(golosAdv);
                    } else {
                        ultimoResultado = "Resultado : " + equipaCasa + ":" + to_string(golosAdv) + " - " + equipaFora + ":" + to_string(golosEDA);
                    }

                    // --- 2. ATUALIZAR PONTOS ---
                    if (golosEDA > golosAdv) edaFC.pontos += 3;
                    else if (golosEDA == golosAdv) edaFC.pontos += 1;

                    // --- 3. PREPARAR A PRÓXIMA JORNADA ---
                    atualizarRecuperacoes(p);
                    jornadaAtual++;
                    adicionarJogLT(edaFC, p, listaNomes, totalNomes);
                }
                break;

            case 'o':
            case 'O':
                menuOperacoesGestao();
                break;

            case 'q':
            case 'Q':
                campeonatoAtivo = false;
                break;

            default:
                cout << "\n[!] Opcao invalida. Usa 's', 'o' ou 'q'.\n";
                break;
        }
    }
}

void menuOperacoesGestao() {
    cout << "[Em construcao] Menu Gestao...\n";
}

void menuOperacoesTransferencias() {
    cout << "[Em construcao] Menu Transferencias...\n";
}

void menuOperacoesMudarPos() {
    cout << "[Em construcao] Menu Mudar Posicao...\n";
}

void menuOperacoesMelhorarQual() {
    cout << "[Em construcao] Menu Melhorar Qualidade...\n";
}

void menuOperacoesTatica() {
    cout << "[Em construcao] Menu Tatica...\n";
}

void menuAlteracoesPlantel() {
    cout << "[Em construcao] Menu Alteracoes Plantel...\n";
}

void menuGravarEquipa() {
    cout << "[Em construcao] A gravar equipa...\n";
}

void menuCarregarEquipa() {
    cout << "[Em construcao] A carregar equipa...\n";
}

void verJornadas() {
    cout << "[Em construcao] A ver calendario...\n";
}