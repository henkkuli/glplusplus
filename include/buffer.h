#pragma once

#include <GL/glew.h>

class vbo;

#include "math/vec3.h"
#include "math/vec4.h"

using namespace math;

enum bufferTarget {
	array = GL_ARRAY_BUFFER,
	shaderStorage = GL_SHADER_STORAGE_BUFFER
};

class buffer {
public:
	buffer() {
		glGenBuffers(1, &glBuffer);

		// Create new instance of the vbo
		instanceCounter = new unsigned int;
		*instanceCounter = 1;
	}

	// Copy constructer (for instance counter)
	buffer(const buffer &other) {
		this->glBuffer = other.glBuffer;
		this->instanceCounter = other.instanceCounter;
		(*instanceCounter)++;
	}

	~buffer() {
		(*instanceCounter)--;
		if (*instanceCounter <= 0) {
			// All instances destroyed
			glDeleteBuffers(1, &glBuffer);
			delete instanceCounter;
		}
	}

	/*!
	 * Sets buffer data
	 *
	 * \param data to be saved
	 * \param count of elements in array.
	 * \param distance between elements
	 */
	template <class T>
	void setData(T *data, int count, int stride) {
		bind(array);
		this->stride = stride;
		glBufferData(array, count * stride, data, GL_STATIC_DRAW);
	}
	
	/*!
	 * Sets buffer data.
	 * Equivalent of calling setData(data, count, sizeof(T))
	 *
	 * NOTE: Element size is computed as sizeof(T). Use void setData(T *data, int count, int stride) if you want to set element size
	 *
	 * \param data to be saved
	 * \param count of elements in array.
	 */
	template <class T>
	void setData(T *data, int count) {
		setData(data, count, sizeof(T));
	}

	/*!
	 * Binds buffer for use
	 *
	 * \param OpenGl buffer target
	 */
	void bind(bufferTarget target) {
		glBindBuffer(target, glBuffer);
	}
	
private:
	GLuint glBuffer;
	GLsizei stride;
	unsigned int *instanceCounter;

	friend class vao;
};
