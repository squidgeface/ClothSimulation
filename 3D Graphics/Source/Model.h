// (c) 2020 Alexander Jenkins
//
// File Name   : model.h
// Description : model declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "ModelMesh.h"
#include "Utils.h"

class CCamera;
class ModelMesh;

class CModel
{
public:
	
	GLuint program;
	CCamera* camera;
	
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.W
	CModel(std::string path, CCamera* camera);
	CModel(CCamera* camera);
	// Draws the model, and thus all its meshes
	void Render();
	void Update(mat4 _model);
	void SetMesh(vector<ModelMesh> _meshvec);
	vector<ModelMesh> GetMesh();
	void SetTexture(vector<MeshTexture> _textrueVec);
	vector<MeshTexture> GetTextures();

private:
	/*  CModel Data  */
	vector<ModelMesh> meshes;
	string directory;
	vector<MeshTexture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	/*  Functions   */
	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string path);

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	ModelMesh processMesh(aiMesh* m_pMesh, const aiScene* scene);

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	GLint TextureFromFile(const char* path, string directory);
};
