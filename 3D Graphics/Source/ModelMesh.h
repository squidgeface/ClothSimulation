// (c) 2020 Alexander Jenkins
//
// File Name   : modelmesh.h
// Description : modelmesh declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "Utils.h"

class CCamera;

struct Vertex {
	
	glm::vec3 Position; // Position	
	glm::vec3 Normal; // Normal	
	glm::vec2 TexCoords; // TexCoords
};

struct MeshTexture {
	GLuint id = 0;
	string type;
	aiString path;
};

class ModelMesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<MeshTexture> Textures;

	/*  Functions  */
	// Constructor
	ModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> Textures);

	// Render the mesh
	void Render(CCamera* camera, GLuint program);

	void Update(mat4 m_m4Model);

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;
	mat4 m_m4Model;
	mat4 m_m4Scale;
	mat4 m_m4MVP;
	vec3 m_v3ObjPosition;
	mat4 m_m4TranslationMatrix;
	vec3 m_v3RotationAxisZ;
	float m_fRotationAngle;
	mat4 m_m4RotationZ;
	vec3 m_v3ObjScale;
	
	mat4 m_m4ScaleMatrix;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh();
};