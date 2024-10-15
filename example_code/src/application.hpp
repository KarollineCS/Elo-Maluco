// Elo Maluco
// application.hpp
// Prof. Giovani Bernardes Vitor
// ECOI24 - 2024

#include <iostream>
#include <vector>
#include <list>

#include <GL/glut.h>

#include "tinyxml2.h"
#include "Objects.hpp"
#include "triangle.hpp"

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

using namespace std;



/////////////////////////////////////////////////////////////
// application.hpp

class Application
{
public:
    Application(int argc, char** argv);
    ~Application();
    void Inicializa(void);
    void rect(float p1[3], float p2[3], float p3[3], float p4[3], GLfloat* cor, int face);
    void desenhaCubo(float s, int cuboIndex);
    void draw();
    void resize(GLsizei w, GLsizei h);
    void KeyboardHandle(unsigned char key, int x, int y);
    void MouseHandle(int button, int state, int x, int y);
    void SpecialKeyHandle(int key, int x, int y);
    void update(int value, void (*func_ptr)(int));
    bool insert_object(void);
    void inicializarTexturas();
    void trocarCoresVertical(int cubo1, int cubo2);

private:
    GLfloat xf, yf, win;
    GLuint texID[3];
    int tipoTextura[4][4];
    int view_w, view_h;
    int time;
    int cuboSelecionado;         // Índice do cubo atualmente selecionado
    int faceSelecionada;         // Face selecionada do cubo
    bool faceAtiva;              // Indica se uma face está ativa para troca de cor
    float angulosDeRotacao[4];   // Ângulos de rotação para cada cubo

    // Matriz 4x4 para armazenar as cores das faces
    GLfloat cores[4][4][3]; // Cada face tem 3 componentes de cor (R, G, B)

    // Lista para armazenar os objetos desenhados
    std::list<Objects*> list_;

    // Função para ler o arquivo XML e preencher a matriz de elos
    bool lerEloDoXML(const char* filename);

    // Função para ler o arquivo XML e preencher a matriz de cores
    bool lerCoresDoXML(const char* filename);
    int cuboAnterior = -1; // Armazena o cubo selecionado anteriormente para a troca
    bool cuboSelecionadoParaTroca = false;
    GLfloat corCinza[3] = {0.5f, 0.5f, 0.5f}; // Definindo a cor cinza

    // Função para trocar as cores de uma face
    void trocarCorFace(int cuboIndex, int faceIndex);
};

#endif // APPLICATION_HPP
