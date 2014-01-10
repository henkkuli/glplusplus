#pragma once

class program;

#include <GL/glew.h>

#include "shader.h"
#include "uniform.h"

enum barrierType {
	vertexAttribArrayBarrier = GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
	elementArrayBarrier = GL_ELEMENT_ARRAY_BARRIER_BIT,
	uniformBarrier = GL_UNIFORM_BARRIER_BIT,
	textureFetchBarrier = GL_TEXTURE_FETCH_BARRIER_BIT,
	shaderImageAccessBarrier = GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,
	commandBarrier = GL_COMMAND_BARRIER_BIT,
	pixelBufferBarrier = GL_PIXEL_BUFFER_BARRIER_BIT,
	textureUpdateBarrier = GL_TEXTURE_UPDATE_BARRIER_BIT,
	bufferUpdateBarrier = GL_BUFFER_UPDATE_BARRIER_BIT,
	queryBufferBarrier = GL_QUERY_BUFFER_BARRIER_BIT,
	clientMappedBuffer = GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT,
	framebufferBarrier = GL_FRAMEBUFFER_BARRIER_BIT,
	transformFeedbackBarrier = GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
	atomicCounterBarrier = GL_ATOMIC_COUNTER_BARRIER_BIT,
	shaderStorageBarrier = GL_SHADER_STORAGE_BARRIER_BIT,
	allBarrier = GL_ALL_BARRIER_BITS
};

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

	/*!
	 * Dispatches the compute shader
	 *
	 * NOTE: Program must contain a compute shader.
	 * NOTE: Requires OpenGl 4.3 or higher
	 *
	 * \param number of groups in x direction
	 * \param number of groups in y direction
	 * \param number of groups in z direction
	 */
	void dispatch(GLuint x, GLuint y, GLuint z) {
		use();		// Make sure that right program is used
		glDispatchCompute(x, y, z);
	}

	/*!
	 * Sets memory barriers for buffers used by compute shader
	 * Barriers are used to ensure that everything touched by compute shader is ready when needed
	 */
	void barrier(barrierType type) {
		glMemoryBarrier(type);
	}

	/*!
	 * Links the shader program
	 */
	void link() {
		glLinkProgram(this->glProgram);

		GLint linked = GL_FALSE;
		glGetProgramiv(this->glProgram, GL_LINK_STATUS, &linked);
		int logLength;
		glGetProgramiv(this->glProgram, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength];
		glGetProgramInfoLog(this->glProgram, logLength, 0, log);
		if (!linked) {
			glDeleteProgram(this->glProgram);
			throw log;
		}
		delete[] log;
	}

	/*!
	 * Gets program uniform by name
	 *
	 * \param name of the uniform in shader
	 */
	uniform getUniform(const char *name) const {
		GLint location = glGetUniformLocation(glProgram, name);
		if (location == -1)
			throw "Didn't find uniform";
		return uniform(location);
	}

private:
	GLuint glProgram;
	unsigned int *instanceCounter;
};

