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

void gerirLesionadosECastigados(Plantel& p) {
    int opcao = -1;

    while (opcao != 0) {
        cout << "\n=============================================\n";
        cout << "    GESTAO DE LESIONADOS E CASTIGADOS\n";
        cout << "=============================================\n";
        cout << "1 -> Ver lista de jogadores indisponiveis\n";
        cout << "2 -> Aplicar / Aumentar Lesao\n";
        cout << "3 -> Reduzir / Remover Lesao (Curar)\n";
        cout << "4 -> Aplicar / Aumentar Castigo\n";
        cout << "5 -> Reduzir / Remover Castigo \n";
        cout << "0 -> Voltar ao Menu Principal\n";
        cout << "Opcao: ";
        cin >> opcao;

        if (opcao == 1) {
            cout << "\n--- LESIONADOS E CASTIGADOS ---\n";
            bool haIndisponiveis = false;
            for (int i = 0; i < p.totalAtual; i++) {
                if (p.jogadores[i].jogosLesao > 0 || p.jogadores[i].jogosCastigo > 0) {
                    haIndisponiveis = true;
                    cout << "Nº " << p.jogadores[i].numero << " - " << p.jogadores[i].nome
                         << " | Lesao: " << p.jogadores[i].jogosLesao << " jogos"
                         << " | Castigo: " << p.jogadores[i].jogosCastigo << " jogos\n";
                }
            }
            if (!haIndisponiveis) cout << "Plantel 100% saudavel e apto a jogar!\n";

        }
        else if (opcao >= 2 && opcao <= 5) {
            cout << "\nIndique o NUMERO da camisola do jogador: ";
            int numCamisola;
            cin >> numCamisola;

            // Procurar o jogador no plantel
            int indice = -1;
            for (int i = 0; i < p.totalAtual; i++) {
                if (p.jogadores[i].numero == numCamisola) {
                    indice = i;
                    break;
                }
            }

            if (indice == -1) {
                cout << " Jogador com o numero " << numCamisola << " nao encontrado no plantel.\n";
            } else {
                int dias;
                cout << "Quantos jogos deseja alterar para o jogador " << p.jogadores[indice].nome << "? ";
                cin >> dias;

                if (opcao == 2) {
                    p.jogadores[indice].jogosLesao += dias;


                    if (p.jogadores[indice].jogosLesao > 10) {
                        p.jogadores[indice].jogosLesao = 10;
                        cout << "Maximo de 10 jogos lesionado. A lesao foi ajustada para o maximo de 10 jogos.\n";
                    } else {
                        cout << "O jogador sofreu uma lesao e fica de fora por " << p.jogadores[indice].jogosLesao << " jogos!\n";
                    }
                }
                else if (opcao == 3) {
                    p.jogadores[indice].jogosLesao -= dias;
                    if (p.jogadores[indice].jogosLesao < 0) p.jogadores[indice].jogosLesao = 0;
                    cout << "Tempo de lesao reduzido. Faltam " << p.jogadores[indice].jogosLesao << " jogos para recuperar.\n";
                    }
                    else if (opcao == 4) {
                    p.jogadores[indice].jogosCastigo += dias;

                    // Verifica o teto máximo de 10 jogos
                    if (p.jogadores[indice].jogosCastigo > 10) {
                        p.jogadores[indice].jogosCastigo = 10;
                        cout << "Maximo de 10 jogos castigado. O castigo foi ajustado para o maximo de 10 jogos.\n";
                    }
                    else {
                        cout << "O jogador foi castigado e fica suspenso por " << p.jogadores[indice].jogosCastigo << " jogos!\n";
                    }
                    }
                    else if (opcao == 5) {
                        p.jogadores[indice].jogosCastigo -= dias;
                        if (p.jogadores[indice].jogosCastigo < 0) p.jogadores[indice].jogosCastigo = 0;
                        cout << "Castigo reduzido. Faltam " << p.jogadores[indice].jogosCastigo << " jogos de suspensao.\n";
                    }
            }
        }
        else if (opcao != 0) {
            cout << "Opcao invalida!\n";
        }
    }
}

void menuTatica(Plantel& t, Plantel& p) {

    int aptosTotal = 0;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
            aptosTotal++;
        }
    }

    if (aptosTotal < 17) {
        cout << "\n O teu plantel foi dizimado por lesoes e castigos!\n";
        cout << "Apenas tens " << aptosTotal << " jogadores aptos.\n";
        cout << "Reduz os jogos dos castigados ou lesionados.\n";
        gerirLesionadosECastigados(p);
    }

    bool sucesso = false;


    while (!sucesso) {

        cout << "\n=========================================\n";
        cout <<  "O que deseja fazer?\n";
        cout << "1 -> Inserir uma NOVA TATICA\n";
        cout << "2 -> Ir ao DEPARTAMENTO MEDICO (Curar jogadores)\n";
        cout << "=========================================\n";
        cout << "Opcao: ";

        int opcao;
        cin >> opcao;

        if (opcao == 1) {
            int fitGR = 0, fitDEF = 0, fitMED = 0, fitAVA = 0;
            for (int i = 0; i < p.totalAtual; i++) {
                if (p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
                    if (p.jogadores[i].pos == GR) fitGR++;
                    else if (p.jogadores[i].pos == DEF) fitDEF++;
                    else if (p.jogadores[i].pos == MED) fitMED++;
                    else if (p.jogadores[i].pos == AVA) fitAVA++;
                }
            }

            int totalFit = fitGR + fitDEF + fitMED + fitAVA;

            if (fitGR < 1 || fitDEF < 3 || fitMED < 2 || fitAVA < 1 || totalFit < 11) {
                cout << "\n Nao tens jogadores base suficientes.\n";
                cout << "(Aptos -> GR: " << fitGR << ", DEF: " << fitDEF << ", MED: " << fitMED << ", AVA: " << fitAVA << ")\n";
                cout << ">> OBRIGATORIO curar jogadores primeiro.\n";
            } else {
                bool taticaValida = false;
                while (!taticaValida) {
                    cout << "\n--- NOVA TÁTICA ---\n";
                    cout << "Insira o numero de Defesas (Min 3): ";
                    cin >> t.tatica[1];
                    cout << "Insira o numero de Medios (Min 2): ";
                    cin >> t.tatica[2];
                    cout << "Insira o numero de Avancados (Min 1): ";
                    cin >> t.tatica[3];

                    int totalJogadores = t.tatica[0] + t.tatica[1] + t.tatica[2] + t.tatica[3];

                    if (t.tatica[1] < 3) cout << " A equipa deve ter no minimo 3 defesas.\n";
                    else if (t.tatica[2] < 2) cout << " A equipa deve ter no minimo 2 medios.\n";
                    else if (t.tatica[3] < 1) cout << " A equipa deve ter no minimo 1 avancado.\n";
                    else if (totalJogadores != 11) cout << " A tatica inserida resulta em " << totalJogadores << " jogadores. Tem de ser 11!\n";
                    else taticaValida = true;
                }

                sucesso = inicializarTatica(t, p);
            }

        }
        else if (opcao == 2) {
            gerirLesionadosECastigados(p);
            sucesso = inicializarTatica(t, p);
        }
        else {
            cout << "Opcao invalida!\n";
        }
    }
}

void menuOperacoesGestao(Plantel& t, Plantel& p, Equipa& e, int& jornadaAtual) {
    int opcaoGestao;
    do {
        cout << "\n========================================\n";
        cout << "            GESTAO DA EQUIPA            \n";
        cout << "========================================\n";
        cout << "1 -> Mercado de Transferencias\n";
        cout << "2 -> Treino Epecifico: Mudar Posicao\n";
        cout << "3 -> Treino Epecifico: Melhorar Qualidade\n";
        cout << "4 -> Alterar Tatica\n";
        cout << "5 -> Gerir lesionados e castigados\n";
        cout << "6 -> Gravar Estado Atual\n";
        cout << "7 -> Carregar Estado Gravado\n";
        cout << "0 -> Voltar ao Jogo\n";
        cout << "Escolha uma opcao: ";
        cin >> opcaoGestao;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcaoGestao) {
            case 1: menuOperacoesTransferencias(p,e); break;
            case 2: menuOperacoesMudarPos(p); break;
            case 3: menuOperacoesMelhorarQual(p); break;
            case 4: menuTatica(t,p); break;
            case 5: gerirLesionadosECastigados(p); break;
            case 6: {
                cout << "Insira o nome do ficheiro para gravar (ex: save.txt): ";
                string ficheiro; cin >> ficheiro;
                if(gravarEstado(ficheiro, e, p, jornadaAtual)) cout << "[SUCESSO] Jogo gravado!\n";
                else cout << "[ERRO] Falha ao gravar.\n";
                break;
            }
            case 7: {
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

void menuOperacoesMudarPos(Plantel& p) {
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
    Posicao posAtual = p.jogadores[idx].pos;
    
    cout << "O jogador " << p.jogadores[idx].nome << " joga atualmente a " << posTxt[posAtual] << ".\n";
    cout << "Para qual posicao o quer treinar? (0: GR, 1: DEF, 2: MED, 3: AVA): ";
    int novaPos;
    cin >> novaPos;

    if (cin.fail() || novaPos < 0 || novaPos > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[ERRO] Posicao invalida.\n";
        return;
    }

    if (posAtual == novaPos) {
        cout << "[AVISO] O jogador ja atua nessa posicao.\n";
        return;
    }

    // A ÚNICA COISA NOVA ESTÁ AQUI: Impede que o plantel fique sem jogadores suficientes na posição antiga!
    int qtdAtual = ContarJogNumaPos(p, posAtual);
    bool podeSair = true;
    
    if (posAtual == GR && qtdAtual <= 2) podeSair = false;
    if (posAtual == DEF && qtdAtual <= 7) podeSair = false;
    if (posAtual == MED && qtdAtual <= 7) podeSair = false;
    if (posAtual == AVA && qtdAtual <= 4) podeSair = false;

    if (!podeSair) {
        cout << "[ERRO] Treino cancelado! O plantel atingiria um numero critico de jogadores na posicao de " << posTxt[posAtual] << ".\n";
        return;
    }

    // Verifica se a NOVA posição tem espaço (Isto já tu tinhas feito!)
    if (PodeContratarParaPosicao(p, novaPos)) {
        p.jogadores[idx].pos = static_cast<Posicao>(novaPos);
        cout << "[SUCESSO] " << p.jogadores[idx].nome << " atua agora a " << posTxt[novaPos] << "!\n";

        // Organiza a lista para ficar bonito
        OrdenarPorPos(p.jogadores, p.totalAtual);
    } else {
        cout << "[ERRO] Treino cancelado! Ja atingiu o limite maximo de jogadores para a posicao " << posTxt[novaPos] << ".\n";
    }
}

void menuOperacoesMelhorarQual(Plantel& p) {
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

void menuAlteracoesPlantel(Plantel& t, const Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};
    cout << "\n--- ALTERACOES MANUAIS NO 11 INICIAL ---\n";

    // Mostrar jogadores do Plantel que NÃO estão na tática e estão aptos
    cout << "\nDisponiveis no Plantel (Nao Convocados):\n";
    cout << "Nº | Posicao | Qual | Nome\n";
    cout << "--------------------------------------------------\n";
    bool haAptos = false;
    for (int i = 0; i < p.totalAtual; i++) {
        if (!p.jogadores[i].jogouHoje && p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
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


void menuPrincipal(Equipa* liga, int totalEquipas, Plantel& p, Plantel& t, string** jornadas, int totalJornadas, string* listaNomes, int totalNomes, string ficheiroLoad) {
    char opcao;
    bool campeonatoAtivo = true;
    int jornadaAtual = 1;

    string ultimoResultado = ""; // Guarda o texto do resultado anterior

    Equipa& edaFC = liga[0];


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

            if (!inicializarTatica(t,p)) {
                menuTatica(t,p);
            }
            else {
                cout << "Resultado Anterior\n";
                cout << ultimoResultado << "\n";
                exibirTatica(t); // Tática que efetivamente jogou
                simularIncidentesPartida(t, p);
            }
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
                    Equipa&  h=encontrarEquipa(jornadas[jornadaAtual-1][0],liga,18);
                    Equipa& a=encontrarEquipa(jornadas[jornadaAtual-1][1],liga,18);
                    ultimoResultado=gerarResultado(h,a);


                    processarMercadoGlobal(liga, totalEquipas, listaNomes, totalNomes);



                    // --- 3. PREPARAR A PRÓXIMA JORNADA ---
                    atualizarRecuperacoes(p);
                    jornadaAtual++;

                }
                break;
            case 'a':
            case 'A':
                menuAlteracoesPlantel(t, p);
                break;

            case 'o':
            case 'O':
                menuOperacoesGestao(t, p, edaFC, jornadaAtual);
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




void menuOperacoesTransferencias(Plantel& p, Equipa& e) {
    int op = -1;
    while (op != 0) {
        cout << "\n========================================\n";
        cout << "       CENTRO DE TRANSFERENCIAS         \n";
        cout << "========================================\n";
        cout << "1 -> Visualizar Lista de Transferencias\n";
        cout << "2 -> Contratar / Trocar Jogador\n";
        cout << "0 -> Voltar\n";
        cout << "Escolha uma opcao: ";
        cin >> op;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (op == 1) {
            exibirListaTransf(e);
        } else if (op == 2) {
            ContratarJogador(p, e);
        }
    }
}


void verJornadas() {
    cout << "[Em construcao] A ver calendario...\n";
}
