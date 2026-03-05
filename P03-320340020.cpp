//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks




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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, 0.0f,-0.5f,	//0
		0.5f,0.0f,-0.5f,	//1
		0.0f,0.0f, 0.36602f,	//2
		0.0f,0.81649f,-0.21132,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

void crearTetra()
{
	GLfloat vertices_piramide[] = {
		-0.5f, 0.0f, -0.5f,	    // 0
		 0.5f, 0.0f, -0.5f,	    // 1
		 0.0f, 0.0f,  0.36602f,	// 2
		 0.0f, 0.81649f, -0.21132f // 3
	};
	unsigned int indices_base[] = { 0, 1, 2 };
	unsigned int indices_trasera[] = { 1, 3, 0 };
	unsigned int indices_derecha[] = { 1, 2, 3 };
	unsigned int indices_izquierda[] = { 3, 2, 0 };

	Mesh* cBase = new Mesh(); cBase->CreateMesh(vertices_piramide, indices_base, 12, 3);
	meshList.push_back(cBase);

	Mesh* cC1 = new Mesh(); cC1->CreateMesh(vertices_piramide, indices_trasera, 12, 3);
	meshList.push_back(cC1);

	Mesh* cC2 = new Mesh(); cC2->CreateMesh(vertices_piramide, indices_derecha, 12, 3);
	meshList.push_back(cC2);

	Mesh* cC3 = new Mesh(); cC3->CreateMesh(vertices_piramide, indices_izquierda, 12, 3);
	meshList.push_back(cC3);
}

void mTetraedro(glm::vec3 pos, float sep, GLuint uniModel, GLuint uniColor, int r, int g, int b, int a) {
	// Matriz base para esta pieza específica
	glm::mat4 modelBase = glm::mat4(1.0f);
	modelBase = glm::translate(modelBase, pos);
	modelBase = glm::scale(modelBase, glm::vec3(0.5f, 0.5f, 0.5f));

	if (r == 1) {
		glm::mat4 m2 = glm::translate(modelBase, glm::vec3(0.0f, sep * 0.5f, -sep));
		glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(m2));
		meshList[3]->RenderMeshGeometry();
	}

	if (g == 1) {
		glm::mat4 m3 = glm::translate(modelBase, glm::vec3(sep, sep * 0.5f, sep * 0.5f));
		glUniform3f(uniColor, 0.0f, 1.0f, 0.0f);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(m3));
		meshList[4]->RenderMeshGeometry();
	}

	if (b == 1) {
		glm::mat4 m4 = glm::translate(modelBase, glm::vec3(-sep, sep * 0.5f, sep * 0.5f));
		glUniform3f(uniColor, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(m4));
		meshList[5]->RenderMeshGeometry();
	}

	if (a == 1) {
		glm::mat4 m1 = glm::translate(modelBase, glm::vec3(0.0f, -sep, 0.0f));
		glUniform3f(uniColor, 1.0f, 1.0f, 0.0f);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(m1));
		meshList[2]->RenderMeshGeometry();
	}
}
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	crearTetra();//índice 1 en MeshList
	CrearCilindro(32, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();



	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

	// Posiciones fijas para las caras rojas
	float rojo1_x = -0.2875f;
	float rojo1_y = 1.5125f;
	float rojo1_z = -2.965f;
	float rojo2_x = 0.2875f;
	float rojo2_y = 1.5125f;
	float rojo2_z = -2.965f;
	float rojo3_x = 0.0f;
	float rojo3_y = 1.9625f;
	float rojo3_z = -2.805f;

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//Base, priamide negra
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.25f, -2.5f));
		model = glm::scale(model, glm::vec3(1.65f, 1.65f, 1.65f));
		glUniform3f(uniformColor, 0.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMeshGeometry();


		float s = 0.03f; // Separación entre piezas

		// piramides en aristas y vertices
		mTetraedro(glm::vec3(0.0f, 2.169f, -2.743f), 0.03f, uniformModel, uniformColor, 1, 1, 1, 0);
		mTetraedro(glm::vec3(-0.575f, 1.25f, -3.075f), 0.03f, uniformModel, uniformColor, 1, 0, 1, 1);
		mTetraedro(glm::vec3(0.575f, 1.25f, -3.075f), 0.03f, uniformModel, uniformColor, 1, 1, 0, 1);
		mTetraedro(glm::vec3(0.0f, 1.25f, -2.079f), 0.03f, uniformModel, uniformColor, 0, 1, 1, 1);
		mTetraedro(glm::vec3(0.0f, 1.25f, -3.075f), 0.03f, uniformModel, uniformColor, 1, 0, 0, 1);
		mTetraedro(glm::vec3(0.2875f, 1.25f, -2.577f), 0.03f, uniformModel, uniformColor, 0, 1, 0, 1);
		mTetraedro(glm::vec3(-0.2875f, 1.25f, -2.577f), 0.03f, uniformModel, uniformColor, 0, 0, 1, 1);
		mTetraedro(glm::vec3(-0.2875f, 1.7095f, -2.909f), 0.03f, uniformModel, uniformColor, 1, 0, 1, 0);
		mTetraedro(glm::vec3(0.2875f, 1.7095f, -2.909f), 0.03f, uniformModel, uniformColor, 1, 1, 0, 0);
		mTetraedro(glm::vec3(0.0f, 1.7095f, -2.411f), 0.03f, uniformModel, uniformColor, 0, 1, 1, 0);
		//cara amarilla completa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2875f, 1.22f, -3.12f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniform3f(uniformColor, 1.0f, 1.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.2875f, 1.22f, -3.12f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniform3f(uniformColor, 1.0f, 1.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.22f, -2.615f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniform3f(uniformColor, 1.0f, 1.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[2]->RenderMeshGeometry();

		float sep_rojo = 0.03f;
		glm::vec3 normalPlanoRojo = glm::normalize(glm::vec3(0.0f, -0.333f, 0.943f));
		//cara roja completa
		glm::mat4 modelBaseRojo1 = glm::mat4(1.0f);
		modelBaseRojo1 = glm::translate(modelBaseRojo1, glm::vec3(rojo1_x, rojo1_y, rojo1_z));
		modelBaseRojo1 = glm::scale(modelBaseRojo1, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo1 = glm::translate(modelBaseRojo1, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo1 = glm::rotate(m_rojo1, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo1));
		meshList[3]->RenderMeshGeometry();

		glm::mat4 modelBaseRojo2 = glm::mat4(1.0f);
		modelBaseRojo2 = glm::translate(modelBaseRojo2, glm::vec3(rojo2_x, rojo2_y, rojo2_z));
		modelBaseRojo2 = glm::scale(modelBaseRojo2, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo2 = glm::translate(modelBaseRojo2, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo2 = glm::rotate(m_rojo2, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo2));
		meshList[3]->RenderMeshGeometry();

		glm::mat4 modelBaseRojo3 = glm::mat4(1.0f);
		modelBaseRojo3 = glm::translate(modelBaseRojo3, glm::vec3(rojo3_x, -0.0075f + rojo3_y, -0.015f + rojo3_z));
		modelBaseRojo3 = glm::scale(modelBaseRojo3, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo3 = glm::translate(modelBaseRojo3, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo3 = glm::rotate(m_rojo3, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo3));
		meshList[3]->RenderMeshGeometry();

		// Centro de rotación: centro de la pirámide negra
		glm::vec3 centroRotacion = glm::vec3(0.0f, 1.587f, -2.849f);

		//cara azul completa
		glm::mat4 modelRojo1_120 = glm::mat4(1.0f);
		modelRojo1_120 = glm::translate(modelRojo1_120, centroRotacion);
		modelRojo1_120 = glm::rotate(modelRojo1_120, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRojo1_120 = glm::translate(modelRojo1_120, -centroRotacion);
		modelRojo1_120 = glm::translate(modelRojo1_120, glm::vec3(rojo1_x, rojo1_y, -0.01f + rojo1_z));
		modelRojo1_120 = glm::scale(modelRojo1_120, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo1_120 = glm::translate(modelRojo1_120, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo1_120 = glm::rotate(m_rojo1_120, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo1_120));
		meshList[3]->RenderMeshGeometry();

		glm::mat4 modelRojo2_120 = glm::mat4(1.0f);
		modelRojo2_120 = glm::translate(modelRojo2_120, centroRotacion);
		modelRojo2_120 = glm::rotate(modelRojo2_120, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRojo2_120 = glm::translate(modelRojo2_120, -centroRotacion);
		modelRojo2_120 = glm::translate(modelRojo2_120, glm::vec3(rojo2_x, rojo2_y, -0.01f + rojo2_z));
		modelRojo2_120 = glm::scale(modelRojo2_120, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo2_120 = glm::translate(modelRojo2_120, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo2_120 = glm::rotate(m_rojo2_120, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo2_120));
		meshList[3]->RenderMeshGeometry();

		glm::mat4 modelRojo3_120 = glm::mat4(1.0f);
		modelRojo3_120 = glm::translate(modelRojo3_120, centroRotacion);
		modelRojo3_120 = glm::rotate(modelRojo3_120, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRojo3_120 = glm::translate(modelRojo3_120, -centroRotacion);
		modelRojo3_120 = glm::translate(modelRojo3_120, glm::vec3(rojo3_x, -0.0075f + rojo3_y, -0.01f + -0.015f + rojo3_z));
		modelRojo3_120 = glm::scale(modelRojo3_120, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo3_120 = glm::translate(modelRojo3_120, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo3_120 = glm::rotate(m_rojo3_120, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo3_120));
		meshList[3]->RenderMeshGeometry();

		//Cara Verde completa
		glm::mat4 modelRojo1_m120 = glm::mat4(1.0f);
		modelRojo1_m120 = glm::translate(modelRojo1_m120, centroRotacion);
		modelRojo1_m120 = glm::rotate(modelRojo1_m120, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRojo1_m120 = glm::translate(modelRojo1_m120, -centroRotacion);
		modelRojo1_m120 = glm::translate(modelRojo1_m120, glm::vec3(rojo1_x, rojo1_y, rojo1_z));
		modelRojo1_m120 = glm::scale(modelRojo1_m120, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo1_m120 = glm::translate(modelRojo1_m120, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo1_m120 = glm::rotate(m_rojo1_m120, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 0.0f, 1.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo1_m120));
		meshList[3]->RenderMeshGeometry();

		glm::mat4 modelRojo2_m120 = glm::mat4(1.0f);
		modelRojo2_m120 = glm::translate(modelRojo2_m120, centroRotacion);
		modelRojo2_m120 = glm::rotate(modelRojo2_m120, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRojo2_m120 = glm::translate(modelRojo2_m120, -centroRotacion);
		modelRojo2_m120 = glm::translate(modelRojo2_m120, glm::vec3(rojo2_x, rojo2_y, rojo2_z));
		modelRojo2_m120 = glm::scale(modelRojo2_m120, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo2_m120 = glm::translate(modelRojo2_m120, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo2_m120 = glm::rotate(m_rojo2_m120, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 0.0f, 1.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo2_m120));
		meshList[3]->RenderMeshGeometry();

		glm::mat4 modelRojo3_m120 = glm::mat4(1.0f);
		modelRojo3_m120 = glm::translate(modelRojo3_m120, centroRotacion);
		modelRojo3_m120 = glm::rotate(modelRojo3_m120, glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelRojo3_m120 = glm::translate(modelRojo3_m120, -centroRotacion);
		modelRojo3_m120 = glm::translate(modelRojo3_m120, glm::vec3(rojo3_x, -0.0075f + rojo3_y, -0.015f + rojo3_z));
		modelRojo3_m120 = glm::scale(modelRojo3_m120, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 m_rojo3_m120 = glm::translate(modelRojo3_m120, glm::vec3(0.0f, sep_rojo * 0.5f, -sep_rojo));
		m_rojo3_m120 = glm::rotate(m_rojo3_m120, glm::radians(180.0f), normalPlanoRojo);
		glUniform3f(uniformColor, 0.0f, 1.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(m_rojo3_m120));
		meshList[3]->RenderMeshGeometry();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


