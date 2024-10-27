// application.cpp

#define STB_IMAGE_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "application.hpp"
#include "stb_image.h"
#include <random>
#include <algorithm>


// Inicializando a classe TinyXML2
#include "tinyxml2.h"
using namespace tinyxml2;

//---------------------------------------------------------------------
// Variaveis globais

Application* Application::instance = nullptr;

///////////////////////////////////////////////////////////////////////
// Application Class
Application::Application(int argc, char** argv)
{
    instance = this;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("ELO MALUCO");
    glutMotionFunc(staticMotionHandle);
    glutPassiveMotionFunc(staticMouseMotionHandle);
    Inicializa();
}

//---------------------------------------------------------------------
Application::~Application()
{
    if (instance == this) {
        instance = nullptr;
    }
}

//---------------------------------------------------------------------
void carregaTextura(GLuint tex_id, std::string filePath){
    unsigned char* imgData;
    int largura, altura, canais;

    imgData = stbi_load(filePath.c_str(), &largura, &altura, &canais, 4);

    if(imgData){
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(imgData);
    }
    else {
        std::cerr << "Erro: Não foi possível carregar a textura!" << filePath.c_str() << std::endl;
    }
}

//---------------------------------------------------------------------
void Application::inicializarTexturas(){
    //Ativação de texturas
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //Gerar texturas
    glGenTextures(3, texID);
    carregaTextura(texID[2],"../image/superior.png");
    carregaTextura(texID[1],"../image/meio.png");
    carregaTextura(texID[0],"../image/inferior.png");
}

//---------------------------------------------------------------------
bool Application::lerEloDoXML(const char* filename) {

    //Cria um objeto do XMLDocument
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError eResult = doc.LoadFile(filename);

    //Carrega o arquivo XML
    if (eResult != tinyxml2::XML_SUCCESS) {
        std::cerr << "Erro ao carregar o arquivo XML: " << filename << std::endl;
        std::cerr << "Erro específico: " << doc.ErrorStr() << std::endl;
        return false;
    }

    //Acessa o nó raiz
    tinyxml2::XMLElement* root = doc.FirstChildElement("EloMaluco");
    if (!root) {
        std::cerr << "Elemento raiz 'EloMaluco' não encontrado!" << std::endl;
        return false;
    }

    //Acessa o estado atual
    tinyxml2::XMLElement* estadoAtual = root->FirstChildElement("EstadoAtual");
    if (!estadoAtual) {
        std::cerr << "Elemento 'EstadoAtual' não encontrado!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement* rowElement = estadoAtual->FirstChildElement("row");
    int row = 0;

    while (rowElement && row < 4) {
        tinyxml2::XMLElement* colElement = rowElement->FirstChildElement("col");
        int col = 0;

        while (colElement && col < 4) {
            const char* faceCode = colElement->GetText();
            if (faceCode && strlen(faceCode) >= 3) {
                char thirdChar = faceCode[2];

                if (thirdChar == 's') {
                    tipoTextura[row][col] = 0; //Superior
                } else if (thirdChar == 'm') {
                    tipoTextura[row][col] = 1; //Meio
                } else if (thirdChar == 'i') {
                    tipoTextura[row][col] = 2; //Inferior
                } else if (thirdChar == 'o') {
                    tipoTextura[row][col] = -1; //Face vazia sem textura
                }
            }

            colElement = colElement->NextSiblingElement("col");
            col++;
        }

        rowElement = rowElement->NextSiblingElement("row");
        row++;
    }

    return true;
}

//---------------------------------------------------------------------
bool Application::lerCoresDoXML(const char* filename) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError eResult = doc.LoadFile(filename);
    if (eResult != tinyxml2::XML_SUCCESS) {
        std::cerr << "Erro ao carregar o arquivo XML: " << filename << std::endl;
        std::cerr << "Erro específico: " << doc.ErrorStr() << std::endl;
        return false;
    }
    tinyxml2::XMLElement* root = doc.FirstChildElement("EloMaluco");
    if (!root) {
        std::cerr << "Elemento raiz 'EloMaluco' não encontrado!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement* estadoAtual = root->FirstChildElement("EstadoAtual");
    if (!estadoAtual) {
        std::cerr << "Elemento 'EstadoAtual' não encontrado!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement* rowElement = estadoAtual->FirstChildElement("row");
    int row = 0;

    while (rowElement && row < 4) {
        tinyxml2::XMLElement* colElement = rowElement->FirstChildElement("col");
        int col = 0;

        while (colElement && col < 4) {
            const char* faceCode = colElement->GetText();
            if (faceCode) {
                if (strncmp(faceCode, "vm", 2) == 0) {
                    cores[row][col][0] = 1.0f; // Vermelho
                    cores[row][col][1] = 0.0f;
                    cores[row][col][2] = 0.0f;
                } else if (strncmp(faceCode, "am", 2) == 0) {
                    cores[row][col][0] = 1.0f; // Amarelo
                    cores[row][col][1] = 1.0f;
                    cores[row][col][2] = 0.0f;
                } else if (strncmp(faceCode, "vr", 2) == 0) {
                    cores[row][col][0] = 0.0f; // Verde
                    cores[row][col][1] = 1.0f;
                    cores[row][col][2] = 0.0f;
                } else if (strncmp(faceCode, "br", 2) == 0) {
                    cores[row][col][0] = 1.0f; // Branco
                    cores[row][col][1] = 1.0f;
                    cores[row][col][2] = 1.0f;
                } else if (strncmp(faceCode, "vz", 2) == 0) {
                    cores[row][col][0] = 0.5f; // Cinza (cor vazia)
                    cores[row][col][1] = 0.5f;
                    cores[row][col][2] = 0.5f;
                }
            }

            colElement = colElement->NextSiblingElement("col");
            col++;
        }

        rowElement = rowElement->NextSiblingElement("row");
        row++;
    }

    return true;
}

void Application::salvarEstadoAtualXML(const char* filename) {
    tinyxml2::XMLDocument doc;
    
    // Create root element
    tinyxml2::XMLElement* root = doc.NewElement("EloMaluco");
    doc.InsertFirstChild(root);
    
    // Create EstadoAtual element
    tinyxml2::XMLElement* estadoAtual = doc.NewElement("EstadoAtual");
    root->InsertEndChild(estadoAtual);
    
    // Save each row
    for (int i = 0; i < 4; i++) {
        tinyxml2::XMLElement* row = doc.NewElement("row");
        estadoAtual->InsertEndChild(row);
        
        // Save each column
        for (int j = 0; j < 4; j++) {
            tinyxml2::XMLElement* col = doc.NewElement("col");
            
            // Create the face code string
            std::string faceCode;
            
            // Determine color prefix
            if (cores[i][j][0] == 1.0f && cores[i][j][1] == 0.0f && cores[i][j][2] == 0.0f) {
                faceCode = "vm"; // Vermelho
            } else if (cores[i][j][0] == 1.0f && cores[i][j][1] == 1.0f && cores[i][j][2] == 0.0f) {
                faceCode = "am"; // Amarelo
            } else if (cores[i][j][0] == 0.0f && cores[i][j][1] == 1.0f && cores[i][j][2] == 0.0f) {
                faceCode = "vr"; // Verde
            } else if (cores[i][j][0] == 1.0f && cores[i][j][1] == 1.0f && cores[i][j][2] == 1.0f) {
                faceCode = "br"; // Branco
            } else {
                faceCode = "vz"; // Vazio/Cinza
            }
            
            // Add face type suffix
            switch(tipoTextura[i][j]) {
                case 0:
                    faceCode += "s"; // Superior
                    break;
                case 1:
                    faceCode += "m"; // Meio
                    break;
                case 2:
                    faceCode += "i"; // Inferior
                    break;
                default:
                    faceCode += "o"; // Vazio
                    break;
            }
            
            col->SetText(faceCode.c_str());
            row->InsertEndChild(col);
        }
    }
    
    // Save to file
    tinyxml2::XMLError result = doc.SaveFile(filename);
    if (result == tinyxml2::XML_SUCCESS) {
        std::cout << "Estado do jogo salvo com sucesso em " << filename << std::endl;
    } else {
        std::cerr << "Erro ao salvar o estado do jogo" << std::endl;
    }
}

void Application::saveSolutionToXML(const char* filename) {
    tinyxml2::XMLDocument doc;
    
    // Create root element
    tinyxml2::XMLElement* root = doc.NewElement("EloMalucoSolution");
    doc.InsertFirstChild(root);
    
    // Add solution steps
    tinyxml2::XMLElement* steps = doc.NewElement("SolutionSteps");
    root->InsertEndChild(steps);
    
    for (const auto& step : solutionSteps) {
        tinyxml2::XMLElement* stepElem = doc.NewElement("Step");
        stepElem->SetText(step.c_str());
        steps->InsertEndChild(stepElem);
    }
    
    // Add final state
    tinyxml2::XMLElement* finalState = doc.NewElement("FinalState");
    root->InsertEndChild(finalState);
    
    for (int i = 0; i < 4; i++) {
        tinyxml2::XMLElement* row = doc.NewElement("row");
        finalState->InsertEndChild(row);
        
        for (int j = 0; j < 4; j++) {
            tinyxml2::XMLElement* col = doc.NewElement("col");
            std::string faceCode;
            

            if (isEmptyFace(i, j)) {
                faceCode = "vzo"; // Empty face
            } else {

                if (cores[i][j][0] == 1.0f && cores[i][j][1] == 0.0f && cores[i][j][2] == 0.0f)
                    faceCode = "vm";
                else if (cores[i][j][0] == 1.0f && cores[i][j][1] == 1.0f && cores[i][j][2] == 0.0f)
                    faceCode = "am";
                else if (cores[i][j][0] == 0.0f && cores[i][j][1] == 1.0f && cores[i][j][2] == 0.0f)
                    faceCode = "vr";
                else if (cores[i][j][0] == 1.0f && cores[i][j][1] == 1.0f && cores[i][j][2] == 1.0f)
                    faceCode = "br";
                

                switch(tipoTextura[i][j]) {
                    case 0: faceCode += "s"; break;
                    case 1: faceCode += "m"; break;
                    case 2: faceCode += "i"; break;
                    default: faceCode += "o"; break;
                }
            }
            
            col->SetText(faceCode.c_str());
            row->InsertEndChild(col);
        }
    }
    
    // Save to file
    doc.SaveFile(filename);
}

//---------------------------------------------------------------------

bool Application::isEmptyFace(int cubo, int face) {
    return (cores[cubo][face][0] == 0.5f &&
            cores[cubo][face][1] == 0.5f &&
            cores[cubo][face][2] == 0.5f);
}

bool Application::isExchangePossible(int currentCubo, int targetCubo) {

    return (abs(currentCubo - targetCubo) == 1);
}

int Application::getTopFace(int cuboIndex) {
    float angle = angulosDeRotacao[cuboIndex];

    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    
    if (angle < 45 || angle >= 315) return 0; 
    if (angle >= 45 && angle < 135) return 1;
    if (angle >= 135 && angle < 225) return 2;
    if (angle >= 225 && angle < 315) return 3; 
    
    return 0; 
}

// Adicione esta nova função à classe Application no arquivo application.hpp:
bool Application::lerEstadoDoJogoXML(const char* filename) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError eResult = doc.LoadFile(filename);
    if (eResult != tinyxml2::XML_SUCCESS) {
        std::cerr << "Erro ao carregar o arquivo XML: " << filename << std::endl;
        std::cerr << "Erro específico: " << doc.ErrorStr() << std::endl;
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("EloMaluco");
    if (!root) {
        std::cerr << "Elemento raiz 'EloMaluco' não encontrado!" << std::endl;
        return false;
    }

    tinyxml2::XMLElement* estadoAtual = root->FirstChildElement("EstadoAtual");
    if (!estadoAtual) {
        std::cerr << "Elemento 'EstadoAtual' não encontrado!" << std::endl;
        return false;
    }

    // Ler cores e texturas
    tinyxml2::XMLElement* rowElement = estadoAtual->FirstChildElement("row");
    int row = 0;

    while (rowElement && row < 4) {
        tinyxml2::XMLElement* colElement = rowElement->FirstChildElement("col");
        int col = 0;

        while (colElement && col < 4) {
            const char* faceCode = colElement->GetText();
            if (faceCode) {
                // Definir cores baseado nos dois primeiros caracteres
                if (strncmp(faceCode, "vm", 2) == 0) {
                    cores[row][col][0] = 1.0f; // Vermelho
                    cores[row][col][1] = 0.0f;
                    cores[row][col][2] = 0.0f;
                } else if (strncmp(faceCode, "am", 2) == 0) {
                    cores[row][col][0] = 1.0f; // Amarelo
                    cores[row][col][1] = 1.0f;
                    cores[row][col][2] = 0.0f;
                } else if (strncmp(faceCode, "vr", 2) == 0) {
                    cores[row][col][0] = 0.0f; // Verde
                    cores[row][col][1] = 1.0f;
                    cores[row][col][2] = 0.0f;
                } else if (strncmp(faceCode, "br", 2) == 0) {
                    cores[row][col][0] = 1.0f; // Branco
                    cores[row][col][1] = 1.0f;
                    cores[row][col][2] = 1.0f;
                } else if (strncmp(faceCode, "vz", 2) == 0) {
                    cores[row][col][0] = 0.5f; // Cinza (vazio)
                    cores[row][col][1] = 0.5f;
                    cores[row][col][2] = 0.5f;
                }

                // Definir tipo de textura baseado no terceiro caractere
                if (strlen(faceCode) >= 3) {
                    char thirdChar = faceCode[2];
                    if (thirdChar == 's') {
                        tipoTextura[row][col] = 0; // Superior
                    } else if (thirdChar == 'm') {
                        tipoTextura[row][col] = 1; // Meio
                    } else if (thirdChar == 'i') {
                        tipoTextura[row][col] = 2; // Inferior
                    } else if (thirdChar == 'o') {
                        tipoTextura[row][col] = -1; // Face vazia
                    }
                }
            }

            colElement = colElement->NextSiblingElement("col");
            col++;
        }

        rowElement = rowElement->NextSiblingElement("row");
        row++;
    }

    return true;
}


void Application::trocarCoresVertical(int cubo1, int cubo2) {

    if (!isExchangePossible(cubo1, cubo2)) {
        std::cout << "Troca impossível: cubos não são adjacentes" << std::endl;
        return;
    }

    int emptyFace1 = -1, emptyFace2 = -1;


    for (int i = 0; i < 4; ++i) {
        if (isEmptyFace(cubo1, i) && emptyFace1 == -1) emptyFace1 = i;
        if (isEmptyFace(cubo2, i) && emptyFace2 == -1) emptyFace2 = i;
        if (emptyFace1 != -1 && emptyFace2 != -1) break;
    }


    if (emptyFace1 != -1 || emptyFace2 != -1) {

        if (emptyFace1 == -1) emptyFace1 = emptyFace2;
        if (emptyFace2 == -1) emptyFace2 = emptyFace1;


        GLfloat tempCor[3];
        int tempTextura;

        tempCor[0] = cores[cubo1][emptyFace1][0];
        tempCor[1] = cores[cubo1][emptyFace1][1];
        tempCor[2] = cores[cubo1][emptyFace1][2];
        tempTextura = tipoTextura[cubo1][emptyFace1];


        cores[cubo1][emptyFace1][0] = cores[cubo2][emptyFace2][0];
        cores[cubo1][emptyFace1][1] = cores[cubo2][emptyFace2][1];
        cores[cubo1][emptyFace1][2] = cores[cubo2][emptyFace2][2];
        tipoTextura[cubo1][emptyFace1] = tipoTextura[cubo2][emptyFace2];

        cores[cubo2][emptyFace2][0] = tempCor[0];
        cores[cubo2][emptyFace2][1] = tempCor[1];
        cores[cubo2][emptyFace2][2] = tempCor[2];
        tipoTextura[cubo2][emptyFace2] = tempTextura;

        std::cout << "Troca realizada com sucesso" << std::endl;
    } else {
        std::cout << "Nenhuma face vazia encontrada nos cubos selecionados" << std::endl;
    }
}

void Application::randomizeColors() {
    std::vector<GLfloat*> availableColors = {
        new GLfloat[3]{1.0f, 0.0f, 0.0f}, // Vermelho
        new GLfloat[3]{1.0f, 1.0f, 0.0f}, // Amarelo
        new GLfloat[3]{0.0f, 1.0f, 0.0f}, // Verde
        new GLfloat[3]{1.0f, 1.0f, 1.0f}  // Branco
    };

    std::random_device rd;
    std::mt19937 g(rd());

    for (int cubo = 0; cubo < 4; ++cubo) {
        std::shuffle(availableColors.begin(), availableColors.end(), g);
        int colorIndex = 0;
        for (int face = 0; face < 4; ++face) {
            if (tipoTextura[cubo][face] != -1) { 
                memcpy(cores[cubo][face], availableColors[colorIndex], 3 * sizeof(GLfloat));
                colorIndex = (colorIndex + 1) % availableColors.size();
            } else {

                memcpy(cores[cubo][face], corCinza, 3 * sizeof(GLfloat));
            }
        }
    }

    for (auto& color : availableColors) {
        delete[] color;
    }

    glutPostRedisplay();
}

//---------------------------------------------------------------------
// Função que configura a iluminação do cenário

void Application::configurarIluminacao(){
    GLfloat luz_ambiente[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat luz_difusa[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat luz_especular[] = {0.5f, 0.5f, 0.5f, 1.0f};

    // Configurar múltiplas fontes de luz
    GLfloat posicao_luz1[] = {50.0f, 50.0f, 50.0f, 0.0f};  // Luz direcional
    GLfloat posicao_luz2[] = {-50.0f, -50.0f, -50.0f, 0.0f};  // Luz direcional oposta

    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz1);

    glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz2);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);

    glEnable(GL_DEPTH_TEST);

    // Configurar modelo de iluminação
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    GLfloat lmodel_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

void Application::SpecialKeyHandle(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:
            if (cuboSelecionado > 0) {
                cuboSelecionado--;
            }
            break;
        case GLUT_KEY_DOWN:
            if (cuboSelecionado < 3) {
                cuboSelecionado++;
            }
            break;
        case GLUT_KEY_LEFT:
            angulosDeRotacao[cuboSelecionado] -= 90.0f;
            if (angulosDeRotacao[cuboSelecionado] < 0) {
                angulosDeRotacao[cuboSelecionado] += 360.0f;
            }
            break;
        case GLUT_KEY_RIGHT:
            angulosDeRotacao[cuboSelecionado] += 90.0f;
            if (angulosDeRotacao[cuboSelecionado] >= 360.0f) {
                angulosDeRotacao[cuboSelecionado] -= 360.0f;
            }
            break;
    }


    glutPostRedisplay();
}

bool Application::checkWinCondition() {

    for (int cubo = 0; cubo < 4; cubo++) {
        bool hasEmptyFace = false;
        GLfloat baseColor[3] = {-1, -1, -1}; 
        
        for (int face = 0; face < 4; face++) {

            if (isEmptyFace(cubo, face)) {
                hasEmptyFace = true;
                continue;
            }
            
            if (baseColor[0] == -1) {
                baseColor[0] = cores[cubo][face][0];
                baseColor[1] = cores[cubo][face][1];
                baseColor[2] = cores[cubo][face][2];
            }

            else if (cores[cubo][face][0] != baseColor[0] ||
                     cores[cubo][face][1] != baseColor[1] ||
                     cores[cubo][face][2] != baseColor[2]) {
                return false;
            }
        }
    }
    return true;
}

void Application::solvePuzzle() {
    if (lerEstadoDoJogoXML("../data/JogoResolvido.xml")) {
        // Desabilita iluminação e textura temporariamente
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        
        // Configura para desenho 2D
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, view_w, view_h, 0, -1, 1);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        
        // Define a cor do texto como verde
        glColor3f(0.0f, 0.0f, 0.0f);
        
        // Posiciona o texto no centro da tela
        const char* message = "JOGO CONCLUIDO!";
        int textWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)message);
        glRasterPos2f(view_w/2 - textWidth/2, view_h/2);
        
        // Desenha o texto
        for (const char* c = message; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        
        // Restaura as matrizes
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        
        // Reativa iluminação e textura
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        
        std::cout << "Jogo resolvido com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro resolver o jogo!" << std::endl;
    }
    glutPostRedisplay();
}

//---------------------------------------------------------------------
void Application::rect(float p1[3], float p2[3], float p3[3], float p4[3], GLfloat* cor, int face)
{

    glColor3fv(cor);
    glBegin(GL_QUADS);

    // Calcula e define a normal para a face
    float v1[3], v2[3], normal[3];
    for (int i = 0; i < 3; i++) {
        v1[i] = p2[i] - p1[i];
        v2[i] = p4[i] - p1[i];
    }
    normal[0] = v1[1]*v2[2] - v1[2]*v2[1];
    normal[1] = v1[2]*v2[0] - v1[0]*v2[2];
    normal[2] = v1[0]*v2[1] - v1[1]*v2[0];
    glNormal3fv(normal);
        
    switch(face) {
        case 0: // Frente
        case 1: // Direita
            glTexCoord2f(0.0, 1.0); glVertex3fv(p1);
            glTexCoord2f(0.0, 0.0); glVertex3fv(p2);
            glTexCoord2f(1.0, 0.0); glVertex3fv(p3);
            glTexCoord2f(1.0, 1.0); glVertex3fv(p4);
            break;
        case 2: // Trás
        case 3: // Esquerda
            glTexCoord2f(0.0, 0.0); glVertex3fv(p1);
            glTexCoord2f(1.0, 0.0); glVertex3fv(p2);
            glTexCoord2f(1.0, 1.0); glVertex3fv(p3);
            glTexCoord2f(0.0, 1.0); glVertex3fv(p4);
            break;
        case 4: // Topo
            glTexCoord2f(0.0, 1.0); glVertex3fv(p1);
            glTexCoord2f(0.0, 0.0); glVertex3fv(p2);
            glTexCoord2f(1.0, 0.0); glVertex3fv(p3);
            glTexCoord2f(1.0, 1.0); glVertex3fv(p4);
            break;
        case 5: // Base
            glTexCoord2f(1.0, 1.0); glVertex3fv(p1);
            glTexCoord2f(0.0, 1.0); glVertex3fv(p2);
            glTexCoord2f(0.0, 0.0); glVertex3fv(p3);
            glTexCoord2f(1.0, 0.0); glVertex3fv(p4);
            break;
    }
    
    glEnd();
}

//---------------------------------------------------------------------
void Application::desenhaCubo(float s, int cuboIndex)
{
    float d = s / 2.0;

    // vértices da frente
    float v1[3] = {-d, d, d};
    float v2[3] = {-d, -d, d};
    float v3[3] = {d, -d, d};
    float v4[3] = {d, d, d};

    // vértices da face de trás
    float v5[3] = {d, d, -d};
    float v6[3] = {d, -d, -d};
    float v7[3] = {-d, -d, -d};
    float v8[3] = {-d, d, -d};

    glPushAttrib(GL_LIGHTING_BIT);
    GLfloat mat_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat mat_shininess[] = { 30.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Frente
    glBindTexture(GL_TEXTURE_2D, texID[tipoTextura[cuboIndex][0]]);
    rect(v1, v2, v3, v4, cores[cuboIndex][0], 0);
    // Direita
    glBindTexture(GL_TEXTURE_2D, texID[tipoTextura[cuboIndex][1]]);
    rect(v4, v3, v6, v5, cores[cuboIndex][1], 1);
    // Trás
    glBindTexture(GL_TEXTURE_2D, texID[tipoTextura[cuboIndex][2]]);
    rect(v5, v8, v7, v6, cores[cuboIndex][2], 2);
    // Esquerda
    glBindTexture(GL_TEXTURE_2D, texID[tipoTextura[cuboIndex][3]]);
    rect(v1, v8, v7, v2, cores[cuboIndex][3], 2);

    glPopAttrib();

    // Se o cubo atual for o selecionado, desenhe um contorno ao redor dele
    if (cuboIndex == cuboSelecionado)
    {
        glColor3f(1.0f, 1.0f, 1.0f); // Cor amarela para o contorno
        glLineWidth(8.0f); // Aumenta a espessura da linha
        glDisable(GL_LIGHTING);

        // Desenha o contorno do cubo
        glBegin(GL_LINE_LOOP);
        glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3); glVertex3fv(v4); // Frente
        glVertex3fv(v4); glVertex3fv(v3); glVertex3fv(v6); glVertex3fv(v5); // Direita
        glVertex3fv(v5); glVertex3fv(v8); glVertex3fv(v7); glVertex3fv(v6); // Trás
        glVertex3fv(v1); glVertex3fv(v8); glVertex3fv(v7); glVertex3fv(v2); // Esquerda
        glVertex3fv(v1); glVertex3fv(v4); glVertex3fv(v5); glVertex3fv(v8); // Topo
        glVertex3fv(v2); glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v3); // Base
        glEnd();
    }
    glEnable(GL_LIGHTING);

}


void Application::desenharBotaoSalvar() {

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
     glOrtho(0, view_w, view_h, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    if (botaoHover) {
        glColor3f(0.7f, 0.7f, 0.7f);
    } else {
        glColor3f(0.5f, 0.5f, 0.5f);
    }
    
    glBegin(GL_QUADS);
    glVertex2f(BUTTON_X, BUTTON_Y);
    glVertex2f(BUTTON_X + BUTTON_WIDTH, BUTTON_Y);
    glVertex2f(BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
    glVertex2f(BUTTON_X, BUTTON_Y + BUTTON_HEIGHT);
    glEnd();
    
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(BUTTON_X, BUTTON_Y);
    glVertex2f(BUTTON_X + BUTTON_WIDTH, BUTTON_Y);
    glVertex2f(BUTTON_X + BUTTON_WIDTH, BUTTON_Y + BUTTON_HEIGHT);
    glVertex2f(BUTTON_X, BUTTON_Y + BUTTON_HEIGHT);
    glEnd();
    
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(BUTTON_X + 10, BUTTON_Y + BUTTON_HEIGHT/2 + 5);
    const char* text = "Save Game";
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glPopAttrib();
}

void Application::desenharBotaoSolver() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, view_w, view_h, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    if (solveBotaoHover) {
        glColor3f(0.7f, 0.7f, 0.7f);
    } else {
        glColor3f(0.5f, 0.5f, 0.5f);
    }
    
    glBegin(GL_QUADS);
    glVertex2f(SOLVE_BUTTON_X, SOLVE_BUTTON_Y);
    glVertex2f(SOLVE_BUTTON_X + SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_Y);
    glVertex2f(SOLVE_BUTTON_X + SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_Y + SOLVE_BUTTON_HEIGHT);
    glVertex2f(SOLVE_BUTTON_X, SOLVE_BUTTON_Y + SOLVE_BUTTON_HEIGHT);
    glEnd();
    
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(SOLVE_BUTTON_X, SOLVE_BUTTON_Y);
    glVertex2f(SOLVE_BUTTON_X + SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_Y);
    glVertex2f(SOLVE_BUTTON_X + SOLVE_BUTTON_WIDTH, SOLVE_BUTTON_Y + SOLVE_BUTTON_HEIGHT);
    glVertex2f(SOLVE_BUTTON_X, SOLVE_BUTTON_Y + SOLVE_BUTTON_HEIGHT);
    glEnd();
    
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(SOLVE_BUTTON_X + 10, SOLVE_BUTTON_Y + SOLVE_BUTTON_HEIGHT/2 + 5);
    const char* text = "Solve Puzzle";
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glPopAttrib();
}

void Application::desenharBotaoRandomizar() {

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, view_w, view_h, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    if (randomBotaoHover) {
        glColor3f(0.7f, 0.7f, 0.7f);
    } else {
        glColor3f(0.5f, 0.5f, 0.5f);
    }
    
    glBegin(GL_QUADS);
    glVertex2f(RANDOM_BUTTON_X, RANDOM_BUTTON_Y);
    glVertex2f(RANDOM_BUTTON_X + RANDOM_BUTTON_WIDTH, RANDOM_BUTTON_Y);
    glVertex2f(RANDOM_BUTTON_X + RANDOM_BUTTON_WIDTH, RANDOM_BUTTON_Y + RANDOM_BUTTON_HEIGHT);
    glVertex2f(RANDOM_BUTTON_X, RANDOM_BUTTON_Y + RANDOM_BUTTON_HEIGHT);
    glEnd();
    
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(RANDOM_BUTTON_X, RANDOM_BUTTON_Y);
    glVertex2f(RANDOM_BUTTON_X + RANDOM_BUTTON_WIDTH, RANDOM_BUTTON_Y);
    glVertex2f(RANDOM_BUTTON_X + RANDOM_BUTTON_WIDTH, RANDOM_BUTTON_Y + RANDOM_BUTTON_HEIGHT);
    glVertex2f(RANDOM_BUTTON_X, RANDOM_BUTTON_Y + RANDOM_BUTTON_HEIGHT);
    glEnd();
    
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(RANDOM_BUTTON_X + 10, RANDOM_BUTTON_Y + RANDOM_BUTTON_HEIGHT/2 + 5);
    const char* text = "Randomize Colors";
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glPopAttrib();
}

void Application::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // First draw the 3D scene
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)view_w/view_h, 0.1, 500.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Calculate camera position using spherical coordinates
    float camX = cameraDistance * cos(cameraRotationX * M_PI/180.0f) * sin(cameraRotationY * M_PI/180.0f);
    float camY = cameraDistance * sin(cameraRotationX * M_PI/180.0f);
    float camZ = cameraDistance * cos(cameraRotationX * M_PI/180.0f) * cos(cameraRotationY * M_PI/180.0f);

    gluLookAt(camX, camY, camZ,  // Camera position
              0.0f, 0.0f, 0.0f,   // Look at point
              0.0f, 1.0f, 0.0f);  // Up vector

    // Enable 3D rendering states
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    // Draw the cubes
    float cubeSize = 5.0;
    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glTranslated(0.0, -i * cubeSize, 0.0);
        glRotated(angulosDeRotacao[i], 0.0, 1.0, 0.0);
        desenhaCubo(cubeSize, i);
        glPopMatrix();
    }

    // Draw other 3D objects
    for (std::list<Objects*>::const_iterator it = list_.begin(); it != list_.end(); ++it) {
        (*it)->draw();
    }
    
    // Now draw the UI elements
    desenharBotaoSalvar();
    desenharBotaoRandomizar();
    desenharBotaoSolver();

    glutSwapBuffers();
}

//---------------------------------------------------------------------
void Application::resize(GLsizei w, GLsizei h)
{ 
    // Especifica as dimensões da Viewport
    glViewport(0, 0, w, h);
    view_w = w;
    view_h = h;                   

    // Inicializa o sistema de coordenadas
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluOrtho2D (-win, win, -win, win);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(GLdouble)view_w/view_h,0.1,500.0);

/*
    double rate=2.5;
    gluLookAt(rate*10,rate*20,rate*20,0,0,0,0,0,1);
    glMatrixMode(GL_MODELVIEW);    
    glLoadIdentity() ;
*/
}


//---------------------------------------------------------------------
void Application::KeyboardHandle(unsigned char key, int x, int y) {
    switch (key) {
        case 13:  // Enter key
            if (!cuboSelecionadoParaTroca) {
                bool hasEmptyFace = false;
                for (int i = 0; i < 4; ++i) {
                    if (isEmptyFace(cuboSelecionado, i)) {
                        hasEmptyFace = true;
                        break;
                    }
                }

                if (hasEmptyFace) {
                    cuboAnterior = cuboSelecionado;
                    cuboSelecionadoParaTroca = true;
                    std::cout << "Cubo com face vazia selecionado. Selecione o cubo para troca." << std::endl;
                } else {
                    std::cout << "O cubo selecionado não possui face vazia." << std::endl;
                }
            } else {
                trocarCoresVertical(cuboAnterior, cuboSelecionado);
                cuboSelecionadoParaTroca = false;
            }
            break;
        case 27:  // Esc key
            exit(0);
            break;
    }
    glutPostRedisplay();
}

        
//---------------------------------------------------------------------
void Application::MouseHandle(int button, int state, int x, int y) {
    if (isPontoNoBotao(x, y)) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            salvarEstadoAtualXML("../data/EloMaluco_estadoAtual_save.xml");
        }
        return;
    }

    if (isPontoNoBotaoRandomizar(x, y)) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            randomizeColors();
        }
        return;
    }

    if (isPontoNoBotaoSolver(x, y)) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            solvePuzzle();
        }
        return;
    }

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mousePressed = false;
        }
    }
}

void Application::MouseMotionHandle(int x, int y) {
    if (isPontoNoBotao(x, y)) {
        botaoHover = true;
        randomBotaoHover = false;
        solveBotaoHover = false;
        glutPostRedisplay();
        return;
    } else if (isPontoNoBotaoRandomizar(x, y)) {
        randomBotaoHover = true;
        botaoHover = false;
        solveBotaoHover = false;
        glutPostRedisplay();
        return;
    } else if (isPontoNoBotaoSolver(x, y)) {
        solveBotaoHover = true;
        botaoHover = false;
        randomBotaoHover = false;
        glutPostRedisplay();
        return;
    } else {
        botaoHover = false;
        randomBotaoHover = false;
        solveBotaoHover = false;
    }

    if (mousePressed) {
        float deltaX = x - lastMouseX;
        float deltaY = y - lastMouseY;

        cameraRotationY += deltaX * rotateSpeed;
        cameraRotationX += deltaY * rotateSpeed;

        if (cameraRotationX > 89.0f) cameraRotationX = 89.0f;
        if (cameraRotationX < -89.0f) cameraRotationX = -89.0f;

        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

//---------------------------------------------------------------------


//---------------------------------------------------------------------
void Application::update(int value, void (*func_ptr)(int))
{	
	glutPostRedisplay();
	glutTimerFunc(30,func_ptr,time);

}
//---------------------------------------------------------------------
bool Application::insert_object(void)
{
	Triangle *obj;
	//Objects * node = reinterpret_cast<Objects*>(obj);
	list_.push_back(new Triangle());
	std::cout << " insert: " << list_.size() << std::endl; 

	return true;
}

//---------------------------------------------------------------------
void Application::Inicializa(void)
{
    // Initialize camera parameters
    cameraDistance = 50.0f;
    cameraRotationX = 30.0f;
    cameraRotationY = 45.0f;
    mousePressed = false;
    cuboSelecionado = 0; 
    faceSelecionada = -1;
    faceAtiva = false;
    for (int i = 0; i < 4; ++i) {
        angulosDeRotacao[i] = 0.0f; // Todos os cubos começam sem rotação
    }
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    xf = 50.0f;
    yf = 50.0f;
    win = 250.0f;
    time = 0;

    //Inicializa as texturas
    inicializarTexturas();

    //Inicializa a iluminação
    configurarIluminacao();

    // Carregar as cores das faces a partir do arquivo XML
    if (!lerCoresDoXML("../data/estadoatual.xml"))
    {
        std::cerr << "Erro ao carregar as cores do arquivo XML." << std::endl;
        exit(1); // Sai do programa se o XML não puder ser carregado
    }
    if (!lerEloDoXML("../data/estadoatual.xml"))
    {
        std::cerr << "Erro ao carregar os elos do arquivo XML." << std::endl;
        exit(1); // Sai do programa se o XML não puder ser carregado
    }
}
