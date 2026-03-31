//
// Created by Sergio on 3/20/2026.
//

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

void adicionarJogLT(Equipa& e, const Plantel& p, string* listaNomes, int totalNomes) {
    for (int i = 0; i < 2; i++) {   //  2 jogadores por jornada

        if (e.totalLT < e.capacidadeLT) {
            Jogador& novo = e.ListaTransf[e.totalLT];

            // Agora passamos o "p" para a função abaixo:
            preencherAtributosIndependentes(novo, p, listaNomes, totalNomes);

            Posicao posicoes[] = { GR, DEF, MED, AVA };
            novo.pos = posicoes[gerarAleatorio(0, 3)];

            int maiorNumero = 0;     //sistema de numeros pra nas transferencias nao duplicarem

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
    OrdenarPorChegadaLT(e);
}


void exibirListaTransf(const Equipa& e) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "************************************************************************\n";
    cout<< "Lista de Transferencias:\n";
    cout << "------------------------------------------------------------------------\n";

    cout << "NUM | POS | IDADE | QUAL | LESAO | CAST | NOME\n";

    for (int i = 0; i < e.totalLT; i++) {
        Jogador& j = e.ListaTransf[i];
        printf("%-3d | %-3s | %-5d | %-4d | %-4d%% | %-3d%% | %s\n",
               j.numero, posTxt[j.pos], j.idade, j.qualidade,
               j.probLesao, j.probCastigo, j.nome.c_str());
    }
    cout << "========================================================================\n";
}

bool nomeJaExisteNoPlantel(const Plantel& p, string nome) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].nome == nome) return true;
    }
    return false;
}


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



// Numero e Posicao dependem das restricoes do plantel
void preencherAtributosIndependentes(Jogador& novo, const Plantel& p, string* listaNomes, int totalNomes) {
    string nomeSorteado;
    // Sorteia nomes até calhar um que NÃO esteja no plantel
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


void OrdenarPorChegadaLT(Equipa& e) {
    for (int i=0; i<e.totalLT - 1 ; i++) {               //percorrer a lista de transferecencias
        for (int j=0; j<e.totalLT - i -1; j++) {           //comparar os da mesma posiçao pra ordenar por chegada(numero)
            if ((e.ListaTransf[j].pos == e.ListaTransf[j+1].pos) &&(e.ListaTransf[j].numero > e.ListaTransf[j+1].numero)){   //verificar posiçao e numero de chegada
                Jogador temporario = e.ListaTransf[j];
                e.ListaTransf[j] = e.ListaTransf[j+1];
                e.ListaTransf[j+1] = temporario;
            }
        }
    }

}

bool numeroJaExiste(const Plantel& p, int num, int indiceIgnorado) {
    for (int i = 0; i < p.totalAtual; i++) {

        // Se estivermos a olhar para a posição do próprio jogador que estamos a avaliar, saltamos!
        if (i == indiceIgnorado) {
            continue;
        }

        // Se encontrarmos o número noutro jogador, devolvemos true (já existe)
        if (p.jogadores[i].numero == num) {
            return true;
        }
    }
    return false; // Percorreu tudo e o número está livre
}

//Gerar numero para o jogador contratado
void GerarNumeroContratacao(Plantel& p, int indice) {
    int a = p.jogadores[indice].numero;

    // Passamos o 'indice' para ele ignorar o próprio jogador na verificação
    if (numeroJaExiste(p, a, indice)) {
        do {
            a = gerarAleatorio(1, 99);
        } while (numeroJaExiste(p, a, indice));
    }

    p.jogadores[indice].numero = a;
}


void TaparBuraco(Jogador* lista, int total,int j) {
    for (int i=j;i <total - 1;i++) {                    //arrasta todos os jogadores 1 posicao pra esquerda a partir do que foi retirado
        lista[i]=lista[i+1];
    }
}


void ContratarJogador(Plantel& p,Equipa& e) {
    if (p.totalAtual < 30) {
        int t = 30 - p.totalAtual;   //t guarda o n de vagas no plantel

        int l=e.totalLT-t;
        int h;
        if (l>=0) {
            h=t;
        }else {
            h=e.totalLT;
        }

        cout<<"O seu Plantel tem neste momento "<<p.totalAtual<<" jogadores.";
        cout<<"Pode contratar no maximo "<<h<<" jogadores de forma direta. Quantos ira querer contratar?""\n";
        int q;
        cin>>q;
        if (!cin.fail() && q<=t && q!= 0 && q <= e.totalLT) {    //verifica se q é inteiro, se nao ultrapassa capoacidade, se não é zero  e se é inferior ao total de jogaodres na lista
            for (int i = 1; i <= q; i++) {
                cout<<"Insira o numero do "<<i<<" jogador que pretende contratar:\n";
                int n;
                cin>>n;
                for (int j = 0; j < e.totalLT; j++) {

                    if (e.ListaTransf[j].numero == n) { //se for igual ao numero escolhido pelo operador
                        int posJog=e.ListaTransf[j].pos;
                        if (PodeContratarParaPosicao(p, posJog) == false) {
                            cout << "Transferencia cancelada! Ja atingiu o limite de jogadores para essa posicao.\n";
                            break;
                        }

                        p.jogadores[p.totalAtual]= e.ListaTransf[j];
                        GerarNumeroContratacao(p,p.totalAtual);                  // para nao haver numeros iguais no plantel ao adiciona lo

                        p.totalAtual++;                             //Jogador entra no plantel
                        OrdenarPorPos(p.jogadores, p.totalAtual);
                        TaparBuraco(e.ListaTransf, e.totalLT,j);
                        e.totalLT--;                                 //jogador sai da LT
                        break;
                    }
                }
            }
        }else {
            cout<<"Nao e possivel realizar esse numero de contratacoes diretas.O plantel pode estar cheio ou a lista de transferencias com jogadores insuficientes. \n";
            int k;
            cin>>k;
        }
    } else {
        cout << "O plantel esta cheio. Tera de realizar uma troca de um jogador do plantel com um da lista de transferencias.\n";
        cout << "Quantos jogadores ira querer trocar?\n";
        int r;
        cin >> r;

        if (!cin.fail() && r > 0 && r <= e.totalLT) {        // verificar se é um inteiro etc

            for (int i = 1; i <= r; i++) {                 //ciclo tendo em conta o numero de trocas
                int numPlantel, numTransf;
                cout << "TROCA " << i << "\n";
                cout << "Insira o numero do jogador do seu PLANTEL que vai sair: ";
                cin >> numPlantel;
                cout << "Insira o numero do jogador da LISTA DE TRANSFERENCIAS que vai entrar: ";
                cin >> numTransf;

                int indicePlantel = -1;  //nao podia ser 0 porque no ciclo for comeca em 0
                int indiceTransf = -1;

                for (int k = 0; k < p.totalAtual; k++) {                //percorre plantel
                    if (p.jogadores[k].numero == numPlantel) indicePlantel = k;
                }
                for (int j = 0; j < e.totalLT; j++) {                     //percorre LT
                    if (e.ListaTransf[j].numero == numTransf) indiceTransf = j;
                }

                if (indicePlantel != -1 && indiceTransf != -1) {  //Se encontrou os dois

                    Posicao posSair = p.jogadores[indicePlantel].pos;
                    Posicao posEntrar = e.ListaTransf[indiceTransf].pos;

                    if (posSair != posEntrar ) {   //se as posicoes sao diferentes
                        cout << "Troca cancelada! Os jogadores teem posicoes diferentes.\n";
                        continue; //salta para a proxima troca (proximo ciclo)
                    }


                    int maiorNumero = 0;                      //dar numero ao jogador que entrou na LT
                    for (int m = 0; m < e.totalLT; m++) {
                        if (e.ListaTransf[m].numero > maiorNumero) maiorNumero = e.ListaTransf[m].numero;
                    }

                    //Troca em si
                    Jogador temporario = p.jogadores[indicePlantel];
                    p.jogadores[indicePlantel] = e.ListaTransf[indiceTransf];
                    e.ListaTransf[indiceTransf] = temporario;

                    GerarNumeroContratacao(p, indicePlantel);


                    e.ListaTransf[indiceTransf].numero = maiorNumero + 1;

                    cout << "Troca realizada com sucesso!\n";
                    OrdenarPorChegadaLT(e);

                } else {
                    cout << "Erro: Um dos numeros inseridos nao existe!\n";
                }
            }
        } else {
            cout << "Numero de trocas invalido!\n";
        }
    }
}


/*//Conta o numero de jogadores por posicao
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
}*/
