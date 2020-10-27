// (c) 2020 Alexander Jenkins
//
// File Name   : mesh.cpp
// Description : mesh implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "mesh.h"


CMesh::~CMesh()
{
	delete m_pTerrain;
	m_pTerrain = 0;
}

//create static quad
void CMesh::CreateQuad()
{
	//generate vertex shape
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndicies), quadIndicies, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)0);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		8 * sizeof(GLfloat),				//Stride = pos + colour + texture
		(GLvoid*)(6 * sizeof(GLfloat)));	//offset from beginning of Vertex (bypass pos and colour)

	glEnableVertexAttribArray(2);

	IndiceCount = sizeof(quadIndicies) / sizeof(GLuint);
	m_iDrawType = GL_TRIANGLES;
}
//Create pyramid
void CMesh::CreatePyramid()
{
	//generate vertex shape
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	IndiceCount = sizeof(pyramidIndices) / sizeof(GLuint);
	m_iDrawType = GL_TRIANGLES;
}
//create cube
void CMesh::CreateCube()
{
	//generate vertex shape
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	IndiceCount = sizeof(cubeIndices) / sizeof(GLuint);
	m_iDrawType = GL_TRIANGLES;
}
//create sphere
void CMesh::CreateSphere()
{
	//sphere
	float radius = 1.0f;

	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	float phi = 0;
	float theta = 0;

	float vertices[(sections) * (sections)*vertexAttrib];
	int offset = 0;

	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = cos(phi) * sin(theta);
			float y = cos(theta);
			float z = sin(phi) * sin(theta);

			vertices[offset++] = x * radius;
			vertices[offset++] = y * radius;
			vertices[offset++] = z * radius;

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (float)((float)PI / ((float)(sections) - 1.0f));
		}

		phi += (float)(2 * (float)PI) / ((float)(sections) - 1.0f);
	}


	GLuint indices[(sections) * (sections)*indexPerQuad];

	offset = 0;

	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);

			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	IndiceCount = sizeof(indices) / sizeof(GLuint);
	m_iDrawType = GL_TRIANGLES;
}

//create sphere
void CMesh::CreateCapsule()
{
	//sphere
	float radius = 1.0f;

	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	float phi = 0;
	float theta = 0;

	float vertices[(sections) * (sections)*vertexAttrib];
	int offset = 0;

	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = cos(phi) * sin(theta);
			float y = cos(theta);
			float z = sin(phi) * sin(theta);

			vertices[offset++] = x * radius;
			if (j < sections / 2) {
				vertices[offset++] = y * radius;
			}
			else {
				vertices[offset++] = (y - 3) * radius ;
			}
			
			
			vertices[offset++] = z * radius;

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (float)((float)PI / ((float)(sections)-1.0f));
		}

		phi += (float)(2 * (float)PI) / ((float)(sections)-1.0f);
	}


	GLuint indices[(sections) * (sections)*indexPerQuad];

	offset = 0;

	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);

			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
		}
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	IndiceCount = sizeof(indices) / sizeof(GLuint);
	m_iDrawType = GL_TRIANGLES;
}
//draw the mesh of regular shapes
void CMesh::Draw()
{
	glBindVertexArray(VAO); //bind VAO
	glDrawElements(m_iDrawType, IndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); //unbind VAO
}
//Render geometry made in the .gs
void CMesh::RenderGeometry()
{
	//render instanced array
	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_POINTS, 0, 1, 7);
	glBindVertexArray(0);
}
//render shapes made in tesselation
void CMesh::RenderTesselated()
{
	//render patches
	glBindVertexArray(VAO);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}
//Create geometry 
void CMesh::CreateGrometry()
{
	GLfloat points[] =
	{	//Point Coords		//Color
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
	};

	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	
	
	glBindVertexArray(0);

}
