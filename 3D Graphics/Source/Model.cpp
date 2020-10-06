// (c) 2020 Alexander Jenkins
//
// File Name   : model.cpp
// Description : model implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "Model.h"
#include "Camera.h"

CModel::CModel(std::string path, CCamera* camera) {

	// EDIT
	this->program = ShaderLoader::CreateProgram("Resources/Shaders/ModelVShader.txt", "Resources/Shaders/ModelFShader.txt");
	// EDIT END

	this->camera = camera;
	this->loadModel(path);
}

CModel::CModel(CCamera* camera)
{
	// EDIT
	this->program = ShaderLoader::CreateProgram("Resources/Shaders/ModelVShader.txt", "Resources/Shaders/ModelFShader.txt");
	this->camera = camera;
}

void CModel::Render()
{
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	for (GLuint i = 0; i < this->meshes.size(); i++) {

		//printf("mesh size: %d \n", meshes.size());

		this->meshes[i].Render(camera, program);
	}

	glDisable(GL_DEPTH_TEST);
}

void CModel::Update(mat4 _model)
{
	for (GLuint i = 0; i < this->meshes.size(); i++) {

		//printf("mesh size: %d \n", meshes.size());

		this->meshes[i].Update(_model);
	}
}

void CModel::SetMesh(vector<ModelMesh> _meshvec)
{
	meshes = _meshvec;
}

vector<ModelMesh> CModel::GetMesh()
{
	return meshes;
}

void CModel::SetTexture(vector<MeshTexture> _textrueVec)
{
	textures_loaded = _textrueVec;
}

vector<MeshTexture> CModel::GetTextures()
{
	return textures_loaded;
}

void CModel::loadModel(string path)
{
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// Retrieve the directory path of the filepath
	this->directory = path.substr(0, path.find_last_of('/'));

	// Process ASSIMP's root node recursively
	this->processNode(scene->mRootNode, scene);
}

// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void CModel::processNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* m_pMesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(m_pMesh, scene));
	}


	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}

}

ModelMesh CModel::processMesh(aiMesh* m_pMesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<MeshTexture> Textures;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < m_pMesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // Positions
		vector.x = m_pMesh->mVertices[i].x;
		vector.y = m_pMesh->mVertices[i].y;
		vector.z = m_pMesh->mVertices[i].z;
		vertex.Position = vector;
		// Normals
		vector.x = m_pMesh->mNormals[i].x;
		vector.y = m_pMesh->mNormals[i].y;
		vector.z = m_pMesh->mNormals[i].z;
		vertex.Normal = vector;
		// Texture Coordinates
		if (m_pMesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = m_pMesh->mTextureCoords[0][i].x;
			vec.y = m_pMesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < m_pMesh->mNumFaces; i++)
	{
		aiFace face = m_pMesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// Process materials
	if (m_pMesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[m_pMesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		// 1. Diffuse maps
		vector<MeshTexture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		Textures.insert(Textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		vector<MeshTexture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		Textures.insert(Textures.end(), specularMaps.begin(), specularMaps.end());
	}

	// Return a mesh object created from the extracted mesh data
	return ModelMesh(vertices, indices, Textures);
}

// Checks all material textures of a given type and loads the textures if they're not loaded yet.
// The required info is returned as a Texture struct.
vector<MeshTexture> CModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<MeshTexture> Textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				Textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // If texture hasn't been loaded already, load it
			MeshTexture m_pTexture;
			m_pTexture.id = TextureFromFile(str.C_Str(), this->directory);
			m_pTexture.type = typeName;
			m_pTexture.path = str;
			Textures.push_back(m_pTexture);
			this->textures_loaded.push_back(m_pTexture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return Textures;
}

GLint CModel::TextureFromFile(const char* path, string directory)
{

	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;

	cout << "texture filename: " << filename << endl;

	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}