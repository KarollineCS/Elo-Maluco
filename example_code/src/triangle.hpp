// Augmented reality
// triangle.cpp
// Prof. Giovani Bernardes Vitor
// ECOI24 - 2024


#include "Objects.hpp"

#ifndef __TRIANGLE_HPP__
#define __TRIANGLE_HPP__

/////////////////////////////////////////////////////////////
class Triangle: public Objects
{
public:	
	Triangle();
	~Triangle();

	void draw();
	void update(int value);
	void colision(int x, int y);

private:

	int x,y;
	float theta;

	glm::vec3 points_base[3];
	glm::vec3 points[3];


private:

	void transform(void);
};

#endif
