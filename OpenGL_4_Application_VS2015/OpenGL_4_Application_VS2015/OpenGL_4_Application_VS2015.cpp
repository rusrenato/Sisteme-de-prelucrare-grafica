//
//  main.cpp
//  OpenGL Shadows
//
//  Created by CGIS on 05/12/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC

#include <iostream>
#include <fstream>

#include <stdio.h> 
#include <stdlib.h>

#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "Shader.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"
#include "glm/ext.hpp"
#define TINYOBJLOADER_IMPLEMENTATION


#include "Model3D.hpp"
#include "Mesh.hpp"


int glWindowWidth = 640;
int glWindowHeight = 480;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat3 lightDirMatrix;
GLuint lightDirMatrixLoc;

glm::vec3 spotLightDir;
GLuint spotLightDirLoc;

glm::vec3 spotLightPos;
GLuint spotLightPosLoc;

int ceLuminaVreau = 0; 
int ceata = 0;
GLfloat ceLuminaVreauLoc;
GLfloat ceataLoc;
GLfloat spotLightCutOff = glm::cos(glm::radians(12.5f));
GLfloat spotLightOuterCutOff = glm::cos(glm::radians(17.5f));

GLuint spotLightCutOffLoc;
GLuint spotLightOuterCutOffLoc;

std::vector<const GLchar*> faces;

gps::SkyBox mySkyBox;
gps::Shader skyboxShader;

float Zobiect1 = 9.64325f;
float Zobiect2 = 45.7896f;

float raza1 = 49.1026 / 2;
float raza2 = 197.626 / 2;

float y = 0;
float m = 0;
float cocoZ = 0.0f;
int nrPicuri = 100;
float valoare = 0;
float inaltime = 0;
float x = 0;
float z = 0;

int conditie = 0;
float unghi = 0;
std::ifstream file("direction1.txt");


std::ofstream myfile;
std::ofstream myfile2;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

gps::Camera myCamera(glm::vec3(0.0f, 1.0f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f));
GLfloat cameraSpeed = 0.80f;

bool pressedKeys[1024];
GLfloat angle;
GLfloat lightAngle;

gps::Model3D Ploaie[500];
gps::Model3D CapacCufar;
gps::Model3D Coco;
gps::Model3D Cufar;
gps::Model3D Dog;
gps::Model3D Insula1;
gps::Model3D Insula2;
gps::Model3D Insula3;
gps::Model3D Insula4;
gps::Model3D Insula5;
gps::Model3D Insula6;
gps::Model3D Insula7;
gps::Model3D InsulaMare;
gps::Model3D Monkey1;
gps::Model3D Monkey2;
gps::Model3D Monkey3;
gps::Model3D Monkey4;
gps::Model3D Oceanul;
gps::Model3D Pirat;
gps::Model3D Pluta1;
gps::Model3D Pluta2;
gps::Model3D Pod;
gps::Model3D Prosop;
gps::Model3D Raft1;
gps::Model3D Raft2;
gps::Model3D Scaun;
gps::Model3D SkyDome;
gps::Model3D Toucan1;
gps::Model3D Toucan2;
gps::Model3D Tun;
gps::Model3D Turn1;
gps::Model3D Turn2;
gps::Model3D Turn3;
gps::Model3D Umbrela;
gps::Model3D ground;
gps::Model3D lightCube;
gps::Model3D lightCube2;
gps::Model3D lightCube3;
gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader depthMapShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;



GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	myCustomShader.useShaderProgram();

	//set projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 10000.0f);
	//send matrix data to shader
	GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	lightShader.useShaderProgram();
	
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//set Viewport transform
	glViewport(0, 0, retina_width, retina_height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	myCamera.mouse_callback(xpos,ypos);
}

void processMovement()
{

	if (pressedKeys[GLFW_KEY_Q]) {
		angle += 0.1f;
		if (angle > 360.0f)
			angle -= 360.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angle -= 0.1f;
		if (angle < 0.0f)
			angle += 360.0f;
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	
	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}
	if (pressedKeys[GLFW_KEY_LEFT_SHIFT]) {
		cameraSpeed = 0.20f;
	}
	else {
		cameraSpeed = 0.80f;
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_J]) {

		lightAngle += 0.3f;
		if (lightAngle > 360.0f)
			lightAngle -= 360.0f;
		glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle -= 0.3f; 
		if (lightAngle < 0.0f)
			lightAngle += 360.0f;
		glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
	}	

	if (pressedKeys[GLFW_KEY_Z]) {
		ceLuminaVreau = 1 - ceLuminaVreau; // 
		glUniform1i(ceLuminaVreauLoc, ceLuminaVreau);
	}

	if (pressedKeys[GLFW_KEY_C]) {
		ceata = 1 - ceata; // 
		glUniform1i(ceataLoc, ceata);
	}


	if (pressedKeys[GLFW_KEY_Y]) {
	

		if (raza1 + raza2 - 20 > Zobiect2 - Zobiect1) {
			y += 0.1f;
			Zobiect1 -= 0.1;
		}
	}

	if (pressedKeys[GLFW_KEY_U]) {
		y -= 0.1f;
		Zobiect1 += 0.1f;
	}

	if (pressedKeys[GLFW_KEY_M]) {
		
		if (m < 16.4) {
			m += 0.1f;
		}
	}

	if (pressedKeys[GLFW_KEY_N]) {

		m -= 0.1f;
	}


	if (pressedKeys[GLFW_KEY_P]) {

		nrPicuri = 500;
	}

	if (pressedKeys[GLFW_KEY_O]) {

		nrPicuri = 1;
	}


	if (pressedKeys[GLFW_KEY_1]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (pressedKeys[GLFW_KEY_2]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (pressedKeys[GLFW_KEY_3]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
    //glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(glWindow, mouseCallback);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initFBOs()
{
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix()
{
	const GLfloat near_plane = 1.0f, far_plane = 10.0f;
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
	glm::mat4 lightView = glm::lookAt(lightDirTr, myCamera.getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}

void initModels()
{
	CapacCufar = gps::Model3D("objects/scena/CapacCufar.obj", "objects/scena/");
	Coco = gps::Model3D("objects/scena/Coco.obj", "objects/scena/");
	Cufar = gps::Model3D("objects/scena/CufarA.obj", "objects/scena/");
	Dog = gps::Model3D("objects/scena/Dog.obj", "objects/scena/");
	Insula1 = gps::Model3D("objects/scena/Insula1A.obj", "objects/scena/");
	Insula2 = gps::Model3D("objects/scena/Insula3A.obj", "objects/scena/");
	Insula3 = gps::Model3D("objects/scena/Insula4A.obj", "objects/scena/"); // turn sparte
	Insula4 = gps::Model3D("objects/scena/Insula2A.obj", "objects/scena/"); // cufar
	Insula5 = gps::Model3D("objects/scena/Insula5.obj", "objects/scena/");
	Insula6 = gps::Model3D("objects/scena/Insula6C.obj", "objects/scena/");
	Insula7 = gps::Model3D("objects/scena/Insula7A.obj", "objects/scena/");
	Monkey1 = gps::Model3D("objects/scena/Monkey1.obj", "objects/scena/");
	Monkey2 = gps::Model3D("objects/scena/Monkey2.obj", "objects/scena/");
	Monkey3 = gps::Model3D("objects/scena/Monkey3.obj", "objects/scena/");
	Monkey4 = gps::Model3D("objects/scena/Monkey4.obj", "objects/scena/");
	InsulaMare = gps::Model3D("objects/scena/InsulaMareA.obj", "objects/scena/");
	Oceanul = gps::Model3D("objects/scena/Oceanul.obj", "objects/scena/");
	Pirat = gps::Model3D("objects/scena/Piratul.obj", "objects/scena/");

	for (int i = 0; i < 500; i++) {
		Ploaie[i] = gps::Model3D("objects/scena/Ploaie.obj", "objects/scena/");
	}

	Pluta1 = gps::Model3D("objects/scena/Pluta1.obj", "objects/scena/");
	Pluta2 = gps::Model3D("objects/scena/Pluta2.obj", "objects/scena/");
	Pod = gps::Model3D("objects/scena/Pod.obj", "objects/scena/");
	Prosop = gps::Model3D("objects/scena/Prosop.obj", "objects/scena/");
	Raft1 = gps::Model3D("objects/scena/Raft1A.obj", "objects/scena/");
	Raft2 = gps::Model3D("objects/scena/Raft2A.obj", "objects/scena/");
	Scaun = gps::Model3D("objects/scena/Scaun.obj", "objects/scena/");
	SkyDome = gps::Model3D("objects/scena/skyDome.obj", "objects/scena/");
	Toucan1 = gps::Model3D("objects/scena/Toucan.obj", "objects/scena/");

	Toucan2 = gps::Model3D("objects/scena/Toucan2.obj", "objects/scena/");
	Tun = gps::Model3D("objects/scena/TunA.obj", "objects/scena/");
	Turn1 = gps::Model3D("objects/scena/Turn1A.obj", "objects/scena/");
	Turn2 = gps::Model3D("objects/scena/Turn2A.obj", "objects/scena/");
	Turn3 = gps::Model3D("objects/scena/Turn3A.obj", "objects/scena/");
	Umbrela = gps::Model3D("objects/scena/Umbrela.obj", "objects/scena/");
	
	lightCube = gps::Model3D("objects/scena/soare.obj", "objects/scena/");
	lightCube3 = gps::Model3D("objects/scena/Sfera.obj", "objects/scena/");

}

void initShaders()
{
	
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");


	depthMapShader.loadShader("shaders/simpleDepthMap.vert", "shaders/simpleDepthMap.frag");

	mySkyBox.Load(faces);
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");

	skyboxShader.useShaderProgram();
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE,
		glm::value_ptr(view));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 10000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projection));

}

void initUniforms()
{
	myCustomShader.useShaderProgram();

	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");

	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	
	lightDirMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDirMatrix");

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 10000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));	

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(30.0f, 20.0f, 10.0f);
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	
	spotLightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "direction");
	myCustomShader.useShaderProgram();
	glUniform3fv(spotLightDirLoc, 1, glm::value_ptr(myCamera.getCameraDirection()));

	spotLightPosLoc = glGetUniformLocation(myCustomShader.shaderProgram, "position");
	myCustomShader.useShaderProgram();
	glUniform3fv(spotLightPosLoc, 1, glm::value_ptr(myCamera.getCameraPosition()));

	spotLightCutOffLoc = glGetUniformLocation(myCustomShader.shaderProgram, "cutOff");
	myCustomShader.useShaderProgram();
	glUniform1f(spotLightCutOffLoc, spotLightCutOff);

	spotLightOuterCutOffLoc = glGetUniformLocation(myCustomShader.shaderProgram, "outerCutOff");
	myCustomShader.useShaderProgram();
	glUniform1f(spotLightOuterCutOffLoc, spotLightOuterCutOff);

	ceLuminaVreauLoc = glGetUniformLocation(myCustomShader.shaderProgram, "ceLuminaVreau");
	glUniform1i(ceLuminaVreauLoc, ceLuminaVreau);

	ceataLoc = glGetUniformLocation(myCustomShader.shaderProgram, "ceata");
	glUniform1i(ceataLoc, ceata);
}



void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	processMovement();	

	//render the scene to the depth buffer (first pass)

	depthMapShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
		
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	
	Cufar.Draw(depthMapShader);

	

	//create model matrix for CapacCufar
	          
	model = glm::rotate(glm::mat4(1.0f), glm::radians(unghi), glm::vec3(x, y, z));
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	CapacCufar.Draw(depthMapShader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, cocoZ, 0.0f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	

	if (m > 16.4) {
		Coco.Draw(depthMapShader);
		if (cocoZ > -34.2) {
			cocoZ -= 0.1;
		}
	}
	else {
		cocoZ = 0;
	}
	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	Dog.Draw(depthMapShader);
	Insula1.Draw(depthMapShader);
	Insula2.Draw(depthMapShader);
	Insula3.Draw(depthMapShader);
	Insula4.Draw(depthMapShader);
	Insula5.Draw(depthMapShader);
	Insula6.Draw(depthMapShader);
	Insula7.Draw(depthMapShader);
	InsulaMare.Draw(depthMapShader);
	Monkey1.Draw(depthMapShader);
	Monkey2.Draw(depthMapShader);
	Monkey3.Draw(depthMapShader);


	model = glm::translate(glm::mat4(1.0f), glm::vec3(m, 0.0f, 0.0f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	Monkey4.Draw(depthMapShader);

	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	Oceanul.Draw(depthMapShader);
	Pirat.Draw(depthMapShader);
	Pluta1.Draw(depthMapShader);
	Pluta2.Draw(depthMapShader);
	Pod.Draw(depthMapShader);
	Prosop.Draw(depthMapShader);
	Raft1.Draw(depthMapShader);

	//create model matrix for nanosuit
	
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, y));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	Raft2.Draw(depthMapShader);

	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	Scaun.Draw(depthMapShader);
	SkyDome.Draw(depthMapShader);
	Toucan1.Draw(depthMapShader);
	Toucan2.Draw(depthMapShader);
	Tun.Draw(depthMapShader);
	Turn1.Draw(depthMapShader);
	Turn2.Draw(depthMapShader);
	Turn3.Draw(depthMapShader);
	Umbrela.Draw(depthMapShader);
	


	//create model matrix for ground
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"), 
						1, 
						GL_FALSE, 
						glm::value_ptr(model));

	ground.Draw(depthMapShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//render the scene (second pass)

	myCustomShader.useShaderProgram();

	//send lightSpace matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));

	//send view matrix to shader
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "view"),
		1,
		GL_FALSE,
		glm::value_ptr(view));	

	//compute light direction transformation matrix
	lightDirMatrix = glm::mat3(glm::inverseTranspose(view));
	//send lightDir matrix data to shader
	glUniformMatrix3fv(lightDirMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightDirMatrix));

	glViewport(0, 0, retina_width, retina_height);
	myCustomShader.useShaderProgram();

	//bind the depth map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);
	
	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	
	Cufar.Draw(myCustomShader);

	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(unghi), glm::vec3(0,1,0));
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	CapacCufar.Draw(myCustomShader);

	//create model matrix for nanosuit
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, cocoZ, 0.0f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	

	if (m > 16.4) {
		Coco.Draw(myCustomShader);
		if (cocoZ > -34.2) {
			cocoZ -= 0.1;
		}
	}
	else {
		cocoZ = 0.0f;
	}


	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


	Dog.Draw(myCustomShader);
	Insula1.Draw(myCustomShader);
	Insula2.Draw(myCustomShader);
	Insula3.Draw(myCustomShader);
	Insula4.Draw(myCustomShader);
	Insula5.Draw(myCustomShader);
	Insula6.Draw(myCustomShader);
	Insula7.Draw(myCustomShader);
	InsulaMare.Draw(myCustomShader);
	Monkey1.Draw(myCustomShader);
	Monkey2.Draw(myCustomShader);
	Monkey3.Draw(myCustomShader);

	//create model matrix for nanosuit
	model = glm::translate(glm::mat4(1.0f), glm::vec3(m, 0.0f, 0.0f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	Monkey4.Draw(myCustomShader);

	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	Oceanul.Draw(myCustomShader);
	Pirat.Draw(myCustomShader);
	Pluta1.Draw(myCustomShader);
	Pluta2.Draw(myCustomShader);
	Pod.Draw(myCustomShader);
	Prosop.Draw(myCustomShader);
	Raft1.Draw(myCustomShader);

	//create model matrix for nanosuit
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, y));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	Raft2.Draw(myCustomShader);


	//create model matrix for nanosuit
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	Scaun.Draw(myCustomShader);
	SkyDome.Draw(myCustomShader);
	Toucan1.Draw(myCustomShader);
	Toucan2.Draw(myCustomShader);
	Tun.Draw(myCustomShader);
	Turn1.Draw(myCustomShader);
	Turn2.Draw(myCustomShader);
	Turn3.Draw(myCustomShader);
	Umbrela.Draw(myCustomShader);
		
	//create model matrix for ground
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//send model matrix data to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//create normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	ground.Draw(myCustomShader);

	//draw a white cube around the light

	lightShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	model = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, lightDir);
	//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	lightCube.Draw(lightShader);
	lightCube2.Draw(lightShader);
	//lightCube3.Draw(lightShader);


	mySkyBox.Draw(skyboxShader, view, projection);
	

		for (int i = 0; i < nrPicuri; i++) {
			float x = static_cast<float> (rand()) / (static_cast<float>(RAND_MAX / 1319.06));
			float y = static_cast<float> (rand()) / (static_cast<float>(RAND_MAX / 1319.06));
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(x, 0.0f, y));
			model = glm::translate(model, glm::vec3(0.0f, -inaltime, 0.0f));

			inaltime += 0.5;
			if (inaltime > 500) {
				inaltime = 0;
			}
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			//create normal matrix 
			normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
			//send normal matrix data to shader
			glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


			Ploaie[i].Draw(myCustomShader);

		
	}
	
		if (pressedKeys[GLFW_KEY_LEFT_CONTROL]) {

				float readA, readB, readC, readD, readE, readF;
				if (!file.eof()) {
						file >> readA >> readB >> readC >> readD >> readE >> readF;
						myCamera.moveFreely(glm::vec3(readD, readE, readF), glm::vec3(readA, readB, readC), 0);
				}
		}

	
}

int main(int argc, const char * argv[]) {

	myfile.open("direction.txt");
	myfile2.open("position.txt");

	faces.push_back("skybox/right.tga");
	faces.push_back("skybox/left.tga");
	faces.push_back("skybox/top.tga");
	faces.push_back("skybox/bottom.tga");
	faces.push_back("skybox/back.tga");
	faces.push_back("skybox/front.tga");

	initOpenGLWindow();
	initOpenGLState();
	initFBOs();
	initModels();
	initShaders();
	initUniforms();	
	glCheckError();
	while (!glfwWindowShouldClose(glWindow)) {
		renderScene();
		glfwSetCursorPosCallback(glWindow, mouseCallback);

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	//close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}
