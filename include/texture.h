#pragma once

#include <GL/glew.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class texture;
class textureLoader;

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

	texture(textureType type) {
		me = new _textureData;

		// Initialize texture
		glGenTextures(1, &me->glTexture);
		me->type = type;
		me->instanceCounter = 1;
	}

	/*!
	 * Safely copies texture object
	 *
	 *
	 * \param the object to be copied here
	 */
	texture(const texture &other) {
		me = other.me;
		me->instanceCounter++;
	}

	/*!
	 * Destructs the texture object. If no object refers to this particular texture, destroys it from the OpenGL memory also.
	 */
	~texture() {
		me->instanceCounter--;
		if (me->instanceCounter <= 0) {
			// All instances destroyed
			glDeleteTextures(1, &me->glTexture);
			delete me;
		}
	}

	/*!
	 * Binds texture to the specified texture slot (eg. GL_TEXTURE0, GL_TEXTURE15...)
	 *
	 *
	 * \param the slot to bind texture into
	 */
	void bind(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(me->type, me->glTexture);
	}

	/*!
	 * Sets a parameter for this texture.
	 *
	 * NOTE: Binds texture to the slot 0
	 *
	 *
	 * \param OpenGL texture parameter type
	 * \param value
	 */
	texture setParameter(textureParameterType param, GLint value) {
		bind(0);
		glTexParameteri(me->type, param, value);
		return *this;
	}

	/*!
	 * DEPRICATED!!! Going to be removed in the future and replaced with new textureLoader class!!!
	 *
	 * Loads a texture from a bmp-file. Supports only single file
	 *
	 * \param filename
	 */
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
				me->type,
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
				me->type,
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
	struct _textureData {
		GLuint glTexture;
		textureType type;
		unsigned int instanceCounter;
	} *me;

	friend class textureLoader;
};

#define PNG_CHUNCK_IHDR (0x49484452)
#define PNG_CHUNCK_PLTE (0x504C5445)
#define PNG_CHUNCK_IDAT (0x49444154)
#define PNG_CHUNCK_IEND (0x49454E44)
#define PNG_BUFFER_LENGTH 3

class pngLoader {
public:
	pngLoader(string path) : ifs(path, ios::in | ios::binary) {
		buffer = new unsigned char[PNG_BUFFER_LENGTH];

		// Create fixed codes tree
		unsigned char codeLengths[288];
		for (int i = 0; i <= 143; i++)
			codeLengths[i] = 8;
		for (int i = 144; i <=255; i++)
			codeLengths[i] = 9;
		for (int i = 256; i <= 279; i++)
			codeLengths[i] = 7;
		for (int i = 280; i <= 287; i++)
			codeLengths[i] = 8;
		fixedLiteralCode = buildCodeTree(codeLengths, 288);

		for (int i = 0; i < 32; i++)
			codeLengths[i] = 5;
		fixedDistanceCode = buildCodeTree(codeLengths, 32);

		// Set some variables
		chunkNum = 0;
		lengthLeft = 0;
		currentBit = -1;
		streamIndex = 0;
	}
	~pngLoader() {
		delete[] buffer;
		delete[] stream;
		delete[] image;
		delete fixedLiteralCode;
		delete fixedDistanceCode;
	}

	void decompress() {
		// Check PNG header
		if (nextFileByte() != 137 || nextFileByte() != 80 || nextFileByte() != 78 || nextFileByte() != 71 ||
			nextFileByte() != 13 || nextFileByte() != 10 || nextFileByte() != 26 || nextFileByte() != 10) {
				throw string ("Not a png-file");
		}

		// Variables for zlib-stream
		unsigned char zCompression = nextIdatByte();
		unsigned char zFlags = nextIdatByte();
		unsigned char cm, cinfo, fcheck, fdict, flevel;
		// Get stream info to variables
		cm = zCompression & 0x0f;
		cinfo = zCompression >> 4;
		fcheck = zFlags & 0x1f;
		fdict = (zFlags>>5) & 1;
		flevel = zFlags >> 6;

		while (1) {
			// Read block info
			bool bfinal = nextIdatBit();
			char btype = nextIdatInt(2);

			cout << "Block:\n"
				<< (bfinal?"\tFINAL\n":"\tNOT FINAL\n")
				<< "\tType: " << btype+0 << "\n";

			switch (btype) {
			case 0:
				decompressUncompressedBlock();
				break;
			case 1:
				// Fixed Huffman code
				decompressHuffmanBlock(fixedLiteralCode, fixedDistanceCode);
				break;
			case 2: {
				// Dynamic Huffman code
				pair<node*, node*> codeLenths = decodeHuffmanCodes();
				decompressHuffmanBlock(codeLenths.first, codeLenths.second);
				delete codeLenths.first;
				delete codeLenths.second;
				break;
			}
			default:
				throw string("Reserved BTYPE in zlib-stream");
			}

			if (bfinal)
				break;		// The last block
		}

		// Read rest of the PNG file
		while (nextIdatByte() != (unsigned char) -1);

		// Now build the image
		streamIndex = 0;

		image = new unsigned char[height*width*4];

		if (colorType == 6) {
			if (bitDepth == 8) {
				for (int y = 0; y < height; y++) {
					unsigned char scanlineFilter = get();
					for (int x = 0; x < width; x++) {
						for (int s = 0; s < 4; s++) {
							unsigned char sample = get();
							switch (scanlineFilter) {
							case 0:
								break;
							case 1:
								sample += getSample(x-1, y, s);
								break;
							case 2:
								sample += getSample(x, y-1, s);
								break;
							case 3:
								sample += (getSample(x-1, y, s) + getSample(x, y-1, s)) / 2;
								break;
							case 4:
								sample += paeth(getSample(x-1, y, s), getSample(x, y-1, s), getSample(x-1, y-1, s));
								break;
							default:
								throw string("Unsupported filter");
							}
							setSample(x, y, s, sample);
						}
					}
				}
			} else if (bitDepth == 16) {
				throw string("Unsupported color bit depth");
			}
		} else {
			throw string("Unsupported color type");
		}
	}
	
	// Image info
	int width, height;
	unsigned char bitDepth, colorType, compression, filter, interlace;
	unsigned char *image;
private:
	struct node;

	// Raw file reading functions
	unsigned char nextFileByte() {
		unsigned char c;
		ifs.read((char*) &c, 1);
		crc = crcTable[(crc ^ c) & 0xff] ^ (crc >> 8);
		return c;
	}
	int nextFileInt() {
		return nextFileByte() << 24
			 | nextFileByte() << 16
			 | nextFileByte() << 8
			 | nextFileByte() << 0;
	}
	unsigned int nextFileUInt() {
		return nextFileByte() << 24
			 | nextFileByte() << 16
			 | nextFileByte() << 8
			 | nextFileByte() << 0;
	}

	// Functions for reading PNG IDAT stream
	unsigned char nextIdatByte() {
		// Read next chunks until next IDAT
		while (lengthLeft <= 0) {
			if (chunkNum) {
				// Not the first chunk -> read last chunks crc
				nextFileUInt();
			}
			lengthLeft = nextFileInt();
			resetCrc();
			type = nextFileInt();
			chunkNum++;

			switch (type) {
			case PNG_CHUNCK_IHDR: {
				// Check header length
				if (lengthLeft != 13)
					throw string("Broken header");
				// Check that the header is the first chunk
				if (chunkNum != 1)
					throw string("Header must be the first chunk");
				// Parse header data
				width = nextFileInt();
				height = nextFileInt();
				bitDepth = nextFileByte();
				colorType = nextFileByte();
				compression = nextFileByte();
				filter = nextFileByte();
				interlace = nextFileByte();

				lengthLeft = 0;
				char samplesPerPixel;
				switch (colorType)
				{
				case 0:
				case 3:
					samplesPerPixel = 1;
					break;
				case 2:
					samplesPerPixel = 3;
					break;
				case 4:
					samplesPerPixel = 2;
					break;
				case 6:
					samplesPerPixel = 4;
					break;
				default:
					throw string("Unsupported compression");
				}

				// Make a output buffer
				stream = new unsigned char[((width+1)*height*bitDepth*samplesPerPixel+7)/8];
				break;
			}
			case PNG_CHUNCK_IDAT:
				bufferIndex = PNG_BUFFER_LENGTH;		// Read the part of the file into a buffer
				break;
				
			case PNG_CHUNCK_IEND:
				return -1;				// End of file
				break;

			default:
				throw (string("Unknown chunk: ") + (char)(type>>24) + (char)(type>>16) + (char)(type>>8) + (char)(type>>0));
			}
		}
		
		if (bufferIndex >= PNG_BUFFER_LENGTH) {
			if (lengthLeft > PNG_BUFFER_LENGTH) {
				ifs.read((char*) buffer, PNG_BUFFER_LENGTH);
				lengthLeft -= PNG_BUFFER_LENGTH;
				bufferLength = PNG_BUFFER_LENGTH;
			} else {
				ifs.read((char *) buffer, lengthLeft);
				bufferLength = lengthLeft;
				lengthLeft = 0;
			}
			bufferIndex = 0;
		}
		if (bufferIndex >= bufferLength) {
			return -1;		// Too far
		}

		currentBit = -1;		// Make nextIdatBit to read from the next byte
		return buffer[bufferIndex++];
	}
	bool nextIdatBit() {
		if (currentBit >= 8) {
			currentByte = nextIdatByte();
			currentBit = 0;
		}
		bool res = (currentByte >> currentBit) & 1;
		currentBit++;
		return res;
	}
	unsigned int nextIdatInt(int length) {
		unsigned int res = 0;
		for (int i = 0; i < length; i++)
			res |= nextIdatBit() << i;
		return res;
	}

	// Decoding message
	unsigned short decodeSymbol(node *codeTree) {
		node *currentNode = codeTree;
		while (1) {
			bool bit = nextIdatBit();
			if (bit)
				currentNode = currentNode->right;
			else
				currentNode = currentNode->left;

			if (currentNode->val != (unsigned short) -1) {
				return currentNode->val;
			}
		}
	}
	unsigned short decodeLength(unsigned short sym) {
		if (sym < 257 || sym > 285)
			throw string("Illeagal index alphabet");

		if (sym <= 264)
			return sym - 254;

		if (sym <= 284) {
			unsigned char extraBits = (sym - 261) / 4;
			return (((sym - 265) % 4 + 4) << extraBits) + 3 + nextIdatInt(extraBits);
		}
		return 258;		// sym == 285
	}
	unsigned short decodeDistance(unsigned short sym) {
		if (sym <= 3)
			return sym + 1;

		unsigned char extraBits = (sym - 2) / 2;
		return ((sym % 2 + 2) << extraBits) + 1 + nextIdatInt(extraBits);
	}
	pair<node*, node*> decodeHuffmanCodes() {
		unsigned short hlit = nextIdatInt(5) + 257;
		unsigned short hdist = nextIdatInt(5) + 1;
		unsigned short hclen = nextIdatInt(4) + 4;
	
		// Print some nice debug info
		cout << "\tHLIT: " << hlit << "\n"
			<< "\tHDIST: " << hdist << "\n"
			<< "\tHCLEN: " << hclen << "\n";
		
		// Decode code codes
		unsigned char codeLengthCodeLengths[19];
		for (int i = 0; i < 19; i++)
			codeLengthCodeLengths[i] = 0;
		codeLengthCodeLengths[16] = nextIdatInt(3);
		codeLengthCodeLengths[17] = nextIdatInt(3);
		codeLengthCodeLengths[18] = nextIdatInt(3);
		codeLengthCodeLengths[ 0] = nextIdatInt(3);
		for (char i = 0; i < hclen - 4; i++) {
			if (i % 2 == 0)
				codeLengthCodeLengths[8 + i / 2] = nextIdatInt(3);
			else
				codeLengthCodeLengths[7 - i / 2] = nextIdatInt(3);
		}
		node *codeLenghtCode = buildCodeTree(codeLengthCodeLengths, 19);

		// Now decode codes
		unsigned char *codeLengths = new unsigned char[hlit + hdist];
		//for (int i = 0; i < hlit + hdist; i++)
		//	codeLengths[i] = 0;
		int runVal = -1;
		int runLen = 0;
		for (int i = 0; i < hlit + hdist; i++) {
			if (runLen > 0) {
				codeLengths[i] = runVal;
				runLen--;
			} else {
				unsigned short sym = decodeSymbol(codeLenghtCode);
				if (sym < 16) {
					codeLengths[i] = (unsigned char) sym;
					runVal = sym;
				} else {
					switch (sym) {
					case 16:
						if (runVal == -1)
							throw string("No code length value to copy");
						runLen = nextIdatInt(2) + 3;
						break;
					case 17:
						runVal = 0;
						runLen = nextIdatInt(3) + 3;
						break;
					case 18:
						runVal = 0;
						runLen = nextIdatInt(7) + 11;
						break;
					default:
						throw string("Unknown symbol");
					}
					i--;
				}
			}
		}

		if (runLen > 0)
			throw string("Run exceeds number of codes");
		
		unsigned char *literalCodeLengths = new unsigned char[hlit];
		for (int i = 0; i < hlit; i++)
			literalCodeLengths[i] = codeLengths[i];
		unsigned char *distanceCodeLengths = new unsigned char[hdist];
		for (int i = 0; i < hdist; i++)
			distanceCodeLengths[i] = codeLengths[i + hlit];

		node *literalCode = buildCodeTree(literalCodeLengths, hlit);
		node *distanceCode = buildCodeTree(distanceCodeLengths, hdist);

		// Free unused stuff
		delete codeLenghtCode;
		delete[] codeLengths;
		delete[] literalCodeLengths;
		delete[] distanceCodeLengths;
		
		return pair<node*, node*>(literalCode, distanceCode);
	}
	void decompressHuffmanBlock(node *literalCode, node *distanceCode) {
		while (1) {
			unsigned short sym = decodeSymbol(literalCode);
			if (sym == 256)
				break;

			if (sym < 256) {
				put((unsigned char) sym);
			} else {
				unsigned short len = decodeLength(sym);
				unsigned short distSym = decodeSymbol(distanceCode);
				unsigned short dist = decodeDistance(distSym);
				copy(dist, len);
			}
		}
	}
	void decompressUncompressedBlock() {
		// Skip to the byte boundary
		while (currentBit != 8)
			nextIdatBit();
		unsigned short length = nextIdatInt(16);
		unsigned short nlength =  nextIdatInt(16);
		if ((unsigned short) ~length != nlength)
			throw string("Invalid length in uncompressed block");

		cout << "\tLength: " << length << "\n"
			<< "\tNLength: " << nlength << "\n";
		
		for (unsigned short i = 0; i < length; i++) {
			put(nextIdatByte());
		}
	}

	// For checksums
	static const unsigned int crcTable[];
	unsigned int crc;
	void resetCrc() {
		crc = 0xFFFFFFFFL;
	}
	unsigned long getCrc() {
		return crc ^ 0xFFFFFFFFL;
	}

	// For output data
	unsigned char *stream;
	unsigned int streamIndex;
	void put(unsigned char val) {
		stream[streamIndex++] = val;
	}
	unsigned char get() {
		return stream[streamIndex++];
	}
	void copy(unsigned short dist, unsigned short len) {
		for (unsigned short i = 0; i < len; i++)
			put(stream[streamIndex - dist]);
	}

	// For filter
	unsigned char paeth(unsigned char a, unsigned char b, unsigned char c) {
		short p = a + b - c;
		unsigned short pa = abs(p - a);
		unsigned short pb = abs(p - b);
		unsigned short pc = abs(p - c);
		if (pa <= pb && pa <= pc) return a;
		if (pb <= pc) return b;
		return c;
	}

	// Input file
	ifstream ifs;

	// For image construction
	unsigned char getSample(int x, int y, int s) {
		if (x < 0 || y < 0 || s < 0 || x >= width || y >= height || s >= 4)
			return 0;
		return image[((height - y - 1)*width+x)*4 + s];
	}
	void setSample(int x, int y, int s, unsigned char val) {
		if (x < 0 || y < 0 || s < 0 || x >= width || y >= height || s >= 4)
			return;
		image[((height - y - 1)*width+x)*4 + s] = val;
	}

	// For IDAT chunk
	unsigned int chunkNum, lengthLeft, bufferIndex, bufferLength, type;
	unsigned char *buffer, currentByte, currentBit;

	// For code trees
	struct node {
		unsigned short val;
		node *left, *right;
		node(unsigned short _val) : val(_val), left(0), right(0) {
		}
		node(node *l, node *r) : val(-1), left(l), right(r) {
		}
		node() : val(-1), left(0), right(0) {
		}
		~node() {
			// Delete both children so deleting the whole tree is easy
			if (left != 0)
				delete left;
			if (right != 0)
				delete right;
		}
	};
	node *fixedLiteralCode, *fixedDistanceCode;
	node *buildCodeTree(unsigned char *codeLenghts, int count) {
		int max = 0;
		for (int i = 0; i < count; i++) {
			if (codeLenghts[i] > max)
				max = codeLenghts[i];
		}
		// Go the tree form the leafs towards the root
		vector<node*> nodes;
		for (int i = max; i >= 1; i--) {
			vector<node*> newNodes;

			// Add new leaves
			for (int j = 0; j < count; j++) {
				if (codeLenghts[j] == i)
					newNodes.push_back(new node(j));
			}

			for (int j = 0; j < nodes.size(); j += 2)
				newNodes.push_back(new node(nodes[j], nodes[j+1]));

			nodes = newNodes;
			if (nodes.size() % 2 != 0)
				throw string("Not a Huffman code tree");
		}

		return new node(nodes[0], nodes[1]);
	}
};

enum textureFileType {
	pngFile
};
class textureLoader {
public:
	textureLoader(string _basePath) : basePath(_basePath) {
	}

	void loadToTexture(texture &t, string filename, textureType type, textureFileType filetype) {
		switch (filetype)
		{
		case pngFile:
			loadPngToTexture(t, filename, type);
			break;
		default:
			break;
		}
	}

	void loadPngToTexture(texture &t, string filename, textureType type) {
		pngLoader pngL(basePath + '/' + filename);
		pngL.decompress();

		t.bind(0);
		glTexImage2D(
			t.me->type,
			0,
			GL_RGBA,
			pngL.width,
			pngL.height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			pngL.image);
	}

private:
	string basePath;
};