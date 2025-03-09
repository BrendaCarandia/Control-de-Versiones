#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	llanta1 = 0.0f;
	llanta2 = 0.0f;
	llanta3 = 0.0f;
	llanta4 = 0.0f;

	pata1 = 0.0;
	pata11 = 0.0;
	pata2 = 0.0;
	pata22 = 0.0;
	pata3 = 0.0;
	pata33 = 0.0;
	pata4 = 0.0;
	pata44 = 0.0;

	orejaD = 0.0f;
	orejaI = 0.0f;

	for (size_t i = 0; i < 1024; i++) {
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
	mainWindow = glfwCreateWindow(width, height, "Practica 04: Modelado Jerarquico", NULL, NULL);

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

	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; // Rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	if (key == GLFW_KEY_F)
	{
		theWindow->articulacion1 += 10.0;
	}
	if (key == GLFW_KEY_G)
	{
		theWindow->articulacion2 += 10.0;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->articulacion3 += 10.0;
	}
	if (key == GLFW_KEY_J)
	{
		theWindow->articulacion4 += 10.0;
	}
	if (key == GLFW_KEY_K)
	{
		theWindow->articulacion5 += 10.0;
	}
	if (key == GLFW_KEY_L)
	{
		theWindow->articulacion6 += 10.0;
	}

	// Rotaciones de las llantas
	if (key == GLFW_KEY_1)
	{
		theWindow->llanta1 += 10.0;
	}
	if (key == GLFW_KEY_2)
	{
		theWindow->llanta2 += 10.0;
	}
	if (key == GLFW_KEY_3)
	{
		theWindow->llanta3 += 10.0;
	}
	if (key == GLFW_KEY_4)
	{
		theWindow->llanta4 += 10.0;
	}

	if (key == GLFW_KEY_Z)
	{
		theWindow->pata1 += 10.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->pata11 += 10.0;
	}
	if (key == GLFW_KEY_C)
	{
		theWindow->pata2 += 10.0;
	}
	if (key == GLFW_KEY_V)
	{
		theWindow->pata22 += 10.0;
	}
	if (key == GLFW_KEY_B)
	{
		theWindow->pata3 += 10.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->pata33 += 10.0;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->pata4 += 10.0;
	}
	if (key == GLFW_KEY_P)
	{
		theWindow->pata44 += 10.0;
	}
	if (key == GLFW_KEY_9)
	{
		theWindow->orejaD+= 10.0;
	}
	if (key == GLFW_KEY_0)
	{
		theWindow->orejaI += 10.0;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
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
