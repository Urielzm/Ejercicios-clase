/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computación Gráfica e interacción humano computadora---------------*/
/*--------------Nombre:      ZAGOYA MELLADO ROBERTO URIEL--------------------------*/
/*--------------Numero de cuenta: 416113778----------------------------------------*/
/*---------------------Grupo: 4----------------------------------------------------*/

#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint shaderProgramRed, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec4 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderRed = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 0.0f, 0.0f);									\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	

	float vertices[] = 
	{
		//PARA LA LETRA C
		// pocision         // color
		//linea superior horizontal de la c 
		-0.7f,  0.2f, 0.0f,  1.0f, 0.0f, 0.0f,// Cv1
		-0.3f, 0.2f, 0.0f,  1.0f, 0.0f, 0.0f,// Cv2
		-0.3f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f, // Cv3
		-0.7f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // Cv4
		//linea vertical  izquierda de la c
		-0.7f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // Cv4
		- 0.65f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f, // Cv5
		-0.65f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // Cv6
		-0.7f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // Cv7
		//linea inferior  horizontal de la c
		-0.7f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // Cv7
		-0.3f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // Cv8
		-0.3f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Cv9
		-0.7f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Cv10

		//PARA LA LETRA G
		//pocision			//color
		//linea superior horizontal de la G
		0.3f,  0.2f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV1
		0.7f,  0.2f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV2
		0.7f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV3
		0.3f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV4
		//linea vertical  izquierda de la G
		0.3f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV4
		0.35f,  0.1f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV5
		0.35f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV6
		0.3f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV7
		//linea inferior  horizontal de la G
		0.3f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV7
		0.7f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV8
		0.7f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV9
		0.3f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // GV10
		//para la linea que distingue a la g de la G
		0.7f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // G´V1
		0.7f,  -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  // G´V2
		0.65f,  -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  // G´V3
		0.65f,  -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,  // G´V4

		0.7f,  -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  // G´V2
		0.7f,  -0.2f, 0.0f, 1.0f, 0.0f, 0.0f,  // G´V5
		0.5f,  -0.2f, 0.0f, 1.0f, 0.0f, 0.0f,  // G´V6
		0.5f,  -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,  // G´V7
		
	};

	unsigned int indices[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
		13, 14, 15, 16, 17, 18, 19, 20,
		21, 22, 23, 24, 25, 26, 27, 28,
		29, 30, 31, 32
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRed, 1, &myFragmentShaderRed, NULL);
	glCompileShader(fragmentShaderRed);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramRed = glCreateProgram();
	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderRed);
	glDeleteShader(fragmentShaderColor);

}

void display(void)
{
	glUseProgram(shaderProgramRed);

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glPointSize(5.0);
	//glDrawElements(GL_POINTS, 5, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_POLYGON, 0, 4);
	glDrawArrays(GL_POLYGON, 4, 4);
	glDrawArrays(GL_POLYGON, 8, 4);
	glDrawArrays(GL_POLYGON, 12, 4);
	glDrawArrays(GL_POLYGON, 16, 4);
	glDrawArrays(GL_POLYGON, 20, 4);
	glDrawArrays(GL_POLYGON, 24, 4);
	glDrawArrays(GL_POLYGON, 28, 4);

	glBindVertexArray(0);

	glUseProgram(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	//Configurar Shaders
	setupShaders();
    

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor(0.0f, 0.5f, 0.6f, 2.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//Mi función de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}