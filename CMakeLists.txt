cmake_minimum_required(VERSION 4.1)
project(ProjetoEDAUMa_Campeonato_de_Futebol)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ProjetoEDAUMa_Campeonato_de_Futebol
        definicoes.h
        funcoes.cpp
        main.cpp
        Estrutura_campeonato.cpp
        Inicializacao_Plantel_EDA_FC.cpp
        Inicializacao_Plantel_EDA_FC.h
        Estrutura_campeonato.h
        Taticas.cpp
        Taticas.h
        Lista_Transferências.cpp
        Lista_Transferências.h
        Castigados_Lesionados.cpp
        Castigados_Lesionados.h
        menu.h
        menu.cpp
        jogo.cpp
        core.cpp)
