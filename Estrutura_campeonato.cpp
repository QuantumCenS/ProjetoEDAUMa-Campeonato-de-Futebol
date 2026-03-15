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
void gerarJornadas( string f, string* equipas) {
    fstream file(f);
    int nEquipas=contaEquipas(f);
    auto **jornadasCasa= new string*[nEquipas];
    auto **jornadasFora= new string*[nEquipas];
    baralhar(equipas,nEquipas);
    for (int i=0;i<nEquipas;i++) {
        jornadasCasa[i]=new string[2];
        jornadasCasa[i][0]="EDA FC";
        jornadasCasa[i][1]=equipas[i];
        jornadasFora[i]=new string[2];
        jornadasFora[i][0]=equipas[i];
        jornadasFora[i][1]="EDA FC";
        cout<<jornadasCasa[i][0]<<" - "<<jornadasCasa[i][1]<<endl;
        cout<<jornadasFora[i][0]<<" - "<<jornadasFora[i][1]<<endl;
        delete[] jornadasCasa[i];
        delete[] jornadasFora[i];
    }
    delete [] jornadasCasa;
    delete [] jornadasFora;
}
