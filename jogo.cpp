//
// Created by Sergio on 4/3/2026.
//

#include "jogo.h"
#include "core.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>


using namespace std;

//Por o nome das equipas num array
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
string** gerarJornadas( Equipa e, string* equipas) {
    int nJornadas =34;
    int nEquipas=17; //Se o ficheiro de equipas tiver 50 equipas, substituir este número por 17 ou 18
    auto **totalJornadas= new string*[nJornadas];
    baralhar(equipas,nEquipas);
    for (int i=0;i<nEquipas;i++) {
        totalJornadas[i] = new string[2];
        totalJornadas[i + nEquipas] = new string[2];
        if (i%2==0) {
            totalJornadas[i][0]=e.nome;
            totalJornadas[i][1]=equipas[i];

            totalJornadas[i+nEquipas][0]=equipas[i];
            totalJornadas[i+nEquipas][1]=e.nome;
        }
        else {
            totalJornadas[i][0]=equipas[i];
            totalJornadas[i][1]=e.nome;

            totalJornadas[i+nEquipas][0]=e.nome;
            totalJornadas[i+nEquipas][1]=equipas[i];
        }
    }
    return totalJornadas;
}

Equipa& encontrarEquipa(string nome, Equipa* liga, int nEquipas) {
    for (int i = 0; i < nEquipas; i++) {
        if (liga[i].nome == nome) {
            return liga[i];
        }
    }
    return liga[0]; // Caso de segurança
}
//Gerar o resultado de uma partida e alterar os pontos c:Casa, f:Fora, h:Home, a:Away
void gerarResultado(Equipa& h, Equipa& a) {
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
    cout<<h.nome<<" "<<c<<" - "<<f<<" "<<a.nome<<endl;
}

// Reduz 1 jogo de castigo/lesão a quem está de fora
// Reduz 1 jogo de castigo/lesão a quem está de fora e aplica os treinos!
void atualizarRecuperacoes(Tatica_Plantel& p) {
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

void simularIncidentesPartida(Tatica& t, Plantel& p) {
    cout << "Substituicoes:\n";
    bool houveSubs = false;
    int substituicoesFeitas = 0; // LIMITADOR DE 3 SUBSTITUIÇÕES

    for (int i = 0; i < 11; i++) {
        Jogador& titular = t.jogadores[i];

        int sorteLesao = gerarAleatorio(1, 100);
        int sorteCastigo = gerarAleatorio(1, 100);

        bool sofreuLesao = (sorteLesao <= titular.probLesao);
        bool sofreuCastigo = (sorteCastigo <= titular.probCastigo);

        if (sofreuLesao || sofreuCastigo) {
            if (sofreuLesao) aplicarIncidenteAoPlantel(p, titular.numero, true, gerarAleatorio(1, 3));
            if (sofreuCastigo) aplicarIncidenteAoPlantel(p, titular.numero, false, gerarAleatorio(1, 2));

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

void exibirCastigadosLesionados(const Tatica_Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "\nCastigados:\n";
    printf("%-20s | %-4s | %-7s | %-5s | %-9s | %-11s | %-9s | %-14s | %s\n",
           "Nome", "Nº", "Posicao", "Idade", "ProbLesao", "ProbCastigo", "Qualidade", "JogosRestantes", "Dias-Treino");
    cout << "-----------------------------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosCastigo > 0) {
            Jogador& j = p.jogadores[i];
            string lesaoStr = to_string(j.probLesao) + "%";
            string castigoStr = to_string(j.probCastigo) + "%";

            printf("%-20s | %-4d | %-7s | %-5d | %-9s | %-11s | %-9d | %-14d | %d\n",
                   j.nome.c_str(), j.numero, posTxt[j.pos], j.idade, lesaoStr.c_str(), castigoStr.c_str(), j.qualidade, j.jogosCastigo, j.semanasTreino);
        }
    }

    cout << "\nLesionados:\n";
    printf("%-20s | %-4s | %-7s | %-5s | %-9s | %-11s | %-9s | %s\n",
           "Nome", "Nº", "Posicao", "Idade", "ProbLesao", "ProbCastigo", "Qualidade", "JogosRestantes");
    cout << "----------------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao > 0) {
            Jogador& j = p.jogadores[i];
            string lesaoStr = to_string(j.probLesao) + "%";
            string castigoStr = to_string(j.probCastigo) + "%";

            printf("%-20s | %-4d | %-7s | %-5d | %-9s | %-11s | %-9d | %d\n",
                   j.nome.c_str(), j.numero, posTxt[j.pos], j.idade, lesaoStr.c_str(), castigoStr.c_str(), j.qualidade, j.jogosLesao);
        }
    }
}


Jogador encontrarMelhorDisponivel(const Plantel& p, const Tatica& t, Posicao pos) {
    Jogador melhor;
    melhor.qualidade = -1;

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador candidato = p.jogadores[i];

        // Só escolhe se for da posição, tiver mais qualidade, NÃO estiver convocado e NÃO estiver lesionado/castigado
        if (candidato.pos == pos && candidato.qualidade > melhor.qualidade) {
            if (!jogadorJaConvocado(t, candidato.nome) && candidato.jogosLesao == 0 && candidato.jogosCastigo == 0) {
                melhor = candidato;
            }
        }
    }
    return melhor;
}

bool inicializarTatica(Tatica& t, const Plantel& p, int formacaoEscolhida) {
    t.capacidade = 17;
    if (t.jogadores == nullptr) t.jogadores = new Jogador[t.capacidade];
    t.totalAtual = 0;

    int titulosNecessarios[4];
    string nomeTatica;

    if (formacaoEscolhida == 0) {
        titulosNecessarios[0] = 1; titulosNecessarios[1] = 4; titulosNecessarios[2] = 4; titulosNecessarios[3] = 2;
        nomeTatica = "4-4-2";
    } else if (formacaoEscolhida == 1) {
        titulosNecessarios[0] = 1; titulosNecessarios[1] = 4; titulosNecessarios[2] = 3; titulosNecessarios[3] = 3;
        nomeTatica = "4-3-3";
    } else {
        titulosNecessarios[0] = 1; titulosNecessarios[1] = 3; titulosNecessarios[2] = 5; titulosNecessarios[3] = 2;
        nomeTatica = "3-5-2";
    }

    Posicao posicoes[] = {GR, DEF, MED, AVA};
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    // ==========================================================
    // CASO 1: VERIFICAR TITULARES (Obrigatório respeitar posição)
    // ==========================================================
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < titulosNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            } else {
                // FALHOU: O plantel não tem jogadores aptos suficientes para esta posição
                cout << "\n[ERRO] Nao tem jogadores aptos suficientes na posicao " << posTxt[i]
                     << " para formar a tatica " << nomeTatica << "!\n";
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
        Jogador melhorGeral;
        melhorGeral.qualidade = -1;

        // Procura em todas as posições pelo melhor livre
        for (int i = 0; i < 4; i++) {
            Jogador candidato = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (candidato.qualidade > melhorGeral.qualidade) {
                melhorGeral = candidato;
            }
        }

        if (melhorGeral.qualidade != -1) {
            t.jogadores[t.totalAtual++] = melhorGeral;
            vagasVazias--;
            cout << "    -> Suplente Adaptado: " << melhorGeral.nome << " (" << posTxt[melhorGeral.pos] << ") foi convocado para a vaga.\n";
        } else {
            cout << "[CRITICO] Plantel dizimado! Nao ha mais jogadores disponiveis para sentar no banco.\n";
            break;
        }
    }

    return true; // Tática formada com sucesso
}

void exibirTatica(const Tatica& t) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "11 Inicial\n";
    cout << "Nome                 | N  | Posicao | Idade | ProbLesao | ProbCastigo | Qualidade\n";
    cout << "---------------------------------------------------------------------------------\n";

    // Imprime os primeiros 11 (Titulares)
    for (int i = 0; i < 11 && i < t.totalAtual; i++) {
        Jogador& j = t.jogadores[i];
        printf("%-20s | %-2d | %-7s | %-5d | %-8d%% | %-10d%% | %-9d\n",
               j.nome.c_str(), j.numero, posTxt[j.pos], j.idade, j.probLesao, j.probCastigo, j.qualidade);
    }

    cout << "Suplentes:\n";
    cout << "Nome                 | N  | Posicao | Idade | ProbLesao | ProbCastigo | Qualidade\n";
    cout << "---------------------------------------------------------------------------------\n";

    // Imprime do 11 para a frente (Suplentes)
    for (int i = 11; i < t.totalAtual; i++) {
        Jogador& j = t.jogadores[i];
        printf("%-20s | %-2d | %-7s | %-5d | %-8d%% | %-10d%% | %-9d\n",
               j.nome.c_str(), j.numero, posTxt[j.pos], j.idade, j.probLesao, j.probCastigo, j.qualidade);
    }
}

void listaTranf(Equipa& e) {
    e.capacidadeLT = 100;
    e.totalLT = 0;
    e.ListaTransf = new Jogador[e.capacidadeLT];                 //array duinamico do tipo jogador
}

void adicionarJogLT(Equipa& e, const Tatica_Plantel& p, string* listaNomes, int totalNomes) {
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

    printf("%-25s | %-4s | %-7s | %-5s | %-9s | %-11s | %s\n",
           "Nome", "Nº", "Posicao", "Idade", "ProbLesao", "ProbCastigo", "Qualidade");

    for (int i = 0; i < e.totalLT; i++) {
        Jogador& j = e.ListaTransf[i];

        string lesaoStr = to_string(j.probLesao) + "%";
        string castigoStr = to_string(j.probCastigo) + "%";

        printf("%-25s | %-4d | %-7s | %-5d | %-9s | %-11s | %d\n",
               j.nome.c_str(),
               j.numero,
               posTxt[j.pos],
               j.idade,
               lesaoStr.c_str(),
               castigoStr.c_str(),
               j.qualidade);
    }
    cout << "****************************************************************************************\n";
}

bool nomeJaExisteNoPlantel(const Tatica_Plantel& p, string nome) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].nome == nome) return true;
    }
    return false;
}

// Numero e Posicao dependem das restricoes do plantel
void preencherAtributosIndependentes(Jogador& novo, const Tatica_Plantel& p, string* listaNomes, int totalNomes) {
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
int ContarJogNumaPos(Tatica_Plantel& p, int j){                    // j indica a posição (0=GR, 1=DEF, 2=MED, 3=AVA)
    int contador = 0;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].pos == j) {
            contador++;
        }
    }
    return contador;
}

//Avalia se pode ser contratado para a posicao
bool PodeContratarParaPosicao(Tatica_Plantel& p, int j) {
    int limitesMaximos[] = {3, 10, 10, 7};
    int quantidadeAtual = ContarJogNumaPos(p, j);

    if (quantidadeAtual >= limitesMaximos[j]) {
        return false; // posicao ja atingiu o max de jogadores
    }

    return true; //ainda ha espaço pra mais jogadores nessa posicao
}

void ContratarJogador(Tatica_Plantel& p, Equipa& e) {
    cout << "\n--- MERCADO DE TRANSFERENCIAS ---\n";
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

bool gravarEstado(const string& filename, const Equipa& e, const Tatica_Plantel& p, int jornadaAtual) {
    ofstream out(filename);
    if (!out.is_open()) return false;

    // Guarda variáveis base
    out << jornadaAtual << "\n" << e.pontos << "\n" << p.totalAtual << "\n";

    // Guarda Plantel
    for (int i = 0; i < p.totalAtual; i++) {
        out << p.jogadores[i].nome << "\n";
        out << p.jogadores[i].numero << " " << p.jogadores[i].pos << " " << p.jogadores[i].idade << " "
            << p.jogadores[i].probLesao << " " << p.jogadores[i].probCastigo << " " << p.jogadores[i].qualidade << " "
            << p.jogadores[i].jogosLesao << " " << p.jogadores[i].jogosCastigo << " " << p.jogadores[i].semanasTreino << "\n";
    }

    // Guarda Lista de Transferências
    out << e.totalLT << "\n";
    for (int i = 0; i < e.totalLT; i++) {
        out << e.ListaTransf[i].nome << "\n";
        out << e.ListaTransf[i].numero << " " << e.ListaTransf[i].pos << " " << e.ListaTransf[i].idade << " "
            << e.ListaTransf[i].probLesao << " " << e.ListaTransf[i].probCastigo << " " << e.ListaTransf[i].qualidade << " "
            << e.ListaTransf[i].jogosLesao << " " << e.ListaTransf[i].jogosCastigo << " " << e.ListaTransf[i].semanasTreino << "\n";
    }

    out.close();
    return true;
}

bool carregarEstado(const string& filename, Equipa& e, Tatica_Plantel& p, int& jornadaAtual) {
    ifstream in(filename);
    if (!in.is_open()) return false;

    in >> jornadaAtual >> e.pontos >> p.totalAtual;
    in.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa a quebra de linha

    for (int i = 0; i < p.totalAtual; i++) {
        getline(in, p.jogadores[i].nome);
        int posInt;
        in >> p.jogadores[i].numero >> posInt >> p.jogadores[i].idade
           >> p.jogadores[i].probLesao >> p.jogadores[i].probCastigo >> p.jogadores[i].qualidade
           >> p.jogadores[i].jogosLesao >> p.jogadores[i].jogosCastigo >> p.jogadores[i].semanasTreino;

        p.jogadores[i].pos = static_cast<Posicao>(posInt);
        p.jogadores[i].jogouHoje = false;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    in >> e.totalLT;
    in.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < e.totalLT; i++) {
        getline(in, e.ListaTransf[i].nome);
        int posInt;
        in >> e.ListaTransf[i].numero >> posInt >> e.ListaTransf[i].idade
           >> e.ListaTransf[i].probLesao >> e.ListaTransf[i].probCastigo >> e.ListaTransf[i].qualidade
           >> e.ListaTransf[i].jogosLesao >> e.ListaTransf[i].jogosCastigo >> e.ListaTransf[i].semanasTreino;

        e.ListaTransf[i].pos = static_cast<Posicao>(posInt);
        e.ListaTransf[i].jogouHoje = false;
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    in.close();
    return true;
}

