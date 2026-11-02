#ifndef ESQUEMA_HPP
#define ESQUEMA_HPP

#include <unordered_map>
#include <string>

struct Esquema {
    int qtd_cols;
    std::unordered_map<std::string, int> nome_para_indice;
};

#endif