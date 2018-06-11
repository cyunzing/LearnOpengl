#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"

class TextureHelper
{
public:
    /*
    /* 成功加载2D纹理则返回纹理对象Id 否则返回0
    */
    static GLuint load2DTexture(const char* filename, GLint internalFormat = GL_RGB, GLenum picFormat = GL_RGB, int loadChannels = SOIL_LOAD_RGB)
    {
        // Step1 创建并绑定纹理对象
        GLuint textureId = 0;

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        // Step2 设定wrap参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
};

#endif // TEXTURE_H
