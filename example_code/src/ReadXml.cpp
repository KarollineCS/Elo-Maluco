// ReadXml.cpp

#include "ReadXml.hpp"

// Construtor
ReadXml::ReadXml() : estado(4, std::vector<Face>(4)) {}

// Função para carregar o estado do jogo a partir de um arquivo XML
bool ReadXml::loadFromXML(const char* filename) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Falha ao carregar o arquivo XML!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("EloMaluco");
    if (!root) {
        std::cerr << "Elemento raiz 'EloMaluco' não encontrado!" << std::endl;
        return false;
    }

    // Itera pelas linhas do estado do jogo no XML
    tinyxml2::XMLElement* lineElement = root->FirstChildElement("Linha");
    int row = 0;
    while (lineElement && row < 4) {
        const char* lineText = lineElement->GetText();
        if (lineText) {
            // Divide a linha em faces
            std::string lineStr(lineText);
            for (int col = 0; col < 4; ++col) {
                std::string faceCode = lineStr.substr(col * 3, 3); // Assume formato "vmc", "vmi", etc.
                estado[row][col].cor = faceCode.substr(0, 2);
                estado[row][col].parte = faceCode[2];
            }
        }
        lineElement = lineElement->NextSiblingElement("Linha");
        row++;
    }

    return true;
}

// Função para acessar uma face específica
ReadXml::Face ReadXml::getFace(int row, int col) const {
    if (row < 0 || row >= 4 || col < 0 || col >= 4) {
        throw std::out_of_range("Índice fora dos limites");
    }
    return estado[row][col];
}

// Função para exibir o estado do jogo (apenas para teste)
void ReadXml::printState() const {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << "Face[" << i << "][" << j << "] - Cor: " << estado[i][j].cor
                      << ", Parte: " << estado[i][j].parte << std::endl;
        }
    }
}
