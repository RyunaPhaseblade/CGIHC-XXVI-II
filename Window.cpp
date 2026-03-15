#include "Window.h"


float p1 = 0.0f, p2=0.0f, p3 = 0.0f, p4 = 0.0f;

Window::Window()
{
	width = 800;
	height = 600;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	art7 = 0.0f;
	art8 = 0.0f;
	art9 = 0.0f;
	art10 = 0.0f;
	art11 = 0.0f;
	art12 = 0.0f;
	art13 = 0.0f;
	art14 = 0.0f;
	art15 = 0.0f;
	art16 = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica05:Optimizacion y carga de modelos - 320340020", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}

	if (key == GLFW_KEY_F)
	{
		if (p1 > -45.0f) {
			p1 -= 5.0f;
			theWindow->articulacion1 -= 5.0;
		}
	}

	if (key == GLFW_KEY_G)
	{
		if (p1 < 45.0f) {
			p1 += 5.0f;
			theWindow->articulacion2 += 5.0;
		}
	}

	if (key == GLFW_KEY_H)
	{
		if (p2 > -45.0f) {
			p2 -= 5.0f;
			theWindow->articulacion3 -= 5.0;
		}
	}
	if (key == GLFW_KEY_J)
	{
		if (p2 < 45.0f) {
			p2 += 5.0f;
			theWindow->articulacion4 += 5.0;
		}
	}
	if (key == GLFW_KEY_K)
	{
		if (p3 > -45.0f) {
			p3 -= 5.0f;
			theWindow->articulacion5 -= 5.0;
		}
	}
	if (key == GLFW_KEY_L)
	{
		if (p3 < 45.0f) {
			p3 += 5.0f;
			theWindow->articulacion6 += 5.0;
		}
	}
	if (key == GLFW_KEY_V)
	{
		if (p4 > -45.0f) {
			p4 -= 5.0f;
			theWindow->art7 -= 5.0;
		}
	}
	if (key == GLFW_KEY_B)
	{
		if (p4 < 45.0f) {
			p4 += 5.0f;
			theWindow->art8 += 5.0;
		}
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->art9 += 10.0;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->art10 += 10.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->art11 += 10.0;
	}
	if (key == GLFW_KEY_I)
	{
		theWindow->art12 += 10.0;
	}
	if (key == GLFW_KEY_O)
	{
		theWindow->art13 += 10.0;
	}
	if (key == GLFW_KEY_P)
	{
		theWindow->art14 += 10.0;
	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->art15 += 10.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->art16 += 10.0;
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
