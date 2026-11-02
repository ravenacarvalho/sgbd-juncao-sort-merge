#ifndef TABELA_HPP
#define TABELA_HPP

#include "pagina.hpp"
#include <vector>
#include <string>

using namespace std;

struct Tabela {

    vector<Pagina> paginas;

    string nome;
};

#endif