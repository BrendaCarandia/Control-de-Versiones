/*
Práctica 5: Optimización y Carga de Modelos
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
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Model CarroCuerpo_M;
Model llantaDIzq_M;
Model llantaDDer_M;
Model llantaTIzq_M;
Model llantaTDer_M;
Model cofre_M;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;



// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";





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

	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 1.0f);

	//Goddard_M = Model();
	//Goddard_M.LoadModel("Models/goddard_base.obj");
	CarroCuerpo_M = Model();
	CarroCuerpo_M.LoadModel("Model carro/cuerpocarro.obj");
	llantaDIzq_M = Model();
	llantaDIzq_M.LoadModel("Model carro/llantaizqDelantera.obj");
	llantaDDer_M = Model();
	llantaDDer_M.LoadModel("Model carro/llantaderDelantera.obj");
	llantaTDer_M = Model();
	llantaTDer_M.LoadModel("Model carro/llantaderTrasera.obj");
	llantaTIzq_M = Model();
	llantaTIzq_M.LoadModel("Model carro/llantaizqTrasera.obj");
	cofre_M = Model();
	cofre_M.LoadModel("Model carro/cofre.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Actualizar las rotaciones de las patas
		mainWindow.ActualizarRotaciones(deltaTime);


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[2]->RenderMesh();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		//COCHE

		// 1. Dibujar el cuerpo del coche con traslación
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, mainWindow.getmovercarro())); //Traslacion del coche en Z
		modelaux = model;
		color = glm::vec3(0.0f, 0.502f, 0.502f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarroCuerpo_M.RenderModel();


		// 2. Dibujar cofre (hija del cuerpo)
		model = modelaux; // Heredar la transformación del cuerpo
		model = glm::translate(model, glm::vec3(1.1f, 22.06f, -13.0f)); // Acomdarlo al cuerpo
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación del cofre E
		color = glm::vec3(0.6f, 0.0f, 0.8f); // Color del cofre
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofre_M.RenderModel();

		// 4. Dibujar las LLANTAS (hijas del cuerpo)
		// Llanta delantera derecha
		model = modelaux; // Restaurar la matriz base del cuerpo
		model = glm::translate(model, glm::vec3(15.0f, 2.6f, -24.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas() + mainWindow.getrotacionLlantasIndependiente(0)), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
		color = glm::vec3(1.0f, 0.980f, 0.941f); // Color de la llanta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaDDer_M.RenderModel();

		//// Llanta delantera izquierda
		model = modelaux; // Restaurar la matriz base del cuerpo
		model = glm::translate(model, glm::vec3(-13.5f, 3.2f, -24.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas() + mainWindow.getrotacionLlantasIndependiente(1)), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaDIzq_M.RenderModel();

		// Llanta trasera derecha
		model = modelaux; // Restaurar la matriz base del cuerpo
		model = glm::translate(model, glm::vec3(16.0f, 3.1f, 28.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas() + mainWindow.getrotacionLlantasIndependiente(2)), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTDer_M.RenderModel();

		// Llanta trasera izquierda
		model = modelaux; // Restaurar la matriz base del cuerpo
		model = glm::translate(model, glm::vec3(-14.2f, 3.4f, 29.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas() + mainWindow.getrotacionLlantasIndependiente(3)), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTIzq_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
