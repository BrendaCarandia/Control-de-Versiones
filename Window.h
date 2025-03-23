#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	// Función para obtener la rotación de las llantas
	GLfloat getrotacionLlantas() { return rotacionLlantas; }

	// Función para obtener la traslación del coche
	GLfloat getmovercarro() { return movercarro; }
	GLfloat getrotacionLlantasIndependiente(int index) {
		if (index >= 0 && index < 4) {
			return rotacionLlantasIndependiente[index];
		}
		return 0.0f;
	}

	// Función para actualizar las rotaciones y traslaciones
	void ActualizarRotaciones(GLfloat deltaTime);

	// Destructor
	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;

	// Variables para rotaciones generales
	GLfloat rotax, rotay, rotaz, cofre, movercarro;

	// Variables para la rotación de las llantas y traslación del coche
	GLfloat rotacionLlantas; // Rotación común para las llantas
	GLfloat rotacionLlantasIndependiente[4];//Rotacion independiente de las llantas

	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

