// Elo Maluco
// Objects.hpp
// Prof. Giovani Bernardes Vitor
// ECOI24 - 2024


#ifndef __OBJECTS_HPP__
#define __OBJECTS_HPP__

#include <iostream>
#include <GL/glut.h>

//#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>



/////////////////////////////////////////////////////////////
class Objects
{
public:	
	virtual void draw() = 0;
	virtual void update(int value) = 0;
	virtual void colision(int x, int y) = 0;

};


#endif
