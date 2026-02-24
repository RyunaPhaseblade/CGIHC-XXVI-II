//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
//Libreria de funciones matematicas
#include<glm.hpp>
//Funciones de matrices y vectores
#include<gtc\matrix_transform.hpp>
//Para que datos que no son de Opengl se puedan referenciar en las variables del shader.
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//Shaders con colores específicos
static const char* vShaderRojo = "shaders/shader_rojo.vert";
static const char* fShaderRojo = "shaders/shader_rojo.frag";
static const char* vShaderAzul = "shaders/shader_azul.vert";
static const char* fShaderAzul = "shaders/shader_azul.frag";
static const char* vShaderVerde = "shaders/shader_verde.vert";
static const char* fShaderVerde = "shaders/shader_verde.frag";
static const char* vShaderCafe = "shaders/shader_cafe.vert";
static const char* fShaderCafe = "shaders/shader_cafe.frag";
static const char* vShaderAzulv = "shaders/shader_azulv.vert";
static const char* fShaderAzulv = "shaders/shader_azulv.frag";

float angulo = 0.0f;
float angulo2 = 0.0f;
float pos = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f*/
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,36);
	meshColorList.push_back(letras);

	GLfloat vertices_cuadrado_rojo[] = {
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f
	};
	MeshColor* cuadrado_rojo = new MeshColor();
	cuadrado_rojo->CreateMeshColor(vertices_cuadrado_rojo, 36);
	meshColorList.push_back(cuadrado_rojo);

	GLfloat vertices_trianguloazul[] = {
		-0.625f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.625f,		-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,		0.0f,		0.5f,			0.0f,	0.0f,	1.0f	
	};
	MeshColor* trianguloAzul = new MeshColor();
	trianguloAzul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloAzul);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.1666f,		-0.1666f,		0.51f,			0.0f,	1.0f,	0.0f,
		0.1666f,		-0.1666f,		0.51f,			0.0f,	1.0f,	0.0f,
		0.1666f,		0.1666f,		0.51f,			0.0f,	1.0f,	0.0f,
		-0.1666f,		-0.1666f,		0.51f,			0.0f,	1.0f,	0.0f,
		0.1666f,		0.1666f,		0.51f,			0.0f,	1.0f,	0.0f,
		-0.1666f,		0.1666f,		0.51f,			0.0f,	1.0f,	0.0f
	};
	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	GLfloat vertices_cuadradoazul[] = {
		//X			Y			Z				R			G			B
		-0.1666f,		-0.1666f,		0.51f,			0.3764f,	0.8078f,	0.7372f,
		0.1666f,		-0.1666f,		0.51f,			0.3764f,	0.8078f,	0.7372f,
		0.1666f,		0.1666f,		0.51f,			0.3764f,	0.8078f,	0.7372f,
		-0.1666f,		-0.1666f,		0.51f,			0.3764f,	0.8078f,	0.7372f,
		0.1666f,		0.1666f,		0.51f,			0.3764f,	0.8078f,	0.7372f,
		-0.1666f,		0.1666f,		0.51f,			0.3764f,	0.8078f,	0.7372f
	};
	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);

	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z				R		G		B
		-0.125f,		-0.125f,		0.52f,			0.478, 0.255, 0.067,
		0.125f,			-0.125f,		0.52f,			0.478, 0.255, 0.067,
		0.125f,			0.125f,			0.52f,			0.478, 0.255, 0.067,
		-0.125f,		-0.125f,		0.52f,			0.478, 0.255, 0.067,
		0.125f,			0.125f,			0.52f,			0.478, 0.255, 0.067,
		-0.125f,		0.125f,			0.52f,			0.478, 0.255, 0.067
	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

	GLfloat vertices_trianguloverde[] = {
		-0.1875f,		-0.1875f,		0.52f,			0.0, 0.5, 0.0,
		0.1875f,		-0.1875f,		0.52f,			0.0, 0.5, 0.0,
		0.0f,			0.275f,			0.52f,			0.0, 0.5, 0.0
	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);
}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	Shader *shaderRojo = new Shader(); 
	shaderRojo->CreateFromFiles(vShaderRojo, fShaderRojo);
	shaderList.push_back(*shaderRojo);

	Shader *shaderAzul = new Shader(); 
	shaderAzul->CreateFromFiles(vShaderAzul, fShaderAzul);
	shaderList.push_back(*shaderAzul);

	Shader *shaderVerde = new Shader(); 
	shaderVerde->CreateFromFiles(vShaderVerde, fShaderVerde);
	shaderList.push_back(*shaderVerde);

	Shader *shaderCafe = new Shader(); 
	shaderCafe->CreateFromFiles(vShaderCafe, fShaderCafe);
	shaderList.push_back(*shaderCafe);

	Shader *shaderAzulV = new Shader(); 
	shaderAzulV->CreateFromFiles(vShaderAzulv, fShaderAzulv);
	shaderList.push_back(*shaderAzulV);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		//angulo += 0.01;
		//printf("%f\n", angulo);
		
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//Piramide azul
		angulo = 110;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.621, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Cubo rojo
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(2.75f, 2.75f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		//Cubo verde
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.9f, -5.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Arbol
		angulo2 = 0.4;
		//angulo2 += 0.0001;
		//printf("%f\n", angulo2);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.35f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(angulo2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.4f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.35f, 0.0f, -5.0f));
		model = glm::rotate(model, glm::radians(angulo2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.4f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Cubos ventanas
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.65f, 0.5f, -5.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.65f, 0.5f, -5.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Cubo troncos
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.35f, -0.9f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.35f, -0.9f, -5.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		



		glUseProgram(0);
		mainWindow.swapBuffers();




	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/