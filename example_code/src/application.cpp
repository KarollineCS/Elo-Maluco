// application.cpp

#define STB_IMAGE_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "application.hpp"
#include "stb_image.h"

// Inicializando a classe TinyXML2
#include "tinyxml2.h"
using namespace tinyxml2;

//---------------------------------------------------------------------
// Variaveis globais


///////////////////////////////////////////////////////////////////////
// Application Class
Application::Application(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("ELO MALUCO");
    Inicializa();
}

//---------------------------------------------------------------------
Application::~Application()
{
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
    carregaTextura(texID[0],"../image/superior.png");
    carregaTextura(texID[1],"../image/meio.png");
    carregaTextura(texID[2],"../image/inferior.png");
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

//---------------------------------------------------------------------
// Função que troca a cor na vertical
void Application::trocarCoresVertical(int cubo1, int cubo2)
{
    for (int i = 0; i < 4; ++i) {
        // Verificar se o cubo 1 ou cubo 2 tem a face cinza na posição i
        if (cores[cubo1][i][0] == 0.5f && cores[cubo1][i][1] == 0.5f && cores[cubo1][i][2] == 0.5f) {
            // Se o cubo 1 tem cinza, troque com o cubo 2
            GLfloat tempCor[3];
            tempCor[0] = cores[cubo1][i][0];
            tempCor[1] = cores[cubo1][i][1];
            tempCor[2] = cores[cubo1][i][2];

            cores[cubo1][i][0] = cores[cubo2][i][0];
            cores[cubo1][i][1] = cores[cubo2][i][1];
            cores[cubo1][i][2] = cores[cubo2][i][2];

            cores[cubo2][i][0] = tempCor[0];
            cores[cubo2][i][1] = tempCor[1];
            cores[cubo2][i][2] = tempCor[2];
        }
    }
}

//---------------------------------------------------------------------
// Função que troca a cor da face selecionada
void Application::trocarCorFace(int cuboIndex, int faceIndex) {
    GLfloat* corAtual = cores[cuboIndex][faceIndex];
    
    if (corAtual[0] == 1.0f && corAtual[1] == 0.0f && corAtual[2] == 0.0f) {  // Vermelho
        corAtual[0] = 0.0f; corAtual[1] = 1.0f; corAtual[2] = 0.0f;  // Troca para verde
    } else if (corAtual[0] == 0.0f && corAtual[1] == 1.0f && corAtual[2] == 0.0f) {  // Verde
        corAtual[0] = 1.0f; corAtual[1] = 1.0f; corAtual[2] = 0.0f;  // Troca para amarelo
    } else if (corAtual[0] == 1.0f && corAtual[1] == 1.0f && corAtual[2] == 0.0f) {  // Amarelo
        corAtual[0] = 1.0f; corAtual[1] = 1.0f; corAtual[2] = 1.0f;  // Troca para branco
    } else {  // Branco
        corAtual[0] = 1.0f; corAtual[1] = 0.0f; corAtual[2] = 0.0f;  // Troca para vermelho
    }

    glutPostRedisplay();  // Reexibe a cena com a nova cor aplicada
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
            cuboSelecionado = (cuboSelecionado - 1 + 4) % 4; // Seleciona o cubo anterior
            break;
        case GLUT_KEY_DOWN:
            cuboSelecionado = (cuboSelecionado + 1) % 4; // Seleciona o próximo cubo
            break;
        case GLUT_KEY_LEFT:
            angulosDeRotacao[cuboSelecionado] -= 15.0f; // Rotaciona para a esquerda
            break;
        case GLUT_KEY_RIGHT:
            angulosDeRotacao[cuboSelecionado] += 15.0f; // Rotaciona para a direita
            break;
        
        case 13: // Tecla Enter (ASCII 13)
            if (!cuboSelecionadoParaTroca) {
                cuboAnterior = cuboSelecionado; // Salva o cubo selecionado
                cuboSelecionadoParaTroca = true; // Indica que o cubo foi selecionado
            } else {
                // Tenta trocar as faces dos cubos selecionados verticalmente
                trocarCoresVertical(cuboAnterior, cuboSelecionado);
                cuboSelecionadoParaTroca = false; // Reseta o estado após a troca
            }
            break;
    }

    // Reexibe a cena após qualquer alteração
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
    // Topo
    glBindTexture(GL_TEXTURE_2D, texID[tipoTextura[cuboIndex][0]]);
    rect(v1, v4, v5, v8, cores[cuboIndex][0], 4);  // Usando a mesma cor da frente
    // Base
    glBindTexture(GL_TEXTURE_2D, texID[tipoTextura[cuboIndex][2]]);
    rect(v2, v7, v6, v3, cores[cuboIndex][2], 5);  // Usando a mesma cor da trás

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

//---------------------------------------------------------------------
void Application::draw()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Parâmetro do tamanho dos cubos
    float cubeSize = 5.0;

    // Desenha os quatro cubos empilhados
    for (int i = 0; i < 4; ++i)
    {
        glPushMatrix();
        glLoadIdentity();
        glTranslated(0.0, -i * cubeSize, -50.0);  // Translada para cima do cubo anterior

        // Aplica a rotação ao cubo
        glRotated(angulosDeRotacao[i], 0.0, 1.0, 0.0);

        desenhaCubo(cubeSize, i);
        glPopMatrix();
    }

    // Desenhar objetos da lista
    for (std::list<Objects*>::const_iterator it = list_.begin(); it != list_.end(); ++it)
    {
        (*it)->draw();
    }

    glFlush();
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
void Application::KeyboardHandle(unsigned char key, int x, int y)
{
    switch (key) {
        case 13:  // Tecla Enter
            if (!faceAtiva) {
                faceAtiva = true;
                faceSelecionada = cuboSelecionado;  // Armazena o cubo selecionado
                std::cout << "Face do cubo " << faceSelecionada << " selecionada." << std::endl;
            } else {
                // Verifica se uma face foi realmente selecionada
                if (faceSelecionada != -1) { 
                    trocarCorFace(faceSelecionada, 0);  // Troca a cor da face selecionada
                } else {
                    std::cout << "Nenhuma face selecionada." << std::endl;
                }
                faceAtiva = false; // Desativa a seleção da face após a troca
            }
            break;
        case 27:  // Tecla Esc (encerra o programa)
            exit(0);
            break;
    }
    
}

        
//---------------------------------------------------------------------
void Application::MouseHandle(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
         if (state == GLUT_DOWN) {
                  // Troca o tamanho do retângulo, que vai do centro da 
                  // janela até a posição onde o usuário clicou com o mouse
                  //xf = ( (2 * win * x) / view_w) - win;
                  //yf = ( ( (2 * win) * (y-view_h) ) / -view_h) - win;
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
    cuboSelecionado = 0; 
    faceSelecionada = -1;
    faceAtiva = false;
    for (int i = 0; i < 4; ++i) {
        angulosDeRotacao[i] = 0.0f; // Todos os cubos começam sem rotação
    }
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
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
    if (!lerCoresDoXML("../data/EloMaluco_estadoAtual_teste01.xml"))
    {
        std::cerr << "Erro ao carregar as cores do arquivo XML." << std::endl;
        exit(1); // Sai do programa se o XML não puder ser carregado
    }
    if (!lerEloDoXML("../data/EloMaluco_estadoAtual_teste01.xml"))
    {
        std::cerr << "Erro ao carregar os elos do arquivo XML." << std::endl;
        exit(1); // Sai do programa se o XML não puder ser carregado
    }
}
