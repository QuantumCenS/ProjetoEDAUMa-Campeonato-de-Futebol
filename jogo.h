//
// Created by Sergio on 4/3/2026.
//

#ifndef PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H
#define PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H

#include <string>
#include <iostream>
#include "core.h"

using namespace std;

/**
 * Le o ficheiro de texto das equipas e carrega os nomes das equipas para um array dinamico de strings.
 * A funcao realiza uma leitura inicial para contar as equipas e aloca a memoria exata necessaria.
 * @param f - String contendo o nome ou o caminho do ficheiro de texto a ser lido.
 * @return Ponteiro para o array dinamico de strings com os nomes das equipas, ou nullptr caso o ficheiro nao possa ser aberto.
 */
string* carregarEquipas(string f);
/**
 * Baralha aleatoriamente a ordem das equipas num array de strings.
 * Utiliza um algoritmo de troca (swap) para garantir uma distribuicao aleatoria.
 * @param equipas - Ponteiro para o array de strings contendo os nomes das equipas.
 * @param nEquipas - Numero total de equipas a serem baralhadas.
 */
void baralhar(string* equipas, int nEquipas);
/**
 * Gera o calendario de jogos para 34 jornadas, criando confrontos de ida e volta.
 * Aloca dinamicamente uma matriz de strings para armazenar os jogos.
 * @param e - Instancia da equipa do utilizador que participara nos confrontos.
 * @param equipas - Array com os nomes das restantes equipas adversarias.
 * @return Ponteiro para um array de ponteiros (matriz) contendo o calendario das jornadas.
 */
string** gerarJornadas(Equipa e, string* equipas);
/**
 * Procura uma equipa especifica dentro do array da liga atraves do seu nome.
 * @param nome - String com o nome da equipa a procurar.
 * @param liga - Ponteiro para o array que contem todas as instancias de Equipa.
 * @param nEquipas - Numero total de equipas na liga.
 * @return Referencia para a instancia da Equipa encontrada ou para a primeira equipa (seguranca).
 */
Equipa& encontrarEquipa(string nome, Equipa* liga, int nEquipas);
/**
 * Simula o resultado de um jogo entre duas equipas, gerando golos aleatoriamente.
 * Atualiza automaticamente a pontuacao de cada equipa (3 pontos vitoria, 1 empate).
 * @param h - Referencia para a instancia da equipa que joga em casa.
 * @param a - Referencia para a instancia da equipa que joga fora.
 * @return String formatada com o resultado final (ex: "Equipa A 2 - 1 Equipa B").
 */
string gerarResultado(Equipa& h, Equipa& a);

/**
 * Atualiza o estado de saude e disciplina dos jogadores, reduzindo o tempo de lesao
 * e castigo. Aplica tambem os beneficios do treino especifico na qualidade dos atletas.
 * @param p - Referencia para a instancia do plantel a ser atualizada pos-jornada.
 */
void atualizarRecuperacoes(Plantel& p);
/**
 * Simula a probabilidade de lesoes e castigos durante um jogo e gere as
 * substituicoes necessarias de forma automatica.
 * @param t - Referencia para a instancia da tatica (jogadores em campo).
 * @param p - Referencia para a instancia do plantel global (para aplicar as punicoes).
 */
void simularIncidentesPartida(Plantel& t, Plantel& p);
/**
 * Apresenta uma listagem detalhada de todos os jogadores que se encontram
 * impedidos de jogar por motivos medicos ou disciplinares.
 * @param p - Referencia constante para a instancia do plantel a visualizar.
 */
void exibirCastigadosLesionados(const Plantel& p);
/**
 * Atribui diretamente um tempo de paragem (lesao ou castigo) a um jogador especifico no plantel.
 * @param p - Referencia para a instancia do plantel onde o jogador se encontra.
 * @param numero - Numero identificador do jogador a ser afetado.
 * @param lesao - Booleano que define se o incidente e uma lesao (true) ou castigo (false).
 * @param tempo - Numero de jornadas de paragem a aplicar.
 */
void aplicarIncidenteAoPlantel(Plantel& p, int numero, bool lesao, int tempo);
/**
 * Pesquisa no plantel o jogador com maior qualidade para uma posicao especifica,
 * garantindo que este se encontra apto e nao foi ja convocado.
 * @param p - Referencia para a instancia do plantel principal.
 * @param t - Referencia para a instancia da tatica em formacao.
 * @param pos - Enumeracao da posicao pretendida (GR, DEF, MED, AVA).
 * @return O exemplar do Jogador que melhor preenche os requisitos, ou um jogador invalido se nao houver opcao.
 */
Jogador encontrarMelhorDisponivel(Plantel& p, Plantel& t, Posicao pos);


/**
 * Preenche a estrutura da tatica com os melhores jogadores disponiveis, respeitando
 * a formacao escolhida e garantindo o preenchimento do banco de suplentes.
 * @param t - Referencia para a instancia da tatica a ser preenchida.
 * @param p - Referencia para a instancia do plantel de onde serao recrutados os jogadores.
 * @return 'true' se for possivel formar o 11 inicial completo, 'false' se nao houver jogadores aptos suficientes.
 */
bool inicializarTatica(Plantel& t, Plantel& p);
/**
 * Imprime de forma organizada a constituicao da equipa para o jogo,
 * separando os titulares dos suplentes.
 * @param t - Referencia constante para a instancia da tatica a exibir.
 */
void exibirTatica(const Plantel& t);


// =======================================================
// GESTÃO DA LISTA DE TRANSFERÊNCIAS
// =======================================================


/**
 * Inicializa a lista de transferencias de uma equipa, definindo a capacidade
 * maxima e alocando a memoria necessaria para o array de jogadores.
 * @param e - Referencia para a equipa onde a lista sera criada
 */
void listaTranf(Equipa& e);
/**
 * Adiciona novos jogadores gerados aleatoriamente a lista de transferencias de uma equipa.
 * @param e - Equipa que recebe os jogadores na sua lista de mercado
 * @param p - Plantel atual para garantir que nao ha nomes duplicados
 * @param listaNomes - Array com o ficheiro de nomes disponiveis no ficheiro
 * @param totalNomes - Numero total de nomes existentes no ficheiro de nomes
 */
void adicionarJogLT(Equipa& e, const Plantel& p, string* listaNomes, int totalNomes);/**
 * Imprime no ecra, de forma formatada e organizada, todos os jogadores
 * atualmente disponiveis na lista de transferencias da equipa.
 * @param e - Equipa cuja lista de transferencias sera visualizada
 */
void exibirListaTransf(const Equipa& e);
/**
 * Gera aleatoriamente os atributos base de um novo jogador (nome, idade, qualidade e probabilidades),
 * garantindo que o nome sorteado nao existe ja no plantel.
 * @param novo - Referencia para o objeto Jogador que sera preenchido
 * @param p - Plantel atual para validacao de nomes unicos
 * @param listaNomes - Ficheiro de nomes para efetuar sorteio aleatorio
 * @param totalNomes - Total de nomes disponiveis para sorteio
 */
void preencherAtributosIndependentes(Jogador& novo, const Plantel& p, string* listaNomes, int totalNomes);
/**
 * Ordena um array de jogadores de acordo com a sua posicao (GR, DEF, MED, AVA)
 * @param lista - Ponteiro para o array de jogadores a ordenar
 * @param total - Numero de jogadores presentes na lista
 */
void OrdenarPorPos(Jogador* lista, int total);

// =======================================================
// LÓGICA DE CONTRATAÇÕES E TROCAS
// =======================================================

/**
 * Gere o processo de contratacao manual para o utilizador. Permite escolher um jogador
 * da lista de transferencias e efetuar a compra direta ou troca, caso o plantel esteja cheio.
 * @param p - Referencia para o plantel que ira receber o novo jogador.
 * @param e - Referencia para equipa que disponibiliza a lista de transferencias associada.
 */
void ContratarJogador(Plantel& p, Equipa& e);
/**
 * Executa a logica de decisao automatica para as equipas controladas pelo computador.
 * O BOT decide aleatoriamente se contrata e gere automaticamente as trocas por posicao.
 * @param p - Referencia para o plantel onde sera feita a gestao de atletas.
 * @param e - Referencia para a instancia da equipa (uma das adversarias do EDA FC) que detem a sua propria lista de mercado.
 */
void ContratarJogadorBOT(Plantel& p, Equipa& e);
/**
 * Funcao coordenadora que atualiza o mercado de todas as equipas da liga a cada jornada.
 * Adiciona novos jogadores as listas e processa as contratacoes automaticas dos BOTs.
 * @param liga - Ponteiro para o array que contem todas as equipas do campeonato
 * @param nEquipas - Numero total de equipas na liga
 * @param bancoNomes - Array com os nomes disponiveis para gerar novos jogadores
 * @param nNomes - Numero total de nomes existentes no ficheiro de nomes
 */
void processarMercadoGlobal(Equipa* liga, int nEquipas, string* bancoNomes, int nNomes);

// =======================================================
// VALIDAÇÕES DE PLANTEL
// =======================================================


/**
 * Percorre o plantel e contabiliza quantos jogadores existem para uma determinada posicao.
 * @param p - Referencia para o plantel a ser analisado.
 * @param j - Inteiro que representa o codigo da posicao (ex: 1 para GR, 2 para DEF, etc.).
 * @return O numero total de jogadores encontrados para a posicao especificada.
 */
int ContarJogNumaPos(Plantel& p, int j);
/**
 * Verifica se o plantel ainda tem vagas disponiveis para uma posicao especifica,
 * de acordo com os limites definidos pela tatica ou capacidade maxima.
 * @param p - Referencia para o plantel onde se pretende adicionar o jogador.
 * @param j - Codigo numerico da posicao que se deseja validar.
 * @return 'true' se for possivel contratar para essa posicao, 'false' caso o limite tenha sido atingido.
 */
bool PodeContratarParaPosicao(Plantel& p, int j);

/**
 * Guarda o progresso atual do jogo num ficheiro de texto, permitindo a persistencia
 * de dados como a jornada atual, pontuacao, plantel e lista de mercado.
 * @param filename - String com o nome ou caminho do ficheiro onde os dados serao gravados.
 * @param e - Referencia constante para a instancia da equipa (dados do clube e transferencias).
 * @param p - Referencia constante para a instancia do plantel (dados dos jogadores).
 * @param jornadaAtual - Inteiro que indica o numero da jornada em que o jogo se encontra.
 * @return 'true' se a gravacao foi bem-sucedida, 'false' caso ocorra um erro ao abrir o ficheiro.
 */
bool gravarEstado(const string& filename, const Equipa& e, const Plantel& p, int jornadaAtual);
/**
 * Recupera o estado de um jogo guardado anteriormente, restaurando todas as variaveis
 * de controlo, o estado dos jogadores e a situacao da liga.
 * @param filename - String com o caminho do ficheiro de onde os dados serao lidos.
 * @param e - Referencia para a instancia da equipa que sera preenchida com os dados lidos.
 * @param p - Referencia para a instancia do plantel que sera reconstruida.
 * @param jornadaAtual - Referencia para a variavel da jornada, que sera atualizada com o valor guardado.
 * @return 'true' se o carregamento foi concluido com sucesso, 'false' se o ficheiro nao puder ser aberto.
 */
bool carregarEstado(const string& filename, Equipa& e, Plantel& p, int& jornadaAtual);

#endif //PROJETOEDAUMA_CAMPEONATO_DE_FUTEBOL_JOGO_H
