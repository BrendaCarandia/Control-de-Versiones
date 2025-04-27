/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen

#define STB_IMAGE_IMPLEMENTATION


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <map>




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

//variables para animación
float movDragon;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
// Constantes de control de velocidad 
float VELOCIDAD_GLOBAL_CABEZAS = 0.5f; // Factor para reducir/incrementar TODOS los movimientos
float VELOCIDAD_CABEZA1 = 0.1f;
float VELOCIDAD_CABEZA2 = 0.1f;
float VELOCIDAD_CABEZA3 = 0.1f;
float VELOCIDAD_CABEZA4 = 0.1f;
float VELOCIDAD_CABEZA5 = 0.1f;
float tiempoCabezas = 0.0f; // Acumulador de tiempo para animaciones


float toffsetletrau = 0.0f;
float toffsetletrav = 0.0f;

// Variables para control de numeros
float tiempoNumeroCambiante = 0.0f;
float velocidadNumeroCambiante = 0.05f; // Cambios por segundo
float tiempoCambioTextura = 0.0f;
bool mostrarNumero1 = true;
float intervaloCambioTextura = 50.0f; // Segundos entre cambios

float desplazamientoX = 0.0f;
float velocidadDesplazamiento = 0.01f; // Ajusta esta velocidad según necesites
std::string mensaje = "PROYECTO CGEIHC F";
float anchoLetra = 5.0f;
float espacioEntreLetras = 0.1f;

float desplazamientoTexto = 0.0f;
const float velocidadDesplazamientoTexto = 50.0f; // píxeles por segundo
float anchoTotalTexto = mensaje.length() * (anchoLetra + espacioEntreLetras);

const float pasoUV = 1.0f / 9.0f;
std::map<char, glm::vec4> uvLetras = {
	// Letras mayúsculas (filas 1-3)
	{'P', {6 * pasoUV, 5 * pasoUV, 7 * pasoUV, 6 * pasoUV}},  // Fila 4, Col 7
	{'R', {8 * pasoUV, 5 * pasoUV, 9 * pasoUV, 6 * pasoUV}},  // Fila 4, Col 9
	{'O', {5 * pasoUV, 5 * pasoUV, 6 * pasoUV, 6 * pasoUV}},  // Fila 4, Col 6
	{'Y', {6 * pasoUV, 4 * pasoUV, 7 * pasoUV, 5 * pasoUV}},  // Fila 5, Col 7
	{'E', {4 * pasoUV, 6 * pasoUV, 5 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 5
	{'C', {2 * pasoUV, 6 * pasoUV, 3 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 3
	{'T', {1 * pasoUV, 4 * pasoUV, 2 * pasoUV, 5 * pasoUV}},  // Fila 5, Col 2
	{'O', {5 * pasoUV, 5 * pasoUV, 6 * pasoUV, 6 * pasoUV}},  // Fila 4, Col 6

	{' ', {8 * pasoUV, 4 * pasoUV, 9 * pasoUV, 5 * pasoUV}},  // Fila 5, Col 9 (espacio)

	{'C', {2 * pasoUV, 6 * pasoUV, 3 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 3
	{'G', {6 * pasoUV, 6 * pasoUV, 7 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 7
	{'E', {4 * pasoUV, 6 * pasoUV, 5 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 5
	{'I', {8 * pasoUV, 6 * pasoUV, 9 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 9
	{'H', {7 * pasoUV, 6 * pasoUV, 8 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 8
	{'C', {2 * pasoUV, 6 * pasoUV, 3 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 3

	{' ', {8 * pasoUV, 4 * pasoUV, 9 * pasoUV, 5 * pasoUV}},  // Fila 5, Col 9 (espacio)

	{'F', {5 * pasoUV, 6 * pasoUV, 6 * pasoUV, 7 * pasoUV}},  // Fila 3, Col 6
	{'a', {0 * pasoUV, 3 * pasoUV, 1 * pasoUV, 4 * pasoUV}},  // Fila 6, Col 1
	{'b', {1 * pasoUV, 3 * pasoUV, 2 * pasoUV, 4 * pasoUV}},  // Fila 6, Col 2
};


float anchoLetraUV = 113.0f / 1024.0f;  
float altoLetraUV = 107.0f / 1024.0f;    

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture Tipografia;
// Variables para controlar los alientos
bool alientoActivo[5] = { false };
float tiempoAliento[5] = { 0.0f };
glm::mat4 modelCabezas[5]; // Almacenar matrices de transformación de cada cabeza

// Texturas para los alientos
Texture texturaFuego;
Texture texturaRayos;
Texture texturaGas;
Texture texturaHielo;
Texture texturaArena;



Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Cabeza1;
Model Cabeza2;
Model Cabeza3;
Model Cabeza4;
Model Cabeza5;
Model Tiamat_M;
Skybox skybox;
Model Ala_izquierda;
Model Ala_derecha;
Model Puerta;
Model Letrero;

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



//cálculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		// Posición X, Y, Z       Coord. U (anchoLetraUV), V (altoLetraUV)   Normal
		-0.5f, 0.0f, 0.5f,        0.04f,           0.7,           0.0f, -1.0f, 0.0f,
		 0.5f, 0.0f, 0.5f,        0.04,   0.7f,           0.0f, -1.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,       0.04,   0.7,    0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,       0.04f,           0.7f ,   0.0f, -1.0f, 0.0f,
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


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

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

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();
	Tipografia = Texture("Textures/Tipografia.tga");
	Tipografia.LoadTextureA();
	texturaFuego= Texture("Textures/Fuego.png");
	texturaFuego.LoadTextureA();
	texturaRayos = Texture("Textures/rayos.png");
	texturaRayos.LoadTextureA();
	texturaGas = Texture("Textures/gas_venenoso.png");
	texturaGas.LoadTextureA();
	texturaHielo = Texture("Textures/Hielo.png");
	texturaHielo.LoadTextureA();
	texturaArena = Texture("Textures/arena.png");
	texturaArena.LoadTextureA();



	Dragon_M = Model();
	Dragon_M.LoadModel("Models/Cuerpo_Dragon.obj");
	Cabeza1 = Model();
	Cabeza1.LoadModel("Models/Cabeza1.obj");
	Cabeza2 = Model();
	Cabeza2.LoadModel("Models/Cabeza2.obj");
	Cabeza3 = Model();
	Cabeza3.LoadModel("Models/Cabeza3.obj");
	Cabeza4 = Model();
	Cabeza4.LoadModel("Models/Cabeza4.obj");
	Cabeza5 = Model();
	Cabeza5.LoadModel("Models/Cabeza5.obj");
	Ala_izquierda = Model();
	Ala_izquierda.LoadModel("Models/Ala_izquierda.obj");
	Ala_derecha = Model();
	Ala_derecha.LoadModel("Models/Ala_derecha2.obj");
	Puerta = Model();
	Puerta.LoadModel("Models/Puerta.obj");
	Letrero = Model();
	Letrero.LoadModel("Models/Letrero.obj");



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
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
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
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movDragon = 0.0f;
	movOffset = 0.2f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	avanza = 0.0f;
	float rotacionDragonY = 0.0f; // Ángulo actual de rotación en Y
	const float velocidadRotacion = 90.0f; // Grados por segundo para la rotación

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Actualizar tiempos de animación
		tiempoNumeroCambiante += deltaTime;
		tiempoCambioTextura += deltaTime;
		tiempoCabezas += deltaTime * VELOCIDAD_GLOBAL_CABEZAS;
		
	
		
			// Actualizar desplazamiento del texto
			desplazamientoTexto -= velocidadDesplazamientoTexto * deltaTime;

		angulovaria += 0.0001f * deltaTime;

		// Lógica de movimiento y rotación del dragón
		if (avanza) {
			if (movDragon > -300.0f) {
				movDragon -= movOffset * deltaTime;
				rotllanta += rotllantaOffset * deltaTime;

				// Rotación gradual hacia adelante (0 grados)
				if (rotacionDragonY > 0.0f) {
					rotacionDragonY -= velocidadRotacion * deltaTime;
					if (rotacionDragonY < 0.0f) rotacionDragonY = 0.0f;
				}
			}
			else {
				avanza = false;
				// Iniciar rotación para mirar hacia atrás (180 grados)
				rotacionDragonY = 180.0f;
			}
		}
		else {
			if (movDragon < 300.0f) {
				movDragon += movOffset * deltaTime;
				rotllanta -= rotllantaOffset * deltaTime;

				// Rotación gradual hacia atrás (180 grados)
				if (rotacionDragonY < 180.0f) {
					rotacionDragonY += velocidadRotacion * deltaTime;
					if (rotacionDragonY > 180.0f) rotacionDragonY = 180.0f;
				}
			}
			else {
				avanza = true;
				// Iniciar rotación para mirar hacia adelante
				rotacionDragonY = 0.0f;
			}
		}
		



		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//teclas para los alientos
		alientoActivo[0] = mainWindow.getsKeys()[TECLA_FUEGO];
		alientoActivo[1] = mainWindow.getsKeys()[TECLA_RAYOS];
		alientoActivo[2] = mainWindow.getsKeys()[TECLA_GAS];
		alientoActivo[3] = mainWindow.getsKeys()[TECLA_HIELO];
		alientoActivo[4] = mainWindow.getsKeys()[TECLA_ARENA];

		// Actualizar tiempos de aliento
		for (int i = 0; i < 5; i++) {
			if (alientoActivo[i]) tiempoAliento[i] += deltaTime;
			else tiempoAliento[i] = 0.0f;
		}

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// Configurar el shader para el texto
		shaderList[0].UseShader();
		Tipografia.UseTexture();

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//Puerta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.5f, -1.95f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puerta.RenderModel();

		//Letrero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.5f, -1.95f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero.RenderModel();


		/*color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//plano con todos las letras
		toffsetletrau = 0.0;
		toffsetletrav = 0.0;
		toffset = glm::vec2(toffsetletrau, toffsetletrav);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Tipografia.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		//for (int i = 1; i < 4; i++)
		//{
		//	//números 2-4
		//	toffsetnumerou = 0.68;
		//	toffsetnumerov = 0.10;
		//	toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		//	model = glm::mat4(1.0);
		//	model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
		//	model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		//	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//	color = glm::vec3(1.0f, 1.0f, 1.0f);
		//	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//	Tipografia.UseTexture();
		//	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//	meshList[6]->RenderMesh();

		// }

		//for (int j = 1; j < 5; j++)
		//{
		//	//números 5-8
		//	toffsetnumerou += 0.25;
		//	toffsetnumerov = -0.33;
		//	toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		//	model = glm::mat4(1.0);
		//	model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
		//	model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//	glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		//	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//	color = glm::vec3(1.0f, 1.0f, 1.0f);
		//	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//	NumerosTexture.UseTexture();
		//	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//	meshList[6]->RenderMesh();
		//}


		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		// Si el tiempo acumulado supera el intervalo definido (2.0/velocidad), actualiza el numero
	// Configurar el modelo y renderizar el texto

			// Reiniciar posición cuando el texto haya salido completamente
		// Incrementa el offset de textura en el eje U (desplazamiento horizontal)
		if (tiempoNumeroCambiante >= 2.0f / velocidadNumeroCambiante) {
			// Incrementa el offset de textura en el eje U (desplazamiento horizontal)
			toffsetnumerocambiau += 0.23f;
			if (toffsetnumerocambiau > 1.0f)
				toffsetnumerocambiau = 0.07f;
			tiempoNumeroCambiante = 0.0f;
		}

		toffsetnumerov = 0.07;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(18.5f, 10.3f, 3.4f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.5f, 1.5f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Tipografia.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		////cambiar automáticamente entre textura número 1 y número 2

		//if (tiempoCambioTextura >= intervaloCambioTextura) {
		//	mostrarNumero1 = !mostrarNumero1;  // Alterna entre true/false (cambia de textura)
		//	tiempoCambioTextura = 0.0f;       // Reinicia el contador de tiempo
		//}
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		//toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//
		//if (mostrarNumero1) {
		//	Numero1Texture.UseTexture(); // Activa la textura del número 1
		//}
		//else {
		//	Numero2Texture.UseTexture(); // Activa la textura del número 2
		//}
		//
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[5]->RenderMesh();




		float angulo_aleteo = sin(glm::radians(angulovaria * 10.0f)) * 2.0f;
		// Dragon 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movDragon, 5.0f + 3 * sin(glm::radians(angulovaria * 1.0)), 6.0f));
		model = glm::rotate(model, glm::radians(rotacionDragonY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación en Y
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_M.RenderModel();
		
		//Ala derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.688f, 5.754f, 1.741f));
		model = glm::rotate(model, glm::radians(angulo_aleteo), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.688f, -5.754f, -1.741f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_derecha.RenderModel();

		//Ala izquierda
		model = glm::translate(model, glm::vec3(-0.551f, 5.186f, -2.691f));
		model = glm::rotate(model, glm::radians(angulo_aleteo), glm::vec3(1.0f, 1.0f, .0f));
		model = glm::translate(model, glm::vec3(0.551f, -5.186f, 2.691f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ala_izquierda.RenderModel();

		// CABEZA 1: Movimiento senoidal (más lento)
		model = modelaux;
		float amplitudSenoidal = 0.1f;
		float offsetX1 = amplitudSenoidal * sin(VELOCIDAD_CABEZA1 * tiempoCabezas * 2.0f); // Más lento
		float offsetZ1 = amplitudSenoidal * cos(VELOCIDAD_CABEZA1 * tiempoCabezas * 2.0f);
		model = glm::translate(model, glm::vec3(offsetX1, 0.0f, offsetZ1));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelCabezas[0] = model; // Para cabeza 1
		Cabeza1.RenderModel();
	

		// CABEZA 2: Espiral de Arquímedes (más controlada)
		model = modelaux;
		float radioEspiral = 0.05f * (tiempoCabezas * 0.3f * VELOCIDAD_CABEZA2); // Más lento
		float anguloEspiral = tiempoCabezas * 1.0f * VELOCIDAD_CABEZA2; // Más lento
		float offsetX2 = radioEspiral * cos(anguloEspiral);
		float offsetY2 = radioEspiral * sin(anguloEspiral);
		model = glm::translate(model, glm::vec3(0.0f, offsetY2, offsetX2));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelCabezas[1] = model; // Para cabeza 1
		Cabeza2.RenderModel();

		// CABEZA 3: Lemniscata (más lenta y suave)
		model = modelaux;
		float aLemniscata = 0.4f;
		float tLemniscata = tiempoCabezas * 1.0f * VELOCIDAD_CABEZA3; // Más lento
		float denomLemniscata = 1.0f + sin(tLemniscata) * sin(tLemniscata);
		float offsetX3 = (aLemniscata * cos(tLemniscata)) / denomLemniscata;
		float offsetZ3 = (aLemniscata * sin(tLemniscata) * cos(tLemniscata)) / denomLemniscata;
		model = glm::translate(model, glm::vec3(offsetX3, 0.0f, offsetZ3));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelCabezas[2] = model; // Para cabeza 1
		Cabeza3.RenderModel();

		
		// CABEZA 4:  Movimiento circular más lento
		model = modelaux;
		float radioCircular = 0.05f; 
		float offsetX4 = radioCircular * sin(tiempoCabezas * 0.8f * VELOCIDAD_CABEZA4); 
		float offsetY4 = radioCircular * cos(tiempoCabezas * 0.8f * VELOCIDAD_CABEZA4) * 0.3f; 
		model = glm::translate(model, glm::vec3(offsetX4, offsetY4, 0.0f));
		model = glm::rotate(model, sin(tiempoCabezas * 1.0f * VELOCIDAD_CABEZA4) * 0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelCabezas[3] = model; // Para cabeza 1
		Cabeza4.RenderModel();
		

		// CABEZA 5: Movimiento vertical unido al cuerpo
		model = modelaux; 
		float alturaCuello = -6.0f; 
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, alturaCuello));
		float anguloVertical = sin(tiempoCabezas * VELOCIDAD_CABEZA5) * 0.15f; 
		model = glm::rotate(model, anguloVertical, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -alturaCuello)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelCabezas[4] = model; 
		Cabeza5.RenderModel();

		// Ubicacion inicial del aliento
		const glm::vec3 OFFSETS_POSICION[5] = {
			glm::vec3(-2.0f, -2.0f, 1.05f),  // Cabeza Cafe
		
			glm::vec3(-1.8f, -1.1f, 1.2f),  // Cabeza Blanca

			glm::vec3(-1.8f, -1.5f, 0.45f), // Cabeza Verde

			glm::vec3(-1.8f, -1.1f, 0.03f),  // Cabeza Azul

			glm::vec3(-1.8f, -1.6f,- 0.01f)  // Cabeza Roja
		};

		const float VELOCIDAD_ANIMACION = 0.01f;
		const float ESCALA_ALIENTO_ANCHO = 0.3f;
		const float ESCALA_ALIENTO_LARGO = 1.5f;

		// Renderizado de alientos
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		for (int i = 0; i < 5; i++) {
			if (!alientoActivo[i] || tiempoAliento[i] <= 0.0f) continue;

			// 1. Posición base + offset
			glm::vec3 posicionAliento = glm::vec3(modelCabezas[i][3]) + OFFSETS_POSICION[i];
			glm::mat4 model = glm::translate(glm::mat4(1.0f), posicionAliento);

			//  Rotación para la cabeza roja
			if (i == 4) { // Cabeza 5 (índice 4)
				// Rotación especial para el aliento 5
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación X
				model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación X
			
			}
			else {
				
				//  Aplicar rotación de -90° en X a TODOS los alientos
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}

			// Rotación para orientar hacia adelante
			glm::vec3 direccion = glm::normalize(glm::vec3(modelCabezas[i] * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
			model = glm::rotate(model, atan2(direccion.x, direccion.z), glm::vec3(0.0f, 1.0f, 0.0f));

			// Escala 
			float escalaVar = 1.0f + sin(tiempoAliento[i] * 2.0f) * 0.1f;
			model = glm::scale(model, glm::vec3(ESCALA_ALIENTO_ANCHO * escalaVar,
				ESCALA_ALIENTO_ANCHO * escalaVar,
				ESCALA_ALIENTO_LARGO));

			// Configurar textura
			switch (i) {
			case 0: texturaArena.UseTexture(); break;
			case 1: texturaHielo.UseTexture(); break;
			case 2: texturaGas.UseTexture(); break;
			case 3: texturaRayos.UseTexture(); break;
			case 4: texturaFuego.UseTexture(); break;
			}

			// Animación de textura más lenta
			glm::vec2 texOffset(0.0f, tiempoAliento[i] * VELOCIDAD_ANIMACION);
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(texOffset));

			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[4]->RenderMesh();
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
