#include "merge_join.hpp"
#include <vector>
#include <string>

using namespace std;

//Percorre todas as páginas da tabela e coloca todas as tuplas em um único vetor
vector<Tupla> transformarTabelaEmVetor(Tabela &tabela) {

    vector<Tupla> tuplas;

    for (auto &pagina : tabela.paginas) {
        for (auto &tupla : pagina.tuplas) {
            tuplas.push_back(tupla);
        }
    }

    return tuplas;
}

// Junta as colunas de duas tuplas em uma só
// cols de uva + cols de vinho = tupla resultado
Tupla juntarTuplas(
    const Tupla &tupla1,
    const Tupla &tupla2
) {

    Tupla resultado;

    // começa com as colunas da tupla da esquerda
    resultado.colunas = tupla1.colunas;

    // depois adiciona as da direita
    for (const auto &coluna : tupla2.colunas) {
        resultado.colunas.push_back(coluna);
    }

    return resultado;
}

// Insere a tupla resultado na última página da tabela
// Se a página estiver cheia (limite de 12 tuplas), abre uma nova
void inserirTupla(
    Tabela &tabela,
    const Tupla &tupla
) {

    if (
        tabela.paginas.empty() || // Verifica se a tabela está sem páginas
        tabela.paginas.back().estaCheia() // Ou se a última página da tabela está cheia
    ) {
        tabela.paginas.push_back(Pagina());
    }

    tabela.paginas.back().tuplas.push_back(tupla);
}

/*
  Realiza o Merge Join entre duas tabelas já ordenadas
  O algoritmo percorre as duas tabelas ao mesmo tempo usando dois ponteiros: i e j
   - se as chaves forem iguais, faz o join
   - se uma chave for menor, avança o ponteiro dela
  O resultado é salvo em uma nova tabela
*/
Tabela mergeJoin(
    Tabela &tabelaEsquerda,
    Tabela &tabelaDireita,
    int indiceJoinEsquerda,
    int indiceJoinDireita
) {

    Tabela resultado;
    resultado.nome = "resultado";

    vector<Tupla> esquerda =
        transformarTabelaEmVetor(tabelaEsquerda); // Gera o vetor de tuplas da tabela esquerda

    vector<Tupla> direita =
        transformarTabelaEmVetor(tabelaDireita); // Gera o vetor de tuplas da tabela direita

    int i = 0;
    int j = 0;

    // Percorre as duas tabelas enquanto ainda houver tuplas
    while ( 
        i < (int) esquerda.size() &&
        j < (int) direita.size()
    ) {

        string chaveEsquerda =
            esquerda[i].colunas[indiceJoinEsquerda]; // Pega a chave atual da esquerda

        string chaveDireita =
            direita[j].colunas[indiceJoinDireita]; // Pega a chave atual da direita

        if (chaveEsquerda == chaveDireita) {

            /* Quando as chaves são iguais,
                gera todas as combinações possíveis
                entre as tuplas das duas tabelas
            */

            int iTemp = i;

            // Continua enquanto a chave for igual
            while (
                iTemp < (int) esquerda.size() &&
                esquerda[iTemp].colunas[indiceJoinEsquerda]
                    == chaveEsquerda
            ) {

                int jTemp = j;

                while (
                    jTemp < (int) direita.size() &&
                    direita[jTemp].colunas[indiceJoinDireita]
                        == chaveDireita
                ) {

                    Tupla novaTupla =
                        juntarTuplas(
                            esquerda[iTemp],
                            direita[jTemp]
                        );

                    inserirTupla(resultado, novaTupla);

                    jTemp++;
                }

                iTemp++;
            }

            // Avança para a próxima chave diferente
            while (
                i < (int) esquerda.size() &&
                esquerda[i].colunas[indiceJoinEsquerda]
                    == chaveEsquerda
            ) {
                i++;
            }

            while (
                j < (int) direita.size() &&
                direita[j].colunas[indiceJoinDireita]
                    == chaveDireita
            ) {
                j++;
            }
        }

        // Chave esquerda é menor: essa uva não tem vinho correspondente
        // Avança para a próxima
        else if (chaveEsquerda < chaveDireita) {
            i++;
        }

        // Chave direita é menor: esse vinho não tem uva correspondente
        // Avança para o próximo
        else {
            j++;
        }
    }

    return resultado;
}
