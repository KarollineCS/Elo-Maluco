// Elo Maluco
// main.cpp
// Prof. Giovani Bernardes Vitor
// ECOI24 - 2024

#include <stdlib.h>
#include <stdio.h>
#include "triangle.hpp"



 



Triangle::Triangle()
{
	x = y = 0;
	theta = 0.0f;


	points_base[0] = glm::vec3( -15.0f,   0.0f , 1.0f);
        points_base[1] = glm::vec3( 15.0f,  0.0f , 1.0f);
        points_base[2] = glm::vec3( 0.0f,  50.0f , 1.0f) ;      

	points[0] = points_base[0];
	points[1] = points_base[1];
	points[2] = points_base[2];
//	std::cout << "inicio: " << glm::to_string(points[0]);
	//points = *point_;

}
//---------------------------------------------------------------------
Triangle::~Triangle()
{

}

//---------------------------------------------------------------------
void Triangle::update(int value)
{
	x += value;
	y += value;

	transform();
}

//---------------------------------------------------------------------
void Triangle::colision(int x, int y)
{

	

}

//---------------------------------------------------------------------
void Triangle::transform(void)
{

	//glm::vec4 Position = glm::vec4(glm::vec3(0.0), 1.0);
        glm::mat3 Model = glm::mat3(1.0);
        Model[2] = glm::vec3(x, y, 1.0);
	

	//std::cout << "tes: " << glm::to_string(Model[0]) << std::endl;
	//std::cout << "tes: " << glm::to_string(Model[1])<< std::endl;
	//std::cout << "tes: " << glm::to_string(Model[2])<< "\n" << std::endl;
	
        //glm::vec4 Transformed = Model * Position;


	for(int i=0; i < 3; i++)
	{
		points[i] = Model * points_base[i];

	}

}



//---------------------------------------------------------------------
void Triangle::draw()
{

	

	glLineWidth(3.0f);
	//glColor3f(0.5f, 0.6f, 0.4f);

	//transform();
	//std::cout << "tes: " << glm::to_string(points[0]);
        glBegin(GL_TRIANGLES);  
		glVertex2f(points[0].x,points[0].y);
	        glVertex2f(points[1].x,points[1].y);
		glVertex2f(points[2].x,points[2].y);
        glEnd();


}
