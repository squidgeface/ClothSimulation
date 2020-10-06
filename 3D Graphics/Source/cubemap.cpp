// (c) 2020 Alexander Jenkins
//
// File Name   : cubemap.cpp
// Description : cubemap implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "cubemap.h"
#include "camera.h"

CCubemap::CCubemap(CCamera* acamera, vector<string> filePaths)
	: Wrap_S(GL_CLAMP_TO_EDGE)
	, Wrap_T(GL_CLAMP_TO_EDGE)
	, Filter_Min(GL_LINEAR)
	, Filter_Max(GL_LINEAR)
	, m_pCamera(acamera)
	, Textures(filePaths)
{
}

void CCubemap::Generate()
{
	//generate vertex shape
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

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
		8 * sizeof(GLfloat),				//Stride = pos + normal + texture
		(GLvoid*)(6 * sizeof(GLfloat)));	//offset from beginning of Vertex (bypass pos and colour)

	glEnableVertexAttribArray(2);


//Texture generation
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);

	unsigned char* image;
	int width, height;

	for (GLuint i = 0; i < 6; i++)
	{
		string fullPathName = "Resources/Textures/CubeMap/";
		fullPathName.append(Textures[i]);

		image = SOIL_load_image(fullPathName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);
	}


	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, this->Filter_Max);


	// Unbind texture
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CCubemap::Update()
{
	mat4 m_m4Model = scale(mat4(), vec3(2000.0f, 2000.0f, 2000.0f));
	MVP = m_pCamera->GetVPMatrix() * m_m4Model;
}

void CCubemap::Render(GLuint program)
{
	glUseProgram(program);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
	glUniform1i(glGetUniformLocation(program, "cubeSampler"), 0);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, value_ptr(MVP));

	glBindVertexArray(VAO); //bind VAO
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); //unbind VAO

}

GLuint CCubemap::GetTextureID()
{
	return Texture;
}