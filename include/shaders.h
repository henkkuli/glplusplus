#pragma once

#include <GL/glew.h>
#include <fstream>

using namespace std;

class shader;
class program;
class uniform;

#include "math/mat4.h"
#include "math/vec.h"

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
	
	int const& operator=(int const &value) {
		glUniform1i(uniformLocation, value);
		return value;
	}

	float const& operator=(float const &value) {
		glUniform1f(uniformLocation, value);
		return value;
	}
	
	double const& operator=(double const &value) {
		glUniform1d(uniformLocation, value);
		return value;
	}

private:
	uniform(GLuint _location) : uniformLocation(_location) {}
	GLuint uniformLocation;

	friend class program;
};

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
		int shaderFileLength = shaderFile.tellg();			// Get estimation of file size (allways big enough?)
		shaderFile.seekg(0, ios::beg);
		char *shaderSource = new char[shaderFileLength+1];	// Reserve enough memory for the whole file
		shaderFile.read(shaderSource, shaderFileLength);
		shaderFileLength = shaderFile.gcount();				// Get real file length
		shaderSource[shaderFileLength] = 0;					// Null terminator

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