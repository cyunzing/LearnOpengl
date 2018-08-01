#ifndef MESH_H
#define MESH_H

#include <vector>
#include <sstream>
#include "glm/glm.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "shader.h"

// ��ʾһ����������
struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

// ��������
enum ETextureType
{
	ETextureTypeDiffuse = 1,  // ������
	ETextureTypeSpecular = 2, // ���淴��
};

// ��ʾһ��Texture
struct Texture {
	GLuint id;
	aiTextureType type;
	std::string path;
};

// ��ʾһ��������Ⱦ����Сʵ��
class Mesh {
public:
	Mesh() : vaoId(0), vboId(0), eboId(0) {}
	Mesh(const std::vector<Vertex> &vv, const std::vector<GLuint> &iv, const std::vector<Texture> &tv)
		: vaoId(0), vboId(0), eboId(0)
	{
		setData(vv, iv, tv);
	}

	~Mesh()
	{
		// ��Ҫ�������ͷ�VBO�ȿռ� ��ΪMesh���󴫵�ʱ ��ʱ�������ٺ����������VBO�ȿռ�
	}

	void setData(const std::vector<Vertex> &vv, const std::vector<GLuint> &iv, const std::vector<Texture> &tv)
	{
		vertices = vv;
		indices = iv;
		textures = tv;
		if (!vertices.empty() && !indices.empty()) {
			setupMesh();
		}
	}

	void draw(Shader &shader) const// ����Mesh
	{
		if (vaoId == 0 || vboId == 0 || eboId == 0)
			return;

		shader.use();

		glBindVertexArray(vaoId);

		int diffuseCnt = 0, specularCnt = 0, reflectCnt = 0, texUnitCnt = 0;
		for (std::vector<Texture>::const_iterator it = textures.begin(); it != textures.end(); ++it) {
			switch (it->type) {
				case aiTextureType_DIFFUSE: {
					glActiveTexture(GL_TEXTURE0 + texUnitCnt);
					glBindTexture(GL_TEXTURE_2D, it->id);

					std::stringstream samplerNameStr;
					samplerNameStr << "texture_diffuse" << diffuseCnt++;
					glUniform1i(glGetUniformLocation(shader.programId, samplerNameStr.str().c_str()), texUnitCnt++);
					break;
				}
				case aiTextureType_SPECULAR: {
					glActiveTexture(GL_TEXTURE0 + texUnitCnt);
					glBindTexture(GL_TEXTURE_2D, it->id);

					std::stringstream samplerNameStr;
					samplerNameStr << "texture_specular" << specularCnt++;
					glUniform1i(glGetUniformLocation(shader.programId, samplerNameStr.str().c_str()), texUnitCnt++);
					break;
				}
				case aiTextureType_AMBIENT:		// ע��: ������Ϊ��Ӧ��AssImp��reflection_map֧�ֲ��� ����ʱ����
				{
					glActiveTexture(GL_TEXTURE0 + texUnitCnt);
					glBindTexture(GL_TEXTURE_2D, it->id);
					std::stringstream samplerNameStr;
					samplerNameStr << "texture_reflection" << reflectCnt++;
					glUniform1i(glGetUniformLocation(shader.programId, samplerNameStr.str().c_str()), texUnitCnt++);
				}
				default: {
					std::cerr << "Warning::Mesh::draw, texture type" << it->type
						<< " current not supported." << std::endl;
					break;
				}
			}
		}

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void final() const
	{
		glDeleteVertexArrays(1, &vaoId);
		glDeleteBuffers(1, &vboId);
		glDeleteBuffers(1, &eboId);
	}

private:
	void setupMesh()
	{
		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(5 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

		// ��������
		glGenBuffers(1, &eboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

private:
	GLuint vaoId, vboId, eboId;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
};

#endif // MESH_H
