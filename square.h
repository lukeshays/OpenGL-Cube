#ifndef SQUARE_H
#define SQUARE_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Square
{
public:

	glm::vec3 position;
	glm::vec3 color;
	glm::mat4 rotationMatrix;
	static GLuint VAO, VBO;
	int modelLoc, colorLoc;
	float rotationRemaining;
	glm::vec3 rotationAxis;
	glm::vec4 xyz;
	int randomTurnsLeft;

	Square(glm::vec3 position, glm::vec3 color, int modelLoc, int ColorLoc);

	void rotateAround(float degrees, glm::vec3 pos, glm::vec3 axis);
	void rotateTop(bool prime = false);
	void rotateRight(bool prime = false);
	void rotateLeft(bool prime = false);
	void rotateFront(bool prime = false);
	void rotateBottom(bool prime = false);
	void draw();
	void initVAO(glm::vec3 color);

};

GLuint Square::VAO = 0;
GLuint Square::VBO = 0;

Square::Square(glm::vec3 _position, glm::vec3 _color, int _modelLoc, int _colorLoc) {
	position = _position;
	color = _color;
	rotationMatrix = glm::mat4(1);
	if (VAO == 0 && VBO == 0) {
		initVAO(color);
	}
	modelLoc = _modelLoc;
	colorLoc = _colorLoc;
	rotationRemaining = 0;
	rotationAxis = glm::vec3();
	xyz = glm::vec4(_position, 1.0f);
	randomTurnsLeft = 0;
}

void Square::rotateAround(float degrees, glm::vec3 pos, glm::vec3 axis) {
    glm::mat4 newRotation = glm::mat4(1);
    //pos = glm::vec3(1.0, 1.0, 1.0);
	newRotation = translate(newRotation, -pos);
	newRotation = glm::rotate(newRotation, degrees, axis);
	newRotation = glm::translate(newRotation, pos);
	rotationMatrix = newRotation * rotationMatrix;
	xyz = rotationMatrix * glm::vec4(position, 1.0);
}

void Square::rotateTop(bool prime) {
	if (round(xyz.y) >= 1.0) {
		rotationRemaining = glm::radians(90.0f);
		if (!prime)
			rotationAxis = glm::vec3(0.0, -1.0, 0.0);
		else
			rotationAxis = glm::vec3(0.0, 1.0, 0.0);
	}
}

void Square::rotateRight(bool prime) {
	if (round(xyz.x) >= 1.0) {
		rotationRemaining = glm::radians(90.0f);
		if (!prime)
			rotationAxis = glm::vec3(-1.0, 0.0, 0.0);
		else
			rotationAxis = glm::vec3(1.0, 0.0, 0.0);
	}
}

void Square::rotateLeft(bool prime) {
	if (round(xyz.x) <= -1.0) {
		rotationRemaining = glm::radians(90.0f);
		if (!prime)
			rotationAxis = glm::vec3(1.0, 0.0, 0.0);
		else
			rotationAxis = glm::vec3(-1.0, 0.0, 0.0);
	}
}

void Square::rotateFront(bool prime) {
	if ((xyz.z) >= -0.8) {
		rotationRemaining = glm::radians(90.0f);
		if (!prime)
			rotationAxis = glm::vec3(0.0, 0.0, -1.0);
		else
			rotationAxis = glm::vec3(0.0, 0.0, 1.0);
	}
}

void Square::rotateBottom(bool prime) {
	if (round(xyz.y) <= -1.0) {
		rotationRemaining = glm::radians(90.0f);
		if (!prime)
			rotationAxis = glm::vec3(0.0, 1.0, 0.0);
		else
			rotationAxis = glm::vec3(0.0, -1.0, 0.0);
	}
}

void Square::draw() {
	glm::mat4 model = glm::mat4(1);
	model = model * rotationMatrix;
	model = glm::translate(model, position);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Square::initVAO(glm::vec3 color) {
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // Bottom Left
		0.5f, -0.5f, 0.0f, // Bottom Right
		-0.5f, 0.5f, 0.0f, // Top Left
		0.5, 0.5f, 0.0f, // Top Right
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	// Create vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

#endif
