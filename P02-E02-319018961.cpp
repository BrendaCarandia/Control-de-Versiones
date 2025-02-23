
//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
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
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
// Definición de las rutas de los archivos de shaders utilizados 
static const char* vShader = "shaders/shader.vert";
static const char* ShaderRojo = "shaders/shader_rojo.frag";
static const char* ShaderVerde = "shaders/shader_verde.frag";
static const char* ShaderAzul = "shaders/shader_azul.frag";
static const char* ShaderCafe = "shaders/shader_cafe.frag";
static const char* ShaderVerdeOscuro = "shaders/shader_verde_oscuro.frag";



//shaders nuevos se crearían acá

float angulo = 0.0f;

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
	Mesh* obj1 = new Mesh();
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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}


void CreateShaders() {

	// Crea un shader azul
	Shader* shaderAzul = new Shader();  // Reserva memoria para un nuevo shader
	shaderAzul->CreateFromFiles(vShader, ShaderAzul);  // Carga los archivos de shaders (vertex y fragment azul)
	shaderList.push_back(*shaderAzul);  // Agrega el shader a la lista de shaders [0]

	// Crea un shader rojo
	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShader, ShaderRojo);
	shaderList.push_back(*shaderRojo); // Agrega el shader a la lista de shaders [1]

	// Crea un shader verde
	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShader, ShaderVerde);
	shaderList.push_back(*shaderVerde); // Agrega el shader a la lista de shaders [2]

	// Crea un shader café
	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShader, ShaderCafe);
	shaderList.push_back(*shaderCafe);// Agrega el shader a la lista de shaders [3]

	// Crea un shader verde oscuro
	Shader* shaderVerdeOscuro = new Shader();
	shaderVerdeOscuro->CreateFromFiles(vShader, ShaderVerdeOscuro);
	shaderList.push_back(*shaderVerdeOscuro);// Agrega el shader a la lista de shaders [4]

}




int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreateShaders();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(50.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad


	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//	//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Fondo Blanco
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Se agrega limpiar el buffer de profundidad


		angulo += 0.001;
		glm::mat4 model(1.0);

		// Piramide azul, 
		shaderList[0].useShader(); // Usa shader azul
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
	
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.4f, -2.2f));//Al centro y arriba
		model = glm::scale(model, glm::vec3(1.0f, 0.8f, 0.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.1f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Cubo rojo
		shaderList[1].useShader(); // Usa shader rojo
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.46f, -2.5f));//Al centro y abajo
		model = glm::scale(model, glm::vec3(0.95f, 1.0f, 0.5f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.2f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo verde izquierda
		shaderList[2].useShader(); // Usa shader verde
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.08f, -0.1f, -1.0f));//ventana izquierda y en la parte superior del cubo rojo
		model = glm::scale(model, glm::vec3(0.10f, 0.15f, 0.2f)); // se hace más pequeña para que quepa en el cubo rojo
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.2f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo verde derecha
		shaderList[2].useShader(); // Usa shader verde
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.08f, -0.1f, -1.0f)); // ventana derecha y en la parte superior del cubo rojo
		model = glm::scale(model, glm::vec3(0.10f, 0.15f, 0.2f));// se hace más pequeña para que quepa en el cubo rojo
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.2f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo verde abajo
		shaderList[2].useShader(); // Usa shader verde
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, -1.0f));//puerta al centro y abajo del cubo rojo
		model = glm::scale(model, glm::vec3(0.10f, 0.15f, 0.2f));// se hace más pequeña para que quepa en el cubo rojo
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.2f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo cafe derecha
		shaderList[3].useShader(); // Usa shader cafe
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.4f, -0.39f, -1.2f));//Tronco derecho del pino, se mueve fuera del cubo rojo
		model = glm::scale(model, glm::vec3(0.10f, 0.15f, 0.2f));// se hace más pequeña para dentro del espacio de ventana
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.2f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Cubo cafe izquierda
		shaderList[3].useShader(); // Usa shader cafe
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.4f, -0.39f, -1.2f));//Tronco izquierdo del pino, se mueve fuera del cubo rojo
		model = glm::scale(model, glm::vec3(0.10f, 0.15f, 0.2f));// se hace más pequeña para dentro del espacio de ventana
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.2f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Piramide verde oscuro derecha
		shaderList[4].useShader(); // Usa shader verde oscuro
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.35f, -0.15f, -1.0f));// pino derecho, se aleja del cubo rojo y cafe
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f)); // se hace más pequeña para dentro del espacio de ventana
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.2f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Piramide verde oscuro izquierdo
		shaderList[4].useShader(); // Usa shader verde oscuro
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.35f, -0.15f, -1.0f));// pino izquierdo, se aleja del cubo rojo y cafe
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.2f));// se hace más pequeña para dentro del espacio de ventana
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 0.2f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}

