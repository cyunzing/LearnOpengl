#ifndef SIMPLEOBJLOADER_H
#define SIMPLEOBJLOADER_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

struct VertexIndex {
	GLuint positionIndex;
	GLuint texCoordIndex;
	GLuint normalIndex;
};

class Mesh {
public:
	Mesh() {}
	Mesh(const std::vector<Vertex> &vert, GLuint tId, bool bShowData = false)
	{
		vertices = vert;
		textureId = tId;
		setupMesh();

		if (bShowData) {
			const char *fileName = "vert-data.txt";
			std::ofstream file(fileName);
			for (std::vector<Vertex>::const_iterator it = vertices.begin(); it != vertices.end(); ++it) {
				file << glm::to_string(it->position) << ","
					<< glm::to_string(it->texCoord) << ","
					<< glm::to_string(it->normal) << std::endl;
			}
			file.close();
			std::cout << "vert data saved in file:" << fileName << std::endl;
		}
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &vaoId);
		glDeleteBuffers(1, &vboId);
	}

	void draw(Shader &shader)
	{
		shader.use();
		glBindVertexArray(vaoId);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glUniform1i(glGetUniformLocation(shader.programId, "tex"), 0);

		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glBindVertexArray(0);
		glUseProgram(0);
	}

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

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	std::vector<Vertex> vertices;
	GLuint vaoId, vboId;
	GLuint textureId;
};

class ObjLoader {
public:
	static bool loadFromFile(const char *path, std::vector<Vertex> &vertices)
	{
		std::vector<VertexIndex> indices;
		std::vector<glm::vec3> vv;
		std::vector<glm::vec2> vtv;
		std::vector<glm::vec3> vnv;

		std::ifstream file(path);
		if (!file) {
			std::cerr << "Error::ObjLoader, could not open obj file:"
				<< path << " for reading." << std::endl;
			return false;
		}

		std::string line;
		while (getline(file, line)) {
			if (line.substr(0, 2) == "vt") {// ����������������
				std::istringstream s(line.substr(2));
				glm::vec2 v;
				s >> v.x; s >> v.y;
				v.y = -v.y;// ע��������ص�dds���� Ҫ��y���з�ת
				vtv.push_back(v);
			} else if (line.substr(0, 2) == "vn") {// ���㷨��������
				std::istringstream s(line.substr(2));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				vnv.push_back(v);
			} else if (line.substr(0, 1) == "v") {// ����λ������
				std::istringstream s(line.substr(1));
				glm::vec3 v;
				s >> v.x; s >> v.y; s >> v.z;
				vv.push_back(v);
			} else if (line.substr(0, 1) == "f") {// ������
				std::istringstream s(line.substr(1));
				std::string vtn;
				while (s >> vtn) {// ����һ���ж����������
					VertexIndex index;
					std::replace(vtn.begin(), vtn.end(), '/', ' ');
					std::istringstream ivtn(vtn);
					if (vtn.find("  ") != std::string::npos) {// û����������
						std::cerr << "Error:ObjLoader, no texture data found within file:"
							<< path << std::endl;
						return false;
					}
					ivtn >> index.positionIndex
						>> index.texCoordIndex
						>> index.normalIndex;

					index.positionIndex--;
					index.texCoordIndex--;
					index.normalIndex--;
					indices.push_back(index);
				}
			} else if (line.substr(0, 2) == "#") {
				// ע�ͺ���
			} else {
				// �������� ��ʱ������
			}
		}
		for (size_t i = 0; i < indices.size(); ++i) {
			Vertex vert;
			VertexIndex index = indices[i];

			vert.position = vv[index.positionIndex];
			vert.texCoord = vtv[index.texCoordIndex];
			vert.normal = vnv[index.normalIndex];

			vertices.push_back(vert);
		}
		return true;
	}
};
#endif // SIMPLEOBJLOADER_H
