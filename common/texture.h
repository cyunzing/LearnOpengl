#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"

class TextureHelper
{
public:
	/*
	/* 成功加载2D纹理则返回纹理对象Id 否则返回0
	*/
	static GLuint load2DTexture(const char* filename, GLint internalFormat = GL_RGB, GLenum picFormat = GL_RGB, int loadChannels = SOIL_LOAD_RGB, GLboolean alpha = false)
	{
		// Step1 创建并绑定纹理对象
		GLuint textureId = 0;

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		// Step2 设定wrap参数
		// 注意这里使用GL_CLAMP_TO_EDGE 避免边缘部分半透明的视觉bug
		// 边缘部分半透明 是因为使用下次重复的纹理做插值导致的
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		// Step3 设定filter参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 为MipMap设定filter方法
		// Step4 加载纹理
		int w, h;
		GLubyte *data = SOIL_load_image(filename, &w, &h, 0, loadChannels);
		if (data == NULL) {
			std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
			return 0;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, picFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// Step5 释放纹理图片资源
		SOIL_free_image_data(data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureId;
	}

	static GLuint loadCubeMapTexture(const std::vector<std::string> &picFilePathVec, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE, int loadChannels = SOIL_LOAD_RGB)
	{
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
		GLubyte *data = NULL;
		int w, h;
		for (size_t i = 0; i < picFilePathVec.size(); ++i) {
			int channels = 0;
			data = SOIL_load_image(picFilePathVec[i].c_str(), &w, &h, &channels, loadChannels);
			if (data == NULL) {
				std::cerr << "Error::loadCubeMapTexture could not load texture file:" << picFilePathVec[i] << std::endl;
				return 0;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, w, h, 0, format, type, data);
			SOIL_free_image_data(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return textureId;
	}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

	static GLuint loadDDS(const char *filename)
	{
		std::ifstream file(filename, std::ios::in | std::ios::binary);
		if (!file) {
			std::cout << "Error::loadDDs, could not open:"
				<< filename << "for read." << std::endl;
			return 0;
		}

		/* verify the type of file */
		char filecode[4];
		file.read(filecode, 4);
		if (strncmp(filecode, "DDS ", 4) != 0) {
			std::cout << "Error::loadDDs, format is not dds :"
				<< filename << std::endl;
			file.close();
			return 0;
		}

		/* get the surface desc */
		char header[124];
		file.read(header, 124);

		unsigned int h = *(unsigned int *)&(header[8]);
		unsigned int w = *(unsigned int *)&(header[12]);
		unsigned int strip = *(unsigned int *)&(header[16]);
		unsigned int minMapCount = *(unsigned int *)&(header[24]);
		unsigned int fourCC = *(unsigned int *)&(header[80]);

		char *buffer = NULL;
		unsigned int bufSize;
		/* how big is it going to be including all mipmaps? */
		bufSize = minMapCount > 1 ? strip * 2 : strip;
		buffer = new char [bufSize];
		file.read(buffer, bufSize);
		/* close the file pointer */
		file.close();

		unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
		unsigned int format;
		switch (fourCC) {
			case FOURCC_DXT1:
				format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				break;
			case FOURCC_DXT3:
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				break;
			case FOURCC_DXT5:
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				break;
			default:
				delete[] buffer;
				return 0;
		}

		// Create one OpenGL texture
		GLuint textureId;
		glGenTextures(1, &textureId);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		/* load the mipmaps */
		for (unsigned int level = 0; level < minMapCount && (w || h); ++level) {
			unsigned int size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, w, h, 0, size, buffer + offset);

			offset += size;
			w /= 2;
			h /= 2;

			// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
			if (w < 1)
				w = 1;
			if (h < 1)
				h = 1;
		}
		
		delete[] buffer;
		return textureId;
	}

	/*
	* 创建framebuffer-attachable texture
	*/
	static GLuint makeAttachmentTexture(GLint level = 0, GLint internalFormat = GL_DEPTH24_STENCIL8, GLsizei width = 800, GLsizei height = 600, GLenum format = GL_DEPTH_STENCIL, GLenum type = GL_UNSIGNED_INT_24_8)
	{
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0, format, type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		return textureId;
	}
};

#endif // TEXTURE_H
