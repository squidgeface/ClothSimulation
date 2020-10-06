// (c) 2020 Alexander Jenkins
//
// File Name   : cubemap.h
// Description : cubemap declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "Utils.h"

class CCamera;

class CCubemap
{
public:
	// Constructors (sets default texture modes)
	CCubemap(CCamera* camera, vector<string> filePaths);
	// Generates texture from image data
	void Generate();
	void Update();
	void Render(GLuint program);

	GLuint GetTextureID();

	//Variables
protected:
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint Texture;
	//Texture
	vector<string> Textures;
	CCamera* m_pCamera;
	mat4 MVP;

	//Vertices for the Pyramid
	GLfloat CubeVertices[216]{

		//position 				//normals				//Texture coords	
		//front
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.5f,-0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		//back
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		-0.5f,-0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
		//left
		-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-0.5f,-0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		//right
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		0.5f,-0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		//top
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		//bottom
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f,-0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
	};
	//Indicies for pyramid
	GLuint CubeIndices[36]{
		0,2,1,
		0,3,2,

		7,5,6,
		7,4,5,

		8,10,9,
		8,11,10,

		12,14,13,
		12,15,14,

		16,18,17,
		16,19,18,

		20,22,21,
		20,23,22,
	};
};