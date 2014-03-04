#pragma once

#include <GL/glew.h>
#include <fstream>

using namespace std;

class shader;
class program;
class uniform;

#include "math/mat.h"
#include "math/vec.h"

class uniform {
public:
	/*!
	 * Dummy default constructor
	 */
	uniform() : uniformLocation(-1) {}

	/*!
	 * Sets shader program uniform to particular value
	 *
	 *
	 * \param value to be send to the shader program
	 */
	math::mat4 const& operator=(math::mat4 const &value) {
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, value.h);
		return value;
	}
	
	/*!
	 * Sets shader program uniform to particular value
	 *
	 *
	 * \param value to be send to the shader program
	 */
	math::vec2 const& operator=(math::vec2 const &value) {
		glUniform2fv(uniformLocation, 1, (GLfloat*) &value);
		return value;
	}
	
	/*!
	 * Sets shader program uniform to particular value
	 *
	 *
	 * \param value to be send to the shader program
	 */
	math::vec3 const& operator=(math::vec3 const &value) {
		glUniform3fv(uniformLocation, 1, (GLfloat*)&value);
		return value;
	}
	
	/*!
	 * Sets shader program uniform to particular value
	 *
	 *
	 * \param value to be send to the shader program
	 */
	int const& operator=(int const &value) {
		glUniform1i(uniformLocation, value);
		return value;
	}
	
	/*!
	 * Sets shader program uniform to particular value
	 *
	 *
	 * \param value to be send to the shader program
	 */
	float const& operator=(float const &value) {
		glUniform1f(uniformLocation, value);
		return value;
	}
	
	/*!
	 * Sets shader program uniform to particular value
	 *
	 *
	 * \param value to be send to the shader program
	 */
	double const& operator=(double const &value) {
		glUniform1d(uniformLocation, value);
		return value;
	}

	/*!
	 * Just copies the uniform location of value to this
	 *
	 *
	 * \param uniform to be assigned
	 */
	uniform const& operator=(uniform const &value) {
		this->uniformLocation = value.uniformLocation;
		return *this;
	}

	/*!
	 * Sets shader program uniform to particular value
	 *
	 *
	 * \param array of values to be send to the shader program
	 * \param count of elements in the array
	 */
	void setArray(math::mat4 const *array, unsigned int count) {
		glUniformMatrix4fv(uniformLocation, count, GL_FALSE, (GLfloat*) array);
	}

private:
	/*!
	 * Private constructor for shader objects to create uniforms
	 *
	 *
	 * \param value to be send to the shader program
	 */
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
	/*!
	 * Constructs a new shader of a given type from a file.
	 *
	 *
	 * \param name of the shader file
	 * \param type of the shader
	 */
	shader(const char *filename, shaderType type) {
		me = new _shaderData;

		me->glShader = glCreateShader(type);
		me->type = type;

		// Open file
		ifstream shaderFile(filename, ios::in);
		if (!shaderFile.is_open())
			throw string("Couldn't open shader file");

		// Load to memory
		shaderFile.seekg(0, ios::end);
		int shaderFileLength = (int) shaderFile.tellg();	// Get estimation of file size (allways big enough?)
		shaderFile.seekg(0, ios::beg);
		char *shaderSource = new char[shaderFileLength+1];	// Reserve enough memory for the whole file
		shaderFile.read(shaderSource, shaderFileLength);
		shaderFileLength = (int) shaderFile.gcount();		// Get real file length
		shaderSource[shaderFileLength] = 0;					// Null terminator

		char const *shaderSourceConst = shaderSource;
		glShaderSource(me->glShader, 1, &shaderSourceConst, &shaderFileLength);
		glCompileShader(me->glShader);

		// Check log
		GLint compiled = GL_FALSE;
		glGetShaderiv(me->glShader, GL_COMPILE_STATUS, &compiled);
		int logLength;
		glGetShaderiv(me->glShader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength];
		glGetShaderInfoLog(me->glShader, logLength, 0, log);
		if (!compiled) {
			glDeleteShader(me->glShader);
			throw string(log);		// Didn't compile
		}

		// Clean up
		delete[] shaderSource;
		delete[] log;

		// Initialize instance counter
		me->instanceCounter = 1;
	}

	/*!
	 * Safely copies shader object
	 *
	 *
	 * \param the object to be copied here
	 */
	shader(const shader &other) {
		me = other.me;
		me->instanceCounter++;
	}

	/*!
	 * Safely assigns shader object
	 *
	 *
	 * \param the object to be assigned here
	 */
	shader& operator=(const shader &other) {
		// Remove the old
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteShader(me->glShader);
			delete me;
		}
		// Assign new
		me = other.me;
		me->instanceCounter++;
		return *this;
	}

	/*!
	 * Destructs the shader object. If no object refers to this particular shader, destroys it from the OpenGL memory also.
	 */
	~shader() {
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteShader(me->glShader);
			delete me;
		}
	}

private:
	struct _shaderData {
		GLuint glShader;
		shaderType type;
		unsigned int instanceCounter;
	} *me;

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
	/*!
	 * Constructs a new OpenGL program.
	 */
	program() {
		me = new _programData;

		// Initialize the program
		me->glProgram = glCreateProgram();
		me->instanceCounter = 1;
	}

	/*!
	 * Safely copies program object
	 *
	 *
	 * \param the object to be copied here
	 */
	program(const program &other) {
		me = other.me;
		me->instanceCounter++;
	}

	/*!
	 * Safely assigns program object
	 *
	 *
	 * \param the object to be assigned here
	 */
	program& operator=(const program &other) {
		// Remove the old
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteProgram(me->glProgram);
			delete me;
		}
		// Assign new
		me = other.me;
		me->instanceCounter++;
		return *this;
	}
	/*!
	 * Destructs the program object. If no object refers to this particular program, destroys it from the OpenGL memory also.
	 */
	~program() {
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteProgram(me->glProgram);
			delete me;
		}
	}

	/*!
	 * Attaches a shader to this program.
	 *
	 *
	 * \param shader to be attached
	 */
	void attachShader(const shader &s) {
		glAttachShader(me->glProgram, s.me->glShader);
	}

	/*!
	 * Sets this program as the active program.
	 */
	void use() {
		glUseProgram(me->glProgram);
	}

	/*!
	 * Dispatches the compute shader
	 *
	 * NOTE: Program must contain a compute shader.
	 * NOTE: Requires OpenGl 4.3 or higher
	 * NOTE: Sets this program as the active program
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
	 *
	 *
	 * \param type of the barrier. Can be multiple barrier types orred together
	 */
	void barrier(barrierType type) {
		glMemoryBarrier(type);
	}

	/*!
	 * Links the shader program
	 */
	void link() {
		glLinkProgram(me->glProgram);

		GLint linked = GL_FALSE;
		glGetProgramiv(me->glProgram, GL_LINK_STATUS, &linked);
		int logLength;
		glGetProgramiv(me->glProgram, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength];
		glGetProgramInfoLog(me->glProgram, logLength, 0, log);
		if (!linked) {
			glDeleteProgram(me->glProgram);
			throw string(log);
		}
		delete[] log;
	}

	/*!
	 * Gets program uniform by name
	 *
	 * \param name of the uniform in shader
	 */
	uniform getUniform(const char *name) const {
		GLint location = glGetUniformLocation(me->glProgram, name);
		if (location == -1)
			throw string("Didn't find uniform: ") + name;
		return uniform(location);
	}

private:
	struct _programData {
		GLuint glProgram;
		unsigned int instanceCounter;
	} *me;
};