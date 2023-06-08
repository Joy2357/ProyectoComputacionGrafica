#define STB_IMAGE_IMPLEMENTATION
#pragma warning(disable : 4996)

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;

float movSun; 
float movySun;
float movSunOffset;
float luz;
bool dia;
float reproduciranimacion, habilitaranimacion;
float reproduciranimacion2, habilitaranimacion2;
float reproduciranimacion3, habilitaranimacion3;
float reproduciranimacion4, habilitaranimacion4;
float reproduciranimacion5, habilitaranimacion5;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture pisoTexture;

Model zoo;
Model prueba;

Model komodo;
Model lizard;
Model coco;
Model toad;
Model camaleon;
Model yellowBird; 
Model tucan;
Model pajaritoAzulAma;
Model littleBird;
Model blueBird;

Model elefante;
Model hipo;
Model leon;
Model cebra;
Model ciervo;
Model jaguar;
Model jirafa;
Model oso;
Model rino;
Model tigre;

//Animados
Model aguilaCuerpo;
Model aguilaADer;
Model aguilaAIzq;

Model snakeCuerpo;
Model snakeCabeza;
Model snakeCascabel;

Model leonC;
Model leonPDD;
Model leonPDI;
Model leonPTD;
Model leonPTI;

Model monocuerpo;
Model monocola;

Model caparazon;
Model pDD;
Model pDI;
Model pTD;
Model pTI;

Skybox skybox;

Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz 
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void inputKeyframes(bool* keys);

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-100.0f, 0.0f, -100.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		100.0f, 0.0f, -100.0f,	5.f, 0.0f,		0.0f, -1.0f, 0.0f,
		-100.0f, 0.0f, 100.0f,	0.0f, 5.0f,		0.0f, -1.0f, 0.0f,
		100.0f, 0.0f, 100.0f,		5.0f, 5.0f,		0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

///////////////////////////////KEYFRAMES/////////////////////

bool animacion = false; //animacion 1
bool animacion2 = false; //animacion 2
bool animacion3 = false; //animacion 3
bool animacion4 = false; //animacion 4
bool animacion5 = false; //animacion 5


//variables para la animacion 1
float posXaguila = 0.0f, posYaguila = 0.0f, posZaguila = 0.0f;
float movAguila_z = 0.0f;
float giroAguila = 0.0f;
float giroAlas = 0.0f;
//variables para la animacion 2
float girocabeza = 0.0f;
float movcascabel = 0.0f;
//variables para la animacion 3
float posXleon = 0.0f, posYleon = 0.0f, posZleon = 0.0f;
float movleonX = 0.0f;
float giroleon = 0.0f;
float movpatasleon = 0.0f;
//variables para la animacion 4
float posXmono = 0.0f, posYmono = 0.0f, posZmono = 0.0f;
float movmonoX = 0.0f, movmonoY = 0.0f, movmonoZ = 0.0f;
float giromono = 0.0f;
float girocola = 0.0f;
//variables para la animacion 5
float girotortuga = 0.0f;

//Definiciones para la animacion 1
#define MAX_FRAMES 12
int i_max_steps = 80;
int i_curr_steps = 11;
typedef struct _frame
{	
	float movAguila_z;		
	float movAguila_zInc;	
	float giroAguila;
	float giroAguilaInc;
	float giroAlas;
	float giroAlasInc;
	
}FRAME; 
FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 11;		
bool play = false;
int playIndex = 0;

//Definiciones para la animacion 2
#define MAX_FRAMES2 10 
int i_max_steps2 = 30;
int i_curr_steps2 = 9;
typedef struct _frame2
{
	float girocabeza;
	float girocabezaInc;
	float movcascabel;
	float movcascabelInc;
}FRAME2;
FRAME2 KeyFrame2[MAX_FRAMES2];
int FrameIndex2 = 9;
bool play2 = false;
int playIndex2 = 0;

//Definiciones para la animacion 3
#define MAX_FRAMES3 12 
int i_max_steps3 = 80;
int i_curr_steps3 = 11;
typedef struct _frame3
{
	float movleonX;
	float movleonXInc;
	float giroleon;
	float giroleonInc;
	float movpatasleon;
	float movpatasleonInc;
}FRAME3;
FRAME3 KeyFrame3[MAX_FRAMES3];
int FrameIndex3 = 11;
bool play3 = false;
int playIndex3 = 0;

//Definiciones para la animacion 4
#define MAX_FRAMES4 22 
int i_max_steps4 = 30;
int i_curr_steps4 = 21;
typedef struct _frame4
{
	float movmonoX;
	float movmonoXInc;
	float movmonoY;
	float movmonoYInc;
	float movmonoZ;
	float movmonoZInc;
	float giromono;
	float giromonoInc;
	float girocola;
	float girocolaInc;
}FRAME4;
FRAME4 KeyFrame4[MAX_FRAMES4];
int FrameIndex4 = 21;
bool play4 = false;
int playIndex4 = 0;

//Definiciones para la animacion 5
#define MAX_FRAMES5 10 
int i_max_steps5 = 40;
int i_curr_steps5 = 9;
typedef struct _frame5
{
	float girotortuga;
	float girotortugaInc;	
}FRAME5;
FRAME5 KeyFrame5[MAX_FRAMES5];
int FrameIndex5 = 9;
bool play5 = false;
int playIndex5 = 0;

//Funciones para animacion 1
void resetElements(void)
{
	movAguila_z = KeyFrame[0].movAguila_z;
	giroAguila = KeyFrame[0].giroAguila;
	giroAlas = KeyFrame[0].giroAlas;
}
void interpolation(void)
{
	KeyFrame[playIndex].movAguila_zInc = (KeyFrame[playIndex + 1].movAguila_z - KeyFrame[playIndex].movAguila_z) / i_max_steps;
	KeyFrame[playIndex].giroAguilaInc = (KeyFrame[playIndex + 1].giroAguila - KeyFrame[playIndex].giroAguila) / i_max_steps;
	KeyFrame[playIndex].giroAlasInc = (KeyFrame[playIndex + 1].giroAlas - KeyFrame[playIndex].giroAlas) / i_max_steps;
}
void animate(void)
{	
	if (play) {
		if (i_curr_steps >= i_max_steps) {
			playIndex++;			
			if (playIndex > FrameIndex - 2) {
				playIndex = 0;
				play = false;
			}
			else {
				i_curr_steps = 0; 
				interpolation();
			}
		}
		else {
			movAguila_z += KeyFrame[playIndex].movAguila_zInc;
			giroAguila += KeyFrame[playIndex].giroAguilaInc;
			giroAlas += KeyFrame[playIndex].giroAlasInc;
			i_curr_steps++;
		}
	}
}
//Funciones para animacion 2
void resetElements2(void)
{
	girocabeza = KeyFrame2[0].girocabeza;
	movcascabel = KeyFrame2[0].movcascabel;	
}
void interpolation2(void)
{
	KeyFrame2[playIndex2].girocabezaInc = (KeyFrame2[playIndex2 + 1].girocabeza - KeyFrame2[playIndex2].girocabeza) / i_max_steps2;
	KeyFrame2[playIndex2].movcascabelInc = (KeyFrame2[playIndex2 + 1].movcascabel - KeyFrame2[playIndex2].movcascabel) / i_max_steps2;	
}
void animate2(void)
{
	if (play2) {
		if (i_curr_steps2 >= i_max_steps2) {
			playIndex2++;
			if (playIndex2 > FrameIndex2 - 2) {
				playIndex2 = 0;
				play2 = false;
			}
			else {
				i_curr_steps2 = 0;
				interpolation2();
			}
		}
		else {
			girocabeza += KeyFrame2[playIndex2].girocabezaInc;
			movcascabel += KeyFrame2[playIndex2].movcascabelInc;			
			i_curr_steps2++;
		}
	}
}
//Funciones para animacion 3
void resetElements3(void)
{
	movleonX = KeyFrame3[0].movleonX;
	giroleon = KeyFrame3[0].giroleon;
	movpatasleon = KeyFrame3[0].movpatasleon;
}
void interpolation3(void)
{
	KeyFrame3[playIndex3].movleonXInc = (KeyFrame3[playIndex3 + 1].movleonX - KeyFrame3[playIndex3].movleonX) / i_max_steps3;
	KeyFrame3[playIndex3].giroleonInc = (KeyFrame3[playIndex3 + 1].giroleon - KeyFrame3[playIndex3].giroleon) / i_max_steps3;
	KeyFrame3[playIndex3].movpatasleonInc = (KeyFrame3[playIndex3 + 1].movpatasleon - KeyFrame3[playIndex3].movpatasleon) / i_max_steps3;
}
void animate3(void)
{
	if (play3) {
		if (i_curr_steps3 >= i_max_steps3) {
			playIndex3++;
			if (playIndex3 > FrameIndex3 - 2) {
				playIndex3 = 0;
				play3 = false;
			}
			else {
				i_curr_steps3 = 0;
				interpolation3();
			}
		}
		else {
			movleonX += KeyFrame3[playIndex3].movleonXInc;
			giroleon += KeyFrame3[playIndex3].giroleonInc;
			movpatasleon += KeyFrame3[playIndex3].movpatasleonInc;
			i_curr_steps3++;
		}
	}
}
//Funciones para animacion 4
void resetElements4(void)
{
	movmonoX = KeyFrame4[0].movmonoX;
	movmonoY = KeyFrame4[0].movmonoY;
	movmonoZ = KeyFrame4[0].movmonoZ;
	giromono = KeyFrame4[0].giromono;
	girocola = KeyFrame4[0].girocola;
}
void interpolation4(void)
{
	KeyFrame4[playIndex4].movmonoXInc = (KeyFrame4[playIndex4 + 1].movmonoX - KeyFrame4[playIndex4].movmonoX) / i_max_steps4;
	KeyFrame4[playIndex4].movmonoYInc = (KeyFrame4[playIndex4 + 1].movmonoY - KeyFrame4[playIndex4].movmonoY) / i_max_steps4;
	KeyFrame4[playIndex4].movmonoZInc = (KeyFrame4[playIndex4 + 1].movmonoZ - KeyFrame4[playIndex4].movmonoZ) / i_max_steps4;
	KeyFrame4[playIndex4].giromonoInc = (KeyFrame4[playIndex4 + 1].giromono - KeyFrame4[playIndex4].giromono) / i_max_steps4;
	KeyFrame4[playIndex4].girocolaInc = (KeyFrame4[playIndex4 + 1].girocola - KeyFrame4[playIndex4].girocola) / i_max_steps4;
}
void animate4(void)
{
	if (play4) {
		if (i_curr_steps4 >= i_max_steps4) {
			playIndex4++;
			if (playIndex4 > FrameIndex4 - 2) {
				playIndex4 = 0;
				play4 = false;
			}
			else {
				i_curr_steps4 = 0;
				interpolation4();
			}
		}
		else {
			movmonoX += KeyFrame4[playIndex4].movmonoXInc;
			movmonoY += KeyFrame4[playIndex4].movmonoYInc;
			movmonoZ += KeyFrame4[playIndex4].movmonoZInc;
			giromono += KeyFrame4[playIndex4].giromonoInc;
			girocola += KeyFrame4[playIndex4].girocolaInc;
			i_curr_steps4++;
		}
	}
}
//Funciones para animacion 5
void resetElements5(void)
{
	girotortuga = KeyFrame5[0].girotortuga;	
}
void interpolation5(void)
{
	KeyFrame5[playIndex5].girotortugaInc = (KeyFrame5[playIndex5 + 1].girotortuga - KeyFrame5[playIndex5].girotortuga) / i_max_steps5;	
}
void animate5(void)
{
	if (play5) {
		if (i_curr_steps5 >= i_max_steps5) {
			playIndex5++;
			if (playIndex5 > FrameIndex5 - 2) {
				playIndex5 = 0;
				play5 = false;
			}
			else {
				i_curr_steps5 = 0;
				interpolation5();
			}
		}
		else {
			girotortuga += KeyFrame5[playIndex5].girotortugaInc;			
			i_curr_steps5++;
		}
	}
}
/* FIN KEYFRAMES*/

int main()
{
	mainWindow = Window(1920, 1080); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();	
	CreateShaders();
	
	movySun = -1.0f;
	movSun = -1.0f;
	movSunOffset =0.001f;
	luz = 1.0f;	
	dia = true;

	//camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 1.0f);
	camera = Camera(glm::vec3(2.0f, 6.0f, 55.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.5f, 0.5f);

	pisoTexture = Texture("Textures/Skybox/pisotextura.png");
	pisoTexture.LoadTextureA();
			
	zoo = Model();
	zoo.LoadModel("Models/zoofinalfinal.obj");
	
	//prueba = Model();
	//prueba.LoadModel("Models/animales.obj");	
	
	komodo = Model();
	komodo.LoadModel("Models/komodo/komodo.obj");
	lizard = Model();
	lizard.LoadModel("Models/lizard/lizard.obj");
	coco = Model();
	coco.LoadModel("Models/coco/coco.obj");
	toad = Model();
	toad.LoadModel("Models/toad/toad.obj"); 
	camaleon = Model(); 
	camaleon.LoadModel("Models/camaleon/camaleon.obj"); 	
	
	yellowBird = Model(); 
	yellowBird.LoadModel("Models/yellowBird/yellowBird.obj"); 
	tucan = Model(); 
	tucan.LoadModel("Models/tucan/tucan.obj"); 
	pajaritoAzulAma = Model(); 
	pajaritoAzulAma.LoadModel("Models/pajaritoAzulAma/pajaritoAzulAma.obj"); 
	littleBird = Model(); 
	littleBird.LoadModel("Models/littleBird/littleBird.obj"); 
	blueBird = Model(); 
	blueBird.LoadModel("Models/blueBird/blueBird.obj");
		
	elefante = Model();
	elefante.LoadModel("Models/elefante/elefante.obj"); 
	hipo = Model();
	hipo.LoadModel("Models/hipopotamo/hipopotamo.obj");
	leon = Model();
	leon.LoadModel("Models/leon/leon.obj");
	cebra = Model();
	cebra.LoadModel("Models/cebra/cebra.obj");
	ciervo = Model();
	ciervo.LoadModel("Models/ciervo/ciervo.obj");
	
	jaguar = Model();
	jaguar.LoadModel("Models/jaguar/jaguar.obj");
	jirafa = Model();
	jirafa.LoadModel("Models/jirafa/jirafa.obj");
	oso = Model();
	oso.LoadModel("Models/oso/oso.obj");
	rino = Model();
	rino.LoadModel("Models/rinoceronte/rinoceronte.obj");
	tigre = Model();
	tigre.LoadModel("Models/tigre/tigre.obj");	
	
	//animados y gerarquizados
	aguilaCuerpo = Model();
	aguilaCuerpo.LoadModel("Models/animados/aguila/aguilaC.obj");
	aguilaADer = Model();
	aguilaADer.LoadModel("Models/animados/aguila/aguilaAD.obj");
	aguilaAIzq = Model();
	aguilaAIzq.LoadModel("Models/animados/aguila/aguilaAI.obj");

	snakeCabeza = Model();
	snakeCabeza.LoadModel("Models/animados/snake/snakecabeza.obj");
	snakeCuerpo = Model();
	snakeCuerpo.LoadModel("Models/animados/snake/snakecuerpo.obj");
	snakeCascabel = Model();
	snakeCascabel.LoadModel("Models/animados/snake/snakecascabel.obj");

	leonC = Model();
	leonC.LoadModel("Models/animados/leon/leoncuerpo.obj");
	leonPDD = Model();
	leonPDD.LoadModel("Models/animados/leon/leonPDD.obj");
	leonPDI = Model();
	leonPDI.LoadModel("Models/animados/leon/leonPDI.obj");
	leonPTD = Model();
	leonPTD.LoadModel("Models/animados/leon/leonPTD.obj");
	leonPTI = Model();
	leonPTI.LoadModel("Models/animados/leon/leonPTI.obj");

	monocuerpo = Model();
	monocuerpo.LoadModel("Models/animados/mono/cuerpomono.obj");
	monocola = Model();
	monocola.LoadModel("Models/animados/mono/colamono.obj");		

	caparazon = Model();
	caparazon.LoadModel("Models/animados/tortuga/caparazon.obj");
	pDD = Model();
	pDD.LoadModel("Models/animados/tortuga/TorPDD.obj");
	pDI = Model();
	pDI.LoadModel("Models/animados/tortuga/TorPDI.obj");
	pTD = Model();
	pTD.LoadModel("Models/animados/tortuga/TorPTD.obj");
	pTI = Model();
	pTI.LoadModel("Models/animados/tortuga/TorPTI.obj");

	std::vector<std::string> skyboxFaces; 
	skyboxFaces.push_back("Textures/Skybox/lados.jpg"); 
	skyboxFaces.push_back("Textures/Skybox/lados.jpg"); 
	skyboxFaces.push_back("Textures/Skybox/piso.jpg"); 
	skyboxFaces.push_back("Textures/Skybox/cielo.jpg"); 
	skyboxFaces.push_back("Textures/Skybox/lados.jpg"); 
	skyboxFaces.push_back("Textures/Skybox/lados.jpg"); 
		
	skybox = Skybox(skyboxFaces); 
		
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, -1.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;	
	//lampara
	spotLights[0] = SpotLight(0.4f, 0.4f, 0.4f,
		0.1f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	//luz faro 1
	pointLights[0] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		30.2f, 17.0f, -23.0f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;		
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		30.2f, 17.0f, -23.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		60.0f);	
	spotLightCount++;
	//luz faro 2	
	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		-26.5f, 17.0f, -23.0f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;	
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		-26.5f, 17.0f, -23.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		60.0f);
	spotLightCount++;
	//luz faro 3
	pointLights[2] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		33.7f, 17.0f, 64.8f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;	
	spotLights[3] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		33.7f, 17.0f, 64.8f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		60.0f);
	spotLightCount++;
	//luz faro 4
	pointLights[3] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		-33.5f, 17.0f, 64.8f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;	
	spotLights[4] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		-33.5f, 17.0f, 64.8f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		60.0f);
	spotLightCount++;
	//luz faro 5
	pointLights[4] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		-56.2f, 17.0f, -37.5f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;
	spotLights[5] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		-56.2f, 17.0f, -37.5f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		60.0f);
	spotLightCount++;
	//INTERRUPTOR
	//luz faro 6
	pointLights[5] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		-54.6f, 14.5f, -21.0f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;
	spotLights[6] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		-54.6f, 14.5f, -21.0f,
		-1.0f, 0.5f, 1.5f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;	
	//luces faro 7
	pointLights[6] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		-53.5f, 14.5f, 71.0f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;		
	spotLights[7] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		-53.5f, 14.5f, 71.0f,
		-1.0f, 0.5f, -1.5f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;
	//luces faro 8
	pointLights[7] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		3.2f, 14.5f, -75.9f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;
	spotLights[8] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		3.2f, 14.5f, -75.9f,
		1.5f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;
	//luces faro 9
	pointLights[8] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		-3.2f, 14.5f, -75.9f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;
	spotLights[9] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		-3.2f, 14.5f, -75.9f,
		-1.5f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;
	//luces faro 10
	pointLights[9] = PointLight(1.0f, 1.0f, 0.0f,
		1.0f, 1.0f,
		51.5f, 14.5f, -48.8f,
		0.1f, 0.2f, 0.3f);
	pointLightCount++;
	spotLights[10] = SpotLight(1.0f, 1.0f, 0.5f,
		0.1f, 2.0f,
		51.5f, 14.5f, -48.8f,
		1.0f, -1.0f, 2.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;	

	pointLightCount = 0;
	spotLightCount = 0;

	//Keyframes para la animacion 1
	glm::vec3 posaguila = glm::vec3(0.0f, 0.0f, 0.0f);	
	KeyFrame[0].movAguila_z = 0.0f;
	KeyFrame[0].giroAguila = 0.0f;
	KeyFrame[0].giroAlas = 0.0f;

	KeyFrame[1].movAguila_z = 20.0f;
	KeyFrame[1].giroAguila = 0.0f;
	KeyFrame[1].giroAlas = 60.0f;

	KeyFrame[2].movAguila_z = 40.0f;
	KeyFrame[2].giroAguila = 0.0f;
	KeyFrame[2].giroAlas = 0.0f;

	KeyFrame[3].movAguila_z = 60.0f;
	KeyFrame[3].giroAguila = 0.0f;
	KeyFrame[3].giroAlas = 60.0f;

	KeyFrame[4].movAguila_z = 80.0f;
	KeyFrame[4].giroAguila = 0.0f;
	KeyFrame[4].giroAlas = 0.0f;

	KeyFrame[5].movAguila_z = 80.0f;
	KeyFrame[5].giroAguila = 180.0f;
	KeyFrame[5].giroAlas = 60.0f;

	KeyFrame[6].movAguila_z = 60.0f;
	KeyFrame[6].giroAguila = 180.0f;
	KeyFrame[6].giroAlas = 0.0f;

	KeyFrame[7].movAguila_z = 40.0f;
	KeyFrame[7].giroAguila = 180.0f;
	KeyFrame[7].giroAlas = 60.0f;

	KeyFrame[8].movAguila_z = 20.0f;
	KeyFrame[8].giroAguila = 180.0f;
	KeyFrame[8].giroAlas = 0.0f;

	KeyFrame[9].movAguila_z = 0.0f;
	KeyFrame[9].giroAguila = 180.0f;
	KeyFrame[9].giroAlas = 60.0f;

	KeyFrame[10].movAguila_z = 0.0f;
	KeyFrame[10].giroAguila = 360.0f;
	KeyFrame[10].giroAlas = 60.0f;

	//Keyframes para la animacion 2
	KeyFrame2[0].girocabeza = 0.0f;
	KeyFrame2[0].movcascabel = 2.0f;

	KeyFrame2[1].girocabeza = 5.0f;
	KeyFrame2[1].movcascabel = -2.0f;

	KeyFrame2[2].girocabeza = 10.0f;
	KeyFrame2[2].movcascabel = 2.0f;

	KeyFrame2[3].girocabeza = 15.0f;
	KeyFrame2[3].movcascabel = -2.0f;

	KeyFrame2[4].girocabeza = 20.0f;
	KeyFrame2[4].movcascabel = 2.0f;

	KeyFrame2[5].girocabeza = 15.0f;
	KeyFrame2[5].movcascabel = -2.0f;

	KeyFrame2[6].girocabeza = 10.0f;
	KeyFrame2[6].movcascabel = 2.0f;

	KeyFrame2[7].girocabeza = 5.0f;
	KeyFrame2[7].movcascabel = -2.0f;

	KeyFrame2[8].girocabeza = 0.0f;
	KeyFrame2[8].movcascabel = 0.0f;

	//Keyframes para la animacion 3
	glm::vec3 posleon = glm::vec3(0.0f, 0.0f, 0.0f);	
	KeyFrame3[0].movleonX = 0.0f;
	KeyFrame3[0].giroleon = 0.0f;
	KeyFrame3[0].movpatasleon = 30.0f;
	
	KeyFrame3[1].movleonX = -10.0f;
	KeyFrame3[1].giroleon = 0.0f;
	KeyFrame3[1].movpatasleon = -30.0f;

	KeyFrame3[2].movleonX = -20.0f;
	KeyFrame3[2].giroleon = 0.0f;
	KeyFrame3[2].movpatasleon = 30.0f;

	KeyFrame3[3].movleonX = -30.0f;
	KeyFrame3[3].giroleon = 0.0f;
	KeyFrame3[3].movpatasleon = -30.0f;

	KeyFrame3[4].movleonX = -40.0f;
	KeyFrame3[4].giroleon = 0.0f;
	KeyFrame3[4].movpatasleon = 30.0f;

	KeyFrame3[5].movleonX = -40.0f;
	KeyFrame3[5].giroleon = 180.0f;
	KeyFrame3[5].movpatasleon = -30.0f;

	KeyFrame3[6].movleonX = -30.0f;
	KeyFrame3[6].giroleon = 180.0f;
	KeyFrame3[6].movpatasleon = 30.0f;

	KeyFrame3[7].movleonX = -20.0f;
	KeyFrame3[7].giroleon = 180.0f;
	KeyFrame3[7].movpatasleon = -30.0f;

	KeyFrame3[8].movleonX = -10.0f;
	KeyFrame3[8].giroleon = 180.0f;
	KeyFrame3[8].movpatasleon = 30.0f;

	KeyFrame3[9].movleonX = 0.0f;
	KeyFrame3[9].giroleon = 180.0f;
	KeyFrame3[9].movpatasleon = -30.0f;

	KeyFrame3[10].movleonX = 0.0f;
	KeyFrame3[10].giroleon = 360.0f;
	KeyFrame3[10].movpatasleon = 0.0f;

	//Keyframes para la animacion 4
	glm::vec3 posmono = glm::vec3(0.0f, 0.0f, 0.0f);
	KeyFrame4[0].movmonoX = -2.0f;
	KeyFrame4[0].movmonoY = 1.0f;
	KeyFrame4[0].movmonoZ = 0.0f;
	KeyFrame4[0].giromono = 0.0f;
	KeyFrame4[0].girocola = 10.0f;

	KeyFrame4[1].movmonoX = -4.0f;
	KeyFrame4[1].movmonoY = -1.0f;
	KeyFrame4[1].movmonoZ = 0.0f;
	KeyFrame4[1].giromono = 0.0f;
	KeyFrame4[1].girocola = -10.0f;

	KeyFrame4[2].movmonoX = -6.0f;
	KeyFrame4[2].movmonoY = 1.0f;
	KeyFrame4[2].movmonoZ = 0.0f;
	KeyFrame4[2].giromono = 0.0f;
	KeyFrame4[2].girocola = 10.0f;

	KeyFrame4[3].movmonoX = -8.0f;
	KeyFrame4[3].movmonoY = -1.0f;
	KeyFrame4[3].movmonoZ = 0.0f;
	KeyFrame4[3].giromono = 0.0f;
	KeyFrame4[3].girocola = -10.0f;

	KeyFrame4[4].movmonoX = -10.0f;
	KeyFrame4[4].movmonoY = 1.0f;
	KeyFrame4[4].movmonoZ = 0.0f;
	KeyFrame4[4].giromono = 0.0f;
	KeyFrame4[4].girocola = 10.0f;

	KeyFrame4[5].movmonoX = -10.0f;
	KeyFrame4[5].movmonoY = -1.0f;
	KeyFrame4[5].movmonoZ = -2.0f;
	KeyFrame4[5].giromono = -90.0f;
	KeyFrame4[5].girocola = -10.0f;

	KeyFrame4[6].movmonoX = -10.0f;
	KeyFrame4[6].movmonoY = 1.0f;
	KeyFrame4[6].movmonoZ = -4.0f;
	KeyFrame4[6].giromono = -90.0f;
	KeyFrame4[6].girocola = 10.0f;

	KeyFrame4[7].movmonoX = -10.0f;
	KeyFrame4[7].movmonoY = -1.0f;
	KeyFrame4[7].movmonoZ = -6.0f;
	KeyFrame4[7].giromono = -90.0f;
	KeyFrame4[7].girocola = -10.0f;

	KeyFrame4[8].movmonoX = -10.0f;
	KeyFrame4[8].movmonoY = 1.0f;
	KeyFrame4[8].movmonoZ = -8.0f;
	KeyFrame4[8].giromono = -90.0f;
	KeyFrame4[8].girocola = 10.0f;

	KeyFrame4[9].movmonoX = -10.0f;
	KeyFrame4[9].movmonoY = -1.0f;
	KeyFrame4[9].movmonoZ = -10.0f;
	KeyFrame4[9].giromono = -90.0f;
	KeyFrame4[9].girocola = -10.0f;

	KeyFrame4[10].movmonoX = -8.0f;
	KeyFrame4[10].movmonoY = 1.0f;
	KeyFrame4[10].movmonoZ = -10.0f;
	KeyFrame4[10].giromono = -180.0f;
	KeyFrame4[10].girocola = 10.0f;

	KeyFrame4[11].movmonoX = -6.0f;
	KeyFrame4[11].movmonoY = -1.0f;
	KeyFrame4[11].movmonoZ = -10.0f;
	KeyFrame4[11].giromono = -180.0f;
	KeyFrame4[11].girocola = -10.0f;

	KeyFrame4[12].movmonoX = -4.0f;
	KeyFrame4[12].movmonoY = 1.0f;
	KeyFrame4[12].movmonoZ = -10.0f;
	KeyFrame4[12].giromono = -180.0f;
	KeyFrame4[12].girocola = 10.0f;

	KeyFrame4[13].movmonoX = -2.0f;
	KeyFrame4[13].movmonoY = -1.0f;
	KeyFrame4[13].movmonoZ = -10.0f;
	KeyFrame4[13].giromono = -180.0f;
	KeyFrame4[13].girocola = -10.0f;

	KeyFrame4[14].movmonoX = 0.0f;
	KeyFrame4[14].movmonoY = 1.0f;
	KeyFrame4[14].movmonoZ = -10.0f;
	KeyFrame4[14].giromono = -180.0f;
	KeyFrame4[14].girocola = 10.0f;

	KeyFrame4[15].movmonoX = 0.0f;
	KeyFrame4[15].movmonoY = -1.0f;
	KeyFrame4[15].movmonoZ = -8.0f;
	KeyFrame4[15].giromono = -270.0f;
	KeyFrame4[15].girocola = -10.0f;

	KeyFrame4[16].movmonoX = 0.0f;
	KeyFrame4[16].movmonoY = 1.0f;
	KeyFrame4[16].movmonoZ = -6.0f;
	KeyFrame4[16].giromono = -270.0f;
	KeyFrame4[16].girocola = 10.0f;

	KeyFrame4[17].movmonoX = 0.0f;
	KeyFrame4[17].movmonoY = -1.0f;
	KeyFrame4[17].movmonoZ = -4.0f;
	KeyFrame4[17].giromono = -270.0f;
	KeyFrame4[17].girocola = -10.0f;

	KeyFrame4[18].movmonoX = 0.0f;
	KeyFrame4[18].movmonoY = 1.0f;
	KeyFrame4[18].movmonoZ = -2.0f;
	KeyFrame4[18].giromono = -270.0f;
	KeyFrame4[18].girocola = 10.0f;

	KeyFrame4[19].movmonoX = 0.0f;
	KeyFrame4[19].movmonoY = -1.0f;
	KeyFrame4[19].movmonoZ = 0.0f;
	KeyFrame4[19].giromono = -270.0f;
	KeyFrame4[19].girocola = -10.0f;

	KeyFrame4[20].movmonoX = 0.0f;
	KeyFrame4[20].movmonoY = 0.0f;
	KeyFrame4[20].movmonoZ = 0.0f;
	KeyFrame4[20].giromono = -360.0f;
	KeyFrame4[20].girocola = 0.0f;

	//Keyframes para la animacion 5
	KeyFrame5[0].girotortuga = 20.0f;	

	KeyFrame5[1].girotortuga = -20.0f;	

	KeyFrame5[2].girotortuga = 20.0f;	

	KeyFrame5[3].girotortuga = -20.0f;	

	KeyFrame5[4].girotortuga = 20.0f;	

	KeyFrame5[5].girotortuga = -20.0f;	

	KeyFrame5[6].girotortuga = 20.0f;	

	KeyFrame5[7].girotortuga = -20.0f;	

	KeyFrame5[8].girotortuga = 0.0f;	

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		if (dia==true)
		{
			if (movSun < 1.0f)
			{
				movSun += movSunOffset;
				pointLightCount = 0;
				spotLightCount = 0;
			}
			else
			{
				dia = false;
				movySun = 1.0f;
				luz = 0.3f;										
			}			
		}
		else
		{
			if (movSun > -1.0f)
			{
				movSun -= movSunOffset;
				if (mainWindow.getinterruptor() == 1)
				{
					pointLightCount = 10;
					spotLightCount = 11;
				}
				else if (mainWindow.getinterruptor() == 0)
				{
					pointLightCount = 5;
					spotLightCount = 6;					
				}
			}
			else
			{				
				dia = true;
				movySun = -1.0f;
				luz = 1.0f;							
			}				
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		if(mainWindow.getcam()==1)			
			camera.mouseControl(mainWindow.getXChange(), NULL);
		else if(mainWindow.getcam()==0)			
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		inputKeyframes(mainWindow.getsKeys());
		animate();
		animate2();
		animate3();
		animate4();
		animate5();
		
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		glm::vec3 dircam = camera.getCameraDirection();
		//lowerLight.y -= 0.3f;
		mainLight = DirectionalLight(luz, luz, luz,
			0.5f, 0.3f,
			0.0f, movySun, movSun);
				
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
						
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		
		//MODELOS

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::mat4 modelaux3(1.0);
		glm::mat4 modelaux4(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f); 				
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();			
						
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//prueba.RenderModel();	

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		zoo.RenderModel();			

		//AGUILA
		model = glm::mat4(1.0);
		posaguila = glm::vec3(posXaguila, posYaguila, posZaguila + movAguila_z); 
		model = glm::translate(model, glm::vec3(-57.0f, 22.0f, -12.0f));
		model = glm::translate(model, posaguila); 
		model = glm::rotate(model, glm::radians(giroAguila), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		modelaux = model;
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess); 
		aguilaCuerpo.RenderModel();
		//Ala
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroAlas), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;				
		model = modelaux;				
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		aguilaADer.RenderModel();
		//Ala
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroAlas), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux2 = model;
		model = modelaux2;		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		aguilaAIzq.RenderModel();

		//SNAKE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(56.0f, 2.2f, 44.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f));
		modelaux = model;
		modelaux2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		snakeCuerpo.RenderModel();
		//cabeza
		model = modelaux;
		model = glm::rotate(model, glm::radians(girocabeza), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		snakeCabeza.RenderModel();
		//cascabel
		model = modelaux2;
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movcascabel), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		model = modelaux2;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		snakeCascabel.RenderModel();

		//LEON
		model = glm::mat4(1.0); 
		posleon = glm::vec3(posXleon + movleonX, posYleon, posZleon);
		model = glm::translate(model, glm::vec3(58.0f, 2.35f, -55.0f));
		model = glm::translate(model, posleon);
		model = glm::rotate(model, glm::radians(giroleon), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		modelaux = model; 
		modelaux2 = model; 
		modelaux3 = model;
		modelaux4 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		leonC.RenderModel();
		//PDD
		model = modelaux;		
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(movpatasleon), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux = model;
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		leonPDD.RenderModel();
		//PDI
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(movpatasleon), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux2 = model;
		model = modelaux2;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		leonPDI.RenderModel();
		//PTD
		model = modelaux3;
		model = glm::translate(model, glm::vec3(5.0f, 0.8f, -0.6f));
		model = glm::rotate(model, glm::radians(movpatasleon), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux3 = model;
		model = modelaux3;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		leonPTD.RenderModel();
		//PTI
		model = modelaux4;
		model = glm::translate(model, glm::vec3(5.0f, 0.8f, 0.6f));
		model = glm::rotate(model, glm::radians(movpatasleon), glm::vec3(0.0f, 0.0f, -1.0f));
		modelaux4 = model;
		model = modelaux4;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		leonPTI.RenderModel();

		//MONO
		model = glm::mat4(1.0);
		posmono = glm::vec3(posXmono + movmonoX, posYmono + movmonoY, posZmono + movmonoZ);
		model = glm::translate(model, glm::vec3(-10.0f, 15.0f, -66.0f));				
		model = glm::translate(model, posmono);		
		model = glm::rotate(model, glm::radians(giromono), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));		
		model = glm::scale(model, glm::vec3(1.0f));
		modelaux = model;		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		monocuerpo.RenderModel();
		//cola
		model = modelaux;
		model = glm::rotate(model, glm::radians(girocola*3), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.49f));
		modelaux = model;
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		monocola.RenderModel();
		
		//TORTUGA
		model = glm::mat4(1.0);		
		model = glm::translate(model, glm::vec3(54.0f, 2.0f, -13.0f));		
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(girotortuga), glm::vec3(0.0f, 0.0f, 1.0f));		
		model = glm::scale(model, glm::vec3(0.3f));
		modelaux = model;
		modelaux2 = model;
		modelaux3 = model;
		modelaux4 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caparazon.RenderModel();
		//PDD
		model = modelaux;		
		model = glm::rotate(model, glm::radians(girotortuga*2), glm::vec3(-1.0f, 0.0f, 0.0f));
		modelaux = model;
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pDD.RenderModel();
		//PDI
		model = modelaux2;		
		model = glm::rotate(model, glm::radians(girotortuga*2), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux2 = model;
		model = modelaux2;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pDI.RenderModel();
		//PTD
		model = modelaux3;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.5f));
		model = glm::rotate(model, glm::radians(girotortuga*2), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux3 = model;
		model = modelaux3;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pTD.RenderModel();
		//PTI
		model = modelaux4;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.5f));
		model = glm::rotate(model, glm::radians(girotortuga*2), glm::vec3(-1.0f, 0.0f, 0.0f));
		modelaux4 = model;
		model = modelaux4;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pTI.RenderModel();

		
		//habitad 1 reptiles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, 0.8f, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		komodo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(61.0f, 4.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lizard.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, 1.0f, 28.0f));
		model = glm::scale(model, glm::vec3(1.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		coco.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(68.0f, 5.0f, -25.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		toad.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 0.2f, -40.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		camaleon.RenderModel();
		
		//habitad 2 aviario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-61.0f, 20.0f, 64.4f));
		model = glm::scale(model, glm::vec3(0.28f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		yellowBird.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-59.5f, 16.0f, 50.5f));
		model = glm::scale(model, glm::vec3(0.3f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tucan.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-61.0f, 20.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pajaritoAzulAma.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-59.0f, 15.5f, 6.5f));
		model = glm::scale(model, glm::vec3(0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		littleBird.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-61.5f, 20.0f, -15.5f));
		model = glm::scale(model, glm::vec3(0.6f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		blueBird.RenderModel();

		//habitad 3 mamiferos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-92.0f, 0.7f, -76.0f));
		model = glm::scale(model, glm::vec3(3.2f));		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		elefante.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-63.0f, 0.7f, -60.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		hipo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 1.1f, -60.0f));
		model = glm::scale(model, glm::vec3(2.2f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cebra.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 1.1f, -90.0f));
		model = glm::scale(model, glm::vec3(2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ciervo.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 1.0f, -30.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		jirafa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, 1.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.04f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		oso.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.0f, 1.0f, -60.0f));
		model = glm::scale(model, glm::vec3(2.4f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		rino.RenderModel();

		//habitad 4 felinos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.9f, -70.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		leon.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 1.2f, -75.0f));
		model = glm::scale(model, glm::vec3(1.4f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		jaguar.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 1.1f, -66.0f));
		model = glm::scale(model, glm::vec3(1.6f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		tigre.RenderModel();	
		

		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
		
	}
	return 0;
}

void inputKeyframes(bool* keys)
{
	//Para la animacion 1
	if (keys[GLFW_KEY_1] || keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;				
				habilitaranimacion = 0;
			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}
	
	//Para la animacion 2
	if (keys[GLFW_KEY_2] || keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion2 < 1)
		{
			if (play2 == false && (FrameIndex2 > 1))
			{
				resetElements2();				
				interpolation2();
				play2 = true;
				playIndex2 = 0;
				i_curr_steps2 = 0;
				reproduciranimacion2++;				
				habilitaranimacion2 = 0;
			}
			else
			{
				play2 = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion2 < 1)
		{
			reproduciranimacion2 = 0;
		}
	}	

	//Para la animacion 3
	if (keys[GLFW_KEY_3] || keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion3 < 1)
		{
			if (play3 == false && (FrameIndex3 > 1))
			{
				resetElements3();
				interpolation3();
				play3 = true;
				playIndex3 = 0;
				i_curr_steps3 = 0;
				reproduciranimacion3++;
				habilitaranimacion3 = 0;
			}
			else
			{
				play3 = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion3 < 1)
		{
			reproduciranimacion3 = 0;
		}
	}

	//Para la animacion 4
	if (keys[GLFW_KEY_4] || keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion4 < 1)
		{
			if (play4 == false && (FrameIndex4 > 1))
			{
				resetElements4();
				interpolation4();
				play4 = true;
				playIndex4 = 0;
				i_curr_steps4 = 0;
				reproduciranimacion4++;
				habilitaranimacion4 = 0;
			}
			else
			{
				play4 = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion4 < 1)
		{
			reproduciranimacion4 = 0;
		}
	}

	//Para la animacion 5
	if (keys[GLFW_KEY_5] || keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion5 < 1)
		{
			if (play5 == false && (FrameIndex5 > 1))
			{
				resetElements5();
				interpolation5();
				play5 = true;
				playIndex5 = 0;
				i_curr_steps5 = 0;
				reproduciranimacion5++;
				habilitaranimacion5 = 0;
			}
			else
			{
				play5 = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion5 < 1)
		{
			reproduciranimacion5 = 0;
		}
	}
}