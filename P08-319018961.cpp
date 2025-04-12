/*
Práctica 8: Iluminación 2
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
Texture CuerpoTexture;
Texture matTexture;
Texture verdeTexture;
Texture addTexture;
Texture tornillosTexture;
Texture cartelTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model CarroCuerpo_M;
Model llantaDIzq_M;
Model llantaDDer_M;
Model llantaTIzq_M;
Model llantaTDer_M;
Model cofre_M;
Model lampara;
Model pared;
Model inator;

bool luz0Encendida = true;  // Para pointLights[0] (tecla O)
bool luz1Encendida = true;  // Para pointLights[1] (tecla P)
bool tecla_o = false;
bool tecla_p = false;

bool luzDelanteraActiva = true;  // Inicia encendida
bool luzTraseraActiva = false;
bool avanzando = false;
bool retrocediendo = false;

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



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
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
	dadoTexture = Texture("Textures/octaedro.jpg");
	dadoTexture.LoadTextureA();

	CuerpoTexture = Texture("Textures/Mat_Cuerpo_albedo.jpg");
	CuerpoTexture.LoadTextureA();
	matTexture = Texture("Textures/Mat_MEtal_albedo.jpg");
	matTexture.LoadTextureA();
	verdeTexture = Texture("Textures/Mat_verde_albedo.jpg");
	verdeTexture.LoadTextureA();
	addTexture = Texture("Textures/Mat_Adds_albedo.jpg");
	addTexture.LoadTextureA();
	tornillosTexture = Texture("Textures/Mat_Tprnillos_albedo.jpg");
	tornillosTexture.LoadTextureA();
	cartelTexture = Texture("Textures/Mat_Cartel_albedo.jpg");
	cartelTexture.LoadTextureA();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
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
	lampara = Model();
	lampara.LoadModel("Models/lampara.obj");
	pared= Model();
	pared.LoadModel("Models/pared.obj");
	inator = Model();
	inator.LoadModel("Models/inator.obj");

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
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, //LAMPARA
		0.8f, 1.0f,
		80.0f, 6.0f, -2.5f,
		0.5f, 0.2f, 0.1f);
	pointLightCount++;

	//Declaración de segunda luz puntual
	pointLights[1] = PointLight(0.68f, 1.0f, 0.18f, //INATOR
		0.8f, 1.0f,
		65.0f, 15.0f, -23.0f,
		1.0f, 0.09f, 0.032f);
	pointLightCount++;

	PointLight pointLightsBackup[2];  // Copia de seguridad de las luces originales
	pointLightsBackup[0] = pointLights[0];  // Luz 0 original
	pointLightsBackup[1] = pointLights[1];  // Luz 1 original

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, //LUZ DELANTERA
		1.0f, 2.0f, //atenuacion
		0.0f, 0.0f,  0.0f,
		0.0f, 0.0f, -1.0,//xDir, yDir, zDir
		1.0f, 0.0f, 0.0f,
		25.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 0.0f, //LUZ TRASERA
		1.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		25.0f);
	spotLightCount++;

	spotLights[3] = SpotLight(1.0f, 0.0f, 0.0f, //LUZ DE COFRE
	1.0f, 2.0f,
	0.0f,0.0f,0.0f,
	0.0f, 0.0f, -1.0f,
	1.0f, 0.0f, 0.0f,
	5.0f);
	spotLightCount++;


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
		// Actualizar las rotaciones de las patas
		mainWindow.ActualizarRotaciones(deltaTime);

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


		//CONTROL DE LAMPARA (Tecla O)
		if (mainWindow.getsKeys()[GLFW_KEY_O] && !tecla_o) {
			tecla_o = true;
			luz0Encendida = !luz0Encendida;
		}
		else if (!mainWindow.getsKeys()[GLFW_KEY_O]) {
			tecla_o = false;
		}

		// Control para INATOR (Tecla P)
		if (mainWindow.getsKeys()[GLFW_KEY_P] && !tecla_p) {
			tecla_p = true;
			luz1Encendida = !luz1Encendida;
		}
		else if (!mainWindow.getsKeys()[GLFW_KEY_P]) {
			tecla_p = false;
		}

		// Reordenar el arreglo pointLights para que las luces encendidas estén primero
		int index = 0;
		if (luz0Encendida) {
			pointLights[index++] = pointLightsBackup[0];  // Mover luz 0 al inicio si está encendida
		}
		if (luz1Encendida) {
			pointLights[index++] = pointLightsBackup[1];  // Mover luz 1 después si está encendida
		}

		// Actualizar pointLightCount (número de luces encendidas)
		pointLightCount = index;


		//CONTROL DE LUCES DEL COCHE
		// Detectar dirección del movimiento
		avanzando = mainWindow.getsKeys()[GLFW_KEY_N];
		retrocediendo = mainWindow.getsKeys()[GLFW_KEY_B];
		
		if (avanzando) {
			luzDelanteraActiva = true;   // Delantera SIEMPRE encendida al avanzar
			luzTraseraActiva = false;
		}
		else if (retrocediendo) {
			luzDelanteraActiva = false;
			luzTraseraActiva = true;     // Trasera encendida al retroceder
		}
		glm::vec3 posCoche = glm::vec3(0.0f, 3.0f, mainWindow.getmovercarro());
		spotLights[1].SetPos(posCoche + glm::vec3(0.0f, 2.0f, -5.0f));  // Delantera
		spotLights[2].SetPos(posCoche + glm::vec3(0.0f, 2.0f, 5.0f));   // Trasera

		// --- Prepara luces a enviar al shader ---
		SpotLight lucesActivas[4];  
		unsigned int contadorLuces = 0;

		// Linterna (siempre va primero)
		lucesActivas[contadorLuces++] = spotLights[0];

		// Añade delantera o trasera
		if (luzDelanteraActiva) {
			lucesActivas[contadorLuces++] = spotLights[1];
		}
		else if (luzTraseraActiva) {
			lucesActivas[contadorLuces++] = spotLights[2];
		}

		// Luz del cofre (siempre última)
		lucesActivas[contadorLuces++] = spotLights[3];


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(lucesActivas, contadorLuces);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 22.3f, -55.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pared.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 12.0f, -10.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		inator.RenderModel();
		
		//lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 4.7f, -2.5f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
				//COCHE

				// 1. Dibujar el cuerpo del coche con traslación
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, mainWindow.getmovercarro())); //Traslacion del coche en Z
		modelaux = model;
		//color = glm::vec3(0.0f, 0.502f, 0.502f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CarroCuerpo_M.RenderModel();

		// 2. Dibujar cofre (hija del cuerpo)
		model = modelaux; // Heredar la transformación del cuerpo
		model = glm::translate(model, glm::vec3(1.1f, 22.06f, -13.0f)); // Acomdarlo al cuerpo
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación del cofre E
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofre_M.RenderModel();

		glm::vec3 cofrePosition = glm::vec3(0.0f, 3.0f, mainWindow.getmovercarro());
		spotLights[3].SetPos(cofrePosition + glm::vec3(1.1f, mainWindow.getrotax(), -30.0f));  // Posición base del cofre
		

		// 4. Dibujar las LLANTAS (hijas del cuerpo)
		// Llanta delantera derecha
		model = modelaux; // Restaurar la matriz base del cuerpo
		model = glm::translate(model, glm::vec3(15.0f, 2.6f, -24.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotacionLlantas() + mainWindow.getrotacionLlantasIndependiente(0)), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación de la llanta
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
