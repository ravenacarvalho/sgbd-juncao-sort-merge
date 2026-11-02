#include "ordenacao_externa.hpp"
#include "pagina.hpp"

#include <algorithm>
#include <vector>
#include <string>

using namespace std;

// Buffer com 5 páginas
#define TAM_BUFFER 5

// Percorre todas as páginas da tabela e coloca as tuplas em um único vetor
vector<Tupla> transformarEmVetor(Tabela &tabela) {

    vector<Tupla> tuplas;

    for (auto &pagina : tabela.paginas) {
        for (auto &tupla : pagina.tuplas) {
            tuplas.push_back(tupla);
        }
    }

    return tuplas;
}

// Insere uma tupla na tabela. Se a página estiver cheia, cria uma nova página
void inserirTuplaOrdenacao(
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

//Ordena as tuplas pela coluna escolhida
void ordenarTuplas(
    vector<Tupla> &tuplas,
    int indiceColuna
) {

    sort(
        tuplas.begin(),
        tuplas.end(),

        [indiceColuna](
            const Tupla &a,
            const Tupla &b
        ) {
            return
                a.colunas[indiceColuna]
                <
                b.colunas[indiceColuna];
        }
    );
}

/*
   Divide a tabela em blocos de até 5 páginas,
   ordena cada bloco e cria as runs
*/
vector<Tabela> criarRuns(
    Tabela &tabela,
    int indiceColuna
) {

    vector<Tabela> runs;

    for (
        int i = 0;
        i < (int) tabela.paginas.size();
        i += TAM_BUFFER
    ) {

        vector<Tupla> bloco;

        // carrega até 5 páginas por vez
        for (
            int j = i;
            j < i + TAM_BUFFER &&
            j < (int) tabela.paginas.size();
            j++
        ) {
            for (auto &tupla : tabela.paginas[j].tuplas) {
                bloco.push_back(tupla);
            }
        }

        ordenarTuplas(bloco, indiceColuna);

        Tabela run;

        for (auto &tupla : bloco) {
            inserirTuplaOrdenacao(run, tupla);
        }

        runs.push_back(run);
    }

    return runs;
}

// Intercala um grupo de runs já ordenadas
Tabela intercalarGrupoDeRuns(
    vector<Tabela> &grupo,
    int indiceColuna
) {

    Tabela resultado;

    int numRuns = grupo.size();

    vector<vector<Tupla>> vetores(numRuns);

    // guarda a posição atual de cada run
    vector<int> indices(numRuns, 0);

    // transforma as runs em vetores
    for (int i = 0; i < numRuns; i++) {
        vetores[i] = transformarEmVetor(grupo[i]);
    }

    vector<Tupla> bufferSaida;

    while (true) {

        int menorIdx = -1;

        // procura a menor tupla entre as runs
        for (int i = 0; i < numRuns; i++) {

            // ignora runs já percorridas
            if (indices[i] >= (int) vetores[i].size()) {
                continue;
            }

            if (
                menorIdx == -1 ||
                vetores[i][indices[i]].colunas[indiceColuna]
                <
                vetores[menorIdx][indices[menorIdx]].colunas[indiceColuna]
            ) {
                menorIdx = i;
            }
        }

        // terminou todas as runs
        if (menorIdx == -1) {
            break;
        }

        bufferSaida.push_back(
            vetores[menorIdx][indices[menorIdx]]
        );

        indices[menorIdx]++;

        // grava a página quando o buffer enche
        if ((int) bufferSaida.size() >= MAX_TUPLAS) {

            for (auto &tupla : bufferSaida) {
                inserirTuplaOrdenacao(resultado, tupla);
            }

            bufferSaida.clear();
        }
    }

    // grava o restante do buffer
    for (auto &tupla : bufferSaida) {
        inserirTuplaOrdenacao(resultado, tupla);
    }

    return resultado;
}

/*
   Executa a ordenação externa completa:
   1. Cria as runs ordenadas
   2. Intercala as runs até sobrar uma só
*/
void ordenacaoExterna(
    Tabela &tabela,
    int indiceColuna
) {

    vector<Tabela> runs =
        criarRuns(
            tabela,
            indiceColuna
        );

    // continua enquanto existir mais de uma run
    while (runs.size() > 1) {

        vector<Tabela> novasRuns;

        for (
            int i = 0;
            i < (int) runs.size();
            i += 4
        ) {

            vector<Tabela> grupo;

            // pega até 4 runs por vez
            for (
                int j = i;
                j < i + 4 &&
                j < (int) runs.size();
                j++
            ) {
                grupo.push_back(runs[j]);
            }

            Tabela novaRun =
                intercalarGrupoDeRuns(
                    grupo,
                    indiceColuna
                );

            novasRuns.push_back(novaRun);
        }

        runs = novasRuns;
    }

    // Substitui pela versão ordenada
    tabela = runs[0];
}