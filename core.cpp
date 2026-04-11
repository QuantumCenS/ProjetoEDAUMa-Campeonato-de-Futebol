//
// Created by Sergio on 4/3/2026.
//

#include "core.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>


using namespace std;


int gerarAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
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

void inicializarPlantel(Plantel& p, string* listaNomes, int totalNomes) {
    p.capacidade = 30;
    p.jogadores = new Jogador[p.capacidade];
    p.totalAtual = 0;

    int limiteInicial = gerarAleatorio(20, 30); //Sorteia com quantos jogadores começas

    int nGR = 2, nDEF = 7, nMED = 7, nAVA = 4;
    int soma = nGR + nDEF + nMED + nAVA;

    //Preenche apenas até ao limite sorteado (ex: 26)
    while (soma < limiteInicial) {
        int r = gerarAleatorio(0, 3);
        if (r == 0 && nGR < 3) { nGR++; soma++; }
        else if (r == 1 && nDEF < 10) { nDEF++; soma++; }
        else if (r == 2 && nMED < 10) { nMED++; soma++; }
        else if (r == 3 && nAVA < 7) { nAVA++; soma++; }
    }

    int quantidades[] = { nGR, nDEF, nMED, nAVA };
    Posicao posicoes[] = { GR, DEF, MED, AVA };
    int contadorCamisola = 1;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < quantidades[i]; j++) {
            Jogador& novo = p.jogadores[p.totalAtual];

            novo.nome = listaNomes[gerarAleatorio(0, totalNomes - 1)];
            novo.pos = posicoes[i];
            novo.idade = gerarAleatorio(17, 38);
            novo.probLesao = gerarAleatorio(0, 15);
            novo.probCastigo = gerarAleatorio(0, 20);
            novo.qualidade = gerarAleatorio(0, 100);
            novo.numero = contadorCamisola++;

            // Inicializamos as variáveis extra por segurança
            novo.jogosLesao = 0;
            novo.jogosCastigo = 0;
            novo.semanasTreino = 0;
            novo.jogouHoje = false;

            p.totalAtual++;
        }
    }
}

void exibirPlantel(const Plantel& p) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "=======================================================================================================\n";
    cout << "***************************** Plantel Disponivel: *****************************************************\n";
    cout << "=======================================================================================================\n";


    cout << left
     << setw(20) << "Nome" << " | "
     << setw(4)  << "Nº" << " | "
     << setw(7)  << "Posicao" << " | "
     << setw(5)  << "Idade" << " | "
     << setw(9)  << "ProbLesao" << " | "
     << setw(11) << "ProbCastigo" << " | "
     << setw(9)  << "Qualidade" << " | "
     << "Dias-Treino\n";
    cout << "-------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador& j = p.jogadores[i];

        // Formatar as percentagens
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
     << j.semanasTreino << "\n";
    }
    cout << "=======================================================================================================\n";
}

void libertarMemoria(Plantel& p, string* listaNomes) {
    delete[] p.jogadores;
}
