#pragma once

#include <GL/glew.h>

class texture;

enum textureType {
	texture1DType = GL_TEXTURE_1D,
	texture2DType = GL_TEXTURE_2D,
	texture3DType = GL_TEXTURE_3D,
	texture1DArrayType = GL_TEXTURE_1D_ARRAY,
	texture2DArrayType = GL_TEXTURE_2D_ARRAY,
	textureRectangleType = GL_TEXTURE_RECTANGLE,
	textureCubeMapType = GL_TEXTURE_CUBE_MAP,
	texture2DMultisampleType = GL_TEXTURE_2D_MULTISAMPLE,
	texture2DMultisampleArrayType = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
	textureBufferType = GL_TEXTURE_BUFFER,
	textureCubeMapArrayType = GL_TEXTURE_CUBE_MAP_ARRAY
};

class texture {
	texture() {
		glGenTextures(1, &glTexture);

		instanceCounter = new unsigned int;
		*instanceCounter = 1;
	}

	// Copy constructer (for instance counter)
	texture(const texture &other) {
		this->glTexture = other.glTexture;
		this->instanceCounter = other.instanceCounter;
		(*instanceCounter)++;
	}

	~texture() {
		(*instanceCounter)--;
		if (*instanceCounter <= 0) {
			// All instances destroyed
			glDeleteTextures(1, &glTexture);
			delete instanceCounter;
		}
	}
private:
	GLuint glTexture;
	unsigned int *instanceCounter;
};