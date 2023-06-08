#pragma once
#include "Light.h"

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, //Son los datos de la superclase
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos, //Su posicion
		GLfloat con, GLfloat lin, GLfloat exp);//Coeficiones de la ecuacion de segundo grados 

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
		GLfloat diffuseIntensityLocation, GLfloat positionLocation,
		GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

	~PointLight();

protected: //Significa que son una super clase 
	glm::vec3 position; 

	GLfloat constant, linear, exponent;
};

