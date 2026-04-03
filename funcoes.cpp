/*
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "definicoes.h"
#include "Estrutura_campeonato.h"
#include "Inicializacao_Plantel_EDA_FC.h"
#include "Castigados_Lesionados.h"
#include "Taticas.h"
#include "Lista_Transferências.h"


using namespace std;

int gerarAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Função auxiliar para verificar se um número de camisola já existe no plantel
/*bool numeroJaExiste(Plantel& p, int num) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == num) return true;
    }
    return false;
}

bool numeroJaExiste2(Tatica& t, int num) {
    for (int i = 0; i < t.totalAtual; i++) {
        if (t.jogadores[i].numero == num) return true;
    }
    return false;
}#1#

string* carregarNomes(string filename, int& totalNomes) {
    ifstream file(filename);
    if (!file.is_open()) return nullptr;

    string linha;
    totalNomes = 0;
    while (getline(file, linha)) {
        if (!linha.empty()) totalNomes++;
    }

    string* nomes = new string[totalNomes];
    file.clear();
    file.seekg(0);

    int i = 0;
    while (getline(file, linha)) {
        if (!linha.empty()) {
            nomes[i] = linha;
            i++;
        }
    }
    file.close();
    return nomes;
}

/*bool jogadorJaConvocado(const Tatica& t, const string& nomeJogador) {
    for (int i = 0; i < t.totalAtual; i++) {
        if (t.jogadores[i].nome == nomeJogador) {
            return true;
        }
    }
    return false;
}#1#

/*Jogador encontrarMelhorDisponivel(const Plantel& p, const Tatica& t, Posicao pos) {
    Jogador melhor;
    melhor.qualidade = -1; // Valor inicial baixo para garantir que encontramos alguém

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador candidato = p.jogadores[i];

        // Verifica se é da posição certa e se é melhor que o anterior encontrado
        if (candidato.pos == pos && candidato.qualidade > melhor.qualidade) {
            // Verifica se NÃO está já convocado
            if (!jogadorJaConvocado(t, candidato.nome)) {
                // NOTA: Aqui mais tarde podes adicionar a verificação de lesões!
                melhor = candidato;
            }
        }
    }
    return melhor;
}

void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes) {
    p.capacidade = gerarAleatorio(20, 30);
    p.jogadores = new Jogador[p.capacidade];
    p.totalAtual = 0;

    // Cálculo das quantidades por posição (Lógica da pergunta anterior)
    int nGR = 2, nDEF = 7, nMED = 7, nAVA = 4;
    int soma = nGR + nDEF + nMED + nAVA;

    while (soma < p.capacidade) {
        int r = gerarAleatorio(0, 3);
        if (r == 0 && nGR < 3) { nGR++; soma++; }
        else if (r == 1 && nDEF < 10) { nDEF++; soma++; }
        else if (r == 2 && nMED < 10) { nMED++; soma++; }
        else if (r == 3 && nAVA < 7) { nAVA++; soma++; }
    }

    // Array com as quantidades para iterar na ordem correta
    int quantidades[] = { nGR, nDEF, nMED, nAVA };
    Posicao posicoes[] = { GR, DEF, MED, AVA };

    for (int i = 0; i < 4; i++) { // Para cada posição (GR, DEF, MED, AVA)
        for (int j = 0; j < quantidades[i]; j++) {
            Jogador& novo = p.jogadores[p.totalAtual];

            novo.nome = listaNomes[gerarAleatorio(0, totalNomes - 1)];
            novo.pos = posicoes[i];
            novo.idade = gerarAleatorio(17, 38);
            novo.probLesao = gerarAleatorio(0, 15);
            novo.probCastigo = gerarAleatorio(0, 20);
            novo.qualidade = gerarAleatorio(0, 100);

            // Gerar número único para a camisola
            int num;
            do {
                num = gerarAleatorio(1, 99);
            } while (numeroJaExiste(p, num));
            novo.numero = num;

            p.totalAtual++;
        }
    }
}

void inicializarTatica(Tatica& t, const Plantel& p) {
    t.capacidade = 17; // 11 Titulares + 6 Suplentes
    t.jogadores = new Jogador[t.capacidade];
    t.totalAtual = 0;

    // 1. ESCOLHER OS TITULARES (1 GR, 4 DEF, 4 MED, 2 AVA)
    int titulosNecessarios[] = {1, 4, 4, 2};
    Posicao posicoes[] = {GR, DEF, MED, AVA};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < titulosNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) { // Se encontrou alguém válido
                t.jogadores[t.totalAtual++] = escolhido;
            }
        }
    }

    // 2. ESCOLHER OS SUPLENTES (1 GR, 2 DEF, 2 MED, 1 AVA)
    int suplentesNecessarios[] = {1, 2, 2, 1};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < suplentesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            }
        }
    }
}

void exibirPlantel(const Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "========================================================================\n";
    cout << "PLANTEL GERADO - Capacidade: " << p.capacidade << "\n";
    cout << "========================================================================\n";
    cout << "NUM | POS | IDADE | QUAL | LESAO | CAST | NOME\n";
    cout << "------------------------------------------------------------------------\n";

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador& j = p.jogadores[i];
        printf("%-3d | %-3s | %-5d | %-4d | %-4d%% | %-3d%% | %s\n",
               j.numero, posTxt[j.pos], j.idade, j.qualidade,
               j.probLesao, j.probCastigo, j.nome.c_str());
    }
    cout << "========================================================================\n";
}

void exibirTatica(const Tatica& t) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "========================================================================\n";
    cout << "Tatica Default"<< "\n";
    cout << "========================================================================\n";
    cout << "NUM | POS | QUAL | NOME\n";
    cout << "------------------------------------------------------------------------\n";

    for (int i = 0; i < t.totalAtual; i++) {
        Jogador& j = t.jogadores[i];
        // O printf agora tem apenas 4 formatadores (%d, %s, %d, %s)
        // que correspondem exatamente às 4 variáveis passadas.
        printf("%-3d | %-3s | %-4d | %s\n",
               j.numero, posTxt[j.pos], j.qualidade, j.nome.c_str());
    }
    cout << "========================================================================\n";
}

void libertarMemoria(Plantel& p, string* listaNomes) {
    delete[] p.jogadores;
}



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
            novo.numero = e.totalLT +1;   //pra nao começar em 0

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

}#1#
*/


