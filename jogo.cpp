#include "jogo.h"
#include "core.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>


using namespace std;

//Por o nome das equipas num array
//Por o nome das equipas num array E BARALHAR

/**
 * carrega a lista de nomes das equipas e baralha as iniciais
 * @param f nome do ficheiro de texto
 * @return array (lista) de strings com os nomes das equipas
 */
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

    // antes de devolver o array para o main.cpp
    baralhar(equipas, nEquipas);

    return equipas;
}

//baralha as equipas aleatóriamente dentro do array

/**
 * baralha aleatoriamente a ordem dos elementos num array de strings
 * @param equipas array a baralhar
 * @param nEquipas tamanho do array
 */
void baralhar(string* equipas, int nEquipas ) {
    // percorre o array do fim para o início
    for (int i = nEquipas - 1; i > 0; i--) {
        // gera um índice aleatório entre 0 e i
        int j = gerarAleatorio(0, i);

        // troca os elementos
        string temp = equipas[i];
        equipas[i] = equipas[j];
        equipas[j] = temp;
    }
}

/**
 * procura uma equipa pelo nome dentro do array da liga
 * @param nome - nome da equipa a procurar
 * @param liga - array contendo todas as equipas do campeonato
 * @param nEquipas - numero total de equipas na liga
 * @return - referencia para a equipa encontrada ou a primeira equipa
 */
Equipa& encontrarEquipa(string nome, Equipa* liga, int nEquipas) {
    for (int i = 0; i < nEquipas; i++) {
        if (liga[i].nome == nome) {
            return liga[i];
        }
    }
    return liga[0]; // caso de segurança
}

/**
 * atualiza o estado fisico dos jogadores e aplica a progressao de qualidade por treino
 * @param p - referencia para o plantel a ser atualizado
 */
void atualizarRecuperacoes(Plantel& p) {
    for (int i = 0; i < p.totalAtual; i++) {
        // recuperação de lesões e castigos
        if (p.jogadores[i].jogosLesao > 0) p.jogadores[i].jogosLesao--;
        if (p.jogadores[i].jogosCastigo > 0) p.jogadores[i].jogosCastigo--;

        // aplicação do treino melhorar qualidade
        // se tem semanas de treino e NÃO está lesionado:
        if (p.jogadores[i].semanasTreino > 0 && p.jogadores[i].jogosLesao == 0) {
            p.jogadores[i].semanasTreino--;       // Desconta 1 semana
            p.jogadores[i].qualidade += 5;        // Aumenta 5 de qualidade

            // o limite máximo de qualidade é 100
            if (p.jogadores[i].qualidade > 100) {
                p.jogadores[i].qualidade = 100;
            }
        }
    }
}

// aplica incidentes da jornada ao plantel real
/**
 * atribui um tempo de paragem por lesao ou castigo a um jogador especifico no plantel
 * @param p - referencia para o plantel que contem o jogador
 * @param numero - numero da camisola do jogador
 * @param lesao - booleano que indica se o incidente é lesao (true) ou castigo (false)
 * @param tempo - numero de jogos de paragem a aplicar
 */
void aplicarIncidenteAoPlantel(Plantel& p, int numero, bool lesao, int tempo) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == numero) {
            if (lesao) p.jogadores[i].jogosLesao = tempo;
            else p.jogadores[i].jogosCastigo = tempo;
            break;
        }
    }
}

/**
 * simula a ocorrencia de lesoes e castigos durante uma partida e gere as substituicoes
 * @param t - referencia para a tatica/equipa em campo
 * @param p - referencia para o plantel global da equipa para persistencia de dados
 */
void simularIncidentesPartida(Plantel& t, Plantel& p) {
    cout << "Substituicoes:\n";
    bool houveSubs = false;
    int substituicoesFeitas = 0; // limite de 3 substituicoes

    for (int i = 11; i < t.totalAtual; i++) {
        t.jogadores[i].jogouHoje = false; // suplentes começam no banco
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

                // procurar suplente da MESMA posição
                for (int j = 11; j < t.totalAtual; j++) {
                    if (t.jogadores[j].pos == titular.pos && !t.jogadores[j].jogouHoje) {
                        idxSuplente = j;
                        break;
                    }
                }

                // se não houver da mesma, procura QUALQUER suplente
                if (idxSuplente == -1) {
                    for (int j = 11; j < t.totalAtual; j++) {
                        if (!t.jogadores[j].jogouHoje) {
                            idxSuplente = j;
                            break;
                        }
                    }
                }

                // efetua substituição
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

/**
 * imprime no ecra as listas separadas de jogadores que se encontram lesionados ou castigados
 * @param p - referencia constante para o plantel a exibir
 */
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

/**
 * procura no plantel o jogador apto com maior qualidade para uma determinada posicao
 * @param p - referencia para o plantel global
 * @param t - referencia para a tatica (para verificar jogadores ja selecionados)
 * @param pos - posicao pretendida
 * @return - objeto Jogador com a maior qualidade encontrada ou erro se nenhum disponivel
 */
Jogador encontrarMelhorDisponivel(Plantel& p, Plantel& t, Posicao pos) {
    Jogador melhor;
    melhor.qualidade = -1;
    int indiceMelhor = -1;

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador candidato = p.jogadores[i];

        // só escolhe se for da posição, tiver mais qualidade, NÃO estiver convocado e NÃO estiver lesionado/castigado
        if (candidato.pos == pos && candidato.qualidade > melhor.qualidade) {
            if (!p.jogadores[i].jogouHoje && candidato.jogosLesao == 0 && candidato.jogosCastigo == 0) {
                melhor = candidato;
                indiceMelhor=i;
            }
        }
    }
    if (indiceMelhor != -1) {
        p.jogadores[indiceMelhor].jogouHoje = true;

        return p.jogadores[indiceMelhor];
    }
    Jogador erro;
    erro.qualidade = -1;
    return erro;
}

/**
 * preenche a lista de convocados (titulares e suplentes) com base na tatica definida e jogadores aptos
 * @param t - referencia para a estrutura de tatica a ser preenchida
 * @param p - referencia para o plantel de onde os jogadores serao retirados
 * @return - true se a tatica foi inicializada com sucesso, false se faltarem titulares obrigatorios
 */
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


    // verifica os titulares
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < titularesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            } else {
                // FALHOU: o plantel não tem jogadores aptos suficientes para esta posição
                cout << "\n[ERRO] Nao tem jogadores aptos suficientes na posicao " << posTxt[i]<< " para formar a tatica " << nomeTatica << "!\n";
                return false; // retorna falso para obrigar o utilizador a mudar
            }
        }
    }

    // verifica os suplentes
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

    // preenche as vagas vazias no banco com qualquer jogador livre (ignora a posição)
    while (vagasVazias > 0 && t.totalAtual < 17) {
        int indiceMelhor = -1;
        int maiorQual = -1;

        // procura DIRETAMENTE no plantel pelo melhor jogador que ainda não jogou
        for (int i = 0; i < p.totalAtual; i++) {
            if (!p.jogadores[i].jogouHoje && p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
                if (p.jogadores[i].qualidade > maiorQual) {
                    maiorQual = p.jogadores[i].qualidade;
                    indiceMelhor = i;
                }
            }
        }

        if (indiceMelhor != -1) {
            // marca o jogador para não ser clonado e adiciona-o à tática
            p.jogadores[indiceMelhor].jogouHoje = true;
            t.jogadores[t.totalAtual++] = p.jogadores[indiceMelhor];
            vagasVazias--;

            cout << "   -> Suplente Adaptado: " << p.jogadores[indiceMelhor].nome
                 << " (" << posTxt[p.jogadores[indiceMelhor].pos] << ") foi convocado para a vaga.\n";
        } else {
            cout << "[CRITICO] Plantel dizimado! Nao ha mais jogadores disponiveis para sentar no banco.\n";
            break; // sai do ciclo se não houver mais ninguém vivo no clube
        }
    }
    return true; // tática formada com sucesso
}

/**
 * exibe a lista de jogadores titulares (11 inicial) e suplentes convocados para a partida
 * @param t - referência para o plantel da tática (convocados) a exibir
 */
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

/**
 * aloca memória e inicializa a lista de transferências de uma equipa com a capacidade padrão
 * @param e - referência para a equipa cuja lista de transferências será inicializada
 */
void listaTranf(Equipa& e) {
    e.capacidadeLT = 100;
    e.totalLT = 0;
    e.ListaTransf = new Jogador[e.capacidadeLT];                 //array duinamico do tipo jogador
}

/**
 * gera novos jogadores aleatórios e adiciona-os à lista de transferências, garantindo números de camisola únicos
 * @param e - referência para a equipa que gere a lista de transferências
 * @param p - referência para o plantel (utilizado para validar nomes únicos)
 * @param listaNomes - array de strings com os nomes base disponíveis
 * @param totalNomes - tamanho do array de nomes
 */
void adicionarJogLT(Equipa& e, const Plantel& p, string* listaNomes, int totalNomes) {
    for (int i = 0; i < 2; i++) {   //  2 jogadores por jornada

        if (e.totalLT < e.capacidadeLT) {
            Jogador& novo = e.ListaTransf[e.totalLT];                              // cria um atalho para a primeira posição vazia no fim da lista

            preencherAtributosIndependentes(novo, p, listaNomes, totalNomes);

            Posicao posicoes[] = { GR, DEF, MED, AVA };
            novo.pos = posicoes[gerarAleatorio(0, 3)];

            int maiorNumero = 30;     //sistema de numeros pra nas transferencias nao duplicarem

            for (int j = 0; j < e.totalLT; j++) {
                if (e.ListaTransf[j].numero > maiorNumero) {      //procura o numero maior na lista de transferencias
                    maiorNumero = e.ListaTransf[j].numero;
                }
            }

            novo.numero = maiorNumero + 1;   // o novo jogador recebe o maior número encontrado + 1
            e.totalLT++;
        }
    }
    OrdenarPorPos(e.ListaTransf, e.totalLT);
}

/**
 * exibe no ecrã o mercado de transferências formatado em tabela
 * @param e - referência para a equipa que detém a lista de transferências a exibir
 */
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

/**
 * valida se um nome já está a ser utilizado por algum jogador no plantel
 * @param p - referência para o plantel a verificar
 * @param nome - string com o nome a procurar
 * @return - true se o nome já existir no plantel, false caso contrário
 */
bool nomeJaExisteNoPlantel(const Plantel& p, string nome) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].nome == nome) return true;
    }
    return false;
}

// numero e posicao dependem das restricoes do plantel
/**
 * preenche um objeto Jogador com atributos aleatórios básicos e um nome único
 * @param novo - referência para o jogador a ser preenchido
 * @param p - referência para o plantel (para validação de nome único)
 * @param listaNomes - array de nomes disponíveis
 * @param totalNomes - total de nomes no array
 */
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

/**
 * ordena um array de jogadores de acordo com a sua posição (GR < DEF < MED < AVA)
 * @param lista - array de jogadores a ordenar
 * @param total - número total de jogadores na lista
 */
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

//conta o numero de jogadores por posicao
/**
 * calcula a quantidade de jogadores que pertencem a uma posição específica num plantel
 * @param p - referência para o plantel
 * @param j - identificador numérico da posição (0-3)
 * @return - contador total de jogadores na posição indicada
 */
int ContarJogNumaPos(Plantel& p, int j){                    // j indica a posição (0=GR, 1=DEF, 2=MED, 3=AVA)
    int contador = 0;
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].pos == j) {
            contador++;
        }
    }
    return contador;
}

//avalia se pode ser contratado para a posicao
/**
 * verifica se a contratação de um jogador para determinada posição é permitida pelos limites do clube
 * @param p - referência para o plantel
 * @param j - identificador numérico da posição a validar
 * @return - true se houver vaga na posição, false se o limite máximo for atingido
 */
bool PodeContratarParaPosicao(Plantel& p, int j) {
    int limitesMaximos[] = {3, 10, 10, 7};
    int quantidadeAtual = ContarJogNumaPos(p, j);

    if (quantidadeAtual >= limitesMaximos[j]) {
        return false; // posicao ja atingiu o max de jogadores
    }

    return true; //ainda ha espaço pra mais jogadores nessa posicao
}

/**
 * gere o menu e a lógica de aquisição de jogadores, permitindo compra direta ou troca forçada por excesso de jogadores
 * @param p - referência para o plantel do utilizador
 * @param e - referência para a equipa que gere o mercado de transferências
 */
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

    if (numTransf == 0) return; // o treinador desistiu de conratar um novo jogador

    // procura o jogador na Lista de Transferências
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

    //avalia os limites (alteraçao falada com o prof)
    int posDesejada = e.ListaTransf[indiceLT].pos;   //guarda posiçao
    bool plantelCheio = (p.totalAtual >= p.capacidade);
    bool posicaoCheia = !PodeContratarParaPosicao(p, posDesejada);

    // decide se será compra direta ou troca
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

    // Organiza plantel e LT
    OrdenarPorPos(p.jogadores, p.totalAtual);
    OrdenarPorPos(e.ListaTransf, e.totalLT);
}


/**
 * gere a lógica de contratação automática para clubes controlados pelo computador, permitindo trocas ou compras diretas da lista de transferências
 * @param p - referência para o plantel da equipa bot
 * @param e - referência para a estrutura da equipa bot
 */
void ContratarJogadorBOT(Plantel& p, Equipa& e) {
    if (e.totalLT == 0) return;

    // tenta analisar até 3 jogadores da lista por jornada
    for (int tentativa = 0; tentativa < 3; tentativa++) {
        if (e.totalLT == 0) break;

        int idxLT = gerarAleatorio(0, e.totalLT - 1);
        Jogador& candidato = e.ListaTransf[idxLT];
        int nNaPos = ContarJogNumaPos(p, candidato.pos);
        int limitesMaximos[] = {3, 10, 10, 7}; //

        // COMPRA: se houver vaga real
        if (nNaPos < limitesMaximos[candidato.pos] && p.totalAtual < p.capacidade) {
            p.jogadores[p.totalAtual++] = candidato;
            for (int i = idxLT; i < e.totalLT - 1; i++) e.ListaTransf[i] = e.ListaTransf[i + 1];
            e.totalLT--;
        }
        // TROCA: se o candidato for MELHOR que o pior do plantel naquela posição
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

/**
 * processa as atividades de mercado (vendas, compras e geração de novos jogadores) para todas as equipas da liga em cada jornada
 * @param liga - array contendo todas as equipas do campeonato
 * @param nEquipas - número total de equipas
 * @param bancoNomes - pool de nomes para a geração de novos jogadores
 * @param nNomes - total de nomes disponíveis no banco
 * @param idUser - identificador do utilizador humano para evitar automação no seu clube
 * @param ehGlobal - booleano que define se a simulação é 100% automática
 */
void processarMercadoGlobal(Equipa* liga, int nEquipas, string* bancoNomes, int nNomes, int idUser, bool ehGlobal) {
    for (int i = 0; i < nEquipas; i++) {
        adicionarJogLT(liga[i], *liga[i].plantel, bancoNomes, nNomes);

        // se for Modo Global, TODOS agem. se for Treinador, apenas os outros clubes (i != idUser)
        if (ehGlobal || i != idUser) {
            ListarJogadorBOT(liga[i]);
            ContratarJogadorBOT(*liga[i].plantel, liga[i]);
        }
    }
}

/**
 * permite que os clubes controlados pelo computador coloquem jogadores excedentários (acima do mínimo por posição) na lista de transferências
 * @param e - referência para a equipa bot que irá listar jogadores
 */
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

/**
 * guarda todos os dados atuais do campeonato, incluindo calendário, estatísticas e plantéis, num ficheiro de texto
 * @param filename - caminho (path) e nome do ficheiro de salvaguarda
 * @param liga - array de equipas a gravar
 * @param totalEquipas - número de equipas no campeonato
 * @param calendario - matriz dinâmica com o calendário de jogos
 * @param jornadaAtual - índice da jornada em que o campeonato se encontra
 * @param modoDeJogo - modo atual (Treinador ou Global)
 * @param idUser - ID da equipa controlada pelo utilizador
 * @return - true se a gravação for bem sucedida, false caso contrário
 */
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

/**
 * recupera o estado de um campeonato guardado anteriormente a partir de um ficheiro de texto
 * @param filename - nome do ficheiro a carregar
 * @param liga - array de equipas a preencher
 * @param totalEquipas - número de equipas esperado
 * @param calendario - matriz do calendário a preencher
 * @param jornadaAtual - referência para atualizar a jornada atual
 * @param modoDeJogo - referência para atualizar o modo de jogo
 * @param idUser - referência para atualizar o ID do utilizador
 * @return - true se o carregamento for bem sucedido, false caso contrário
 */
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


// o novo gerador de calendário (substitui o gerarJornadas antigo)
/**
 * cria uma matriz dinâmica que contém o escalonamento de todas as 34 jornadas do campeonato para 18 equipas (todos contra todos)
 * @return - matriz dinâmica de ponteiros para Partida representando o calendário completo
 */
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

// SUBSTITUI A FUNCAO simularJornadaCompleta POR ESTA
/**
 * executa todos os jogos de uma jornada específica, gera resultados aleatórios e atualiza as estatísticas e incidentes dos bots
 * @param cal - matriz do calendário do campeonato
 * @param jornadaAtual - número da jornada a simular
 * @param liga - array de equipas para atualização de pontos e golos
 * @param idUserToIgnore - ID do utilizador para saltar a simulação automática de incidentes no seu clube
 */
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



// TENTA TODAS AS FORMAÇÕES ATÉ ENCONTRAR UMA VÁLIDA
/**
 * define aleatoriamente uma formação tática válida para um bot com base na disponibilidade atual de jogadores aptos
 * @param t - referência para a estrutura de tática a preencher
 * @param p - referência para o plantel de origem
 * @return - true se encontrar uma formação válida, false se for necessária intervenção de cura
 */
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
        // verifica se o plantel aguenta a tática sorteada + suplentes (2 DEF, 2 MED, 1 AVA)
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
    // preencher Titulares
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < t.tatica[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) t.jogadores[t.totalAtual++] = escolhido;
        }
    }
    // preencher suplentes (regra fixa 1-2-2-1)
    int suplentesNecessarios[] = {1, 2, 2, 1};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < suplentesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) t.jogadores[t.totalAtual++] = escolhido;
        }
    }
    return true;
}

// recupera jogadores apenas se faltarem para os mínimos
/**
 * aplica lesões e castigos aleatórios aos jogadores de um bot e força recuperações cirúrgicas se o plantel ficar abaixo dos mínimos funcionais
 * @param p - referência para o plantel do bot a processar
 */
void simularIncidentesBOT(Plantel& p) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].jogosLesao == 0 && p.jogadores[i].jogosCastigo == 0) {
            if (gerarAleatorio(1, 100) <= p.jogadores[i].probCastigo) p.jogadores[i].jogosCastigo = gerarAleatorio(1, 5);
            else if (gerarAleatorio(1, 100) <= p.jogadores[i].probLesao) p.jogadores[i].jogosLesao = gerarAleatorio(1, 5);
        }
    }

    // mínimos para garantir que pelo menos UMA tática é possível (ex: 5-4-1 ou 3-5-2)
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
            if (!curouAlguem) break; // sai se não houver mais ninguém para curar nesta posição
        }
    }
}


// lógica para ordenar equipas na Tabela
/**
 * função auxiliar para comparar duas equipas visando a ordenação da tabela classificativa
 * @param a - primeira equipa para comparação
 * @param b - segunda equipa para comparação
 * @return - true se a equipa 'a' deve estar acima da 'b' (pontos, DG, GM), false caso contrário
 */
bool compararEquipas(const Equipa& a, const Equipa& b) {
    if (a.pontos != b.pontos) return a.pontos > b.pontos;
    int difA = a.golosMarcados - a.golosSofridos;
    int difB = b.golosMarcados - b.golosSofridos;
    if (difA != difB) return difA > difB;
    return a.golosMarcados > b.golosMarcados;
}

/**
 * ordena as equipas por mérito desportivo e imprime a tabela classificativa formatada no ecrã
 * @param liga - array de equipas a classificar
 * @param totalEquipas - número total de equipas na liga
 */
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

// função para preparar todas as equipas da liga para a jornada
/**
 * percorre as equipas da liga para inicializar as suas táticas e preparar os 11 iniciais antes da simulação de uma jornada
 * @param liga - array de equipas da liga
 * @param totalEquipas - total de equipas
 * @param idUserModoTreinador - ID do utilizador para evitar alterar a sua tática manual
 * @param modoGlobal - booleano que indica se o sistema deve agir em todas as equipas
 */
void prepararTaticasJornada(Equipa* liga, int totalEquipas, int idUserModoTreinador, bool modoGlobal) {
    for (int i = 0; i < totalEquipas; i++) {
        // No Modo Treinador, não queremos que o PC mude a tática do utilizador
        if (!modoGlobal && i == idUserModoTreinador) continue;

        Plantel& p = *liga[i].plantel;

        // Criar um Plantel temporário para a tática (o "11" daquela jornada)
        // Nota: No nosso projeto, cada equipa devia ter o seu próprio t para evitar fugas de memória
        // Se a tática for guardada dentro da Equipa, usamos esse ponteiro

        Plantel t;
        t.jogadores = nullptr;

        // tentar inicializar a tática automática
        if (!inicializarTaticaAutomatica(t, p)) {
            // se falhou, curamos os necessários e tentamos de novo uma única vez
            simularIncidentesBOT(p);
            inicializarTaticaAutomatica(t, p);
        }

        // Limpeza de memória: Como t foi apenas para teste de viabilidade ou visualização,
        if (t.jogadores != nullptr) delete[] t.jogadores;
    }
}
