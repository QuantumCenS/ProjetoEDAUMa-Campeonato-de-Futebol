cmake_minimum_required(VERSION 4.1)
project(ProjetoEDAUMa_Campeonato_de_Futebol)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ProjetoEDAUMa_Campeonato_de_Futebol
        definicoes.h
        funcoes.cpp
        main.cpp)
