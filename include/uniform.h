#pragma once

class uniform;

#include <GL/glew.h>

#include "math/mat4.h"
#include "math/vec2.h"
//#include "math/vec3.h"
//#include "math/vec4.h"
class program;
//#include "program.h"

class uniform {
public:
	math::mat4 const& operator=(math::mat4 const &value) {
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value.h);
		return value;
	}
	math::vec2 const& operator=(math::vec2 const &value) {
		glUniform2fv(uniformLocation, 1, value.h);
		return value;
	}
	math::vec3 const& operator=(math::vec3 const &value) {
		glUniform3fv(uniformLocation, 1, value.h);
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