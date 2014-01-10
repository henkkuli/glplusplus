#pragma once

#include <GL/glew.h>

class vbo;

#include "math/vec3.h"
#include "math/vec4.h"

using namespace math;

enum bufferTarget {
	arrayBuffer = GL_ARRAY_BUFFER,
	elementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
	copyReadBuffer = GL_COPY_READ_BUFFER,
	copyWriteBuffer = GL_COPY_WRITE_BUFFER,
	pixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
	pixelPackBuffer = GL_PIXEL_PACK_BUFFER,
	queryBuffer = GL_QUERY_BUFFER,
	textureBuffer = GL_TEXTURE_BUFFER,
	transformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
	uniformBuffer = GL_UNIFORM_BUFFER,
	drawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER,
	atomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,
	dispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER,
	shaderStorageBuffer = GL_SHADER_STORAGE_BUFFER
};

class buffer {
public:
	/*!
	 * Initializes new buffer
	 *
	 * \param default type of buffer as which the buffer should mainly be used
	 */
	buffer(bufferTarget defaultTarget) {
		glGenBuffers(1, &glBuffer);
		bind(defaultTarget);

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
	 * Allocates new buffer with data specified by data
	 *
	 * \param data to be written into buffer
	 * \param count of elements in array.
	 * \param size of an element
	 */
	template <class T>
	void allocate(T *data, int count, int elementSize) {
		bind(arrayBuffer);
		this->elementSize = elementSize;
		this->bufferSize = count * elementSize;
		glBufferData(arrayBuffer, this->bufferSize, data, GL_STATIC_DRAW);
	}

	/*!
	 * Allocates new buffer with data specified by data
	 * Equivalent of calling allocate(data, count, sizeof(T))
	 *
	 * NOTE: Element size is computed as sizeof(T). Use void allocate(T *data, int count, int elementSize) if you want to set element size
	 *
	 * \param data to be written into buffer
	 * \param count of elements in array.
	 */
	template <class T>
	void allocate(T *data, int count) {
		allocate(data, count, sizeof(T));
	}

	/*!
	 * Sets data to buffer without reallocating it
	 *
	 * \param data to be written into buffer
	 * \param starting index of buffer in bytes
	 * \param count of bytes to be written
	 */
	template <class T>
	void setData(T *data, int offset, int size) {
		bind(arrayBuffer);
		glBufferSubData(arrayBuffer, offset, size, data);
	}

	/*!
	 * Replaces all data in buffer whitout reallocating it
	 * Size of data must be equal to the size of buffer
	 * Equivalent of calling setData(data, 0, bufferSize)
	 *
	 * \param data to be written into buffer
	 */
	template <class T>
	void setData(T *data) {
		setData(data, 0, bufferSize);
	}

	/*!
	 * Binds buffer for use
	 *
	 * \param OpenGl buffer target
	 */
	void bind(bufferTarget target) {
		glBindBuffer(target, glBuffer);
	}

	/*!
	 * Binds buffer to an indexed target
	 * 
	 * \param OpenGl buffer target
	 * \param index in target buffer
	 */
	void bind(bufferTarget target, GLuint index) {		
		glBindBufferBase(target, index, glBuffer);
	}

private:
	GLuint glBuffer;
	GLsizei elementSize, bufferSize;
	unsigned int *instanceCounter;

	friend class vao;
};
