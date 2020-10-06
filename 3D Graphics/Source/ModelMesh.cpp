// (c) 2020 Alexander Jenkins
//
// File Name   : modelmesh.cpp
// Description : modelmesh implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "ModelMesh.h"
#include "Camera.h"

ModelMesh::ModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> Textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->Textures = Textures;
	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	this->setupMesh();

	//position
	m_v3ObjPosition = vec3(0.0f, 0.0f, 0.0f);
	m_m4TranslationMatrix = translate(mat4(), m_v3ObjPosition);

	//Rotation
	m_v3RotationAxisZ = vec3(0.0f, 0.0f, 1.0f);
	m_fRotationAngle = 0.0f;
	m_m4RotationZ = rotate(glm::mat4(), radians(m_fRotationAngle), m_v3RotationAxisZ);

	//Scale
	m_v3ObjScale = vec3(1.0f, 1.0f, 1.0f);
	m_m4ScaleMatrix = scale(mat4(), m_v3ObjScale);

	m_m4Model = m_m4TranslationMatrix * m_m4RotationZ * m_m4ScaleMatrix;
}

// Render the mesh
void ModelMesh::Render(CCamera* camera, GLuint program)
{
	glUseProgram(program);

	// Bind appropriate textures
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < this->Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		stringstream ss;
		string number;
		string name = this->Textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // Transfer GLuint to stream
		else if (name == "texture_specular")
			ss << specularNr++; // Transfer GLuint to stream
		number = ss.str();
		// Now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, this->Textures[i].id);
	}

	// EDIT
	
	m_m4MVP = camera->GetVPMatrix() * m_m4Model;
	GLint mvpLoc = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(m_m4MVP));
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_m4Model));
	// EDIT END

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < this->Textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void ModelMesh::Update(mat4 _model)
{
	m_m4Model = _model;
}

void ModelMesh::setupMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}