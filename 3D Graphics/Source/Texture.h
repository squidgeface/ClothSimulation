// (c) 2020 Alexander Jenkins
//
// File Name   : Texture.h
// Description : Texture declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "Utils.h"

class CTexture
{
public:
	// Constructor (sets default texture modes)
	CTexture();
	// Deconstructor
	~CTexture();
	// Generates texture from image data
	void Generate(const char* data, int slot);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Activate(GLuint program, int slot);

	//Variables
protected:
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
	//Texture
	vector<GLuint*> Textures;
	int m_iTextureCount;

};
