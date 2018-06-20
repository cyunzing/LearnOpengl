#ifndef MODEL_H
#define MODEL_H

#include <map>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "texture.h"
#include "mesh.h"

/*
* 代表一个模型 模型可以包含一个或多个Mesh
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
	* 递归处理模型的结点
	*/
	bool processNode(const aiNode* node, const aiScene* scene)
	{
		if (!node || !scene) {
			return false;
		}
		// 先处理自身结点
		for (size_t i = 0; i < node->mNumMeshes; ++i) {
			// 注意node中的mesh是对sceneObject中mesh的索引
			const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			if (mesh) {
				Mesh obj;
				if (processMesh(mesh, scene, obj)) {
					meshes.push_back(obj);
				}
			}
		}
		// 处理孩子结点
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
		// 从Mesh得到顶点数据、法向量、纹理数据
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			Vertex vert;
			// 获取顶点位置
			if (mesh->HasPositions()) {
				vert.position.x = mesh->mVertices[i].x;
				vert.position.y = mesh->mVertices[i].y;
				vert.position.z = mesh->mVertices[i].z;
			}
			// 获取纹理数据 目前只处理0号纹理
			if (mesh->HasTextureCoords(0)) {
				vert.texCoord.x = mesh->mTextureCoords[0][i].x;
				vert.texCoord.y = mesh->mTextureCoords[0][i].y;
			} else {
				vert.texCoord = glm::vec2(0.0f, 0.0f);
			}
			// 获取法向量数据
			if (mesh->HasNormals()) {
				vert.normal.x = mesh->mNormals[i].x;
				vert.normal.y = mesh->mNormals[i].y;
				vert.normal.z = mesh->mNormals[i].z;
			}
			vertices.push_back(vert);
		}
		// 获取索引数据
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
		// 获取纹理数据
		if (mesh->mMaterialIndex >= 0) {
			const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			// 获取diffuse类型
			std::vector<Texture> diffuseTexture;
			processMaterial(material, scene, aiTextureType_DIFFUSE, diffuseTexture);
			textures.insert(textures.end(), diffuseTexture.begin(), diffuseTexture.end());
			// 获取specular类型
			std::vector<Texture> specularTexture;
			processMaterial(material, scene, aiTextureType_SPECULAR, specularTexture);
			textures.insert(textures.end(), specularTexture.begin(), specularTexture.end());
		}

		obj.setData(vertices, indices, textures);
		return true;
	}

	/*
	* 获取一个材质中的纹理
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
			if (it == loadedTexMapType.end()) {// 检查是否已经加载过了
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
	std::vector<Mesh> meshes;// 保存Mesh
	std::string modelFileDir;// 保存模型文件的文件夹路径
	LoadedTexMapType loadedTexMapType;// 保存已经加载的纹理
};

#endif // MODEL_H
