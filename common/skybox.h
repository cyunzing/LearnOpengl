#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include "shader.h"
#include "texture.h"

/*
* 用cube实现的天空包围盒
*/
class SkyBox {
public:
    SkyBox() : skyboxTexId(0) {}
    ~SkyBox()
    {
		glDeleteVertexArrays(1, &skyboxVaoId);
		glDeleteBuffers(1, &skyboxVboId);
    }

	bool init(std::vector<std::string> picFilePathVec, GLint internalFormat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_BYTE, int loadChannels = SOIL_LOAD_RGB)
	{
		setupData();
		skyboxTexId = TextureHelper::loadCubeMapTexture(picFilePathVec, internalFormat, format, type, loadChannels);
		return skyboxTexId != 0;
	}

	void draw(Shader &shader)
	{
		GLint oldfunc;
		glGetIntegerv(GL_DEPTH_FUNC, &oldfunc);

		glDepthFunc(GL_LEQUAL);

		shader.use();
		glBindVertexArray(skyboxVaoId);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glDepthFunc(oldfunc);
	}

	GLuint textureId() const
	{
		return skyboxTexId;
	}

private:
	void setupData()
	{
		// 指定包围盒的顶点属性 位置
		GLfloat skyboxVertices[] = {
			// 背面
			-1.0f, 1.0f, -1.0f,		// A
			-1.0f, -1.0f, -1.0f,	// B
			1.0f, -1.0f, -1.0f,		// C
			1.0f, -1.0f, -1.0f,		// C
			1.0f, 1.0f, -1.0f,		// D
			-1.0f, 1.0f, -1.0f,		// A

			// 左侧面
			-1.0f, -1.0f, 1.0f,		// E
			-1.0f, -1.0f, -1.0f,	// B
			-1.0f, 1.0f, -1.0f,		// A
			-1.0f, 1.0f, -1.0f,		// A
			-1.0f, 1.0f, 1.0f,		// F
			-1.0f, -1.0f, 1.0f,		// E

			// 右侧面
			1.0f, -1.0f, -1.0f,		// C
			1.0f, -1.0f, 1.0f,		// G
			1.0f, 1.0f, 1.0f,		// H
			1.0f, 1.0f, 1.0f,		// H
			1.0f, 1.0f, -1.0f,		// D
			1.0f, -1.0f, -1.0f,		// C

			// 正面
			-1.0f, -1.0f, 1.0f,  // E
			-1.0f, 1.0f, 1.0f,  // F
			1.0f, 1.0f, 1.0f,  // H
			1.0f, 1.0f, 1.0f,  // H
			1.0f, -1.0f, 1.0f,  // G
			-1.0f, -1.0f, 1.0f,  // E

			// 顶面
			-1.0f, 1.0f, -1.0f,  // A
			1.0f, 1.0f, -1.0f,  // D
			1.0f, 1.0f, 1.0f,  // H
			1.0f, 1.0f, 1.0f,  // H
			-1.0f, 1.0f, 1.0f,  // F
			-1.0f, 1.0f, -1.0f,  // A

			// 底面
			-1.0f, -1.0f, -1.0f,  // B
			-1.0f, -1.0f, 1.0f,   // E
			1.0f, -1.0f, 1.0f,    // G
			1.0f, -1.0f, 1.0f,    // G
			1.0f, -1.0f, -1.0f,   // C
			-1.0f, -1.0f, -1.0f,  // B
		};

		glGenVertexArrays(1, &skyboxVaoId);
		glBindVertexArray(skyboxVaoId);
		glGenBuffers(1, &skyboxVboId);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
private:
    GLuint skyboxTexId;
    GLuint skyboxVaoId, skyboxVboId;
};

#endif // SKYBOX_H
