#ifndef MODEL_H
#define MODEL_H

#include <map>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "texture.h"
#include "mesh.h"

/*
* ����һ��ģ�� ģ�Ϳ��԰���һ������Mesh
*/

class Model {
	typedef std::map<std::string, Texture> LoadedTexMapType;// key = texture file path

public:
	Model() {}
	~Model()
	{
		for (std::vector<Mesh>::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
			it->final();
		}
	}

	void draw(Shader &shader)
	{
		for (std::vector<Mesh>::const_iterator it = meshes.begin(); it != meshes.end(); ++it) {
			it->draw(shader);
		}
	}

	bool loadModel(const std::string &filePath)
	{
		Assimp::Importer importer;
		if (filePath.empty()) {
			std::cerr << "Error:Model::loadModel, empty model file path." << std::endl;
			return false;
		}

		const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "Error:Model::loadModel, description: " << importer.GetErrorString() << std::endl;
			return false;
		}

		modelFileDir = filePath.substr(0, filePath.find_last_of('/'));
		if (!processNode(scene->mRootNode, scene)) {
			std::cerr << "Error:Model::loadModel, process node failed." << std::endl;
			return false;
		}
		return true;
	}

private:
	/*
	* �ݹ鴦��ģ�͵Ľ��
	*/
	bool processNode(const aiNode* node, const aiScene* scene)
	{
		if (!node || !scene) {
			return false;
		}
		// �ȴ���������
		for (size_t i = 0; i < node->mNumMeshes; ++i) {
			// ע��node�е�mesh�Ƕ�sceneObject��mesh������
			const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh) {
				Mesh obj;
				if (processMesh(mesh, scene, obj)) {
					meshes.push_back(obj);
				}
			}
		}
		// �����ӽ��
		for (size_t i = 0; i < node->mNumChildren; ++i) {
			processNode(node->mChildren[i], scene);
		}
		return true;
	}

	bool processMesh(const aiMesh *mesh, const aiScene *scene, Mesh &obj)
	{
		if (!mesh || !scene) {
			return false;
		}

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		// ��Mesh�õ��������ݡ�����������������
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			Vertex vert;
			// ��ȡ����λ��
			if (mesh->HasPositions()) {
				vert.position.x = mesh->mVertices[i].x;
				vert.position.y = mesh->mVertices[i].y;
				vert.position.z = mesh->mVertices[i].z;
			}
			// ��ȡ�������� Ŀǰֻ����0������
			if (mesh->HasTextureCoords(0)) {
				vert.texCoord.x = mesh->mTextureCoords[0][i].x;
				vert.texCoord.y = mesh->mTextureCoords[0][i].y;
			} else {
				vert.texCoord = glm::vec2(0.0f, 0.0f);
			}
			// ��ȡ����������
			if (mesh->HasNormals()) {
				vert.normal.x = mesh->mNormals[i].x;
				vert.normal.y = mesh->mNormals[i].y;
				vert.normal.z = mesh->mNormals[i].z;
			}
			vertices.push_back(vert);
		}
		// ��ȡ��������
		for (size_t i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			if (face.mNumIndices != 3) {
				std::cerr << "Error:Model::processMesh, mesh not transformed to triangle mesh." << std::endl;
				return false;
			}
			for (size_t j = 0; j < face.mNumIndices; ++j) {
				indices.push_back(face.mIndices[j]);
			}
		}
		// ��ȡ��������
		if (mesh->mMaterialIndex >= 0) {
			const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			// ��ȡdiffuse����
			std::vector<Texture> diffuseTexture;
			processMaterial(material, scene, aiTextureType_DIFFUSE, diffuseTexture);
			textures.insert(textures.end(), diffuseTexture.begin(), diffuseTexture.end());
			// ��ȡspecular����
			std::vector<Texture> specularTexture;
			processMaterial(material, scene, aiTextureType_SPECULAR, specularTexture);
			textures.insert(textures.end(), specularTexture.begin(), specularTexture.end());
		}

		obj.setData(vertices, indices, textures);
		return true;
	}

	/*
	* ��ȡһ�������е�����
	*/
	bool processMaterial(const aiMaterial *material, const aiScene *scene, aiTextureType textureType, std::vector<Texture> &textures)
	{
		textures.clear();

		if (!material || !scene) {
			return false;
		}

		unsigned int cnt = material->GetTextureCount(textureType);
		if (cnt <= 0) {
			return true;
		}

		for (size_t i = 0; i < cnt; ++i) {
			Texture texture;
			aiString path;
			aiReturn ret = material->GetTexture(textureType, i, &path);
			if (ret != aiReturn_SUCCESS || path.length == 0) {
				std::cerr << "Warning, load texture type=" << textureType
					<< "index= " << i << " failed with return value= "
					<< ret << std::endl;
				continue;
			}

			std::string absolutePath = modelFileDir + "/" + path.C_Str();
			LoadedTexMapType::const_iterator it = loadedTexMapType.find(absolutePath);
			if (it == loadedTexMapType.end()) {// ����Ƿ��Ѿ����ع���
				GLuint tId = TextureHelper::load2DTexture(absolutePath.c_str());
				texture.id = tId;
				texture.path = absolutePath;
				texture.type = textureType;
				textures.push_back(texture);
				loadedTexMapType[absolutePath] = texture;
			} else {
				textures.push_back(it->second);
			}
		}
		return true;
	}

private:
	std::vector<Mesh> meshes;// ����Mesh
	std::string modelFileDir;// ����ģ���ļ����ļ���·��
	LoadedTexMapType loadedTexMapType;// �����Ѿ����ص�����
};

#endif // MODEL_H
