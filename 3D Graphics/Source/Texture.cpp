// (c) 2020 Alexander Jenkins
//
// File Name   : Texture.cpp
// Description : Texture implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//
#include "Texture.h"

CTexture::CTexture()
	: Wrap_S(GL_CLAMP_TO_EDGE)
	, Wrap_T(GL_CLAMP_TO_EDGE)
	, Filter_Min(GL_LINEAR_MIPMAP_LINEAR)
	, Filter_Max(GL_LINEAR)
	, m_iTextureCount(0)
{
}

CTexture::~CTexture()
{
	//clear vectors
	while (Textures.size() > 0)
	{
		GLuint* aTexture = Textures[Textures.size() - 1];
		Textures.pop_back();
		delete aTexture;
	}
}
//generate textues
void CTexture::Generate(const char* data, int slot)
{
	GLuint* m_pTexture = new GLuint;
	//create texture vector for multiple textures
	Textures.push_back(m_pTexture);

	glGenTextures(1, Textures[m_iTextureCount]);
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, *Textures[m_iTextureCount]);
	int width, height;
	unsigned char* image = SOIL_load_image(data, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	//increment texture count for multiple textures
	m_iTextureCount++;
	
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_REPEAT, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
}
//activate textures
void CTexture::Activate(GLuint program, int slot)
{
	//render textures
	if (Textures.size() > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *Textures[slot - 1]);
		glUniform1i(glGetUniformLocation(program, "tex"), 0);
	}
}