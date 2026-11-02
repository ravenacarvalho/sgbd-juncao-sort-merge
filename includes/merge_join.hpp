#ifndef MERGE_JOIN_HPP
#define MERGE_JOIN_HPP

#include "tabela.hpp"

Tabela mergeJoin(
    Tabela &tabelaEsquerda,
    Tabela &tabelaDireita,
    int indiceJoinEsquerda,
    int indiceJoinDireita
);

#endif