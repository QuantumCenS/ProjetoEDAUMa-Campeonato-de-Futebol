#include "Inicializacao_Plantel_EDA_FC.h"
#include "Taticas.h"
#include <iostream>

using namespace std;

// Verifica se o jogador já foi adicionado à tática atual
bool jogadorJaConvocado(const Tatica& t, const string& nomeJogador) {
    for (int i = 0; i < t.totalAtual; i++) {
        if (t.jogadores[i].nome == nomeJogador) {
            return true;
        }
    }
    return false;
}

// Procura o jogador com melhor qualidade disponível para uma dada posição
Jogador encontrarMelhorDisponivel(const Plantel& p, const Tatica& t, Posicao pos) {
    Jogador melhor;
    melhor.qualidade = -1; // Valor inicial baixo para garantir que encontramos alguém

    for (int i = 0; i < p.totalAtual; i++) {
        Jogador candidato = p.jogadores[i];

        // Verifica se é da posição certa e se é melhor que o anterior encontrado
        if (candidato.pos == pos && candidato.qualidade > melhor.qualidade) {
            // Verifica se NÃO está já convocado
            if (!jogadorJaConvocado(t, candidato.nome)) {
                melhor = candidato;
            }
        }
    }
    return melhor;
}

// Inicializa a tática default 1-4-4-2
void inicializarTatica(Tatica& t, const Plantel& p) {
    t.capacidade = 17; // 11 Titulares + 6 Suplentes
    t.jogadores = new Jogador[t.capacidade];
    t.totalAtual = 0;

    // 1. ESCOLHER OS TITULARES (1 GR, 4 DEF, 4 MED, 2 AVA)
    int titularesNecessarios[] = {1, 4, 4, 2};
    Posicao posicoes[] = {GR, DEF, MED, AVA};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < titularesNecessarios[i]; j++) {
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

// Apresenta a Tática na consola
void exibirTatica(const Tatica& t) {
    const char* posTxt[] = {"GR", "DEF", "MED", "AVA"};

    cout << "========================================================================\n";
    cout << "TATICA ATUAL\n";
    cout << "========================================================================\n";
    cout << "NUM | POS | QUAL | NOME\n";
    cout << "------------------------------------------------------------------------\n";

    for (int i = 0; i < t.totalAtual; i++) {
        Jogador& j = t.jogadores[i];
        
        if (i == 0) cout << "--- TITULARES ---\n";
        if (i == 11) cout << "--- SUPLENTES ---\n";

        printf("%-3d | %-3s | %-4d | %s\n",
               j.numero, posTxt[j.pos], j.qualidade, j.nome.c_str());
    }
    cout << "========================================================================\n";
}

// 4.3 Alteracao Tatica: Pede ao utilizador os novos valores e refaz a convocatoria
bool alterarTatica(Tatica& t, const Plantel& p) {
    int nDef, nMed, nAva;
    cout << "\n--- ALTERAR TATICA ---\n";
    cout << "A tatica deve ter 11 jogadores titulares (1 GR ja esta garantido).\n";
    cout << "Minimos exigidos: 3 DEF, 2 MED, 1 AVA.\n";
    
    cout << "Quantos Defesas? ";
    cin >> nDef;
    cout << "Quantos Medios? ";
    cin >> nMed;
    cout << "Quantos Avancados? ";
    cin >> nAva;

    // Validar minimos do enunciado
    if (nDef < 3 || nMed < 2 || nAva < 1) {
        cout << "Erro: A tatica nao respeita os minimos de jogadores por posicao!\n";
        return false;
    }

    // Validar total (1 GR + Def + Med + Ava tem de ser exatamente 11)
    if (1 + nDef + nMed + nAva != 11) {
        cout << "Erro: A soma dos jogadores de campo e do GR (1) tem de ser exatamente 11!\n";
        cout << "Tu escolheste 1 + " << nDef << " + " << nMed << " + " << nAva << " = " << (1 + nDef + nMed + nAva) << "\n";
        return false;
    }

    // Se chegou aqui, a tática é válida. Limpar a tática atual e refazer.
    t.totalAtual = 0; 

    // RE-ESCOLHER OS TITULARES COM A NOVA TÁTICA
    int titularesNecessarios[] = {1, nDef, nMed, nAva};
    Posicao posicoes[] = {GR, DEF, MED, AVA};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < titularesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            } else {
                cout << "Aviso: Nao ha jogadores suficientes no plantel para a posicao " << posTxt[posicoes[i]] << "!\n";
            }
        }
    }

    // RE-ESCOLHER OS SUPLENTES (Mantêm-se os limites originais do enunciado: 1-2-2-1)
    int suplentesNecessarios[] = {1, 2, 2, 1};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < suplentesNecessarios[i]; j++) {
            Jogador escolhido = encontrarMelhorDisponivel(p, t, posicoes[i]);
            if (escolhido.qualidade != -1) {
                t.jogadores[t.totalAtual++] = escolhido;
            }
        }
    }

    cout << "Sucesso! Tatica alterada para o sistema 1-" << nDef << "-" << nMed << "-" << nAva << "!\n";
    return true;
}
