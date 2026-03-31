#include "Inicializacao_Plantel_EDA_FC.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include "Estrutura_campeonato.h"
#include "Castigados_Lesionados.h"
#include "Taticas.h"
#include "Lista_Transferências.h"


void listaTranf(Equipa& e) {
    e.capacidadeLT = 100;
    e.totalLT = 0;
    e.ListaTransf = new Jogador[e.capacidadeLT];                 //array duinamico do tipo jogador 


}





void adicionarJogLT(Equipa& e, string* listaNomes, int totalNomes) {
    for (int i = 0; i < 2; i++) {   //  2 jogadores por jornada

        if (e.totalLT < e.capacidadeLT) {
            Jogador& novo = e.ListaTransf[e.totalLT];                              // 2. Criar um atalho para a primeira posição vazia no fim da lista
            preencherAtributosIndependentes(novo,listaNomes,totalNomes);
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



// Numero e Posicao dependem das restricoes do plantel
void preencherAtributosIndependentes(Jogador& novo, string* listaNomes, int totalNomes) {
    novo.nome = listaNomes[gerarAleatorio(0, totalNomes - 1)];
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








void TaparBuraco(Jogador* lista, int total,int j) {
    for (int i=j;i <total - 1;i++) {                    //arrasta todos os jogadores 1 posicao pra esquerda a partir do que foi retirado
        lista[i]=lista[i+1];
    }
}


void ContratarJogador(Plantel& p, Equipa& e) {
    cout << "\n--- MERCADO DE TRANSFERENCIAS ---\n";
    cout << "Insira o numero do jogador da LISTA DE TRANSFERENCIAS que pretende adquirir (digite 0 para cancelar): ";
    int numTransf;
    cin >> numTransf;

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
    bool plantelCheio = (p.totalAtual >= 30);
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

        TaparBuraco(e.ListaTransf, e.totalLT, indiceLT);
        e.totalLT--;

        cout << "Contratacao direta realizada com sucesso!\n";
    }

    // Organizar plantel e LT
    OrdenarPorPos(p.jogadores, p.totalAtual);
    OrdenarPorPos(e.ListaTransf, e.totalLT);
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


