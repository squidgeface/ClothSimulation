// (c) 2020 Alexander Jenkins
//
// File Name   : mesh.h
// Description : mesh declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "Utils.h"

class CMesh
{
public:
	CMesh();
	~CMesh();

	void CreateQuad();
	void CreateCube();
	void CreateSphere();
	void SetFrames(float frameCount);
	void Draw();

	//unused functions
	void CreateAniQuad(float frames);
	void CreatePyramid();
	void CreateAniCube();
	void UpdateAniCube(float _frames);

protected:

	GLuint m_pTexture;
	GLuint VBO;
	GLuint VAO;
	GLuint EBO;

	GLuint IndiceCount;
	int m_iDrawType;
	float m_fFrames;
	
	//Vertices for a Quad
	GLfloat quadVertices[32]{

		//quad 4 corners
		//position 1			//colour 1			//Texture coords	
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		//top left
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		//bottom left
		0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f , 0.0f,		//bottm right
		0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f , 1.0f,		//top right
	};

	//Indices for the quad
	GLuint quadIndicies[6]{
		0, 1, 2,	 //first triangle
		0, 2, 3,	//second triangle
	};

	//Vertices for a Hex
	GLfloat hexVertices[56]{

	
		//position 				//colour 			//Texture coords	
		-0.525f, -0.25f, 0.0f,	1.0f, 1.0f, 0.0f,	-0.5f, 0.5f,	//middle left
		-0.4f, -0.50f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,		//bottom left
		-0.15f, -0.50f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		//bottom right
		-0.025f, -0.25f, 0.0f,	1.0f, 1.0f, 0.0f,	1.5f, 0.5f,		//middle right
		-0.15f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,		//top right
		-0.4f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		//top left
		-0.25f, -0.25f, 0.0f,	0.0f, 0.0f, 0.0f,	0.5f, 0.5f,		//centre

	};

	//Indices for the hexagons
	GLuint hexIndicies[18]{
		
		1, 6, 0,
		2, 6, 1,
		3, 6, 2,
		4, 6, 3,
		5, 6, 4,
		0, 6, 5,
		
	};

	//Vertices for the Pyramid
	GLfloat pyramidVertices[104]{

		//position 				//normals			//Texture coords	
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		//side 1
		-0.5f, 0.0f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
		0.5f, 0.0f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		//side 2
		0.5f, 0.0f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		//side 3
		0.5f, 0.0f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		//side 4
		-0.5f, 0.0f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f,		-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		//top point
		0.0f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.05f, 0.0f,
	};
	//Indicies for pyramid
	GLuint pyramidIndices[18]{
		1,0,3,
		1,3,2,

		4,12,5,
		6,12,7,
		8,12,9,
		10,12,11,
	};
	//Vertices for the Cube
	GLfloat cubeVertices[216]{

		//position 				//normals			//Texture coords	
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
	//Indicies for Cube
	GLuint cubeIndices[36]{
		0,1,2,
		0,2,3,

		7,6,5,
		7,5,4,

		8,9,10,
		8,10,11,

		12,13,14,
		12,14,15,

		16,17,18,
		16,18,19,

		20,21,22,
		20,22,23,
	};
		

};