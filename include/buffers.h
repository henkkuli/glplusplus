#pragma once

#include <GL/glew.h>

class buffer;
class vao;

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
	 * Initializes new buffer without default target
	 *
	 * NOTE: Does not bind the buffer to anything. The driver may optimize the buffer based on the type of the first bind
	 */
	buffer() {
		me = new _bufferData;

		// Initialize buffer
		glGenBuffers(1, &me->glBuffer);
		me->instanceCounter = 1;
	}

	/*!
	 * Initializes new buffer
	 *
	 * NOTE: Binds the created buffer as default target
	 *
	 *
	 * \param default type of buffer as which the buffer should mainly be used
	 */
	buffer(bufferTarget defaultTarget) {
		me = new _bufferData;

		// Initialize buffer
		glGenBuffers(1, &me->glBuffer);
		bind(defaultTarget);				// Bind to default target for driver optimisation
		me->instanceCounter = 1;
	}

	/*!
	 * Safely copies buffer object
	 *
	 *
	 * \param the object to be copied here
	 */
	buffer(const buffer &other) {
		me = other.me;
		me->instanceCounter++;
	}

	/*!
	 * Safely assigns buffer object
	 *
	 *
	 * \param the object to be assigned here
	 */
	buffer& operator=(const buffer &other) {
		// Remove the old
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteBuffers(1, &me->glBuffer);
			delete me;
		}
		// Assign new
		me = other.me;
		me->instanceCounter++;
		return *this;
	}

	/*!
	 * Destructs the buffer object. If no object refers to this particular buffer, destroys it from the OpenGL memory also.
	 */
	~buffer() {
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteBuffers(1, &me->glBuffer);
			delete me;
		}
	}

	/*!
	 * Allocates new buffer with data specified by data
	 *
	 * NOTE: Binds buffer as array buffer
	 *
	 *
	 * \param data to be written into buffer
	 * \param count of elements in array.
	 * \param size of an element
	 */
	template <class T>
	void allocate(T *data, int count, int elementSize) {
		bind(arrayBuffer);
		me->elementSize = elementSize;
		me->bufferSize = count * elementSize;
		glBufferData(arrayBuffer, me->bufferSize, data, GL_STATIC_DRAW);
	}

	/*!
	 * Allocates new buffer with data specified by data
	 * Equivalent of calling allocate(data, count, sizeof(T))
	 *
	 * NOTE: Element size is computed as sizeof(T). Use void allocate(T *data, int count, int elementSize) if you want to set element size
	 * NOTE: Binds buffer as array object
	 *
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
	 * NOTE: Binds buffer as array object
	 *
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
	 * NOTE: Binds buffer as array object
	 *
	 *
	 * \param data to be written into buffer
	 */
	template <class T>
	void setData(T *data) {
		setData(data, 0, me->bufferSize);
	}

	/*!
	 * Binds buffer for use
	 *
	 *
	 * \param OpenGl buffer target
	 */
	void bind(bufferTarget target) {
		glBindBuffer(target, me->glBuffer);
	}

	/*!
	 * Binds buffer to an indexed target
	 *
	 *
	 * \param OpenGl buffer target
	 * \param index in target buffer
	 */
	void bind(bufferTarget target, GLuint index) {		
		glBindBufferBase(target, index, me->glBuffer);
	}

private:
	struct _bufferData {
		GLuint glBuffer;
		GLsizei elementSize, bufferSize;
		unsigned int instanceCounter;
	} *me;

	friend class vao;
};

class vao {
public:
	/*!
	 * Initializes new Vertex Array Object
	 */
	vao() {
		me = new _vaoData;

		// Initialize vao
		glGenVertexArrays(1, &me->glVao);
		me->instanceCounter = 1;
	}
	
	/*!
	 * Safely copies vao object
	 *
	 *
	 * \param the object to be copied here
	 */
	vao(const vao &other) {
		me = other.me;
		me->instanceCounter++;
	}

	/*!
	 * Safely assigns vao object
	 *
	 *
	 * \param the object to be assigned here
	 */
	vao& operator=(const vao &other) {
		// Remove the old
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteVertexArrays(1, &me->glVao);
			delete me;
		}
		// Assign new
		me = other.me;
		me->instanceCounter++;
		return *this;
	}
	
	/*!
	 * Destructs the vao object. If no object refers to this particular vao, destroys it from the OpenGL memory also.
	 */
	~vao() {
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteVertexArrays(1, &me->glVao);
			delete me;
		}
	}

	/*!
	 * Sets vertex attribute point to buffer
	 *
	 * NOTE: Binds this Vertex Array Object to use
	 *
	 *
	 * \param index of vertex attribute (must be same as in shaders)
	 * \param components per one attribute. Must be 1, 2, 3 or 4
	 * \param buffer to be added
	 */
	void setAttribute(const GLuint index, GLint size, buffer &buf) {
		bind();
		glEnableVertexAttribArray(index);
		buf.bind(arrayBuffer);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, buf.me->elementSize, 0);
	}

	/*!
	 * Sets vertex attribute point to buffer
	 *
	 * NOTE: Binds this Vertex Array Object to use
	 *
	 *
	 * \param index of vertex attribute (must be same as in shaders)
	 * \param components per one attribute. Must be 1, 2, 3 or 4
	 * \param buffer to be added
	 * \param offset of the first element in buffer
	 */
	void setAttribute(const GLuint index, const GLint size, buffer &buf, const int offset) {
		bind();
		glEnableVertexAttribArray(index);
		buf.bind(arrayBuffer);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, buf.me->elementSize, (void*) offset);
	}

	/*!
	 * Sets a buffer to a specific target for this Vertex Array Object
	 *
	 * NOTE: binds the buffer to the target
	 *
	 *
	 * \param buffer to be added
	 * \param target for the buffer
	 */
	void setBuffer(buffer &buf, const bufferTarget target) {
		buf.bind(target);
	}

	/*!
	 * Binds this Vertex Array Object to use
	 */
	void bind() {
		glBindVertexArray(me->glVao);
	}

private:
	struct _vaoData {
		GLuint glVao;
		unsigned int instanceCounter;
	} *me;
};