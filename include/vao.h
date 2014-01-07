#pragma once

#include <gl\glew.h>

class vao;

#include "math\vec3.h"
#include "math\vec4.h"
#include "vbo.h"

using namespace math;

class vao {
public:
	vao() {
		glGenVertexArrays(1, &glVao);

		// Create new instance of the vbo
		instanceCounter = new unsigned int;
		*instanceCounter = 1;
	}

	// Copy constructer (for instance counter)
	vao(const vao &other) {
		this->glVao = other.glVao;
		this->instanceCounter = other.instanceCounter;
		(*instanceCounter)++;
	}

	~vao() {
		(*instanceCounter)--;
		if (*instanceCounter <= 0) {
			// All instances destroyed
			glDeleteVertexArrays(1, &glVao);
			delete instanceCounter;
		}
	}
	
	void setVbo(const GLuint index, vbo &vbo) {
		bindIfNeeded();
		glEnableVertexAttribArray(index);
		vbo.bind();
		glVertexAttribPointer(index, vbo.size, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void bind() {
		glBindVertexArray(glVao);
		binded = glVao;
	}
	void bindIfNeeded() {
		if (binded != glVao)
			bind();
	}

private:
	GLuint glVao;
	unsigned int *instanceCounter;

	static GLuint binded;
};