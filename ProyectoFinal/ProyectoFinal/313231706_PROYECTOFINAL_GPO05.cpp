#include <iostream> //Procesamiento de entradas y salidas en forma de una secuencia de bytes.
#include <cmath> //onjunto de funciones para realizar operaciones matemáticas.

// GLEW
#include <GL/glew.h> //Determina qué extensiones de OpenGL son compatibles con la plataforma

// GLFW
#include <GLFW/glfw3.h> //Permite crear y administrar ventanas.

// Other Libs
#include "stb_image.h" //Procesamiento de imagenes

// GLM Mathematics
#include <glm/glm.hpp> //Uso de diferentes librerías dadas por C.
#include <glm/gtc/matrix_transform.hpp> //Define funciones que generan matrices de transformación
#include <glm/gtc/type_ptr.hpp> //Maneja la interacción entre punteros y vectores, tipos de matrices.

//Load Models
#include "SOIL2/SOIL2.h" //Carga de texturas dentro de OpenGL


// Other includes
#include "Shader.h" //Se ejecuta en una etapa del proceso
#include "Camera.h" //Carga de la camara dentro de la escena
#include "Model.h" //Carga de modelos en OpenGL.
#include "Texture.h" //Carga de texturas en OpenGL.
#include "modelAnim.h" //Carga de animaciones en OpenGL.

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode); //Lectura de teclado
void MouseCallback(GLFWwindow *window, double xPos, double yPos); //Lectura de mouse
void DoMovement(); //Movimiento de las posiciones de la cámara según la entrada del usuario
void animacion(); //Uso para animación

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera

Camera  camera(glm::vec3(-10.0f, 30.0f, 110.0f));

//Camera  camera(glm::vec3(-10.0f, 10.0f, -50.0f));

GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
float movCamera = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
//glm::vec3 PosIni(-98.0f, 10.0f, -5.0f);
glm::vec3 lightDirection(0.0f, 1.0f, -0.0f);


bool active;


//Animación CUADRO
bool animacionCUADRO = false;
bool regresoCUADRO = false;



//Animación PELOTA
float auxiliar;
float PELOTA_x = -11.0f;
float PELOTA_z = 0.0f;
float PELOTA_y = 8.8f;
int est = 0;
bool animacionPELOTA = false;
bool regresoPELOTA = false;
float altura1 = 8.8f;
float Tiempo = 0.0f;
float altura2 = altura1;
float velocidadPELOTA = 1.0f;





//Animación  MESA
bool animacionMESA = false;
float pos = -5.0f;
bool regresoMESA = false;




// Animación SILLA
bool animacionSILLA = false;
float rotSILLA = 5.7f;
bool regresoSILLA = false;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame




// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(44.0f,0.0f,23.0f), //lado derecho, enfrente
	glm::vec3(-46.0f,0.0f,-95.0f), //lado izquierdo, parte trasera
	glm::vec3(-45.0f,0.0f,24.0f), //lado izquierdo, enfrente
	glm::vec3(48.0f,0.0f,-95.0f)  //lado derecho, parte trasera
};


glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);


glm::vec3 LightP1;
glm::vec3 LightP2;
glm::vec3 LightP3;
glm::vec3 LightP4;



int main()
{
	// Init GLFW
	glfwInit();




	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");

	Model Casa((char*)"Models/Casa/casa.obj");
	Model Cuadro((char*)"Models/Cuadro/cuadro.obj");
	Model Esfera((char*)"Models/Esfera/esfera.obj");
	Model Sillon((char*)"Models/Sillon/sillon.obj");
	Model Piso((char*)"Models/Esfera dragon/Piso.obj");
	Model Lampara((char*)"Models/Lampara/lampara.obj");
	Model Mesa((char*)"Models/Mesa/mesa.obj");
	Model Mueble((char*)"Models/Mueble/mueblelisto.obj");
	Model Pantalla((char*)"Models/Pantalla/pantalla.obj");
	Model Armario((char*)"Models/Armario/armario.obj");
	Model Cama((char*)"Models/Cama/cama.obj");
	Model Escritorio((char*)"Models/Escritorio/escritorio.obj");
	Model Silla((char*)"Models/Silla/silla.obj");
	Model Objeto1((char*)"Models/Objetos/tapete.obj");
	Model Objeto2((char*)"Models/Objetos/cuadroCuarto.obj");
	Model Objeto3((char*)"Models/Objetos/pelotaAzul.obj");
	Model Objeto4((char*)"Models/Objetos/pelotaVerde.obj");
	Model Objeto5((char*)"Models/Objetos/pelotaGrande.obj");
	Model Objeto6((char*)"Models/Objetos/bote.obj");
	Model Objeto7((char*)"Models/Objetos/pesa.obj");
	Model Objeto8((char*)"Models/Objetos/pesita.obj");
	
	
	

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 Light1 = glm::vec3(0);

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.png");
	faces.push_back("SkyBox/left.png");
	faces.push_back("SkyBox/top.png");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.png");
	faces.push_back("SkyBox/detras.png");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		

			// Clear the colorbuffer
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			// Use cooresponding shader when setting uniforms/drawing objects
			lightingShader.Use();
			GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
			glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
			// Set material properties
			glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
			// == ==========================
			// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
			// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
			// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
			// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
			// == ==========================
			// Directional light
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.7f, 0.7f, 0.7f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.7f, 0.7f, 0.7f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);


			// Point light 1
			glm::vec3 lightColor;
			lightColor.x = abs(sin(glfwGetTime() * Light1.x));
			lightColor.y = abs(sin(glfwGetTime() * Light2.y));
			lightColor.z = sin(glfwGetTime() * Light1.z);


			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.50f, 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.22f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.20f);


			// Point light 2
			glm::vec3 light2Color;
			light2Color.x = abs(sin(glfwGetTime() * Light4.x));
			light2Color.y = abs(sin(glfwGetTime() * Light1.y));
			light2Color.z = sin(glfwGetTime() * Light3.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), light2Color.x, light2Color.y, light2Color.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), light2Color.x, light2Color.y, light2Color.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.9f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.22f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.20f);
		

			// Point light 3
			glm::vec3 light3Color;
			light3Color.x = abs(sin(glfwGetTime() * Light2.x));
			light3Color.y = abs(sin(glfwGetTime() * Light3.y));
			light3Color.z = sin(glfwGetTime() * Light4.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), light3Color.x, light3Color.y, light3Color.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), light3Color.x, light3Color.y, light3Color.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.5f, 0.5f, 0.1f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.22f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.20f);

			// Point light 4
			glm::vec3 light4Color;
			light4Color.x = abs(sin(glfwGetTime() * Light1.x));
			light4Color.y = abs(sin(glfwGetTime() * Light4.y));
			light4Color.z = sin(glfwGetTime() * Light3.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), light4Color.x, light4Color.y, light4Color.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), light4Color.x, light4Color.y, light4Color.z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.1f, 0.85f, 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.22f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.20f);

			// SpotLight
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.14f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.48f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

			// Set material properties
			glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

			// Create camera transformations
			glm::mat4 view;
			view = camera.GetViewMatrix();


			// Get the uniform locations
			GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
			GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
			GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
			GLint uniformColor = glGetUniformLocation(lightingShader.Program, "color");

			// Pass the matrices to the shader
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			

			glBindVertexArray(VAO);
			



	//Carga de modelos 
			
			//Casa
			view = camera.GetViewMatrix();
			glm::mat4 model(1);
			model = glm::mat4(1);
			glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0.0f);
			glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Casa.Draw(lightingShader);

			//Sillon
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			model = glm::translate(model, glm::vec3(-3.0f, 0.3f, -6.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Sillon.Draw(lightingShader);

			//Piso
			view = camera.GetViewMatrix();
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(20.0f, 35.0f, 20.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Piso.Draw(lightingShader);

			//	Cuadro 1 arriba de sillon
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(3.0f, 2.0f, 3.0f));
			model = glm::translate(model, glm::vec3(-4.0f, 1.1f, -5.65f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Cuadro.Draw(lightingShader);

			//Mesa 1 frente sillon
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.5f));
			model = glm::translate(model, glm::vec3(-6.0f, 0.5f, -3.81f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mesa.Draw(lightingShader);

			//Mesa 2 a un costado del sillon
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-24.0f, 0.1f, -15.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mesa.Draw(lightingShader);

			//Lampara 1 en mesa 2
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			model = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0f, 4.8f, -15.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Lampara.Draw(lightingShader);

			//Mueble 1 al fondo
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.3f));
			model = glm::translate(model, glm::vec3(11.0f, 0.2f, -8.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mueble.Draw(lightingShader);

			//Pantalla
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			model = glm::translate(model, glm::vec3(0.6f, 2.0f, -7.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Pantalla.Draw(lightingShader);

			//Esfera 1
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
			model = glm::translate(model, glm::vec3(-4.5f, 8.8f, -11.9f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Esfera.Draw(lightingShader);

			//Mueble 2 con lampara 2 inicial
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.1f, 1.3f, 1.1f));
			model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(2.0f, 0.4f, -16.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mueble.Draw(lightingShader);

			//Cuadro 2 con animación
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-25.7f, 5.0f, -30.0f));
			model = glm::rotate(model, glm::radians(rot), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Cuadro.Draw(lightingShader);

			//Mesa 3 con animación
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(0.8f, 2.2f, 0.8f));
			model = glm::translate(model, glm::vec3(-4.0f, 0.6f, -33.0f));
			model = glm::translate(model, glm::vec3(-10.0f, 0.1f, pos));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mesa.Draw(lightingShader);
			
			//Armario
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
			model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(17.0f, 0.9f, 45.7f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Armario.Draw(lightingShader);

			//Cama
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(178.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(15.0f, 2.5f, 82.7f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Cama.Draw(lightingShader);

			//Escritorio
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-14.0f, 2.7f, 77.8f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Escritorio.Draw(lightingShader);

			//Silla
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-85.5f, 1.5f, 32.5f));
			model = glm::rotate(model, rotSILLA * glm::radians(-12.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Silla.Draw(lightingShader);

			//Bote
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-10.0f, 1.5f, 97.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto6.Draw(lightingShader);
			
			//Tapete
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.5f, 1.7f, 1.2f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-5.7f, 1.4f, 71.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto1.Draw(lightingShader);
			
			//Pesa
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(1.0f, 2.5f, 72.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto7.Draw(lightingShader);

			//Pesita 1 izquierda
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(3.0f, 2.5f, 74.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto8.Draw(lightingShader);

			//Pesita 2 derecha
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.9f, 2.5f, 74.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto8.Draw(lightingShader);

			//Pelota azul
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(1.7f, 2.6f, 73.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto3.Draw(lightingShader);

			//Pelota verde
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(1.5f, 2.6f, 75.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto4.Draw(lightingShader);

			//Pelota azul grande
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(1.5f, 2.7f, 75.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto5.Draw(lightingShader);

			//Cuadro cuarto
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.2f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-1.2f, 8.0f, 77.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto2.Draw(lightingShader);

			//Sillon en cuarto
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
			model = glm::translate(model, glm::vec3(-8.0f, 1.3f, -26.9f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Sillon.Draw(lightingShader);
			
			//Mueble en cuarto
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.1f, 1.3f, 1.1f));
			model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-70.0f, 1.3f, -21.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mueble.Draw(lightingShader);

			//Pelota azul grande CON ANIMACION
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(glm::mat4(1.0f), glm::vec3(PELOTA_x, PELOTA_y + 0.4f, PELOTA_z));
			model = glm::translate(model, glm::vec3(8.5f, 2.2f, -87.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto5.Draw(lightingShader);
			
			//Mesa en cuarto
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-22.0f, 0.1f, -64.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mesa.Draw(lightingShader);

			//	Cuadro en cuarto
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(2.6f, 2.0f, 2.6f));
			model = glm::translate(model, glm::vec3(-4.8f, 0.5f, -25.65f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Cuadro.Draw(lightingShader);

			//Mesa en cuarto
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 4.0f, 1.0f));
			model = glm::translate(model, glm::vec3(42.0f, 0.1f, -50.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Mesa.Draw(lightingShader);

			//Escritorio
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(-1.0f, 2.7f, 39.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Escritorio.Draw(lightingShader);

			//Lampara en cuarto
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			model = glm::translate(glm::mat4(1.0f), glm::vec3(-21.0f, 4.7f, -67.0f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Lampara.Draw(lightingShader);

			//Bote
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(5.0f, 1.5f, 41.1f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto6.Draw(lightingShader);

			//Pelota azul
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(8.5f, 2.6f, 30.6f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto3.Draw(lightingShader);

			//Pelota verde
			model = glm::mat4(1);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(8.5f, 2.6f, 30.4f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			Objeto4.Draw(lightingShader);




			model = glm::mat4(1);
			glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 0.0f, 0.75f);


			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			glBindVertexArray(0);




			// Also draw the lamp object, again binding the appropriate shader
			lampShader.Use();
			// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
			modelLoc = glGetUniformLocation(lampShader.Program, "model");
			viewLoc = glGetUniformLocation(lampShader.Program, "view");
			projLoc = glGetUniformLocation(lampShader.Program, "projection");

			// Set matrices
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			model = glm::mat4(1);
			model = glm::translate(model, lightPos);
			//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			// Draw the light object (using light's vertex attributes)
			glBindVertexArray(lightVAO);
			for (GLuint i = 0; i < 4; i++)
			{
				model = glm::mat4(1);
				model = glm::translate(model, pointLightPositions[i]);
				model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glBindVertexArray(0);


			// Draw skybox as last
			glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
			SkyBoxshader.Use();
			view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
			glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			//skybox cube
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS); // Set depth function back to default




			// Swap the screen buffers
			glfwSwapBuffers(window);
		}




		glDeleteVertexArrays(1, &VAO);
		glDeleteVertexArrays(1, &lightVAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteBuffers(1, &skyboxVBO);
		// Terminate GLFW, clearing any resources allocated by GLFW.
		glfwTerminate();


		return 0;
	}


	void animacion()
	{

	// Animación 1 CUADRO
		if (animacionCUADRO)
		{
			if (!regresoCUADRO)
			{
				if (rot < 15.0f)
				{
					rot += 0.05f;
				}
				else
				{
					regresoCUADRO = true;
				}
			}
			else
			{
				if (rot > 5.0f)
				{
					rot -= 0.05f;
				}
				else
				{
					regresoCUADRO = false;
				}
			}
		}



		//Animación 2 MESA
		if (animacionMESA)
		{
			if (!regresoMESA)
			{
				if (pos < 2.0f)
				{
					pos += 0.09f;
				}
				else
				{
					regresoMESA = true;
				}
			}
			else
			{
				if (pos > -6.0f)
				{
					pos -= 0.09f;
				}
				else
				{
					regresoMESA = false;
				}
			}
		}





		// Animación 3 SILLA
		if (animacionSILLA)
		{
			if (!regresoSILLA)
			{
				if (rotSILLA < 5.5f)
				{
					rotSILLA += 0.01f;
				}
				else
				{
					regresoSILLA = true;
				}
			}
			else
			{
				if (rotSILLA > 4.1f)
				{
					rotSILLA -= 0.01f;
				}
				else
				{
					regresoSILLA = false;
				}
			}
		}







		//Animación 4 PELOTA

		if (animacionPELOTA)
		{
			switch (est)
			{
			case 0: //Hacia abajo
				if (PELOTA_y > 0.0f)
				{
					Tiempo += 0.05;
					auxiliar = (altura2 - (0.5 * 9.81 * pow(Tiempo, 2)));
					if (auxiliar < 0.0)
						PELOTA_y = 0.0;
					else
						PELOTA_y = auxiliar;
				}
				else
				{
					velocidadPELOTA = (9.81 * Tiempo) / 0.5; //
					altura2 = altura2 * 0.6;
					Tiempo = 0.0f;
					if (altura2 < 0.2)
						est = 2;
					else
						est = 1;
				}
				break;
			case 1: //Hacia arriba
				if (PELOTA_y < altura2)
				{
					Tiempo += 0.05;
					auxiliar = 9.81 * pow(Tiempo, 2);
					if (auxiliar > altura2)
						PELOTA_y = altura2;
					else
						PELOTA_y += 0.05;
				}
				else
				{
					Tiempo = 0.0f;
					est = 0;
				}

			case 2:
				PELOTA_y = PELOTA_y;
				break;
			default:
				break;
			}
		}


	

	}


// Is called whenever a key is pressed/released via GLFW
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}



	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 0.0f, 1.0f);
			Light2 = glm::vec3(0.04f, 0.16f, 0.45f);
			Light3 = glm::vec3(0.27f, 1.0f, 0.4f);
			Light4 = glm::vec3(0.2f, 0.0f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
			Light2 = glm::vec3(0);
			Light3 = glm::vec3(0);
			Light4 = glm::vec3(0);
		}
	}



	//Animación CUADRO
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		animacionCUADRO ^= true;
	    regresoCUADRO = false;

	//Animación MESA
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		animacionMESA ^= true;
	    regresoMESA = false;
		
	//Animación SILLA
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		animacionSILLA ^= true;
		regresoSILLA = false;
	

	//Animación PELOTA
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		PELOTA_x = -11.0f;
		PELOTA_z = 0.0f;
		PELOTA_y = 8.5f;
		est = 0;
		animacionPELOTA ^= true;
		regresoPELOTA = false;
		altura1 = 8.5f;
		Tiempo = 0.0f;
		altura2 = altura1;
		velocidadPELOTA = 1.0f;
	}
	
}



void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	
	/*pointLight*/

	//Lado izquierdo - detras
	if (keys[GLFW_KEY_1])
	{
		pointLightPositions[1].x += 0.01f;
	}

	if (keys[GLFW_KEY_2])
	{
		pointLightPositions[1].x -= 0.01f;
	}


	//Lado izquierdo - frente
	if (keys[GLFW_KEY_3])
	{
		pointLightPositions[2].x += 0.01f;
	}

	if (keys[GLFW_KEY_4])
	{
		pointLightPositions[2].x -= 0.01f;
	}
	
	
	//Lado derecho - detras
	if (keys[GLFW_KEY_5])
	{
		pointLightPositions[3].x += 0.01f;
	}

	if (keys[GLFW_KEY_6])
	{
		pointLightPositions[3].x -= 0.01f;
	}


	//Lado derecho - enfrente
	if (keys[GLFW_KEY_7])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_8])
	{
		pointLightPositions[0].x -= 0.01f;
	}




	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}






}