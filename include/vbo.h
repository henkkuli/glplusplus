#pragma once

#include <gl\glew.h>

class vbo;

#include "math\vec3.h"
#include "math\vec4.h"

using namespace math;

class vbo {
public:
	vbo(const char _size) : size(_size) {
		glGenBuffers(1, &glVbo);

		// Create new instance of the vbo
		instanceCounter = new unsigned int;
		*instanceCounter = 1;
	}

	// Copy constructer (for instance counter)
	vbo(const vbo &other) : size(other.size) {
		this->glVbo = other.glVbo;
		this->instanceCounter = other.instanceCounter;
		(*instanceCounter)++;
	}

	~vbo() {
		(*instanceCounter)--;
		if (*instanceCounter <= 0) {
			// All instances destroyed
			glDeleteBuffers(1, &glVbo);
			delete instanceCounter;
		}
	}
	
	void setData(math::vec3 *data, int count) {
		bindIfNeeded();
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(math::vec3), data, GL_STATIC_DRAW);
	}
	
	void setData(math::vec4 *data, int count) {
		bindIfNeeded();
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(math::vec4), data, GL_STATIC_DRAW);
	}

	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER, glVbo);
		binded = glVbo;
	}
	void bindIfNeeded() {
		if (binded != glVbo)
			bind();
	}
	
	const char size;
private:
	GLuint glVbo;
	unsigned int *instanceCounter;

	static GLuint binded;
};