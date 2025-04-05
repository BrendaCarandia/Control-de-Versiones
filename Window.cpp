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
	rotacionLlantas = 0.0f; // Rotación para las llantas
	movercarro = 0.0f; // Traslación del coche
	moverhelicoptero = 0.0f;
	cofre = true;

	for (int i = 0; i < 4; i++) {
		rotacionLlantasIndependiente[i] = 0.0f; // Inicializar rotación independiente de cada llanta
	}

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
	mainWindow = glfwCreateWindow(width, height, "Practica 07: Iluminacion", NULL, NULL);

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

	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; // Rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	//Traslacion coche
	if (key == GLFW_KEY_B && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		theWindow->movercarro += 5.0f; // Mover hacia atras
		theWindow->rotacionLlantas += 10.0f;
	}
	if (key == GLFW_KEY_N && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		theWindow->movercarro -= 5.0f; // Mover hacia adelante
		theWindow->rotacionLlantas -= 10.0f;
	}
	//Traslacion helicoptero
	if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		theWindow->moverhelicoptero += 5.0f; // Mover hacia atras
		
	}
	if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		theWindow->moverhelicoptero -= 5.0f; // Mover hacia adelante
		
	}
	// Rotación independiente de las llantas (adelante)
	if (key == GLFW_KEY_G && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		for (int i = 0; i < 4; i++) {
			theWindow->rotacionLlantasIndependiente[i] += 5.0f; // Girar todas las llantas a la derecha
		}
	}

	// Rotación independiente de las llantas (atras)
	if (key == GLFW_KEY_H && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		for (int i = 0; i < 4; i++) {
			theWindow->rotacionLlantasIndependiente[i] -= 5.0f; // Girar todas las llantas a la derecha
		}
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


void Window::ActualizarRotaciones(GLfloat deltaTime)
{

	GLfloat velocidadRotacion = 5.0f;

	// rotación de llantas con movimiento en el carro
	if (rotacionLlantas > 360.0f) {
		rotacionLlantas -= 360.0f;
	}
	else if (rotacionLlantas < -360.0f) {
		rotacionLlantas += 360.0f;
	}
	//  rotación independiente para cada llanta
	for (int i = 0; i < 4; i++) {
		if (rotacionLlantasIndependiente[i] > 360.0f) {
			rotacionLlantasIndependiente[i] = 360.0f; // Límite máximo de giro de las llantas
		}
		else if (rotacionLlantasIndependiente[i] < -360.0f) {
			rotacionLlantasIndependiente[i] = -360.0f; // Límite máximo de giro de las llantas
		}
	}

	//Cofre
	if (keys[GLFW_KEY_E])
	{
		if (cofre)
		{
			rotax += velocidadRotacion * deltaTime; // Rotar hacia adelante
			if (rotax > 45.0f)
			{
				rotax = 45.0f; // Limitar a 45°
				cofre = false; // Cambiar dirección
			}
		}
		else
		{
			rotax -= velocidadRotacion * deltaTime; // Rotar hacia atrás
			if (rotax < 0.0f)
			{
				rotax = 0.0f; // Limitar a 0°
				cofre = true; // Cambiar dirección
			}
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
