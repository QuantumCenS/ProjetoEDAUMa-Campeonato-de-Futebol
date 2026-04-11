//
// Created by Sergio on 4/3/2026.
//

#include "jogo.h"
#include "core.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>


using namespace std;

//Por o nome das equipas num array
//Por o nome das equipas num array E BARALHAR!
string* carregarEquipas(string f) {
    fstream file(f);
    if (!file.is_open()) return nullptr;

    string line;
    int nEquipas = 0;

    // conta equipas
    while (getline(file, line)) {
        if (!line.empty()) nEquipas++;
    }

    // "rewind" no file
    file.clear();
    file.seekg(0);

    // Aloca e grava
    auto* equipas = new string[nEquipas];
    int i = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            equipas[i] = line;
            i++;
        }
    }
    file.close();

    // -> A MAGIA ACONTECE AQUI: Chama a tua função de baralhar
    // antes de devolver o array para o main.cpp!
    baralhar(equipas, nEquipas);

    return equipas;
}

//Baralha as equipas aleatóriamente dentro do array
void baralhar(string* equipas, int nEquipas ) {
    // Percorre o array do fim para o início
    for (int i = nEquipas - 1; i > 0; i--) {
        // Gera um índice aleatório entre 0 e i
        int j = gerarAleatorio(0, i);

        // Troca os elementos (Swap)
        string temp = equipas[i];
        equipas[i] = equipas[j];
        equipas[j] = temp;
    }
}

//Gerar Jornadas do campeonato
// string** gerarJornadas( Equipa e, string* equipas) {
//     int nJornadas =34;
//     int nEquipas=17; //Se o ficheiro de equipas tiver 50 equipas, substituir este número por 17 ou 18
//     auto **totalJornadas= new string*[nJornadas];
//     baralhar(equipas,nEquipas);
//     for (int i=0;i<nEquipas;i++) {
//         totalJornadas[i] = new string[2];
//         totalJornadas[i + nEquipas] = new string[2];
//         if (i%2==0) {
//             totalJornadas[i][0]=e.nome;
//             totalJornadas[i][1]=equipas[i];
//
//             totalJornadas[i+nEquipas][0]=equipas[i];
//             totalJornadas[i+nEquipas][1]=e.nome;
//         }
//         else {
//             totalJornadas[i][0]=equipas[i];
//             totalJornadas[i][1]=e.nome;
//
//             totalJornadas[i+nEquipas][0]=e.nome;
//             totalJornadas[i+nEquipas][1]=equipas[i];
//         }
//     }
//     return totalJornadas;
// }

Equipa& encontrarEquipa(string nome, Equipa* liga, int nEquipas) {
    for (int i = 0; i < nEquipas; i++) {
        if (liga[i].nome == nome) {
            return liga[i];
        }
    }
    return liga[0]; // Caso de segurança
}
//Gerar o resultado de uma partida e alterar os pontos c:Casa, f:Fora, h:Home, a:Away
string gerarResultado(Equipa& h, Equipa& a) {
    int c= gerarAleatorio(0,8);
    int f= gerarAleatorio(0,8);
    if (c==f) {
        h.pontos+=1;
        a.pontos+=1;
    }
    else if (c>f) {
        h.pontos+=3;
    }
    else if (c<f) {
        a.pontos+=3;
    }
    return h.nome +" "+ to_string(c) +" - " + to_string(f) + " " + a.nome;
}

// Reduz 1 jogo de castigo/lesão a quem está de fora
// Reduz 1 jogo de castigo/lesão a quem está de fora e aplica os treinos!
void atualizarRecuperacoes(Plantel& p) {
    for (int i = 0; i < p.totalAtual; i++) {
        // Recuperação de lesões e castigos
        if (p.jogadores[i].jogosLesao > 0) p.jogadores[i].jogosLesao--;
        if (p.jogadores[i].jogosCastigo > 0) p.jogadores[i].jogosCastigo--;

        // Aplicação do Treino Específico (Melhorar Qualidade)
        // Se tem semanas de treino e NÃO está lesionado:
        if (p.jogadores[i].semanasTreino > 0 && p.jogadores[i].jogosLesao == 0) {
            p.jogadores[i].semanasTreino--;       // Desconta 1 semana
            p.jogadores[i].qualidade += 5;        // Aumenta 5 de qualidade

            // O limite máximo de qualidade é 100
            if (p.jogadores[i].qualidade > 100) {
                p.jogadores[i].qualidade = 100;
            }
        }
    }
}

// Aplica incidentes da jornada ao plantel real
void aplicarIncidenteAoPlantel(Plantel& p, int numero, bool lesao, int tempo) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == numero) {
            if (lesao) p.jogadores[i].jogosLesao = tempo;
            else p.jogadores[i].jogosCastigo = tempo;
            break;
        }
    }
}

void simularIncidentesPartida(Plantel& t, Plantel& p) {
    cout << "Substituicoes:\n";
    bool houveSubs = false;
    int substituicoesFeitas = 0; // LIMITADOR DE 3 SUBSTITUIÇÕES

    for (int i = 11; i < t.totalAtual; i++) {
        t.jogadores[i].jogouHoje = false; // Suplentes começam no banco
    }

    for (int i = 0; i < 11; i++) {
        Jogador& titular = t.jogadores[i];

        int sorteLesao = gerarAleatorio(1, 100);
        int sorteCastigo = gerarAleatorio(1, 100);

        bool sofreuLesao = (sorteLesao <= titular.probLesao);
        bool sofreuCastigo = (sorteCastigo <= titular.probCastigo);

        if (sofreuCastigo) {
            aplicarIncidenteAoPlantel(p, titular.numero, false, gerarAleatorio(1, 10));
            cout<<titular.nome<<" castigado, nao pode ser substituido, a equipa joga com menos 1 jogador\n";
        }

        if (sofreuLesao && !sofreuCastigo) {
            aplicarIncidenteAoPlantel(p, titular.numero, true, gerarAleatorio(1, 10));


            if (substituicoesFeitas < 3) {
                int idxSuplente = -1;

                // 1. TENTATIVA: Procurar suplente da MESMA posição
                for (int j = 11; j < t.totalAtual; j++) {
                    if (t.jogadores[j].pos == titular.pos && !t.jogadores[j].jogouHoje) {
                        idxSuplente = j;
                        break;
                    }
                }

                // 2. TENTATIVA: Se não houver da mesma, procura QUALQUER suplente
                if (idxSuplente == -1) {
                    for (int j = 11; j < t.totalAtual; j++) {
                        if (!t.jogadores[j].jogouHoje) {
                            idxSuplente = j;
                            break;
                        }
                    }
                }

                // 3. Efetuar substituição
                if (idxSuplente != -1) {
                    Jogador& suplente = t.jogadores[idxSuplente];
                    cout << titular.nome << " -> " << suplente.nome << "\n";
                    suplente.jogouHoje = true; // <-- CORREÇÃO: Usa a flag em vez de destruir a qualidade
                    substituicoesFeitas++;
                    houveSubs = true;
                } else {
                    cout << titular.nome << " (Saiu, mas ja nao ha suplentes!)\n";
                    houveSubs = true;
                }
            }
        }
    }
    if (!houveSubs) cout << "Nenhuma\n";
}

void exibirCastigadosLesionados(const Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "\nCastigados:\n";
    cout << left
     << setw(20) << "Nome" << " | "
     << setw(4)  << "Nº" << " | "
     << setw(7)  << "Posicao" << " | "
     << setw(5)  << "Idade" << " | "
     << setw(9)  << "ProbLesao" << " | "
     << setw(11) << "ProbCastigo" << " | "
     << setw(9)  << "Qualidade" << " | "
     << setw(14) << "JogosRestantes" << " | "
     << "Dias-Treino\n";
    cout << "-----------------------------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosCastigo > 0) {
            Jogador& j = p.jogadores[i];
            string lesaoStr = to_string(j.probLesao) + "%";
            string castigoStr = to_string(j.probCastigo) + "%";

            cout << left
         << setw(20) << j.nome << " | "
         << setw(4)  << j.numero << " | "
         << setw(7)  << posTxt[j.pos] << " | "
         << setw(5)  << j.idade << " | "
         << setw(9)  << lesaoStr << " | "
         << setw(11) << castigoStr << " | "
         << setw(9)  << j.qualidade << " | "
         << setw(14) << j.jogosCastigo << " | "
         << j.semanasTreino << "\n";
        }
    }

    cout << "\nLesionados:\n";
    cout << left
     << setw(20) << "Nome" << " | "
     << setw(4)  << "Nº" << " | "
     << setw(7)  << "Posicao" << " | "
     << setw(5)  << "Idade" << " | "
     << setw(9)  << "ProbLesao" << " | "
     << setw(11) << "ProbCastigo" << " | "
     << setw(9)  << "Qualidade" << " | "
     << "JogosRestantes\n";
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao > 0) {
            Jogador& j = p.jogadores[i];
            string lesaoStr = to_string(j.probLesao) + "%";
            string castigoStr = to_string(j.probCastigo) + "%";

            cout << left
         << setw(20) << j.nome << " | "
         << setw(4)  << j.numero << " | "
         << setw(7)  << posTxt[j.pos] << " | "
         << setw(5)  << j.idade << " | "
         << setw(9)  << lesaoStr << " | "
         << setw(11) << castigoStr << " | "
         << setw(9)  << j.qualidade << " | "
         << j.jogosLesao << "\n";
        }
    }
}


Jogador encontrarMelhorDisponivel(Plantel& p, Plantel& t, Posicao pos) {
    Jogador melhor;
    melhor.qualidade = -1;
    int indiceMelhor = -1;

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador candidato = p.jogadores[i];

        // Só escolhe se for da posição, tiver mais qualidade, NÃO estiver convocado e NÃO estiver lesionado/castigado
        if (candidato.pos == pos && candidato.qualidade > melhor.qualidade) {
            if (!p.jogadores[i].jogouHoje && candidato.jogosLesao == 0 && candidato.jogosCastigo == 0) {
                melhor = candidato;
                indiceMelhor=i;
            }
        }
    }
    if (indiceMelhor != -1) {
        // 2. MARCAR O JOGADOR COMO "EM USO" PARA NÃO SER CLONADO
        p.jogadores[indiceMelhor].jogouHoje = true;

        return p.jogadores[indiceMelhor];
    }
    Jogador erro;
    erro.qualidade = -1;
    return erro;
}

bool inicializarTatica(Plantel& t, Plantel& p) {
    t.capacidade = 17;
    if (t.jogadores == nullptr) t.jogadores = new Jogador[t.capacidade];
    t.totalAtual = 0;
    for (int k = 0; k < p.totalAtual; k++) {
        p.jogadores[k].jogouHoje = false;
    }

    int titularesNecessarios[4];
    string nomeTatica;

    nomeTatica=to_string(t.tatica[0])+"-"+to_string(t.tatica[1])+"-"+to_string(t.tatica[2])+"-"+to_string(t.tatica[3]);

    titularesNecessarios[0] = t.tatica[0];
    titularesNecessarios[1] = t.tatica[1];
    titularesNecessarios[2] = t.tatica[2];
    titularesNecessarios[3] = t.tatica[3];

    Posicao posicoes[] = {GR, DEF, MED, AVA};
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};


    // ==========================================================
    // CASO 1: VERIFICAR TITULARES (Obrigatório respeitar posição)
    // ==========================================================
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < titularesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            } else {
                // FALHOU: O plantel não tem jogadores aptos suficientes para esta posição
                cout << "\n[ERRO] Nao tem jogadores aptos suficientes na posicao " << posTxt[i]<< " para formar a tatica " << nomeTatica << "!\n";
                return false; // Retorna falso para obrigar o utilizador a mudar
            }
        }
    }

    // ==========================================================
    // CASO 2: VERIFICAR SUPLENTES (Flexível em caso de falha)
    // ==========================================================
    int suplentesNecessarios[] = {1, 2, 2, 1};
    int vagasVazias = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < suplentesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            } else {
                vagasVazias++;
                cout << "[AVISO] Nao ha suplentes aptos suficientes para a posicao " << posTxt[i] << ". O sistema ira preencher com outros jogadores!\n";
            }
        }
    }

    // Preencher as vagas vazias no banco com qualquer jogador livre (ignora a posição)
    while (vagasVazias > 0 && t.totalAtual < 17) {
        int indiceMelhor = -1;
        int maiorQual = -1;

        // Procura DIRETAMENTE no plantel pelo melhor jogador que ainda não jogou
        for (int i = 0; i < p.totalAtual; i++) {
            if (!p.jogadores[i].jogouHoje && p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
                if (p.jogadores[i].qualidade > maiorQual) {
                    maiorQual = p.jogadores[i].qualidade;
                    indiceMelhor = i;
                }
            }
        }

        if (indiceMelhor != -1) {
            // Marca o jogador para não ser clonado e adiciona-o à tática
            p.jogadores[indiceMelhor].jogouHoje = true;
            t.jogadores[t.totalAtual++] = p.jogadores[indiceMelhor];
            vagasVazias--;

            cout << "   -> Suplente Adaptado: " << p.jogadores[indiceMelhor].nome
                 << " (" << posTxt[p.jogadores[indiceMelhor].pos] << ") foi convocado para a vaga.\n";
        } else {
            cout << "[CRITICO] Plantel dizimado! Nao ha mais jogadores disponiveis para sentar no banco.\n";
            break; // Sai do ciclo se não houver mais ninguém vivo no clube
        }
    }
    return true; // Tática formada com sucesso
}

void exibirTatica(const Plantel& t) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "11 Inicial\n";
    cout << "Nome                 | N  | Posicao | Idade | ProbLesao | ProbCastigo | Qualidade\n";
    cout << "---------------------------------------------------------------------------------\n";

    // Imprime os primeiros 11 (Titulares)
    for (int i = 0; i < 11 && i < t.totalAtual; i++) {
        Jogador& j = t.jogadores[i];
        cout << left
     << setw(20) << j.nome << " | "
     << setw(2)  << j.numero << " | "
     << setw(7)  << posTxt[j.pos] << " | "
     << setw(5)  << j.idade << " | "
     << setw(8)  << j.probLesao << "% | "
     << setw(10) << j.probCastigo << "% | "
     << setw(9)  << j.qualidade << "\n";
    }

    cout << "Suplentes:\n";
    cout << "Nome                 | N  | Posicao | Idade | ProbLesao | ProbCastigo | Qualidade\n";
    cout << "---------------------------------------------------------------------------------\n";

    // Imprime do 11 para a frente (Suplentes)
    for (int i = 11; i < t.totalAtual; i++) {
        Jogador& j = t.jogadores[i];
        cout << left
     << setw(20) << j.nome << " | "
     << setw(2)  << j.numero << " | "
     << setw(7)  << posTxt[j.pos] << " | "
     << setw(5)  << j.idade << " | "
     << setw(8)  << j.probLesao << "% | "
     << setw(10) << j.probCastigo << "% | "
     << j.qualidade << "\n";
    }
}

void listaTranf(Equipa& e) {
    e.capacidadeLT = 100;
    e.totalLT = 0;
    e.ListaTransf = new Jogador[e.capacidadeLT];                 //array duinamico do tipo jogador
}

void adicionarJogLT(Equipa& e, const Plantel& p, string* listaNomes, int totalNomes) {
    for (int i = 0; i < 2; i++) {   //  2 jogadores por jornada

        if (e.totalLT < e.capacidadeLT) {
            Jogador& novo = e.ListaTransf[e.totalLT];                              // 2. Criar um atalho para a primeira posição vazia no fim da lista

            preencherAtributosIndependentes(novo, p, listaNomes, totalNomes);

            Posicao posicoes[] = { GR, DEF, MED, AVA };
            novo.pos = posicoes[gerarAleatorio(0, 3)];

            int maiorNumero = 30;     //sistema de numeros pra nas transferencias nao duplicarem

            for (int j = 0; j < e.totalLT; j++) {
                if (e.ListaTransf[j].numero > maiorNumero) {      //procura o numero maior na lista de transferencias
                    maiorNumero = e.ListaTransf[j].numero;
                }
            }

            novo.numero = maiorNumero + 1;   // O novo jogador recebe o maior número encontrado + 1
            e.totalLT++;
        }
    }
    OrdenarPorPos(e.ListaTransf, e.totalLT);
}

void exibirListaTransf(const Equipa& e) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "****************************************************************************************\n";
    cout << "Transferencias:\n";
    cout << "                                                                                        \n";

    cout << left
     << setw(20) << "Nome" << " | "
     << setw(4)  << "Nº" << " | "
     << setw(7)  << "Posicao" << " | "
     << setw(5)  << "Idade" << " | "
     << setw(9)  << "ProbLesao" << " | "
     << setw(11) << "ProbCastigo" << " | "
     << "Qualidade\n";

    for (int i = 0; i < e.totalLT; i++) {
        Jogador& j = e.ListaTransf[i];

        string lesaoStr = to_string(j.probLesao) + "%";
        string castigoStr = to_string(j.probCastigo) + "%";

        cout << left
     << setw(20) << j.nome << " | "
     << setw(4)  << j.numero << " | "
     << setw(7)  << posTxt[j.pos] << " | "
     << setw(5)  << j.idade << " | "
     << setw(9)  << lesaoStr << " | "
     << setw(11) << castigoStr << " | "
     << j.qualidade << "\n";
    }
    cout << "****************************************************************************************\n";
}

bool nomeJaExisteNoPlantel(const Plantel& p, string nome) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].nome == nome) return true;
    }
    return false;
}

// Numero e Posicao dependem das restricoes do plantel
void preencherAtributosIndependentes(Jogador& novo, const Plantel& p, string* listaNomes, int totalNomes) {
    string nomeSorteado;
    do {
        nomeSorteado = listaNomes[gerarAleatorio(0, totalNomes - 1)];
    } while (nomeJaExisteNoPlantel(p, nomeSorteado));

    novo.nome = nomeSorteado;
    novo.idade = gerarAleatorio(17, 38);
    novo.probLesao = gerarAleatorio(0, 15);
    novo.probCastigo = gerarAleatorio(0, 20);
    novo.qualidade = gerarAleatorio(0, 100);
}

void OrdenarPorPos(Jogador* lista, int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (lista[j].pos > lista[j+1].pos) {
                Jogador temporario = lista[j];
                lista[j] = lista[j+1];
                lista[j+1] = temporario;
            }
        }
    }
}

//Conta o numero de jogadores por posicao
int ContarJogNumaPos(Plantel& p, int j){                    // j indica a posição (0=GR, 1=DEF, 2=MED, 3=AVA)
    int contador = 0;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].pos == j) {
            contador++;
        }
    }
    return contador;
}

//Avalia se pode ser contratado para a posicao
bool PodeContratarParaPosicao(Plantel& p, int j) {
    int limitesMaximos[] = {3, 10, 10, 7};
    int quantidadeAtual = ContarJogNumaPos(p, j);

    if (quantidadeAtual >= limitesMaximos[j]) {
        return false; // posicao ja atingiu o max de jogadores
    }

    return true; //ainda ha espaço pra mais jogadores nessa posicao
}

void ContratarJogador(Plantel& p, Equipa& e) {
    cout << "\n--- MERCADO DE TRANSFERENCIAS ---\n";
    exibirListaTransf(e);
    cout << "Insira o numero do jogador da LISTA DE TRANSFERENCIAS que pretende adquirir (digite 0 para cancelar): ";
    int numTransf;
    cin >> numTransf;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Erro: Input invalido!\n";
        return;
    }

    if (numTransf == 0) return; // O treinador desistiu de conratar um novo jogador

    // Procurar o jogador na Lista de Transferências
    int indiceLT = -1;
    for (int j = 0; j < e.totalLT; j++) {
        if (e.ListaTransf[j].numero == numTransf) {
            indiceLT = j;
            break;
        }
    }

    if (indiceLT == -1) {
        cout << "Erro: Jogador com o numero " << numTransf << " nao encontrado na lista!\n";
        return;
    }

    //Avaliar os Limites (alteraçao falada com o prof)
    int posDesejada = e.ListaTransf[indiceLT].pos;   //guarda posiçao
    bool plantelCheio = (p.totalAtual >= p.capacidade);
    bool posicaoCheia = !PodeContratarParaPosicao(p, posDesejada);

    // Decidir se será compra direta ou troca
    if (plantelCheio || posicaoCheia) {
        cout << "\n[AVISO] Nao e possivel fazer contratacao direta!\n";
        if (plantelCheio) cout << "- O plantel ja atingiu o limite de 30 jogadores.\n";
        if (posicaoCheia) cout << "- Ja atingiu o limite maximo de jogadores para essa posicao.\n";

        //trocas
        cout << "\nTera de realizar uma TROCA por um jogador da mesma posicao.\n";
        cout << "Insira o numero do jogador do seu PLANTEL que vai sair (digite 0 para cancelar): ";
        int numPlantel;
        cin >> numPlantel;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Erro: Input invalido!\n";
            return;
        }

        if (numPlantel == 0) return;

        int indicePlantel = -1;
        for (int k = 0; k < p.totalAtual; k++) {
            if (p.jogadores[k].numero == numPlantel) {
                indicePlantel = k;
                break;
            }
        }

        if (indicePlantel != -1) {
            // só pode trocar por alguém da mesma posição
            if (p.jogadores[indicePlantel].pos != posDesejada) {
                cout << "Erro: Troca cancelada! Tem de trocar um jogador da MESMA posicao do que vai comprar.\n";
                return;
            }

            // troca em si (nao ha mudança de numeros)
            Jogador temporario = p.jogadores[indicePlantel];
            p.jogadores[indicePlantel] = e.ListaTransf[indiceLT];
            e.ListaTransf[indiceLT] = temporario;

            cout << "Troca realizada com sucesso!\n";
        } else {
            cout << "Erro: Jogador do plantel nao encontrado!\n";
        }
    } else {
        // COMPRA DIRETA (Há vagas no plantel e na posição)
        p.jogadores[p.totalAtual] = e.ListaTransf[indiceLT];
        p.totalAtual++;

        for (int i = indiceLT; i < e.totalLT - 1; i++) {
            e.ListaTransf[i] = e.ListaTransf[i + 1];
        }
        e.totalLT--;

        cout << "Contratacao direta realizada com sucesso!\n";
    }

    // Organizar plantel e LT
    OrdenarPorPos(p.jogadores, p.totalAtual);
    OrdenarPorPos(e.ListaTransf, e.totalLT);
}



void ContratarJogadorBOT(Plantel& p, Equipa& e) {
    if (e.totalLT == 0) return;

    // Tenta analisar até 3 jogadores da lista por jornada
    for (int tentativa = 0; tentativa < 3; tentativa++) {
        if (e.totalLT == 0) break;

        int idxLT = gerarAleatorio(0, e.totalLT - 1);
        Jogador& candidato = e.ListaTransf[idxLT];
        int nNaPos = ContarJogNumaPos(p, candidato.pos);
        int limitesMaximos[] = {3, 10, 10, 7}; //

        // COMPRA: Se houver vaga real
        if (nNaPos < limitesMaximos[candidato.pos] && p.totalAtual < p.capacidade) {
            p.jogadores[p.totalAtual++] = candidato;
            for (int i = idxLT; i < e.totalLT - 1; i++) e.ListaTransf[i] = e.ListaTransf[i + 1];
            e.totalLT--;
        }
        // TROCA: Se o candidato for MELHOR que o pior do plantel naquela posição
        else {
            int piorIdx = -1;
            int piorQual = candidato.qualidade;

            for (int i = 0; i < p.totalAtual; i++) {
                if (p.jogadores[i].pos == candidato.pos && p.jogadores[i].qualidade < piorQual) {
                    piorQual = p.jogadores[i].qualidade;
                    piorIdx = i;
                }
            }

            if (piorIdx != -1) {
                Jogador temp = p.jogadores[piorIdx];
                p.jogadores[piorIdx] = candidato;
                e.ListaTransf[idxLT] = temp;
            }
        }
    }
    OrdenarPorPos(p.jogadores, p.totalAtual);
    OrdenarPorPos(e.ListaTransf, e.totalLT);
}


void processarMercadoGlobal(Equipa* liga, int nEquipas, string* bancoNomes, int nNomes, int idUser, bool ehGlobal) {
    for (int i = 0; i < nEquipas; i++) {
        adicionarJogLT(liga[i], *liga[i].plantel, bancoNomes, nNomes);

        // Se for Modo Global, TODOS agem. Se for Treinador, apenas os outros clubes (i != idUser)
        if (ehGlobal || i != idUser) {
            ListarJogadorBOT(liga[i]);
            ContratarJogadorBOT(*liga[i].plantel, liga[i]);
        }
    }
}

void ListarJogadorBOT(Equipa& e) {
    Plantel& p = *e.plantel;
    int minimos[] = {2, 5, 4, 2}; // GR, DEF, MED, AVA

    // Percorre todas as posições para ver onde há jogadores a mais
    for (int pos = 0; pos < 4; pos++) {
        int nNaPos = ContarJogNumaPos(p, pos);

        // Se tiver mais do que o mínimo, coloca o pior dessa posição à venda
        if (nNaPos > minimos[pos] && e.totalLT < e.capacidadeLT) {
            int piorIdx = -1;
            int piorQualidade = 101;

            for (int i = 0; i < p.totalAtual; i++) {
                if (p.jogadores[i].pos == pos && p.jogadores[i].qualidade < piorQualidade) {
                    piorQualidade = p.jogadores[i].qualidade;
                    piorIdx = i;
                }
            }

            if (piorIdx != -1) {
                e.ListaTransf[e.totalLT++] = p.jogadores[piorIdx];
                for (int i = piorIdx; i < p.totalAtual - 1; i++) p.jogadores[i] = p.jogadores[i + 1];
                p.totalAtual--;
            }
        }
    }
    OrdenarPorPos(e.ListaTransf, e.totalLT);
}

bool gravarEstadoGlobal(const string& filename, Equipa* liga, int totalEquipas, Partida** calendario, int jornadaAtual, int modoDeJogo, int idUser) {
    ofstream out(filename);
    if (!out.is_open()) return false;
    out << totalEquipas << " " << jornadaAtual << " " << modoDeJogo << " " << idUser << "\n";
    for(int i = 0; i < 34; i++) {
        for(int j = 0; j < 9; j++) out << calendario[i][j].idCasa << " " << calendario[i][j].idFora << " " << calendario[i][j].golosCasa << " " << calendario[i][j].golosFora << " " << calendario[i][j].realizada << "\n";
    }
    for(int e = 0; e < totalEquipas; e++) {
        out << liga[e].pontos << " " << liga[e].vitorias << " " << liga[e].empates << " " << liga[e].derrotas << " " << liga[e].golosMarcados << " " << liga[e].golosSofridos << "\n";
        Plantel& p = *liga[e].plantel;

        out << p.tatica[0] << " " << p.tatica[1] << " " << p.tatica[2] << " " << p.tatica[3] << "\n";

        out << p.totalAtual << "\n";
        for(int i = 0; i < p.totalAtual; i++) {
            out << p.jogadores[i].nome << "\n" << p.jogadores[i].numero << " " << p.jogadores[i].pos << " " << p.jogadores[i].idade << " " << p.jogadores[i].probLesao << " " << p.jogadores[i].probCastigo << " " << p.jogadores[i].qualidade << " " << p.jogadores[i].jogosLesao << " " << p.jogadores[i].jogosCastigo << " " << p.jogadores[i].semanasTreino << "\n";
        }
        out << liga[e].totalLT << "\n";
        for(int i = 0; i < liga[e].totalLT; i++) {
            out << liga[e].ListaTransf[i].nome << "\n" << liga[e].ListaTransf[i].numero << " " << liga[e].ListaTransf[i].pos << " " << liga[e].ListaTransf[i].idade << " " << liga[e].ListaTransf[i].probLesao << " " << liga[e].ListaTransf[i].probCastigo << " " << liga[e].ListaTransf[i].qualidade << " " << liga[e].ListaTransf[i].jogosLesao << " " << liga[e].ListaTransf[i].jogosCastigo << " " << liga[e].ListaTransf[i].semanasTreino << "\n";
        }
    }
    out.close(); return true;
}

bool carregarEstadoGlobal(const string& filename, Equipa* liga, int totalEquipas, Partida** calendario, int& jornadaAtual, int& modoDeJogo, int& idUser) {
    ifstream in(filename);
    if (!in.is_open()) return false;
    int lidoTotalEquipas; in >> lidoTotalEquipas >> jornadaAtual >> modoDeJogo >> idUser;
    if (lidoTotalEquipas != totalEquipas) return false;
    for(int i = 0; i < 34; i++) {
        for(int j = 0; j < 9; j++) in >> calendario[i][j].idCasa >> calendario[i][j].idFora >> calendario[i][j].golosCasa >> calendario[i][j].golosFora >> calendario[i][j].realizada;
    }
    for(int e = 0; e < totalEquipas; e++) {
        in >> liga[e].pontos >> liga[e].vitorias >> liga[e].empates >> liga[e].derrotas >> liga[e].golosMarcados >> liga[e].golosSofridos;
        Plantel& p = *liga[e].plantel;

        in >> p.tatica[0] >> p.tatica[1] >> p.tatica[2] >> p.tatica[3];

        in >> p.totalAtual;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
        for(int i = 0; i < p.totalAtual; i++) {
            getline(in, p.jogadores[i].nome);
            int posInt;
            in >> p.jogadores[i].numero >> posInt >> p.jogadores[i].idade >> p.jogadores[i].probLesao >> p.jogadores[i].probCastigo >> p.jogadores[i].qualidade >> p.jogadores[i].jogosLesao >> p.jogadores[i].jogosCastigo >> p.jogadores[i].semanasTreino;
            p.jogadores[i].pos = static_cast<Posicao>(posInt);
            p.jogadores[i].jogouHoje = false;
            in.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        in >> liga[e].totalLT; in.ignore(numeric_limits<streamsize>::max(), '\n');
        for(int i = 0; i < liga[e].totalLT; i++) {
            getline(in, liga[e].ListaTransf[i].nome); int posInt;
            in >> liga[e].ListaTransf[i].numero >> posInt >> liga[e].ListaTransf[i].idade >> liga[e].ListaTransf[i].probLesao >> liga[e].ListaTransf[i].probCastigo >> liga[e].ListaTransf[i].qualidade >> liga[e].ListaTransf[i].jogosLesao >> liga[e].ListaTransf[i].jogosCastigo >> liga[e].ListaTransf[i].semanasTreino;
            liga[e].ListaTransf[i].pos = static_cast<Posicao>(posInt); liga[e].ListaTransf[i].jogouHoje = false; in.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    in.close(); return true;
}


// O novo gerador de calendário (Substitui o gerarJornadas antigo)
Partida** gerarCalendarioCompleto() {
    int totalJornadas = 34;
    Partida** cal = new Partida*[totalJornadas];
    for(int i = 0; i < totalJornadas; i++) cal[i] = new Partida[9];

    int equipas[18];
    for(int i = 0; i < 18; i++) equipas[i] = i;

    for(int ronda = 0; ronda < 17; ronda++) {
        for(int jogo = 0; jogo < 9; jogo++) {
            int casa = equipas[jogo];
            int fora = equipas[17 - jogo];
            if (jogo == 0 && ronda % 2 == 1) { int temp = casa; casa = fora; fora = temp; }

            cal[ronda][jogo].idCasa = casa; cal[ronda][jogo].idFora = fora;
            cal[ronda + 17][jogo].idCasa = fora; cal[ronda + 17][jogo].idFora = casa;
        }
        int ultimo = equipas[17];
        for(int i = 17; i > 1; i--) equipas[i] = equipas[i-1];
        equipas[1] = ultimo;
    }
    return cal;
}

// --- SUBSTITUIR A FUNÇÃO simularJornadaCompleta POR ESTA ---
void simularJornadaCompleta(Partida** cal, int jornadaAtual, Equipa* liga, int idUserToIgnore) {
    int idx = jornadaAtual - 1;
    cout << "\n=== RESULTADOS DA JORNADA " << jornadaAtual << " ===\n";
    for(int i = 0; i < 9; i++) {
        Partida& p = cal[idx][i];
        p.golosCasa = gerarAleatorio(0, 5); p.golosFora = gerarAleatorio(0, 5);
        p.realizada = true;
        Equipa& c = liga[p.idCasa]; Equipa& f = liga[p.idFora];

        c.golosMarcados += p.golosCasa; c.golosSofridos += p.golosFora;
        f.golosMarcados += p.golosFora; f.golosSofridos += p.golosCasa;

        if (p.golosCasa > p.golosFora) { c.pontos += 3; c.vitorias++; f.derrotas++; }
        else if (p.golosCasa < p.golosFora) { f.pontos += 3; f.vitorias++; c.derrotas++; }
        else { c.pontos++; f.pontos++; c.empates++; f.empates++; }
        cout << c.nome << " " << p.golosCasa << " - " << p.golosFora << " " << f.nome << "\n";

        // Aplica lesões, castigos e auto-cura aos BOTS (Ignorando a tua equipa no Modo Treinador)
        if (p.idCasa != idUserToIgnore) simularIncidentesBOT(*c.plantel);
        if (p.idFora != idUserToIgnore) simularIncidentesBOT(*f.plantel);
    }
}

// --- COLAR ESTAS DUAS FUNÇÕES NOVAS DE SEGUIDA ---
// Verifica de forma invisível (sem spamar erros no ecrã) se a equipa consegue formar o 11
bool podeFormarTatica(Plantel& p) {
    int aptosPos[4] = {0, 0, 0, 0};
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
            aptosPos[p.jogadores[i].pos]++;
        }
    }
    // Verifica se cumpre os mínimos para a tática default (1 GR, 4 DEF, 4 MED, 2 AVA)
    if (aptosPos[0] < p.tatica[0] || aptosPos[1] < p.tatica[1] ||
        aptosPos[2] < p.tatica[2] || aptosPos[3] < p.tatica[3]) {
        return false;
    }
    return true;
}

// 1. GERA TÁTICA ALEATÓRIA (SÓ PARA BOTS E MODO GLOBAL)
// GERA TÁTICA ALEATÓRIA SEGURA (Sem ciclos infinitos)
// 1. TENTA TODAS AS FORMAÇÕES ATÉ ENCONTRAR UMA VÁLIDA
bool inicializarTaticaAutomatica(Plantel& t, Plantel& p) {
    int aptos[4] = {0, 0, 0, 0};
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) aptos[p.jogadores[i].pos]++;
    }

    if (aptos[0] < 2) return false; // Precisa de pelo menos 2 GR (titular + suplente)

    int formacoes[9][3] = {
        {5,4,1}, {5,3,2}, {5,2,3}, {4,5,1}, {4,4,2}, {4,3,3}, {4,2,4}, {3,5,2}, {3,4,3}
    };

    bool encontrou = false;
    int d, m, a;
    for (int i = 0; i < 9; i++) {
        int idx = gerarAleatorio(0, 8);
        d = formacoes[idx][0]; m = formacoes[idx][1]; a = formacoes[idx][2];
        // Verifica se o plantel aguenta a tática sorteada + suplentes (2 DEF, 2 MED, 1 AVA)
        if (aptos[1] >= (d + 2) && aptos[2] >= (m + 2) && aptos[3] >= (a + 1)) {
            encontrou = true; break;
        }
    }

    if (!encontrou) return false;

    // GRAVAR A TÁTICA SORTEADA NO PLANTEL PERMANENTE DA EQUIPA
    p.tatica[0] = 1; p.tatica[1] = d; p.tatica[2] = m; p.tatica[3] = a;
    for(int k = 0; k < 4; k++) t.tatica[k] = p.tatica[k];

    t.capacidade = 17;
    t.jogadores = new Jogador[t.capacidade];
    t.totalAtual = 0;
    for (int k = 0; k < p.totalAtual; k++) p.jogadores[k].jogouHoje = false;

    Posicao posicoes[] = {GR, DEF, MED, AVA};
    // Preencher Titulares
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < t.tatica[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) t.jogadores[t.totalAtual++] = escolhido;
        }
    }
    // Preencher Suplentes (Regra fixa 1-2-2-1)
    int suplentesNecessarios[] = {1, 2, 2, 1};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < suplentesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) t.jogadores[t.totalAtual++] = escolhido;
        }
    }
    return true;
}

// 2. CURA DIRECIONADA: Recupera jogadores apenas se faltarem para os mínimos
void simularIncidentesBOT(Plantel& p) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
            if (gerarAleatorio(1, 100) <= p.jogadores[i].probCastigo) p.jogadores[i].jogosCastigo = gerarAleatorio(1, 5);
            else if (gerarAleatorio(1, 100) <= p.jogadores[i].probLesao) p.jogadores[i].jogosLesao = gerarAleatorio(1, 5);
        }
    }

    // Mínimos para garantir que pelo menos UMA tática é possível (ex: 5-4-1 ou 3-5-2)
    int minimos[4] = {2, 5, 4, 2}; // GR, DEF, MED, AVA
    for (int pos = 0; pos < 4; pos++) {
        int contaAptos = 0;
        for (int i = 0; i < p.totalAtual; i++)
            if (p.jogadores[i].pos == pos && p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) contaAptos++;

        while (contaAptos < minimos[pos]) {
            bool curouAlguem = false;
            for (int i = 0; i < p.totalAtual; i++) {
                if (p.jogadores[i].pos == pos && (p.jogadores[i].jogosLesao > 0 || p.jogadores[i].jogosCastigo > 0)) {
                    p.jogadores[i].jogosLesao = 0;
                    p.jogadores[i].jogosCastigo = 0;
                    contaAptos++;
                    curouAlguem = true;
                    break;
                }
            }
            if (!curouAlguem) break; // Sai se não houver mais ninguém para curar nesta posição
        }
    }
}


// Lógica para ordenar equipas na Tabela
bool compararEquipas(const Equipa& a, const Equipa& b) {
    if (a.pontos != b.pontos) return a.pontos > b.pontos;
    int difA = a.golosMarcados - a.golosSofridos;
    int difB = b.golosMarcados - b.golosSofridos;
    if (difA != difB) return difA > difB;
    return a.golosMarcados > b.golosMarcados;
}

// Mostra a Tabela Classificativa (Sem usar <algorithm>)
// Mostra a Tabela Classificativa (100% C++)
void exibirClassificacao(Equipa* liga, int totalEquipas) {
    Equipa** tabela = new Equipa*[totalEquipas];
    for(int i = 0; i < totalEquipas; i++) tabela[i] = &liga[i];
    for (int i = 0; i < totalEquipas - 1; i++) {
        for (int j = 0; j < totalEquipas - i - 1; j++) {
            if (!compararEquipas(*tabela[j], *tabela[j+1])) {
                Equipa* temp = tabela[j]; tabela[j] = tabela[j+1]; tabela[j+1] = temp;
            }
        }
    }
    cout << "\n====================================================================\n";
    cout << "                       TABELA CLASSIFICATIVA                        \n";
    cout << "====================================================================\n";
    cout << left << setw(4) << "POS" << "| " << setw(20) << "EQUIPA" << " | " << setw(3) << "PTS" << " | "
         << setw(2) << "V" << " | " << setw(2) << "E" << " | " << setw(2) << "D" << " | "
         << setw(3) << "GM" << " | " << setw(3) << "GS" << " | DG\n";
    cout << "--------------------------------------------------------------------\n";
    for(int i = 0; i < totalEquipas; i++) {
        int dg = tabela[i]->golosMarcados - tabela[i]->golosSofridos;
        cout << left << setw(3) << (i + 1) << " | " << setw(20) << tabela[i]->nome << " | "
             << setw(3) << tabela[i]->pontos << " | " << setw(2) << tabela[i]->vitorias << " | "
             << setw(2) << tabela[i]->empates << " | " << setw(2) << tabela[i]->derrotas << " | "
             << setw(3) << tabela[i]->golosMarcados << " | " << setw(3) << tabela[i]->golosSofridos << " | " << dg << "\n";
    }
    cout << "====================================================================\n";
    delete[] tabela;
}

// Função para preparar todas as equipas da liga para a jornada
void prepararTaticasJornada(Equipa* liga, int totalEquipas, int idUserModoTreinador, bool modoGlobal) {
    for (int i = 0; i < totalEquipas; i++) {
        // No Modo Treinador, não queremos que o PC mude a tática do utilizador!
        if (!modoGlobal && i == idUserModoTreinador) continue;

        Plantel& p = *liga[i].plantel;

        // 1. Criar um Plantel temporário para a tática (o "11" daquela jornada)
        // Nota: No teu projeto, cada equipa devia ter o seu próprio t para evitar fugas de memória.
        // Se a tática for guardada dentro da Equipa, usamos esse ponteiro.

        Plantel t;
        t.jogadores = nullptr;

        // 2. Tentar inicializar a tática automática
        // Se falhar (retornar false), a função simularIncidentesBOT já trata da cura direcionada
        if (!inicializarTaticaAutomatica(t, p)) {
            // Se falhou, curamos os necessários e tentamos de novo uma única vez
            simularIncidentesBOT(p);
            inicializarTaticaAutomatica(t, p);
        }

        // 3. Limpeza de memória: Como t foi apenas para teste de viabilidade ou visualização,
        // num simulador real, os resultados dos golos seriam baseados na qualidade deste t.
        if (t.jogadores != nullptr) delete[] t.jogadores;
    }
}
