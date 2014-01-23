#pragma once

#include <GL/glew.h>
#include <fstream>
#include <string>

using namespace std;

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
enum textureParameterType {
	parameterDepthStencilTextureMode = GL_DEPTH_STENCIL_TEXTURE_MODE,
	parameterBaseLevel = GL_TEXTURE_BASE_LEVEL,
	parameterCompareFunc = GL_TEXTURE_COMPARE_FUNC,
	parameterCompareMode = GL_TEXTURE_COMPARE_MODE,
	parameterLodBias = GL_TEXTURE_LOD_BIAS,
	parameterMinFilter = GL_TEXTURE_MIN_FILTER,
	parameterMagFilter = GL_TEXTURE_MAG_FILTER,
	parameterMinLod = GL_TEXTURE_MIN_LOD,
	parameterMaxLod = GL_TEXTURE_MAX_LOD,
	parameterMaxLevel = GL_TEXTURE_MAX_LEVEL,
	parameterSwizzleR = GL_TEXTURE_SWIZZLE_R,
	parameterSwizzleG = GL_TEXTURE_SWIZZLE_G,
	parameterSwizzleB = GL_TEXTURE_SWIZZLE_B,
	parameterSwizzleA = GL_TEXTURE_SWIZZLE_A,
	parameterWrapS = GL_TEXTURE_WRAP_S,
	parameterWrapT = GL_TEXTURE_WRAP_T,
	parameterWrapR = GL_TEXTURE_WRAP_R
};

class texture {
public:
	texture(textureType _type) : type(_type) {
		glGenTextures(1, &glTexture);

		instanceCounter = new unsigned int;
		*instanceCounter = 1;
	}

	// Copy constructer (for instance counter)
	texture(const texture &other) : type(other.type) {
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

	void bind(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(type, glTexture);
	}

	texture setParameter(textureParameterType param, GLint value) {
		bind(0);
		glTexParameteri(type, param, value);
		//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		return *this;
	}

	void loadBmp(const char *filename) {
#pragma pack(push, 1)
		struct {
			char type[2];
			unsigned int size;
			unsigned short unused1;
			unsigned short unused2;
			unsigned int dataOffset;
			// DIB
			unsigned int dibSize;
			unsigned int width;
			unsigned int height;
			unsigned short planes;
			unsigned short bitsPerPixel;
			unsigned int compression;
			unsigned int dataSize;
			unsigned int xPixelsPerMeter;
			unsigned int yPixelsPerMeter;
			unsigned int paletteSize;
			unsigned int importantColors;
		} header;
#pragma pack(pop)

		ifstream bmpFile(filename, ios::in | ios::binary);
		bmpFile.read((char *) &header, sizeof(header));
		if (!bmpFile)
			throw string("Error while reading the file");

		char *data = new char[header.dataSize];

		switch (header.bitsPerPixel)
		{
		case 24:
		{
			if (header.compression != 0)
				throw string("Unsupported compression");
			bmpFile.seekg(header.dataOffset);		// Move to right offset
			bmpFile.read(data, header.dataSize);
			if (header.dataSize != bmpFile.gcount())
				throw string("Wasn't able to read enough data from file");
			if (!bmpFile)
				throw string("Error while reading the file");

			if (header.width % 4 != 0)		// TODO Handle other cases
				throw string("Image width is not multiple of 4");
			bind(0);
			glTexImage2D(
				type,
				0,
				GL_RGBA,
				header.width,
				header.height,
				0,
				GL_BGR,
				GL_UNSIGNED_BYTE,
				data);

			break;
		}
		case 32:
		{
			if (header.compression != 3)
				throw string("Unsupported compression");
			bmpFile.seekg(header.dataOffset);		// Move to right offset
			bmpFile.read(data, header.dataSize);
			if (header.dataSize != bmpFile.gcount())
				throw string("Wasn't able to read enough data from file");
			if (!bmpFile)
				throw string("Error while reading the file");

			bind(0);
			glTexImage2D(
				type,
				0,
				GL_RGBA,
				header.width,
				header.height,
				0,
				GL_ABGR_EXT,			// TODO Try to find somethin not extension
				GL_UNSIGNED_BYTE,
				data);

			break;
		}
		default:
			throw string("Unsupported amount of bits per pixel");
			break;
		}

		delete[] data;
	}
private:
	GLuint glTexture;
	const textureType type;
	unsigned int *instanceCounter;
};