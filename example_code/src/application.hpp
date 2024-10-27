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
    void configurarIluminacao();
    void MouseMotionHandle(int x, int y); // For button hover effect
    void desenharBotaoSalvar(); // To draw the save button
    bool isEmptyFace(int cubo, int face);
    bool isExchangePossible(int currentCubo, int targetCubo);
    int getTopFace(int cuboIndex);
    void randomizeColors();
    void desenharBotaoRandomizar();
    void desenharBotaoSolver();
    bool checkWinCondition();
    bool lerEstadoDoJogoXML(const char* filename);

private:
    GLfloat xf, yf, win;
    GLuint texID[3];
    int tipoTextura[4][4];
    int view_w, view_h;
    int time;
    int cuboSelecionado;         // Índice do cubo atualmente selecionado
    int faceSelecionada;         // Face selecionada do cubo
    bool faceAtiva;              // Indica se uma face está ativa para troca de cor
    float angulosDeRotacao[4];
    bool botaoHover = false;
    const int BUTTON_X = 10;
    const int BUTTON_Y = 10;
    const int BUTTON_WIDTH = 120;
    const int BUTTON_HEIGHT = 30;
    const int BUTTON_SPACING = 10;
    const int RANDOM_BUTTON_X = BUTTON_X;
    const int RANDOM_BUTTON_Y = BUTTON_Y + BUTTON_HEIGHT + BUTTON_SPACING;
    static Application* instance;
    float cameraDistance = 50.0f;
    float cameraRotationX = 30.0f;
    float cameraRotationY = 45.0f;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;
    bool mousePressed = false;
    float zoomSpeed = 1.0f;
    float rotateSpeed = 0.5f;
    const int RANDOM_BUTTON_WIDTH = BUTTON_WIDTH;
    const int RANDOM_BUTTON_HEIGHT = BUTTON_HEIGHT;
    bool randomBotaoHover = false;

    bool isPontoNoBotaoRandomizar(int x, int y) {
        return (x >= RANDOM_BUTTON_X && x <= RANDOM_BUTTON_X + RANDOM_BUTTON_WIDTH &&
                y >= RANDOM_BUTTON_Y && y <= RANDOM_BUTTON_Y + RANDOM_BUTTON_HEIGHT);
    }

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

    void salvarEstadoAtualXML(const char* filename);
    bool salvarXML; // Flag to indicate if we should save

    bool isPontoNoBotao(int x, int y) {
        // Mouse coordinates are from top-left, need to check against button position
        return (x >= BUTTON_X && x <= BUTTON_X + BUTTON_WIDTH &&
                y >= BUTTON_Y && y <= BUTTON_Y + BUTTON_HEIGHT);
    }   

    static void staticMouseMotionHandle(int x, int y) {
        if (instance) instance->MouseMotionHandle(x, y);
    }

    static void staticMotionHandle(int x, int y) {
        if (instance) instance->MouseMotionHandle(x, y);
    }

    const int SOLVE_BUTTON_X = 10;
    const int SOLVE_BUTTON_Y = 90; // Below the randomize button
    const int SOLVE_BUTTON_WIDTH = 120;
    const int SOLVE_BUTTON_HEIGHT = 30;
    bool solveBotaoHover = false;
    std::vector<std::string> solutionSteps; // To store solution steps

    // Add these new member functions in the private section
    bool isPontoNoBotaoSolver(int x, int y) {
        return (x >= SOLVE_BUTTON_X && x <= SOLVE_BUTTON_X + SOLVE_BUTTON_WIDTH &&
                y >= SOLVE_BUTTON_Y && y <= SOLVE_BUTTON_Y + SOLVE_BUTTON_HEIGHT);
    }

    bool isValidSolution();
    void solvePuzzle();
    void executeStep(const std::string& step);
    void saveSolutionToXML(const char* filename);

};

#endif // APPLICATION_HPP
