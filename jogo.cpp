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
            aplicarIncidenteAoPlantel(p, titular.numero, false, gerarAleatorio(1, 2));
            cout<<titular.nome<<" castigado, nao pode ser substituido, a equipa joga com menos 1 jogador\n";
        }

        if (sofreuLesao && !sofreuCastigo) {
            aplicarIncidenteAoPlantel(p, titular.numero, true, gerarAleatorio(1, 3));


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
    // Se não há jogadores no mercado, o Bot não faz nada
    if (e.totalLT == 0) return;

    // O Bot tem 50% de hipóteses de querer ir ao mercado nesta jorada
    if (gerarAleatorio(0, 1) == 0) return;

    // O Bot escolhe um jogador aleatório
    int indiceLT = gerarAleatorio(0, e.totalLT - 1);
    int posDesejada = e.ListaTransf[indiceLT].pos;

    // Avaliar Limites
    bool plantelCheio = (p.totalAtual >= p.capacidade);
    bool posicaoCheia = !PodeContratarParaPosicao(p, posDesejada);

    if (plantelCheio || posicaoCheia) {
        // LÓGICA DE TROCA: O Bot precisa de encontrar alguém da mesma posição no plantel
        int indicePlantel = -1;
        for (int k = 0; k < p.totalAtual; k++) {
            if (p.jogadores[k].pos == posDesejada) {
                indicePlantel = k;
                break; // Encontrou o jogador a dispensar(trocar)
            }
        }

        // O Bot só efetua a troca se tiver alguém da mesma posição para sair
        if (indicePlantel != -1) {
            Jogador temporario = p.jogadores[indicePlantel];
            p.jogadores[indicePlantel] = e.ListaTransf[indiceLT];
            e.ListaTransf[indiceLT] = temporario;

        }
    } else {
        // LÓGICA DE COMPRA DIRETA
        p.jogadores[p.totalAtual] = e.ListaTransf[indiceLT];
        p.totalAtual++;

        // "Tapar o buraco"
        for (int i = indiceLT; i < e.totalLT - 1; i++) {
            e.ListaTransf[i] = e.ListaTransf[i + 1];
        }
        e.totalLT--;
    }

    //Manter as listas sempre organizadas por posição
    OrdenarPorPos(p.jogadores, p.totalAtual);
    OrdenarPorPos(e.ListaTransf, e.totalLT);
}


void processarMercadoGlobal(Equipa* liga, int nEquipas, string* bancoNomes, int nNomes) {
    for (int i = 0; i < nEquipas; i++) {
        // 1. TODAS as equipas (incluindo EDA FC) recebem 2 jogadores novos na lista
        adicionarJogLT(liga[i], *liga[i].plantel, bancoNomes, nNomes);

        // 2. Os BOTS (equipas 1 a 17) decidem se querem contratar
        if (i > 0) {
            ContratarJogadorBOT(*liga[i].plantel, liga[i]);
        }

        // O utilizador humano não faz nada aqui, porque ele vai ao mercado manualmente quando quiser através do Menu de Gestão
    }
}

bool gravarEstado(const string& filename, const Equipa& e, const Plantel& p, int jornadaAtual) {
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

bool carregarEstado(const string& filename, Equipa& e, Plantel& p, int& jornadaAtual) {
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

