// Elo Maluco
// main.cpp
// Prof. Giovani Bernardes Vitor
// ECOI24 - 2024



#include <iostream>
#include <thread>

#include "wraps.hpp"
#include "application.hpp"
#include "ReadXml.hpp"

     
// Programa Principal 
int main(int argc, char** argv)
{

     int time=0;

     Application app(argc,argv);
     ptr_ = &app;

     glutDisplayFunc(draw);
     glutReshapeFunc(resize);
     glutKeyboardFunc(KeyboardHandle);
     glutMouseFunc(MouseHandle);
     glutSpecialFunc(SpecialKeyHandle); 
     glutTimerFunc(30,update,time);
     glutMainLoop();

     return 0;
}
