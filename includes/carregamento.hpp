// carregamento.hpp

#ifndef CARREGAMENTO_HPP
#define CARREGAMENTO_HPP

#include "tabela.hpp"

Tabela carregarCSV(
    string nomeArquivo,
    string nomeTabela
);

void imprimirTabela(Tabela &tabela);

#endif