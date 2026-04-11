#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
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
        cout << "0 -> Voltar ao Menu\n";
        cout << "Opcao: ";
        cin >> opcao;

        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

        if (opcao == 1) {
            cout << "\n--- LESIONADOS E CASTIGADOS ---\n";
            bool haIndisponiveis = false;
            for (int i = 0; i < p.totalAtual; i++) {
                if (p.jogadores[i].jogosLesao > 0 || p.jogadores[i].jogosCastigo > 0) {
                    haIndisponiveis = true;
                    cout << "Nº " << p.jogadores[i].numero << " - " << p.jogadores[i].nome
                         << " | Lesao: " << p.jogadores[i].jogosLesao << " jogos | Castigo: " << p.jogadores[i].jogosCastigo << " jogos\n";
                }
            }
            if (!haIndisponiveis) cout << "Plantel 100% saudavel e apto a jogar!\n";
        }
        else if (opcao >= 2 && opcao <= 5) {
            cout << "\nIndique o NUMERO da camisola do jogador: ";
            int numCamisola; cin >> numCamisola;

            int indice = -1;
            for (int i = 0; i < p.totalAtual; i++) {
                if (p.jogadores[i].numero == numCamisola) { indice = i; break; }
            }

            if (indice == -1) cout << "Jogador nao encontrado.\n";
            else {
                int dias;
                cout << "Quantos jogos deseja alterar para " << p.jogadores[indice].nome << "? ";
                cin >> dias;

                if (opcao == 2) {
                    p.jogadores[indice].jogosLesao += dias;
                    if (p.jogadores[indice].jogosLesao > 10) p.jogadores[indice].jogosLesao = 10;
                    cout << "Lesao ajustada para " << p.jogadores[indice].jogosLesao << " jogos!\n";
                } else if (opcao == 3) {
                    p.jogadores[indice].jogosLesao -= dias;
                    if (p.jogadores[indice].jogosLesao < 0) p.jogadores[indice].jogosLesao = 0;
                    cout << "Faltam " << p.jogadores[indice].jogosLesao << " jogos para recuperar.\n";
                } else if (opcao == 4) {
                    p.jogadores[indice].jogosCastigo += dias;
                    if (p.jogadores[indice].jogosCastigo > 10) p.jogadores[indice].jogosCastigo = 10;
                    cout << "Castigo ajustado para " << p.jogadores[indice].jogosCastigo << " jogos!\n";
                } else if (opcao == 5) {
                    p.jogadores[indice].jogosCastigo -= dias;
                    if (p.jogadores[indice].jogosCastigo < 0) p.jogadores[indice].jogosCastigo = 0;
                    cout << "Faltam " << p.jogadores[indice].jogosCastigo << " jogos de suspensao.\n";
                }
            }
        }
    }
}

void menuTatica(Plantel& t, Plantel& p) {
    int aptosTotal = 0;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) aptosTotal++;
    }

    if (aptosTotal < 17) {
        cout << "\nO teu plantel foi dizimado! Apenas tens " << aptosTotal << " jogadores aptos.\n";
        gerirLesionadosECastigados(p);
    }

    bool sucesso = false;
    while (!sucesso) {
        cout << "\n=========================================\n";
        cout << "1 -> Inserir uma NOVA TATICA\n";
        cout << "2 -> Ir ao DEPARTAMENTO MEDICO (Curar jogadores)\n";
        cout << "Opcao: ";
        int opcao; cin >> opcao;

        if (opcao == 1) {
            bool taticaValida = false;
            while (!taticaValida) {
                cout << "\n--- NOVA TATICA ---\n";
                cout << "Insira o numero de Defesas (Min 3): "; cin >> t.tatica[1];
                cout << "Insira o numero de Medios (Min 2): "; cin >> t.tatica[2];
                cout << "Insira o numero de Avancados (Min 1): "; cin >> t.tatica[3];

                int totalJogadores = t.tatica[0] + t.tatica[1] + t.tatica[2] + t.tatica[3];
                if (t.tatica[1] < 3 || t.tatica[2] < 2 || t.tatica[3] < 1 || totalJogadores != 11) {
                    cout << "Tatica invalida! Tem de somar 11 jogadores e respeitar os minimos.\n";
                } else taticaValida = true;
            }
            sucesso = inicializarTatica(t, p);
        } else if (opcao == 2) {
            gerirLesionadosECastigados(p);
            sucesso = inicializarTatica(t, p);
        }
    }
}

void menuOperacoesMudarPos(Plantel& p) {
    cout << "\n--- TREINO: MUDAR POSICAO ---\n";
    cout << "Numero do jogador (0 para cancelar): ";
    int numJog; cin >> numJog;
    if (cin.fail() || numJog == 0) return;

    int idx = -1;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == numJog) { idx = i; break; }
    }

    if (idx == -1) { cout << "[ERRO] Jogador nao encontrado.\n"; return; }

    cout << "Para qual posicao o quer treinar? (0: GR, 1: DEF, 2: MED, 3: AVA): ";
    int novaPos; cin >> novaPos;

    if (PodeContratarParaPosicao(p, novaPos)) {
        p.jogadores[idx].pos = static_cast<Posicao>(novaPos);
        cout << "[SUCESSO] Posicao alterada!\n";
        OrdenarPorPos(p.jogadores, p.totalAtual);
    } else {
        cout << "[ERRO] Limite de jogadores nessa posicao atingido.\n";
    }
}

void menuOperacoesMelhorarQual(Plantel& p) {
    cout << "\n--- TREINO: MELHORAR QUALIDADE ---\n";
    cout << "Numero do jogador (0 para cancelar): ";
    int numJog; cin >> numJog;
    if (cin.fail() || numJog == 0) return;

    int idx = -1;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == numJog) { idx = i; break; }
    }

    if (idx == -1 || p.jogadores[idx].jogosLesao > 0) {
        cout << "[ERRO] Jogador nao encontrado ou lesionado.\n"; return;
    }

    cout << "Semanas de treino (Max 5): ";
    int semanas; cin >> semanas;
    if (semanas >= 1 && semanas <= 5) {
        p.jogadores[idx].semanasTreino = semanas;
        cout << "[SUCESSO] Treino aplicado!\n";
    }
}

void menuAlteracoesPlantel(Plantel& t, const Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};
    cout << "\n--- ALTERACOES MANUAIS NO 11 INICIAL E BANCO ---\n";

    // Mostrar jogadores do Plantel que NÃO estão na tática e estão aptos
    cout << "\nDisponiveis no Plantel (Nao Convocados):\n";
    cout << left << setw(4) << "Nº" << " | "
         << setw(7) << "Posicao" << " | "
         << setw(4) << "Qual" << " | "
         << "Nome\n";
    cout << "--------------------------------------------------\n";

    bool haAptos = false;
    for (int i = 0; i < p.totalAtual; i++) {
        // CORREÇÃO: Usar a flag nativa do teu projeto (jogouHoje) em vez da função antiga
        if (!p.jogadores[i].jogouHoje && p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {

            // CORREÇÃO: Substituição do printf pelo cout com setw()
            cout << left << setw(4) << p.jogadores[i].numero << " | "
                 << setw(7) << posTxt[p.jogadores[i].pos] << " | "
                 << setw(4) << p.jogadores[i].qualidade << " | "
                 << p.jogadores[i].nome << "\n";
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
    if(p.jogadores[idxEntrar].jogouHoje) {
        cout << "[ERRO] O jogador selecionado ja esta convocado na tatica atual!\n"; return;
    }

    // Fazemos a substituição diretamente na Tática
    string nomeSair = t.jogadores[idxSair].nome;
    t.jogadores[idxSair] = p.jogadores[idxEntrar];

    // Atualizar as flags para a nova tática
    t.jogadores[idxSair].jogouHoje = true;

    cout << "\n[SUCESSO] O jogador " << p.jogadores[idxEntrar].nome << " entrou no lugar de " << nomeSair << " para esta partida!\n";
}

void menuOperacoesTransferencias(Plantel& p, Equipa& e) {
    int op = -1;
    while (op != 0) {
        cout << "\n========================================\n";
        cout << "1 -> Visualizar Lista de Transferencias\n";
        cout << "2 -> Contratar / Trocar Jogador\n";
        cout << "0 -> Voltar\n";
        cout << "Escolha uma opcao: ";
        cin >> op;

        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

        if (op == 1) exibirListaTransf(e);
        else if (op == 2) ContratarJogador(p, e);
    }
}

void menuOperacoesGestao(Equipa* liga, int totalEquipas, Partida** calendario, int& modoDeJogo, int& idUser, int& jornadaAtual, Plantel& t) {
    Plantel& p = *liga[idUser].plantel;
    Equipa& e = liga[idUser];

    int opcaoGestao;
    do {
        cout << "\n========================================\n";
        cout << "            GESTAO DA EQUIPA            \n";
        cout << "========================================\n";
        cout << "1 -> Mercado de Transferencias\n";
        cout << "2 -> Treino Epecifico: Mudar Posicao\n";
        cout << "3 -> Treino Epecifico: Melhorar Qualidade\n";
        cout << "4 -> Alterar Tatica Base\n";
        cout << "5 -> Gerir lesionados e castigados\n";
        cout << "6 -> Gravar Estado Atual (GLOBAL)\n";
        cout << "0 -> Voltar ao Jogo\n";
        cout << "Escolha uma opcao: ";
        cin >> opcaoGestao;

        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

        switch (opcaoGestao) {
            case 1: menuOperacoesTransferencias(p, e); break;
            case 2: menuOperacoesMudarPos(p); break;
            case 3: menuOperacoesMelhorarQual(p); break;
            case 4: /* Não podemos chamar o menuTatica sem o t, isso faz-se no jogo normal */ cout << "Tatica altera-se no menu do jogo!\n"; break;
            case 5: gerirLesionadosECastigados(p); break;
            case 6: {
                cout << "Insira o nome do ficheiro para gravar (ex: save.txt): ";
                string ficheiro; cin >> ficheiro;
                if(gravarEstadoGlobal(ficheiro, liga, totalEquipas, calendario, jornadaAtual, modoDeJogo, idUser))
                    cout << "[SUCESSO] Campeonato gravado!\n";
                else cout << "[ERRO] Falha ao gravar.\n";
                break;
            }
            case 0: break;
        }
    } while (opcaoGestao != 0);
}

// O NOVO MENU PRINCIPAL
void menuPrincipal(Equipa* liga, int totalEquipas, Plantel& p, Plantel& t, Partida** calendario, string* listaNomes, int totalNomes, string ficheiroLoad) {
    int modoDeJogo = 0; // 1 = Treinador, 2 = Global
    int idUser = -1;
    int jornadaAtual = 1;
    bool campeonatoAtivo = true;

    if (!ficheiroLoad.empty()) {
        if (carregarEstadoGlobal(ficheiroLoad, liga, totalEquipas, calendario, jornadaAtual, modoDeJogo, idUser)) {
            cout << "\n[!] Campeonato carregado com sucesso!\n";
        } else cout << "\n[ERRO] Falha ao carregar. A iniciar novo jogo.\n";
    }

    if (modoDeJogo == 0) {
        cout << "\n========================================================\n";
        cout << "           BEM-VINDO AO SIMULADOR DE CAMPEONATO           \n";
        cout << "========================================================\n";
        cout << "1 -> Controlar uma Equipa (Modo Treinador)\n";
        cout << "2 -> Simular Campeonato Completo (Modo Global)\n";
        cout << "3 -> Carregar Jogo Guardado\n";
        cout << "0 -> Sair\n";
        cout << "Opcao: ";
        int opEntrada; cin >> opEntrada;

        if (opEntrada == 1) {
            modoDeJogo = 1;
            cout << "\n--- ESCOLHA A SUA EQUIPA ---\n";
            for (int i = 0; i < totalEquipas; i++) cout << i << " -> " << liga[i].nome << "\n";
            cout << "Qual o ID da equipa? ";
            cin >> idUser;
            if (idUser < 0 || idUser >= totalEquipas) idUser = 0;

            if (!inicializarTatica(t, *liga[idUser].plantel)) menuTatica(t, *liga[idUser].plantel);
        } else if (opEntrada == 2) {
            modoDeJogo = 2;
            cout << "\nModo de Simulacao Global ativado!\n";
        } else if (opEntrada == 3) {
            cout << "Ficheiro a carregar: "; string ficheiro; cin >> ficheiro;
            if (carregarEstadoGlobal(ficheiro, liga, totalEquipas, calendario, jornadaAtual, modoDeJogo, idUser)) {
                cout << "[!] Sucesso!\n";
                if (modoDeJogo == 1) inicializarTatica(t, *liga[idUser].plantel);
            } else { cout << "[ERRO] Cancelado. A fechar.\n"; return; }
        } else return;
    }

    while (campeonatoAtivo) {
        if (jornadaAtual > 34) {
            cout << "\n*** FIM DO CAMPEONATO ***\n";
            exibirClassificacao(liga, totalEquipas);
            break;
        }

        // ============================================
        // MODO TREINADOR
        // ============================================
        if (modoDeJogo == 1) {
            Equipa& minhaEquipa = liga[idUser];
            Plantel& meuPlantel = *minhaEquipa.plantel;

            cout << "\n* " << minhaEquipa.nome << " - " << jornadaAtual << "a Jornada - " << minhaEquipa.pontos << " pts *\n";
            exibirTatica(t);
            exibirPlantel(meuPlantel);
            exibirCastigadosLesionados(meuPlantel);

            // ADICIONA ESTA LINHA AQUI:
            exibirListaTransf(minhaEquipa);

            // Menu Limpo!
            cout << "\n(s)eguinte (a)lteracoes (o)coes (c)lassificacao (q)sair: ";
            char opcao; cin >> opcao;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer

            if (opcao == 's' || opcao == 'S') {
                simularJornadaCompleta(calendario, jornadaAtual, liga, idUser);
                for(int i=0; i<9; i++){
                    if(calendario[jornadaAtual-1][i].idCasa == idUser || calendario[jornadaAtual-1][i].idFora == idUser){
                        simularIncidentesPartida(t, meuPlantel);
                    }
                }
                for (int i = 0; i < totalEquipas; i++) atualizarRecuperacoes(*liga[i].plantel);
                // Aqui ehGlobal é false porque estamos no modo treinador
                processarMercadoGlobal(liga, totalEquipas, listaNomes, totalNomes, idUser, false);
                jornadaAtual++;
                if (jornadaAtual <= 34) if (!inicializarTatica(t, meuPlantel)) menuTatica(t, meuPlantel);
            }
            else if (opcao == 'a' || opcao == 'A') menuAlteracoesPlantel(t, meuPlantel);

            // CHAMADA ATUALIZADA (com t, modoDeJogo e idUser passados como ref)
            else if (opcao == 'o' || opcao == 'O') menuOperacoesGestao(liga, totalEquipas, calendario, modoDeJogo, idUser, jornadaAtual, t);

            else if (opcao == 'c' || opcao == 'C') exibirClassificacao(liga, totalEquipas);
            else if (opcao == 'q' || opcao == 'Q') campeonatoAtivo = false;
        }

        // ============================================
        // MODO SIMULAÇÃO GLOBAL
        // ============================================
        else {
            cout << "\n--- SIMULACAO GLOBAL: JORNADA " << jornadaAtual << " ---\n";
            exibirClassificacao(liga, totalEquipas);

            cout << "\n(s)imular jornada (v)er equipa (g)ravar (q)sair: ";
            char op; cin >> op;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer para evitar repetições

            if (op == 's' || op == 'S') {
                bool ehGlobal = (modoDeJogo == 2);
                // 1. Prepara as táticas aleatórias para todos
                prepararTaticasJornada(liga, totalEquipas, idUser, ehGlobal);

                // 2. Simular os jogos (APENAS UMA VEZ)
                simularJornadaCompleta(calendario, jornadaAtual, liga, idUser);

                for (int i = 0; i < totalEquipas; i++) atualizarRecuperacoes(*liga[i].plantel);
                // Aqui ehGlobal é false porque estamos no modo treinador
                // Aqui ehGlobal é true porque estamos no modo de simulação completa
                processarMercadoGlobal(liga, totalEquipas, listaNomes, totalNomes, idUser, true);
                jornadaAtual++;
            }
            else if (op == 'v' || op == 'V') {
                cout << "\n--- ESCOLHA A EQUIPA ---\n";
                for (int i = 0; i < totalEquipas; i++) cout << i << " -> " << liga[i].nome << "\n";
                cout << "Qual o ID da equipa? ";
                int idInsp; cin >> idInsp;
                if (!cin.fail() && idInsp >= 0 && idInsp < totalEquipas) {
                    cout << "\n=== " << liga[idInsp].nome << " ===\n";
                    Plantel taticaTemp;
                    taticaTemp.jogadores = nullptr;

                    // CORREÇÃO: Copiar a tática que o BOT realmente escolheu
                    for(int k = 0; k < 4; k++) {
                        taticaTemp.tatica[k] = liga[idInsp].plantel->tatica[k];
                    }

                    if (inicializarTatica(taticaTemp, *liga[idInsp].plantel)) {
                        exibirTatica(taticaTemp);
                    }
                    exibirPlantel(*liga[idInsp].plantel);
                    exibirCastigadosLesionados(*liga[idInsp].plantel);
                    exibirListaTransf(liga[idInsp]);
                    delete[] taticaTemp.jogadores;
                } else { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
            }
            else if (op == 'g' || op == 'G') {
                cout << "Ficheiro para gravar (ex: save.txt): "; string f; cin >> f;
                if(gravarEstadoGlobal(f, liga, totalEquipas, calendario, jornadaAtual, modoDeJogo, idUser)) cout << "[SUCESSO] Gravado!\n";
                else cout << "[ERRO] Falha.\n";
            }
            else if (op == 'q' || op == 'Q') campeonatoAtivo = false;
        }
    }
}