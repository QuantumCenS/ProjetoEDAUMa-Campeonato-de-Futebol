#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include "menu.h"
#include "core.h"
#include "jogo.h"

using namespace std;

/**
 * gere a interface do menu para visualizar e modificar os tempos de paragem por lesao ou castigo.
 * permite ao utilizador pesquisar um jogador pelo numero e ajustar manualmente o seu estado de disponibilidade
 * @param p - referencia para o plantel onde as alteracoes de saude/disciplina serao aplicadas.
 */
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

/**
 * apresenta o menu de definicao de tatica manual para o utilizador
 * valida se o numero de jogadores por posicao cumpre os requisitos minimos e se o plantel tem jogadores aptos suficientes
 * @param t - referencia para a estrutura de tatica (convocados) a ser preenchida.
 * @param p - referencia para o plantel total de onde os jogadores serao selecionados.
 */
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

/**
 * gere a operacao de treino para alterar a posicao principal de um jogador no plantel.
 * verifica a viabilidade da mudanca com base nos limites maximos permitidos para cada posicao.
 * @param p - referencia para o plantel que contem o jogador a ser treinado.
 */
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

/**
 * permite ao utilizador agendar semanas de treino especifico para um jogador melhorar a sua qualidade.
 * bloqueia a aplicacao de treino a jogadores que se encontrem atualmente lesionados.
 * @param p - referencia para o plantel onde o tempo de treino sera registado.
 */
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

/**
 * gere a interface para a realizacao de substituicoes manuais entre a tatica convocada e os jogadores aptos no banco/plantel.
 * garante que os jogadores que entram nao estao lesionados, castigados ou ja a jogar na tatica atual.
 * @param t - referencia para a tatica ativa (11 inicial e banco) onde a substituicao sera efetuada.
 * @param p - referencia constante para o plantel global para consulta de jogadores disponiveis.
 */
void menuAlteracoesPlantel(Plantel& t, const Plantel& p) {
    int escolha = -1;
    while (escolha != 0) {
        cout << "\n=========================================\n";
        cout << "          GESTAO TATICA E 11            \n";
        cout << "=========================================\n";
        cout << "1 -> Substituir Jogadores (Titulares/Suplentes)\n";
        cout << "2 -> Alterar Formacao (ex: 4-4-2, 4-3-3)\n";
        cout << "0 -> Voltar ao Menu Principal\n";
        cout << "Opcao: ";
        cin >> escolha;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (escolha == 1) {
            const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};
            cout << "\n--- SUBSTITUICOES NO 11 INICIAL E BANCO ---\n";

            cout << "\nDisponiveis no Plantel (Nao Convocados):\n";
            cout << left << setw(4) << "Nº" << " | " << setw(7) << "Posicao" << " | " << setw(4) << "Qual" << " | " << "Nome\n";
            cout << "--------------------------------------------------\n";

            bool haAptos = false;
            for (int i = 0; i < p.totalAtual; i++) {
                if (!p.jogadores[i].jogouHoje && p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
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
            if(cin.fail() || numSair == 0) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

            int idxSair = -1;
            for(int i = 0; i < t.totalAtual; i++) if(t.jogadores[i].numero == numSair) idxSair = i;
            if(idxSair == -1) { cout << "[ERRO] Jogador nao encontrado na tatica atual.\n"; continue; }

            cout << "Insira o numero do jogador do PLANTEL a ENTRAR: ";
            int numEntrar; cin >> numEntrar;
            if(cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

            int idxEntrar = -1;
            for(int i = 0; i < p.totalAtual; i++) if(p.jogadores[i].numero == numEntrar) idxEntrar = i;

            if(idxEntrar == -1) { cout << "[ERRO] Jogador nao encontrado no plantel.\n"; continue; }
            if(p.jogadores[idxEntrar].jogosLesao > 0 || p.jogadores[idxEntrar].jogosCastigo > 0) {
                cout << "[ERRO] O jogador selecionado esta lesionado ou castigado e nao pode jogar!\n"; continue;
            }
            if(p.jogadores[idxEntrar].jogouHoje) {
                cout << "[ERRO] O jogador selecionado ja esta convocado na tatica atual!\n"; continue;
            }

            // Fazemos a substituição diretamente na Tática
            string nomeSair = t.jogadores[idxSair].nome;
            t.jogadores[idxSair] = p.jogadores[idxEntrar];
            t.jogadores[idxSair].jogouHoje = true;

            cout << "\n[SUCESSO] O jogador " << p.jogadores[idxEntrar].nome << " entrou no lugar de " << nomeSair << " para esta partida!\n";
        }
        else if (escolha == 2) {
            // Salta o menu intermédio e pede a tática diretamente
            bool taticaValida = false;

            // Grava a tática antiga em caso de erro
            int defBkp = t.tatica[1], medBkp = t.tatica[2], avaBkp = t.tatica[3];

            while (!taticaValida) {
                cout << "\n--- ALTERAR FORMACAO ---\n";
                cout << "Insira o numero de Defesas (Min 3): "; cin >> t.tatica[1];
                cout << "Insira o numero de Medios (Min 2): "; cin >> t.tatica[2];
                cout << "Insira o numero de Avancados (Min 1): "; cin >> t.tatica[3];

                int totalJogadores = 1 + t.tatica[1] + t.tatica[2] + t.tatica[3]; // Guarda-redes é sempre 1
                if (t.tatica[1] < 3 || t.tatica[2] < 2 || t.tatica[3] < 1 || totalJogadores != 11) {
                    cout << "[ERRO] Tatica invalida! Os jogadores de campo tem de somar 10 e respeitar os minimos.\n";
                } else {
                    taticaValida = true;
                }
            }

            // Tenta inicializar a nova tática
            if (inicializarTatica(t, (Plantel&)p)) {
                cout << "[SUCESSO] Nova formacao aplicada e plantel convocado automaticamente.\n";
            } else {
                // Se falhar (ex: escolheu 4 avançados mas só tem 2 aptos), reverte para a formação anterior
                t.tatica[1] = defBkp;
                t.tatica[2] = medBkp;
                t.tatica[3] = avaBkp;
                inicializarTatica(t, (Plantel&)p);
                cout << "[AVISO] Formacao cancelada. A sua equipa manteve a tatica anterior.\n";
            }
        }
    }
}

/**
 * apresenta e gere o sub-menu dedicado às operações de mercado.
 * permite ao utilizador alternar entre a visualização da lista de transferências e a execução de contratações ou trocas.
 * @param p - referência para o plantel do utilizador onde serão inseridos os novos jogadores.
 * @param e - referência para a equipa do utilizador que contém a lista de transferências ativa.
 */
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

/**
 * centraliza as operações de gestão interna da equipa e do estado do jogo.
 * inclui acesso ao mercado, treinos específicos, gestão médica e a funcionalidade de gravação do progresso global.
 * @param liga - array contendo todas as equipas do campeonato.
 * @param totalEquipas - número total de equipas na liga.
 * @param calendario - matriz dinâmica com o agendamento de todos os jogos.
 * @param modoDeJogo - referência para o modo atual (Treinador ou Global).
 * @param idUser - referência para o identificador da equipa do utilizador.
 * @param jornadaAtual - referência para o contador da jornada em curso.
 * @param t - referência para a tática ativa para validação de alterações.
 */
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
        cout << "4 -> Gerir lesionados e castigados\n"; // Antigo 5
        cout << "5 -> Gravar Estado Atual (GLOBAL)\n";    // Antigo 6
        cout << "0 -> Voltar ao Jogo\n";
        cout << "Escolha uma opcao: ";
        cin >> opcaoGestao;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (opcaoGestao) {
            case 1:
                menuOperacoesTransferencias(p, e);
                break;
            case 2:
                menuOperacoesMudarPos(p);
                break;
            case 3:
                menuOperacoesMelhorarQual(p);
                break;
            case 4:
                gerirLesionadosECastigados(p);
                break;
            case 5: {
                cout << "Insira o nome do ficheiro para gravar (ex: save.txt): ";
                string ficheiro; cin >> ficheiro;
                if(gravarEstadoGlobal(ficheiro, liga, totalEquipas, calendario, jornadaAtual, modoDeJogo, idUser))
                    cout << "[SUCESSO] Campeonato gravado!\n";
                else
                    cout << "[ERRO] Falha ao gravar.\n";
                break;
            }
            case 0:
                break;
            default:
                cout << "Opcao invalida!\n";
                break;
        }
    } while (opcaoGestao != 0);
}

/**
 * motor principal do simulador que gere o fluxo completo do campeonato.
 * controla o carregamento de ficheiros, a seleção inicial de modos, e o ciclo de vida das 34 jornadas,
 * diferenciando a lógica entre o Modo Treinador (interativo) e o Modo Global (autónomo).
 * @param liga - array com a estrutura de dados de todas as equipas.
 * @param totalEquipas - número total de equipas participantes.
 * @param p - referência para o plantel base do utilizador.
 * @param t - referência para a estrutura de tática ativa do utilizador.
 * @param calendario - estrutura de dados com o calendário completo de jogos.
 * @param listaNomes - pool de nomes para a geração dinâmica de jogadores no mercado.
 * @param totalNomes - total de nomes disponíveis no banco de dados.
 * @param ficheiroLoad - nome do ficheiro para carregamento de estado anterior (string vazia para novo jogo).
 */
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

        // MODO TREINADOR
        if (modoDeJogo == 1) {
            Equipa& minhaEquipa = liga[idUser];
            Plantel& meuPlantel = *minhaEquipa.plantel;

            cout << "\n* " << minhaEquipa.nome << " - " << jornadaAtual << "a Jornada - " << minhaEquipa.pontos << " pts *\n";
            exibirTatica(t);
            exibirPlantel(meuPlantel);
            exibirCastigadosLesionados(meuPlantel);

            exibirListaTransf(minhaEquipa);

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

            else if (opcao == 'o' || opcao == 'O') menuOperacoesGestao(liga, totalEquipas, calendario, modoDeJogo, idUser, jornadaAtual, t);

            else if (opcao == 'c' || opcao == 'C') exibirClassificacao(liga, totalEquipas);
            else if (opcao == 'q' || opcao == 'Q') campeonatoAtivo = false;
        }

        // MODO SIMULAÇÃO GLOBAL
        else {
            cout << "\n--- SIMULACAO GLOBAL: JORNADA " << jornadaAtual << " ---\n";
            exibirClassificacao(liga, totalEquipas);

            cout << "\n(s)imular jornada (v)er equipa (g)ravar (q)sair: ";
            char op; cin >> op;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer para evitar repetições

            if (op == 's' || op == 'S') {
                bool ehGlobal = (modoDeJogo == 2);
                // prepara as táticas aleatórias para todos
                prepararTaticasJornada(liga, totalEquipas, idUser, ehGlobal);

                // simula os jogos (APENAS UMA VEZ)
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

                    // copia a tática que o BOT realmente escolheu
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