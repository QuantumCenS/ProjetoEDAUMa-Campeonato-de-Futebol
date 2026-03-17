#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "definicoes.h"

using namespace std;

int gerarAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Função auxiliar para verificar se um número de camisola já existe no plantel
bool numeroJaExiste(Plantel& p, int num) {
    for (int i = 0; i < p.totalAtual; i++) {
        if (p.jogadores[i].numero == num) return true;
    }
    return false;
}

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

void libertarMemoria(Plantel& p, string* listaNomes) {
    delete[] p.jogadores;
    delete[] listaNomes;
}



void listaTranf(Equipa& e) {
    e.capacidadeLT = 100;
    e.totalLT = 0;
    e.ListaTransf = new Jogador[e.capacidadeLT];                 //array duinamico do tipo jogador 


}


void adicionarJogLT(Equipa& e, string* listaNomes, int totalNomes) {

    for (int i = 0; i < 2; i++) {   //  2 jogadores por jornada
        if (e.totalLT < e.capacidadeLT) {

            Jogador& novo = e.ListaTransf[e.totalLT];                              //  Criar um atalho para a primeira posição vazia no fim da lista
            preencherAtributosIndependentes(novo,listaNomes,totalNomes);
            Posicao posicoes[] = { GR, DEF, MED, AVA };
            novo.pos = posicoes[gerarAleatorio(0, 3)];
            novo.numero = gerarAleatorio(1, 99);

            e.totalLT++;
        }
    }
    OrdenarJogadores(e.ListaTransf, e.totalLT);

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



void OrdenarJogadores(Jogador* lista, int total) {

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
