#pragma once

class uniform;

#include <GL/glew.h>

#include "math/mat4.h"
#include "program.h"

class uniform {
public:
	math::mat4 const& operator=(math::mat4 const &value) {
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value.h);
		return value;
	}

	float const& operator=(float const &value) {
		glUniform1f(uniformLocation, value);
		return value;
	}
private:
	uniform(GLuint _location) : uniformLocation(_location) {}
	GLuint uniformLocation;

	friend class program;
};