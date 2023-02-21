#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") // stops command prompt from opening in windows

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>

#include "shader.h"
#include "square.h"

// Globals
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
int CUBE_SPEED = 2;
glm::vec3 XAXIS = glm::vec3(1.0, 0.0, 0.0);
glm::vec3 YAXIS = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 ZAXIS = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 ORIGIN = glm::vec3(0.0, 0.0, 1.5);

std::unordered_map<char, bool> keyMap = { 
	{ '^', false },
	{ 'v', false },
	{ '<', false },
	{ '>', false },
	{ 'u', false },
	{ 'U', false },
	{ 'd', false },
	{ 'D', false },
	{ 'l', false },
	{ 'L', false },
	{ 'r', false },
	{ 'R', false },
	{ 'f', false },
	{ 'F', false },
	{ 'X', false },
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_HEIGHT = height;
	SCREEN_WIDTH = width;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	keyMap['^'] = glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
	keyMap['v'] = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
	keyMap['<'] = glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
	keyMap['>'] = glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
	keyMap['u'] = glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && mods != GLFW_MOD_SHIFT;
	keyMap['U'] = glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && mods == GLFW_MOD_SHIFT;
	keyMap['d'] = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && mods != GLFW_MOD_SHIFT;
	keyMap['D'] = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && mods == GLFW_MOD_SHIFT;
	keyMap['l'] = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && mods != GLFW_MOD_SHIFT;
	keyMap['L'] = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && mods == GLFW_MOD_SHIFT;
	keyMap['r'] = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && mods != GLFW_MOD_SHIFT;
	keyMap['R'] = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && mods == GLFW_MOD_SHIFT;
	keyMap['f'] = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && mods != GLFW_MOD_SHIFT;
	keyMap['F'] = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && mods == GLFW_MOD_SHIFT;
	keyMap['X'] = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && mods == GLFW_MOD_SHIFT;
}

int main()
{
	// Initialized GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
	// Create window using GLFW
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Shader Program Object
	Shader ourShader("./vertex.hlsl", "./fragment.hlsl");

	// Set Callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	// Model View Projection matricies
	// model controlled by individual objects
	glm::mat4 view;
	glm::mat4 projection;
	// Note that we're translating the scene in the reverse direction of where we want to move
	view = glm::lookAt(
		glm::vec3(0.0f, 3.0f, 8.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(45.0f), SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	// Query uniform locations
	int modelLoc = glGetUniformLocation(ourShader.ID, "model");
	int viewLoc = glGetUniformLocation(ourShader.ID, "view");
	int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
	int colorLoc = glGetUniformLocation(ourShader.ID, "ourColor");

	// Create vector of square objects
	std::vector<Square> mySquares;

	// Front Face
	for (int i = 0; i <= 8; i++) {
		mySquares.push_back(Square(
			glm::vec3(-1.0 + i%3, -1.0 + i/3, 0.0),	// Location
			glm::vec3((i+10) / 18.0, 0.0, 0.0),		// Color
			modelLoc, 
			colorLoc
		));
	}
	// Back Face
	for (int i = 0; i <= 8; i++) {
		Square temp = Square(
			glm::vec3(-1.0 + i % 3, -1.0 + i / 3, 0.0),	// Location
			glm::vec3((i + 10) / 18.0, (i+10) / 36.0, 0.0),		// Color
			modelLoc,
			colorLoc
		);
		temp.rotateAround(glm::radians(180.0f), ORIGIN, YAXIS);
		mySquares.push_back(temp);
	}
	// Right Face
	for (int i = 0; i <= 8; i++) {
		Square temp = Square(
			glm::vec3(-1.0 + i % 3, -1.0 + i / 3, 0.0),	// Location
			glm::vec3(0.0, 0.0, (i + 10) / 18.0),		// Color
			modelLoc,
			colorLoc
		);
		temp.rotateAround(glm::radians(90.0f), ORIGIN, YAXIS);
		mySquares.push_back(temp);
	}
	// Left Face
	for (int i = 0; i <= 8; i++) {
		Square temp = Square(
			glm::vec3(-1.0 + i % 3, -1.0 + i / 3, 0.0),	// Location
			glm::vec3(0.0, (i + 10) / 18.0, 0.0),		// Color
			modelLoc,
			colorLoc
		);
		temp.rotateAround(glm::radians(-90.0f), ORIGIN, YAXIS);
		mySquares.push_back(temp);
	}
	// Top Face
	for (int i = 0; i <= 8; i++) {
		Square temp = Square(
			glm::vec3(-1.0 + i % 3, -1.0 + i / 3, 0.0),	// Location
			glm::vec3((i + 10) / 18.0, (i + 10) / 18.0, (i + 10) / 18.0),		// Color
			modelLoc,
			colorLoc
		);
		temp.rotateAround(glm::radians(-90.0f), ORIGIN, XAXIS);
		mySquares.push_back(temp);
	}
	// Bottom Face
	for (int i = 0; i <= 8; i++) {
		Square temp = Square(
			glm::vec3(-1.0 + i % 3, -1.0 + i / 3, 0.0),	// Location
			glm::vec3((i + 10) / 18.0, (i + 10) / 18.0, 0.0),		// Color
			modelLoc,
			colorLoc
		);
		temp.rotateAround(glm::radians(90.0f), ORIGIN, XAXIS);
		mySquares.push_back(temp);
	}

	int output;

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        // initialize debug output 
		glEnable(GL_DEBUG_OUTPUT);
    	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    	//glDebugMessageCallback(glDebugOutput, nullptr);
    	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		bool inMotion = false;
		for (int i = 0; i < mySquares.size(); i++) {
			
			if (mySquares[i].rotationRemaining > 0) {
				inMotion = true;
				//mySquares[i].rotateAround(glm::radians(90.0/CUBE_SPEED) , ORIGIN, mySquares[i].rotationAxis);
				//mySquares[i].rotationRemaining -= glm::radians(90.0/CUBE_SPEED);
				mySquares[i].rotateAround(glm::radians((1/3.0)*CUBE_SPEED) , ORIGIN, mySquares[i].rotationAxis);
				mySquares[i].rotationRemaining -= glm::radians((1/3.0)*CUBE_SPEED);
			}
			mySquares[i].draw();
		}
		if (!inMotion) {
			output = 0 + (rand() % static_cast<int>(9 - 0 + 1));
			for (int i = 0; i < mySquares.size(); i++) {

				if (mySquares[i].randomTurnsLeft > 0) {
					mySquares[i].randomTurnsLeft -= 1;
					switch (output)
					{
					case 0:
						mySquares[i].rotateFront();
						break;
					case 1:
						mySquares[i].rotateFront(true);
						break;
					case 2:
						mySquares[i].rotateTop();
						break;
					case 3:
						mySquares[i].rotateTop(true);
						break;
					case 4:
						mySquares[i].rotateBottom();
						break;
					case 5:
						mySquares[i].rotateBottom(true);
						break;
					case 6:
						mySquares[i].rotateLeft();
						break;
					case 7:
						mySquares[i].rotateLeft(true);
						break;
					case 8:
						mySquares[i].rotateRight();
						break;
					case 9:
						mySquares[i].rotateRight(true);
						break;
					default:
						break;
					}
				}
				else {
					if (keyMap['<']) {
						mySquares[i].rotationAxis = glm::vec3(0.0, -1.0, 0.0);
						mySquares[i].rotationRemaining = glm::radians(90.0);
					}
					else if (keyMap['>']) {
						mySquares[i].rotationAxis = glm::vec3(0.0, 1.0, 0.0);
						mySquares[i].rotationRemaining = glm::radians(90.0);
					}
					else if (keyMap['^']) {
						mySquares[i].rotationAxis = glm::vec3(-1.0, 0.0, 0.0);
						mySquares[i].rotationRemaining = glm::radians(90.0);
					}
					else if (keyMap['v']) {
						mySquares[i].rotationAxis = glm::vec3(1.0, 0.0, 0.0);
						mySquares[i].rotationRemaining = glm::radians(90.0);
					}
					else if (keyMap['u']) mySquares[i].rotateTop();
					else if (keyMap['U']) mySquares[i].rotateTop(true);
					else if (keyMap['r']) mySquares[i].rotateRight();
					else if (keyMap['R']) mySquares[i].rotateRight(true);
					else if (keyMap['l']) mySquares[i].rotateLeft();
					else if (keyMap['L']) mySquares[i].rotateLeft(true);
					else if (keyMap['f']) mySquares[i].rotateFront();
					else if (keyMap['F']) mySquares[i].rotateFront(true);
					else if (keyMap['d']) mySquares[i].rotateBottom();
					else if (keyMap['D']) mySquares[i].rotateBottom(true);
					else if (keyMap['X']) mySquares[i].randomTurnsLeft = 30;
				}
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
