#pragma once

class program;

#include <gl\glew.h>

#include "shader.h"

class program {
public:

	program() {
		this->glProgram = glCreateProgram();

		// Create new instance of the program
		instanceCounter = new unsigned int;
		*instanceCounter = 1;
	}

	// Copy constructer (for instance counter)
	program(const program &other) {
		this->glProgram = other.glProgram;
		this->instanceCounter = other.instanceCounter;
		(*instanceCounter)++;
	}

	~program() {
		(*instanceCounter)--;
		if (*instanceCounter <= 0) {
			// All instances destroyed
			glDeleteProgram(this->glProgram);
			delete instanceCounter;
		}
	}

	void attachShader(const shader &s) {
		glAttachShader(this->glProgram, s.glShader);
	}

	void use() {
		glUseProgram(this->glProgram);
	}

	void compile() {
		glLinkProgram(this->glProgram);

		// TODO Check log!!!
	}

private:
	GLuint glProgram;
	unsigned int *instanceCounter;
};

