//
// Created by Henrique Nóbrega on 12/03/2026.
//
#include <iostream>
#include <fstream>
#include <definicoes.h>
using namespace std;



//Contar o numero de equipas do ficheiro
int contaEquipas(string f) {
    fstream file(f);
    string line;
    if (!file.is_open()) {
        return 0;
    }
    int nEquipas=0;
    int i=0;
    while (getline(file,line)) {
        if (!line.empty()) {
            nEquipas++;
            i++;
        }
    }
    file.clear();
    file.seekg(0);
    return nEquipas;
}
//Por o nome das equipas num array
string* carregarEquipas(string f) {
    fstream file(f);
    string line;

    if (!file.is_open()) {
        return nullptr;
    }

    int nEquipas=contaEquipas(f);

    auto* equipas= new string[nEquipas];

    int i = 0;
    while (getline(file,line)) {
        if (!line.empty()) {
            equipas[i] = line;
            i++;
        }
    }
    file.clear();
    file.seekg(0);
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