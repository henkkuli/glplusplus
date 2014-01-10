#pragma once

#include <GL/glew.h>

class vao;

#include "math/vec3.h"
#include "math/vec4.h"
#include "buffer.h"

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

	/*!
	 * Sets vertex attribute pointed by buffer
	 *
	 *
	 * \param index of vertex attribute (must be same as in shaders)
	 * \param components per one attribute. Must be 1, 2, 3 or 4
	 * \param buffer to be added
	 */
	void setAttribute(const GLuint index, GLint size, buffer &buf) {
		bindIfNeeded();
		glEnableVertexAttribArray(index);
		buf.bind(arrayBuffer);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, buf.elementSize, 0);
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
