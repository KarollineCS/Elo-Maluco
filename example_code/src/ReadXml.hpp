// ReadXml.hpp

#ifndef READ_XML_HPP
#define READ_XML_HPP

#include "tinyxml2.h"
#include <vector>
#include <string>
#include <iostream>

class ReadXml {
public:
    // Estrutura para representar uma face do cubo
    struct Face {
        std::string cor;
        char parte;
    };

    // Construtor
    ReadXml();

    // Função para carregar o estado do jogo a partir de um arquivo XML
    bool loadFromXML(const char* filename);

    // Função para acessar uma face específica
    Face getFace(int row, int col) const;

    // Função para exibir o estado do jogo (apenas para teste)
    void printState() const;

private:
    // Estado do jogo representado por uma matriz 4x4 de faces
    std::vector<std::vector<Face>> estado;
};

#endif // READ_XML_HPP
