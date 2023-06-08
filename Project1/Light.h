#pragma once

#include <glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue,  //Componenete RGB
			GLfloat aIntensity, GLfloat dIntensity); //Componente para la intensidad 

	~Light();

protected:  //Super clases 
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity; //Color del que vemos al objeto 

};

