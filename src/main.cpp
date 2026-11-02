#include <iostream>
#include <sstream>

#include "tabela.hpp"
#include "merge_join.hpp"
#include "ordenacao_externa.hpp"

Tabela carregarCSV(
    string nomeArquivo,
    string nomeTabela
);

void imprimirTabela(Tabela &tabela);

// Monta uma tupla a partir de uma string 
Tupla criarTupla(string linha) {

    Tupla tupla;
    stringstream ss(linha);
    string valor;

    while (getline(ss, valor, ',')) {
        tupla.colunas.push_back(valor);
    }

    return tupla;
}

// Insere tupla na tabela abrindo nova página quando necessário
void inserirTupla(Tabela &tabela, Tupla tupla) {

    if (
        tabela.paginas.empty() ||
        tabela.paginas.back().estaCheia()
    ) {
        tabela.paginas.push_back(Pagina());
    }

    tabela.paginas.back().tuplas.push_back(tupla);
}

// Compara o resultado obtido com o esperado e imprime passou ou falhou
// Se falhar, mostra o que era esperado e o que veio de fato
void verificarResultado(
    string nomeCaso,
    Tabela &resultado,
    vector<string> esperado
) {

    vector<string> obtido;

    for (auto &pagina : resultado.paginas) {
        for (auto &tupla : pagina.tuplas) {

            string linha = "";

            for (int i = 0; i < (int) tupla.colunas.size(); i++) {
                if (i > 0) linha += ",";
                linha += tupla.colunas[i];
            }

            obtido.push_back(linha);
        }
    }

    bool passou = (obtido == esperado);

    cout << nomeCaso << ": " << (passou ? "PASSOU" : "FALHOU") << endl;

    if (!passou) {
        cout << "  Esperado (" << esperado.size() << " tuplas):" << endl;
        for (auto &l : esperado) cout << "    " << l << endl;

        cout << "  Obtido (" << obtido.size() << " tuplas):" << endl;
        for (auto &l : obtido) cout << "    " << l << endl;
    }
}

// Caso de teste 1: cada uva tem exatamente um vinho correspondente
void testeCaso1() {

    Tabela uvas;
    uvas.nome = "uvas";
    inserirTupla(uvas, criarTupla("1,Cabernet,Franca"));
    inserirTupla(uvas, criarTupla("2,Merlot,Italia"));

    Tabela vinhos;
    vinhos.nome = "vinhos";
    inserirTupla(vinhos, criarTupla("10,1,Brasil,Tinto,Seco"));
    inserirTupla(vinhos, criarTupla("11,2,Chile,Tinto,Suave"));

    ordenacaoExterna(uvas, 0);
    ordenacaoExterna(vinhos, 1);

    Tabela resultado = mergeJoin(uvas, vinhos, 0, 1);

    verificarResultado("Caso 1", resultado, {
        "1,Cabernet,Franca,10,1,Brasil,Tinto,Seco",
        "2,Merlot,Italia,11,2,Chile,Tinto,Suave"
    });
}

// Caso de teste 2: uma uva com dois vinhos, onde o join deve repetir a uva pra cada vinho
void testeCaso2() {

    Tabela uvas;
    uvas.nome = "uvas";
    inserirTupla(uvas, criarTupla("1,Cabernet,Franca"));

    Tabela vinhos;
    vinhos.nome = "vinhos";
    inserirTupla(vinhos, criarTupla("10,1,Brasil,Tinto,Seco"));
    inserirTupla(vinhos, criarTupla("11,1,Chile,Tinto,Suave"));

    ordenacaoExterna(uvas, 0);
    ordenacaoExterna(vinhos, 1);

    Tabela resultado = mergeJoin(uvas, vinhos, 0, 1);

    verificarResultado("Caso 2", resultado, {
        "1,Cabernet,Franca,10,1,Brasil,Tinto,Seco",
        "1,Cabernet,Franca,11,1,Chile,Tinto,Suave"
    });
}

// Caso de teste 3: nenhuma chave em comum, logo, o resultado deve ser vazio
void testeCaso3() {

    Tabela uvas;
    uvas.nome = "uvas";
    inserirTupla(uvas, criarTupla("1,Cabernet,Franca"));

    Tabela vinhos;
    vinhos.nome = "vinhos";
    inserirTupla(vinhos, criarTupla("10,2,Brasil,Tinto,Seco"));

    ordenacaoExterna(uvas, 0);
    ordenacaoExterna(vinhos, 1);

    Tabela resultado = mergeJoin(uvas, vinhos, 0, 1);

    verificarResultado("Caso 3", resultado, {});
}

// Caso de teste 4: duas uvas e dois vinhos com a mesma chave, faz-se o produto cartesiano
void testeCaso4() {

    Tabela uvas;
    uvas.nome = "uvas";
    inserirTupla(uvas, criarTupla("1,Cabernet,Franca"));
    inserirTupla(uvas, criarTupla("1,Cabernet Reserva,Italia"));

    Tabela vinhos;
    vinhos.nome = "vinhos";
    inserirTupla(vinhos, criarTupla("10,1,Brasil,Tinto,Seco"));
    inserirTupla(vinhos, criarTupla("11,1,Chile,Tinto,Suave"));

    ordenacaoExterna(uvas, 0);
    ordenacaoExterna(vinhos, 1);

    Tabela resultado = mergeJoin(uvas, vinhos, 0, 1);

    verificarResultado("Caso 4", resultado, {
        "1,Cabernet,Franca,10,1,Brasil,Tinto,Seco",
        "1,Cabernet,Franca,11,1,Chile,Tinto,Suave",
        "1,Cabernet Reserva,Italia,10,1,Brasil,Tinto,Seco",
        "1,Cabernet Reserva,Italia,11,1,Chile,Tinto,Suave"
    });
}

// Caso de teste 5: testa a ordenação externa de forma isolada, sem join
void testeCaso5() {

    Tabela tabela;
    tabela.nome = "teste";
    inserirTupla(tabela, criarTupla("3,Merlot"));
    inserirTupla(tabela, criarTupla("1,Cabernet"));
    inserirTupla(tabela, criarTupla("2,Malbec"));

    ordenacaoExterna(tabela, 0);

    verificarResultado("Caso 5", tabela, {
        "1,Cabernet",
        "2,Malbec",
        "3,Merlot"
    });
}

int main() {

    cout << "=== Casos de Teste ===" << endl;
    testeCaso1();
    testeCaso2();
    testeCaso3();
    testeCaso4();
    testeCaso5();
    cout << endl;

    cout << "=== Resultado com dados reais ===" << endl;

    Tabela uvas =
        carregarCSV(
            "dados/grapes.csv",
            "uvas"
        );

    Tabela vinhos =
        carregarCSV(
            "dados/wines.csv",
            "vinhos"
        );

    ordenacaoExterna(uvas, 0);
    ordenacaoExterna(vinhos, 1);

    Tabela resultado =
        mergeJoin(
            uvas,
            vinhos,
            0,
            1
        );

    imprimirTabela(resultado);

    return 0;
}
