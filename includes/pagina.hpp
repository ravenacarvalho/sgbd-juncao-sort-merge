#ifndef PAGINA_HPP
#define PAGINA_HPP

#include "tupla.hpp"

#define MAX_TUPLAS 12

struct Pagina {

    vector<Tupla> tuplas;

    bool estaCheia() {
        return tuplas.size() >= MAX_TUPLAS;
    }
};

#endif