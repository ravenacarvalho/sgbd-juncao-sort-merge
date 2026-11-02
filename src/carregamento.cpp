// carregamento.cpp
// Responsável por ler os CSVs e montar as estruturas
// de Tabela/Pagina/Tupla na memória.

#include "tabela.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

//Insere uma tupla na tabela, abrindo nova página se cheia (12 tuplas/página)
void inserirTuplaCSV(
    Tabela &tabela,
    Tupla &tupla
) {

    if (
        tabela.paginas.empty() ||
        tabela.paginas.back().estaCheia()
    ) {
        tabela.paginas.push_back(Pagina());
    }

    tabela.paginas.back().tuplas.push_back(tupla);
}

// Lê um arquivo CSV e carrega os dados na memória como uma tabela com páginas de 12 tuplas
// Cada linha do CSV vira uma tupla, e cada campo separado por vírgula vira uma coluna da tupla

Tabela carregarCSV(
    string nomeArquivo,
    string nomeTabela
) {

    Tabela tabela;
    tabela.nome = nomeTabela;

    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo: " << nomeArquivo << "\n";
        return tabela;
    }

    string linha;

    while (getline(arquivo, linha)) {

        Tupla tupla;
        stringstream ss(linha);
        string valor;

        while (getline(ss, valor, ',')) {

            // remove \r de fim de campo (CSVs gerados no Windows)
            if (!valor.empty() && valor.back() == '\r') {
                valor.pop_back();
            }

            tupla.colunas.push_back(valor);
        }

        inserirTuplaCSV(tabela, tupla);
    }

    arquivo.close();

    return tabela;
}

// Imprime todas as tuplas da tabela no formato CSV, separando colunas por vírgula e cada tupla numa linha
void imprimirTabela(Tabela &tabela) {

    for (auto &pagina : tabela.paginas) {

        for (auto &tupla : pagina.tuplas) {

            for (int i = 0; i < (int) tupla.colunas.size(); i++) {

                cout << tupla.colunas[i];

                if (i < (int) tupla.colunas.size() - 1) {
                    cout << ",";
                }
            }

            cout << endl;
        }
    }
}
