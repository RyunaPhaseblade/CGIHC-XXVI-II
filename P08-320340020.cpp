/*
Práctica 7: Iluminación 1
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

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
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;
Texture peceraT;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Camaro;
Model CamaroC;
Model Dado_a;
Model Farola;
Model PezC;
Model PezF;
Model PezA;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
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
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
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

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
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
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.51f,  0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.74f,	0.34f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.74f,	0.65f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.51f,	0.65f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.99f,  0.34f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.75f,	0.34f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.75f,	0.66f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.99f,	0.66f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.01f,  0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.24f,	0.34f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.24f,	0.66f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.01f,	0.66f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.51f,  0.01f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.51f,	0.33f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.74f,	0.33f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.74f,	0.01f,		0.0f,	1.0f,	0.0f,
		//0.51   0.34
		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.51f,  0.99f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.51f,	0.67f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.74f,	0.67f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.74,	0.99f,		0.0f,	-1.0f,	0.0f,
		 //0.51f, 0.99f
	};


	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void crearPecera()
{
	unsigned int pecera_indices[] = {
		
		0, 1, 2, 2, 3, 0, 
		8, 9, 10, 10, 11, 8, 
		12, 13, 14, 14, 15, 12, 
		16, 17, 18, 18, 19, 16, 
		4, 5, 6, 6, 7, 4, 
		20, 21, 22, 22, 23, 20, 

		24, 25, 26, 26, 27, 24, 
		28, 29, 30, 30, 31, 28, 
		32, 33, 34, 34, 35, 32, 
		36, 37, 38, 38, 39, 36, 

		40, 41, 42, 42, 43, 40, 
		44, 45, 46, 46, 47, 44, 
		48, 49, 50, 50, 51, 48, 
		52, 53, 54, 54, 55, 52  
	};

	GLfloat pecera_vertices[] = {
		-0.5f, -0.5f,  0.5f,	0.7f,   0.4f,		0.0f,	0.0f,	1.0f,	
		 0.5f, -0.5f,  0.5f,	0.7f,	0.4f,		0.0f,	0.0f,	1.0f,	
		 0.5f,  0.5f,  0.5f,	0.7f,	0.4f,		0.0f,	0.0f,	1.0f,	
		-0.5f,  0.5f,  0.5f,	0.7f,	0.4f,		0.0f,	0.0f,	1.0f,	
		
		 0.5f, -0.5f,  0.5f,	0.7f,   0.4f,		1.0f,	0.0f,	0.0f,
		 0.5f, -0.5f, -0.5f,	0.7f,	0.4f,		1.0f,	0.0f,	0.0f,
		 0.5f,  0.5f, -0.5f,	0.7f,	0.4f,		1.0f,	0.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.7f,	0.4f,		1.0f,	0.0f,	0.0f,
		
		-0.5f, -0.5f, -0.5f,	0.7f,   0.4f,		0.0f,	0.0f,	-1.0f,
		 0.5f, -0.5f, -0.5f,	0.7f,	0.4f,		0.0f,	0.0f,	-1.0f,
		 0.5f,  0.5f, -0.5f,	0.7f,	0.4f,		0.0f,	0.0f,	-1.0f,
		-0.5f,  0.5f, -0.5f,	0.7f,	0.4f,		0.0f,	0.0f,	-1.0f,
		
		-0.5f, -0.5f, -0.5f,	0.7f,   0.4f,		-1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.7f,	0.4f,		-1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.7f,	0.4f,		-1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f, -0.5f,	0.7f,	0.4f,		-1.0f,	0.0f,	0.0f,
		
		-0.5f, -0.5f,  0.5f,	0.7f,   0.4f,		0.0f,	-1.0f,	0.0f,
		 0.5f, -0.5f,  0.5f,	0.7f,	0.4f,		0.0f,	-1.0f,	0.0f,
		 0.5f, -0.5f, -0.5f,	0.7f,	0.4f,		0.0f,	-1.0f,	0.0f,
		-0.5f, -0.5f, -0.5f,	0.7f,	0.4f,		0.0f,	-1.0f,	0.0f,
		
		-0.5f,  0.4f,  0.5f,	0.6f,   0.9f,		0.0f,	1.0f,	0.0f,
		 0.5f,  0.4f,  0.5f,	0.9f,	0.9f,		0.0f,	1.0f,	0.0f,
		 0.5f,  0.4f, -0.5f,	0.9f,	0.6f,		0.0f,	1.0f,	0.0f,
		-0.5f,  0.4f, -0.5f,	0.6f,	0.6f,		0.0f,	1.0f,	0.0f,

		-0.51f,  0.4f,  0.51f,	0.1f,   0.9f,		0.0f,	0.0f,	1.0f,
		 0.51f,  0.4f,  0.51f,	0.1f,   0.9f,		0.0f,	0.0f,	1.0f,
		 0.51f,  0.5f,  0.51f,	0.1f,   0.9f,		0.0f,	0.0f,	1.0f,
		-0.51f,  0.5f,  0.51f,	0.1f,   0.9f,		0.0f,	0.0f,	1.0f,

		 0.51f,  0.4f, -0.51f,	0.1f,   0.9f,		0.0f,	0.0f,  -1.0f,
		-0.51f,  0.4f, -0.51f,	0.1f,   0.9f,		0.0f,	0.0f,  -1.0f,
		-0.51f,  0.5f, -0.51f,	0.1f,   0.9f,		0.0f,	0.0f,  -1.0f,
		 0.51f,  0.5f, -0.51f,	0.1f,   0.9f,		0.0f,	0.0f,  -1.0f,

		 0.51f,  0.4f, -0.51f,	0.1f,   0.9f,		1.0f,	0.0f,	0.0f,
		 0.51f,  0.4f,  0.51f,	0.1f,   0.9f,		1.0f,	0.0f,	0.0f,
		 0.51f,  0.5f,  0.51f,	0.1f,   0.9f,		1.0f,	0.0f,	0.0f,
		 0.51f,  0.5f, -0.51f,	0.1f,   0.9f,		1.0f,	0.0f,	0.0f,

		-0.51f,  0.4f,  0.51f,	0.1f,   0.9f,	   -1.0f,	0.0f,	0.0f,
		-0.51f,  0.4f, -0.51f,	0.1f,   0.9f,	   -1.0f,	0.0f,	0.0f,
		-0.51f,  0.5f, -0.51f,	0.1f,   0.9f,	   -1.0f,	0.0f,	0.0f,
		-0.51f,  0.5f,  0.51f,	0.1f,   0.9f,	   -1.0f,	0.0f,	0.0f,

		-0.51f, -0.5f,  0.51f,	0.1f,   0.9f,		0.0f,	0.0f,	1.0f,
		 0.51f, -0.5f,  0.51f,	0.1f,   0.9f,		0.0f,	0.0f,	1.0f,
		 0.51f, -0.4f,  0.51f,	0.1f,   0.9f,		0.0f,	0.0f,	1.0f,
		-0.51f, -0.4f,  0.51f,	0.1f,   0.9f,		0.0f,	0.0f,	1.0f,
		
		 0.51f, -0.5f, -0.51f,	0.1f,   0.9f,		0.0f,	0.0f,  -1.0f,
		-0.51f, -0.5f, -0.51f,	0.1f,   0.9f,		0.0f,	0.0f,  -1.0f,
		-0.51f, -0.4f, -0.51f,	0.1f,   0.9f,		0.0f,	0.0f,  -1.0f,
		 0.51f, -0.4f, -0.51f,	0.1f,   0.9f,		0.0f,	0.0f,  -1.0f,
		
		 0.51f, -0.5f, -0.51f,	0.1f,   0.9f,		1.0f,	0.0f,	0.0f,
		 0.51f, -0.5f,  0.51f,	0.1f,   0.9f,		1.0f,	0.0f,	0.0f,
		 0.51f, -0.4f,  0.51f,	0.1f,   0.9f,		1.0f,	0.0f,	0.0f,
		 0.51f, -0.4f, -0.51f,	0.1f,   0.9f,		1.0f,	0.0f,	0.0f,
	
		-0.51f, -0.5f,  0.51f,	0.1f,   0.9f,	   -1.0f,	0.0f,	0.0f,
		-0.51f, -0.5f, -0.51f,	0.1f,   0.9f,	   -1.0f,	0.0f,	0.0f,
		-0.51f, -0.4f, -0.51f,	0.1f,   0.9f,	   -1.0f,	0.0f,	0.0f,
		-0.51f, -0.4f,  0.51f,	0.1f,   0.9f,	   -1.0f,	0.0f,	0.0f,
	};

	Mesh* pecera_marcos = new Mesh();
	pecera_marcos->CreateMesh(pecera_vertices, &pecera_indices[36], 448, 48);
	meshList.push_back(pecera_marcos); 

	Mesh* pecera_cristal = new Mesh();
	pecera_cristal->CreateMesh(pecera_vertices, pecera_indices, 448, 36);
	meshList.push_back(pecera_cristal);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	crearPecera();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado_animales_edited.png");
	dadoTexture.LoadTextureA();
	peceraT = Texture("Textures/pecera.png");
	peceraT.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camaro = Model();
	Camaro.LoadModel("Models/camaro_m2.fbx");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/camaro_r.obj");
	CamaroC = Model();
	CamaroC.LoadModel("Models/camaro_c.obj");
	Dado_a = Model();
	Dado_a.LoadModel("Models/dadoanimales.fbx");
	PezC = Model();
	PezC.LoadModel("Models/cuerpo_pez.obj");
	PezF = Model();	
	PezF.LoadModel("Models/foco_pez.obj");
	PezA = Model();	
	PezA.LoadModel("Models/antena_pez.obj");


	Farola = Model();
	Farola.LoadModel("Models/farola.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		//intensidad ambiental, intensidad difusa
		0.1f, 0.1f,
		0.0f, -1.0f, 0.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.2f, 0.2f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		1.2f, 4.5f, -14.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[2] = PointLight(0.0f, 0.9f, 0.6f, 
		0.2f, 0.8f,                       
		0.0f, 0.0f, 0.0f,                 
		1.0f, 0.45f, 0.9f);               
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		0.5f, 0.5f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f,
		0.5f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		15.0f);
	spotLightCount++;

	spotLights[3] = SpotLight(1.0f, 0.0f, 0.0f,
		0.5f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.045f, 0.0075f,
		15.0f);
	spotLightCount++;	

	spotLights[4] = SpotLight(1.0f, 1.0f, 0.0f,
		0.5f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.45f, 0.25f,
		15.0f);
	spotLightCount++;


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

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
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		//borrar??//spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		PointLight lucesActivas[MAX_POINT_LIGHTS];
		unsigned int conteoActivas = 0;
		lucesActivas[conteoActivas] = pointLights[0];
		conteoActivas++;
		if (mainWindow.getlamp() == 1) {
			lucesActivas[conteoActivas] = pointLights[1];
			conteoActivas++;
		}
		if (mainWindow.getbulb() == 1) {
			lucesActivas[conteoActivas] = pointLights[2];
			conteoActivas++;
		}

		shaderList[0].SetPointLights(lucesActivas, conteoActivas);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::mat4 modelaux3(1.0);
		glm::mat4 modelaux4(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		///Camaro
		model = glm::mat4(1.0);
		color = glm::vec3(0.19f, 0.19f, 0.19f); //Cuerpo del Auto
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(-10.0f, 0.2f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux2 = model;
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Camaro.RenderModel();

		//Ruedas
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.79f, -0.45f, 1.45f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-0.79f, -0.45f, 1.45f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.79f, -0.45f, -1.45f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-0.79f, -0.45f, -1.45f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Cofre
		model = modelaux;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.22f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CamaroC.RenderModel();

		//Luz
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.65f, -0.05f, 1.2f));
		glm::vec3 posLuz = glm::vec3(model[3]);
		glm::vec3 dirLuz = glm::normalize(glm::vec3(model[2]));
		if (mainWindow.getf1() == 0)
			spotLights[3].SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
		else if (mainWindow.getf1() == 1)
			spotLights[3].SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		else if (mainWindow.getf1() == 2)
			spotLights[3].SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
		else if (mainWindow.getf1() == 3)
			spotLights[3].SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
		else if (mainWindow.getf1() == 4)
			spotLights[3].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
		else if (mainWindow.getf1() == 5)
			spotLights[3].SetColor(glm::vec3(0.0f, 1.0f, 1.0f));
		spotLights[3].SetFlash(posLuz, dirLuz);


		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getart15() + mainWindow.getart16(), 5.0f, 6.0));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		model = modelaux3;
		glm::vec3 posLuz3 = glm::vec3(model[3]);
		glm::vec3 dirLuz3 = glm::vec3(0.0f, 0.0f, 0.0f);
		if (mainWindow.getEart15()) {
			dirLuz3 = glm::vec3(0.25f, -1.0f, 0.0f);
		}
		if (mainWindow.getEart16()) {
			dirLuz3 = glm::vec3(-0.25f, -1.0f, 0.0f);
		}
		spotLights[2].SetFlash(posLuz3, dirLuz3);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
		model = glm::translate(model, glm::vec3(0.0f, 0.1f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farola.RenderModel();


		//dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();
	
		//Pecera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.1f, -2.0f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		peceraT.UseTexture();
		meshList[5]->RenderMesh();
		glEnable(GL_BLEND);
		glBlendColor(0.0f, 0.0f, 0.0f, 0.4f);
		glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
		glDepthMask(GL_FALSE);
		meshList[6]->RenderMesh();
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
			
		//Pez
		model = modelaux4;
		float posZ = mainWindow.getartn6() + mainWindow.getartn7();
		float negZ= mainWindow.getartn8() + mainWindow.getartn9();
		float movZ = (posZ - negZ) * 0.05f;
		float movY = (mainWindow.getartn6() - mainWindow.getartn7() + mainWindow.getartn8() - mainWindow.getartn9()) * 0.05f;

		model = glm::translate(model, glm::vec3(0.0f, movY, movZ));

		if (mainWindow.getub() || mainWindow.getdb()) {
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		float inclinacion = 0.0f;
		if (mainWindow.getuf() || mainWindow.getub()) { 
			inclinacion = -45.0f;
		} else if (mainWindow.getdf() || mainWindow.getdb()) { 
			inclinacion = 45.0f;
		}

		model = glm::rotate(model, inclinacion * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux4 = model;	
		color = glm::vec3(0.19f, 0.19f, 0.19f);
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezC.RenderModel();
		
		//Pez Antena
		model = modelaux4;
		model = glm::translate(model, glm::vec3(0.025f, 0.125f, -0.05f));
		modelaux4 = model;
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PezA.RenderModel();

		//Pez Foco
		model = modelaux4;
		modelaux4 = model;
		model = glm::translate(model, glm::vec3(-0.012f, 0.175f, 0.145f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pointLights[2].SetPos(glm::vec3(model[3])+glm::vec3(0.0f, 0.0f, 0.2f));
		PezF.RenderModel();

		model = modelaux4;
		float pitch = mainWindow.getrotu() + mainWindow.getrotd(); 
		float yaw   = 90.0f + mainWindow.getrotr() + mainWindow.getrotl();
		dirLuz.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		dirLuz.y = sin(glm::radians(pitch));
		dirLuz.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		spotLights[4].SetFlash(glm::vec3(model[3]), glm::normalize(dirLuz));


		glDisable(GL_BLEND);
		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
