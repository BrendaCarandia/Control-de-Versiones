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
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getllanta1() { return llanta1; }
	GLfloat getllanta2() { return llanta2; }
	GLfloat getllanta3() { return llanta3; }
	GLfloat getllanta4() { return llanta4; }
	GLfloat getpata1() { return pata1; }
	GLfloat getpata11() { return pata11; }
	GLfloat getpata2() { return pata2; }
	GLfloat getpata22() { return pata22; }
	GLfloat getpata3() { return pata3; }
	GLfloat getpata33() { return pata33; }
	GLfloat getpata4() { return pata4; }
	GLfloat getpata44() { return pata44; }
	GLfloat getorejaD() { return orejaD; }
	GLfloat getorejaI() { return orejaI; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6;private:
	GLfloat llanta1,llanta2,llanta3,llanta4; // Rotaciones de las 4 llantas
	GLfloat pata1, pata11, pata2, pata22, pata3, pata33, pata4, pata44; //Rotaciones de las patas y articulaciones
	GLfloat orejaD, orejaI;//Rotaciones de orejas
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

