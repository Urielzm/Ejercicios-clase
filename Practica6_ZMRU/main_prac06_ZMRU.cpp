/*---------------------------------------------------------*/
/* ----------------   Práctica 6 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computación gráfica e interacción humano computadora ---------------*/
/*-------------Zagoya Mellado Roberto Uriel------------------------------------*/
/*-----------------Version de visual studio 2017-----------------------------*/

#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>

#define M_PI 3.14159265358979323846264338327950288
//#include "Sphere.h"

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);
void animate(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

float	sol = 0.0f;
float mercurio = 0.0f;
float venus = 0.0f;
float tierra = 0.0f;
float marte = 0.0f;
float jupiter = 0.0f;
float saturno = 0.0f;
float urano = 0.0f;
float neptuno = 0.0f;

//To Buid the Sphere
//---------------------------------------------------------------------------
//const int na = 36;        // vertex grid size
const int meridianos = 36;			// vertex grid size   na -> meridianos
const int paralelos = 8;			// nb -> paralelos
const int meridianos3 = meridianos * 3;     // line in grid size
const int nn = paralelos * meridianos3;    // whole grid size
GLfloat sphere_pos[nn]; // vertex
GLfloat sphere_nor[nn]; // normal
//GLfloat sphere_col[nn];   // color
GLuint  sphere_ix[meridianos*(paralelos - 1) * 6];    // indices
GLuint sphere_vbo[4] = { -1,-1,-1,-1 };
GLuint sphere_vao[4] = { -1,-1,-1,-1 };

void sphere_init()
{
	// generate the sphere data
	GLfloat x, y, z, a, b, da, db, r = 1.0;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(meridianos);
	db = (GLfloat)M_PI / GLfloat(paralelos - 1);
	// [Generate sphere point data]
	// spherical angles a,b covering whole sphere surface
	for (ix = 0, b = (GLfloat)-0.5*M_PI, ib = 0; ib < paralelos; ib++, b += db)
		for (a = 0.0, ia = 0; ia < meridianos; ia++, a += da, ix += 3)
		{
			// unit sphere
			x = cos(b)*cos(a);
			y = cos(b)*sin(a);
			z = sin(b);
			sphere_pos[ix + 0] = x * r;
			sphere_pos[ix + 1] = y * r;
			sphere_pos[ix + 2] = z * r;
			sphere_nor[ix + 0] = x;
			sphere_nor[ix + 1] = y;
			sphere_nor[ix + 2] = z;
		}
	// [Generate GL_TRIANGLE indices]
	for (ix = 0, iy = 0, ib = 1; ib < paralelos; ib++)
	{
		for (ia = 1; ia < meridianos; ia++, iy++)
		{
			// first half of QUAD
			sphere_ix[ix] = iy;      ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + meridianos;   ix++;
			// second half of QUAD
			sphere_ix[ix] = iy + meridianos;   ix++;
			sphere_ix[ix] = iy + 1;    ix++;
			sphere_ix[ix] = iy + meridianos + 1; ix++;
		}
		// first half of QUAD
		sphere_ix[ix] = iy;       ix++;
		sphere_ix[ix] = iy + 1 - meridianos;  ix++;
		sphere_ix[ix] = iy + meridianos;    ix++;
		// second half of QUAD
		sphere_ix[ix] = iy + meridianos;    ix++;
		sphere_ix[ix] = iy - meridianos + 1;  ix++;
		sphere_ix[ix] = iy + 1;     ix++;
		iy++;
	}
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, sphere_vao);
	glGenBuffers(4, sphere_vbo);
	glBindVertexArray(sphere_vao[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_pos), sphere_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_ix), sphere_ix, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	i = 2; // normal
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_nor), sphere_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	/*
		i=3; // color
		glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_col),sphere_col,GL_STATIC_DRAW);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);
	*/
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void sphere_exit()
{
	glDeleteVertexArrays(4, sphere_vao);
	glDeleteBuffers(4, sphere_vbo);
}
void sphere_draw()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glBindVertexArray(sphere_vao[0]);
	//  glDrawArrays(GL_POINTS,0,sizeof(sphere_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
	glDrawElements(GL_LINE_LOOP, sizeof(sphere_ix) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	//glDrawElements(GL_TRIANGLES, sizeof(sphere_ix) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{	
		GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
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
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void animate(void)
{
	sol += 0.05f;//HACE QUE SE AUMENTE SOLO, INTRODUCCION A LA ANIMACIÓN.
	mercurio += 0.08f;
	venus += 0.07f;
	tierra += 0.06f;
	marte += 0.05f;
	jupiter += 0.04f;
	saturno += 0.03f;
	urano += 0.02f;
	neptuno += 0.01;
	
	
}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 temp01 = glm::mat4(1.0f); //Temp

	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	model = glm::rotate(model, glm::radians(sol), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	sphere_draw();	//Sun en lugar de draw elements solo llamamos la esfera.
	model = glm::mat4(1.0f);//resetea la posicion, en este caso a la del sol.

	
	//mercurio
	model = glm::rotate(model, glm::radians(-mercurio), glm::vec3(0, 1, 0));//rotacion ver la funcion de animacion
	model = glm::translate(model, glm::vec3(3, 0, 0));//trasladando la matriz 
	/*se necesita aplicar una rotacion cuando el pivote se encuentra en el sol*/
	/*si se quiere que tenga luna, antes de la escala se debe guardar la matriz en la temporal
	para que esa escala no me afecte a la siguiente*/
	//temp01 = model; -> si se requiere una luna
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.5843f, 0.372549f, 0.1254f));
	sphere_draw();//dibujamos mercurio
	model = glm::mat4(1.0f);//-> aqui es cuando 

	/*/luna1
	model = temp01;
	//model = glm::rotate(model, glm::radians(mercurio*3), glm::vec3(0, 1, 0));
	//model = glm::translate(model, glm::vec3(1.5, 0, 0));//trasladando la matriz 
	//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	model = glm::rotate(model, glm::radians(mercurio * 3), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(1.2f, 0.1f, 1.2f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();//dibujamos mercurio
	model = glm::mat4(1.0f);*/

	//venus
	model = glm::rotate(model, glm::radians(venus), glm::vec3(0, 1, 0));//rotacion ver la funcion de animacion
	model = glm::translate(model, glm::vec3(4, 0, 0));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.9529f, 0.6236f, 0.0941f));
	sphere_draw();//dibujamos mercurio
	model = glm::mat4(1.0f);//-> aqui es cuando 

	//tierra
	model = glm::rotate(model, glm::radians(-tierra), glm::vec3(0, 1, 0));//rotacion ver la funcion de animacion
	model = glm::translate(model, glm::vec3(5, 0, 0));
	temp01 = model;//->si se requiere una luna
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.023529f, 0.2235f, 0.4431f));
	sphere_draw();//dibujamos mercurio
	//model = glm::mat4(1.0f);//-> aqui es cuando no lleva una luna se descomenta
	//luna1
	model = temp01;
	model = glm::rotate(model, glm::radians(tierra * 6), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.5, 0, 0));//trasladando la matriz 
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();//dibujamos luna
	model = glm::mat4(1.0f);

	//Marte
	model = glm::rotate(model, glm::radians(-marte), glm::vec3(0, 1, 0));//rotacion ver la funcion de animacion
	model = glm::translate(model, glm::vec3(6, 0, 0));
	model = glm::scale(model, glm::vec3(0.135f, 0.135f, 0.135f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	sphere_draw();//dibujamos mercurio
	model = glm::mat4(1.0f);//-> aqui es cuando

	//jupiter
	model = glm::rotate(model, glm::radians(-jupiter), glm::vec3(0, 1, 0));//rotacion ver la funcion de animacion
	model = glm::translate(model, glm::vec3(8, 0, 0));
	temp01 = model;//Se guarda la posicion para las lunas
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.7568f, 0.5294f, 0.4196f));
	sphere_draw();//dibujamos mercurio
	//model = glm::mat4(1.0f);//-> aqui se comenta porque debe haber una luna
	//luna1
	model = temp01;
	model = glm::rotate(model, glm::radians(jupiter * 6), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.9, 0, 0));//trasladando la matriz 
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();//dibujamos luna
	//model = glm::mat4(1.0f);-> aqui se comenta porque debe haber una luna

	//luna2
	model = temp01;
	model = glm::rotate(model, glm::radians(-jupiter * 6), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(1.2f, 0, 0));//trasladando la matriz 
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();//dibujamos luna
	model = glm::mat4(1.0f);

	//Saturno
	model = glm::rotate(model, glm::radians(-saturno), glm::vec3(0, 1, 0));//rotacion ver la funcion de animacion
	model = glm::translate(model, glm::vec3(10, 0, 0));
	temp01 = model;
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.9019f, 0.8392f, 0.5647f));
	sphere_draw();//dibujamos mercurio
	//model = glm::mat4(1.0f);//-> aqui es cuando se comenta para dibujar luna o anillo
	//anillo
	model = temp01;
	model = glm::rotate(model, glm::radians(saturno * 3), glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(0.5f, 0.1f, 0.5f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();//dibujamos mercurio
	model = glm::mat4(1.0f);

	//Urano
	model = glm::rotate(model, glm::radians(urano), glm::vec3(0, 1, 0));//rotacion ver la funcion de animacion
	model = glm::translate(model, glm::vec3(12, 0, 0));
	temp01 = model;//Se guarda la posicion para las lunas
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.3764f, 0.4352f, 0.54901f));
	sphere_draw();//dibujamos mercurio
	//model = glm::mat4(1.0f);//-> aqui se comenta para dibujar las lunas
	
	//luna1
	model = temp01;
	model = glm::rotate(model, glm::radians(urano * 6), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.5, 0, 0));//trasladando la matriz 
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();//dibujamos luna
	//model = glm::mat4(1.0f);-> aqui se comenta porque debe haber una luna

	//luna2
	model = temp01;
	model = glm::rotate(model, glm::radians(-urano* 6), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.8f, 0, 0));//trasladando la matriz 
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();//dibujamos luna
	model = glm::mat4(1.0f);

	//Neptuno
	model = glm::rotate(model, glm::radians(-neptuno), glm::vec3(0, 1, 0));//rotacion ver la funcion de animacion
	model = glm::translate(model, glm::vec3(14, 0, 0));
	temp01 = model;//Se guarda la posicion para las lunas
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0.1058f, 0.3333f, 0.5137f));
	sphere_draw();//dibujamos mercurio
	model = glm::mat4(1.0f);//-> aqui es cuando
	//luna1
	model = temp01;
	model = glm::rotate(model, glm::radians(-neptuno * 6), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.5f, 0, 0));//trasladando la matriz 
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	sphere_draw();//dibujamos luna
	model = glm::mat4(1.0f);


	glBindVertexArray(0);

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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 6", NULL, NULL);
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
	sphere_init();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);
		animate();// ES la que manda a llamar la funcion que hace que gire solo

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	sphere_exit();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.18f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.18f;
	

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}