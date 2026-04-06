//
// Created by Sergio on 3/22/2026.
//
#include <iostream>
#include <limits>
#include <fstream>
#include "menu.h"
#include "core.h"
#include "jogo.h"

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

void menuOperacoesGestao(Tatica_Plantel& p, Equipa& e, int& jornadaAtual) {
    int opcaoGestao;
    do {
        cout << "\n========================================\n";
        cout << "            GESTAO DA EQUIPA            \n";
        cout << "========================================\n";
        cout << "1 -> Mercado de Transferencias\n";
        cout << "2 -> Treino Epecifico: Mudar Posicao\n";
        cout << "3 -> Treino Epecifico: Melhorar Qualidade\n";
        cout << "4 -> Gravar Estado Atual\n";
        cout << "5 -> Carregar Estado Gravado\n";
        cout << "0 -> Voltar ao Jogo\n";
        cout << "Escolha uma opcao: ";
        cin >> opcaoGestao;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcaoGestao) {
            case 1: ContratarJogador(p, e); break;
            case 2: menuOperacoesMudarPos(p); break;
            case 3: menuOperacoesMelhorarQual(p); break;
            case 4: {
                cout << "Insira o nome do ficheiro para gravar (ex: save.txt): ";
                string ficheiro; cin >> ficheiro;
                if(gravarEstado(ficheiro, e, p, jornadaAtual)) cout << "[SUCESSO] Jogo gravado!\n";
                else cout << "[ERRO] Falha ao gravar.\n";
                break;
            }
            case 5: {
                cout << "Insira o nome do ficheiro para carregar (ex: save.txt): ";
                string ficheiro; cin >> ficheiro;
                if(carregarEstado(ficheiro, e, p, jornadaAtual)) cout << "[SUCESSO] Jogo carregado!\n";
                else cout << "[ERRO] Falha ao carregar.\n";
                break;
            }
            case 0: break;
        }
    } while (opcaoGestao != 0);
}

void menuOperacoesMudarPos(Tatica_Plantel& p) {
    cout << "\n--- TREINO: MUDAR POSICAO ---\n";
    cout << "Insira o numero do jogador que pretende treinar para nova posicao (0 para cancelar): ";
    int numJog;
    cin >> numJog;

    if (cin.fail() || numJog == 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int idx = -1;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == numJog) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "[ERRO] Jogador com a camisola " << numJog << " nao encontrado no plantel.\n";
        return;
    }

    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};
    cout << "O jogador " << p.jogadores[idx].nome << " joga atualmente a " << posTxt[p.jogadores[idx].pos] << ".\n";
    cout << "Para qual posicao o quer treinar? (0: GR, 1: DEF, 2: MED, 3: AVA): ";
    int novaPos;
    cin >> novaPos;

    if (cin.fail() || novaPos < 0 || novaPos > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[ERRO] Posicao invalida.\n";
        return;
    }

    if (p.jogadores[idx].pos == novaPos) {
        cout << "[AVISO] O jogador ja atua nessa posicao.\n";
        return;
    }

    // A nossa função de transferências já faz o cálculo se há espaço! Reutilizamos aqui:
    if (PodeContratarParaPosicao(p, novaPos)) {
        p.jogadores[idx].pos = static_cast<Posicao>(novaPos);
        cout << "[SUCESSO] " << p.jogadores[idx].nome << " atua agora a " << posTxt[novaPos] << "!\n";

        // Reorganiza o plantel após a mudança
        OrdenarPorPos(p.jogadores, p.totalAtual);
    } else {
        cout << "[ERRO] Treino cancelado! Ja atingiu o limite maximo de jogadores para a posicao " << posTxt[novaPos] << ".\n";
    }
}

void menuOperacoesMelhorarQual(Tatica_Plantel& p) {
    cout << "\n--- TREINO: MELHORAR QUALIDADE ---\n";
    cout << "Insira o numero do jogador para receber treino especifico (0 para cancelar): ";
    int numJog;
    cin >> numJog;

    if (cin.fail() || numJog == 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int idx = -1;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == numJog) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        cout << "[ERRO] Jogador com a camisola " << numJog << " nao encontrado.\n";
        return;
    }

    // Regra: Jogadores lesionados não podem treinar
    if (p.jogadores[idx].jogosLesao > 0) {
        cout << "[ERRO] Operacao invalida! O jogador " << p.jogadores[idx].nome << " esta lesionado e nao pode treinar.\n";
        return;
    }

    cout << "Quantas semanas (jornadas) durara o treino especifico? (Maximo 5): ";
    int semanas;
    cin >> semanas;

    if (cin.fail() || semanas < 1 || semanas > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[ERRO] O numero de semanas deve ser entre 1 e 5.\n";
        return;
    }

    p.jogadores[idx].semanasTreino = semanas;
    cout << "[SUCESSO] O jogador " << p.jogadores[idx].nome << " foi colocado num programa de treino de " << semanas << " semanas!\n";
    cout << "A cada jornada disputada, a sua qualidade subira em 5 pontos automaticamente.\n";
}

void menuAlteracoesPlantel(Tatica_Plantel& t, const Tatica_Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};
    cout << "\n--- ALTERACOES MANUAIS NA TATICA ---\n";

    // Mostrar jogadores do Plantel que NÃO estão na tática e estão aptos
    cout << "\nDisponiveis no Plantel (Nao Convocados):\n";
    cout << "Nº | Posicao | Qual | Nome\n";
    cout << "--------------------------------------------------\n";
    bool haAptos = false;
    for (int i = 0; i < p.totalAtual; i++) {
        if (!jogadorJaConvocado(t, p.jogadores[i].nome) && p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
            printf("%-2d | %-7s | %-4d | %s\n", p.jogadores[i].numero, posTxt[p.jogadores[i].pos], p.jogadores[i].qualidade, p.jogadores[i].nome.c_str());
            haAptos = true;
        }
    }

    if(!haAptos) cout << "(Nao ha jogadores extra aptos disponiveis)\n";

    cout << "\nInsira o numero do jogador a SAIR da tatica (ou 0 para cancelar): ";
    int numSair; cin >> numSair;
    if(cin.fail() || numSair == 0) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }

    int idxSair = -1;
    for(int i = 0; i < t.totalAtual; i++) if(t.jogadores[i].numero == numSair) idxSair = i;
    if(idxSair == -1) { cout << "[ERRO] Jogador nao encontrado na tatica atual.\n"; return; }

    cout << "Insira o numero do jogador do PLANTEL a ENTRAR: ";
    int numEntrar; cin >> numEntrar;
    if(cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }

    int idxEntrar = -1;
    for(int i = 0; i < p.totalAtual; i++) if(p.jogadores[i].numero == numEntrar) idxEntrar = i;
    if(idxEntrar == -1) { cout << "[ERRO] Jogador nao encontrado no plantel.\n"; return; }

    if(p.jogadores[idxEntrar].jogosLesao > 0 || p.jogadores[idxEntrar].jogosCastigo > 0) {
        cout << "[ERRO] O jogador selecionado esta lesionado ou castigado e nao pode jogar!\n"; return;
    }

    // Fazemos a substituição diretamente na Tática
    string nomeSair = t.jogadores[idxSair].nome;
    t.jogadores[idxSair] = p.jogadores[idxEntrar];

    cout << "\n[SUCESSO] O jogador " << p.jogadores[idxEntrar].nome << " entrou no lugar de " << nomeSair << " para esta partida!\n";
}


void menuPrincipal(Equipa* liga, int totalEquipas, Tatica_Plantel& p, Tatica_Plantel& t, string** jornadas, int totalJornadas, string* listaNomes, int totalNomes, string ficheiroLoad) {
    char opcao;
    bool campeonatoAtivo = true;
    int jornadaAtual = 1;

    string ultimoResultado = ""; // Guarda o texto do resultado anterior

    Equipa& edaFC = liga[0];

    definirTatica(t, p, 0);

    // Carregar jogo pela linha de comandos se o utilizador enviou um argumento!
    if (!ficheiroLoad.empty()) {
        if (carregarEstado(ficheiroLoad, edaFC, p, jornadaAtual)) {
            cout << "\n[!] Estado carregado com sucesso a partir de: " << ficheiroLoad << "\n";
        } else {
            cout << "\n[ERRO] Nao foi possivel carregar o ficheiro: " << ficheiroLoad << ". A iniciar novo jogo.\n";
        }
    }

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

        cout << "\n(s)seguinte (a)lteracoes (o)coes (q)sair: ";
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
            case 'a':
            case 'A':
                menuAlteracoesPlantel(t, p);
                break;

            case 'o':
            case 'O':
                menuOperacoesGestao(p, edaFC, jornadaAtual);
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

void menuGravarEquipa() {
    cout << "[Em construcao] A gravar equipa...\n";
}

void menuCarregarEquipa() {
    cout << "[Em construcao] A carregar equipa...\n";
}

void verJornadas() {
    cout << "[Em construcao] A ver calendario...\n";
}