#pragma once

class shader;

#include <GL/glew.h>
#include <fstream>

#include "info.h"

using namespace std;

enum shaderType {
	vertexShader = GL_VERTEX_SHADER,
	tesselationControlShade = GL_TESS_CONTROL_SHADER,
	tesselationEvaluationShader = GL_TESS_EVALUATION_SHADER,
	fragmentShader = GL_FRAGMENT_SHADER,
	computeShader = GL_COMPUTE_SHADER,
	geometryShader = GL_GEOMETRY_SHADER
};

class shader {
public:

	shader(const char *filename, shaderType type) {
		this->glShader = glCreateShader(type);
		this->type = type;

		// Open file
		ifstream shaderFile(filename, ios::in);
		if (!shaderFile.is_open())
			throw "Couldn't open shader file";

		// Load to memory
		shaderFile.seekg(0, ios::end);
		int shaderFileLength = shaderFile.tellg();
		shaderFile.seekg(0, ios::beg);
		char *shaderSource = new char[shaderFileLength];
		shaderFile.read(shaderSource, shaderFileLength);

		// Compile
		char const *shaderSourceConst = shaderSource;
		glShaderSource(this->glShader, 1, &shaderSourceConst, &shaderFileLength);
		glCompileShader(this->glShader);

		// Check log
		GLint compiled = GL_FALSE;
		glGetShaderiv(this->glShader, GL_COMPILE_STATUS, &compiled);
		int logLength;
		glGetShaderiv(this->glShader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength];
		glGetShaderInfoLog(this->glShader, logLength, 0, log);
		if (!compiled) {
			glDeleteShader(this->glShader);
			throw log;		// Didn't compile
		}

		// Clean up
		delete[] shaderSource;
		delete[] log;

		// Create new instance of the shader
		instanceCounter = new unsigned int;
		*instanceCounter = 1;
	}

	// Copy constructer (for instance counter)
	shader(const shader &other) {
		this->glShader = other.glShader;
		this->type = other.type;
		this->instanceCounter = other.instanceCounter;
		(*instanceCounter)++;
	}

	~shader() {
		(*instanceCounter)--;
		if (*instanceCounter <= 0) {
			// All instances destroyed
			glDeleteShader(this->glShader);
			delete instanceCounter;
		}
	}

private:
	GLuint glShader;
	shaderType type;
	unsigned int *instanceCounter;

	friend class program;
};
