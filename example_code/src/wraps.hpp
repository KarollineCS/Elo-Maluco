// Elo Maluco
// wraps.hpp
// Prof. Giovani Bernardes Vitor
// ECOI24 - 2024


#ifndef __WRAPS_HPP__
#define __WRAPS_HPP__

#include <iostream>
#include <GL/glut.h>
#include "application.hpp"

Application *ptr_;

/////////////////////////////////////////////////////////////


void draw() {ptr_->draw();}
void resize(GLsizei w, GLsizei h) {ptr_->resize(w, h);}
void KeyboardHandle(unsigned char key, int x, int y){ptr_->KeyboardHandle(key, x, y);}
void MouseHandle(int button, int state, int x, int y){ptr_->MouseHandle(button, state, x, y);}
void SpecialKeyHandle(int key, int x, int y){ptr_->SpecialKeyHandle(key, x, y);}
void update(int value){ptr_->update(value,&update);}


#endif







