// (c) 2020 Alexander Jenkins
//
// File Name   : TextLabel.cpp
// Description : TextLabel implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//
#include "TextLabel.h"

CTextLabel::CTextLabel()
	: text("")
	, scale(0)
	, color(0)
	, position(0)
	, VAO(0)
	, VBO(0)
	, textProgram(0)
{
}
CTextLabel::~CTextLabel()
{
	Characters.erase(Characters.begin(), Characters.end());
}

void CTextLabel::SetLabel(string newText, string font, vec2 pos, vec3 color, float scale)
{
	text = newText;
	SetColor(color);
	SetScale(scale);
	SetPosition(pos);

	proj = ortho(-Utils::HalfScreenW, Utils::HalfScreenW, -Utils::HalfScreenH, Utils::HalfScreenH);
	textProgram = ShaderLoader::CreateProgram("Resources/Shaders/textVertexShader.txt", "Resources/Shaders/textFragmentShader.txt");

	FT_Library ft;
	FT_Face face;

	if (FT_Init_FreeType(&ft) != 0)
	{
		//error
		cout << "Error" << endl;
	}

	if (FT_New_Face(ft, font.c_str(), 0, &face) != 0)
	{
		//error
		cout << "Error" << endl;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//load first 128 characters of asci set
	for (GLubyte character = 0; character < 128; character++)
	{
		//load the character into the glyph face
		if (FT_Load_Char(face, character, FT_LOAD_RENDER))
		{
			//error
			continue;
		}

		GLuint m_pTexture = GenerateTexture(face); //generate a texture for each char

		//store character for later use
		FontChar fontChar = { m_pTexture,
			ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};

		Characters.insert(pair<GLchar, FontChar>(character, fontChar));
	}

	//destroy FreeType objects
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//configure VAO/VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

void CTextLabel::Render()
{

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(textProgram);
	glUniform3f(glGetUniformLocation(textProgram, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(textProgram, "proj"), 1, GL_FALSE, value_ptr(proj));
	glBindVertexArray(VAO);

	vec2 textPos = position;
	for (string::const_iterator character = text.begin(); character != text.end(); character++)
	{
		FontChar fontChar = Characters[*character];
		GLfloat xpos = textPos.x + fontChar.Bearing.x * scale;
		GLfloat ypos = textPos.y - (fontChar.Size.y - fontChar.Bearing.y) * scale;
		GLfloat charWidth = fontChar.Size.x * scale;
		GLfloat charHeight = fontChar.Size.y * scale;

		//update VBO
		GLfloat vertices[6][4] = {
			{xpos, ypos + charHeight, 0.0, 0.0}, { xpos, ypos, 0.0, 1.0 }, {xpos + charWidth, ypos, 1.0, 1.0}, 
			{xpos, ypos + charHeight, 0.0, 0.0}, {xpos + charWidth, ypos, 1.0, 1.0}, {xpos + charWidth, ypos + charHeight, 1.0, 0.0},
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		//render the glyph
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fontChar.TextureID);
		glUniform1i(glGetUniformLocation(textProgram, "tex"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		textPos.x += (fontChar.Advance >> 6) * scale; //advance cursors to the next glyph
	}

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

}

GLuint CTextLabel::GenerateTexture(FT_Face face)
{
	GLuint m_pTexture;
	glGenTextures(1, &m_pTexture);
	glBindTexture(GL_TEXTURE_2D, m_pTexture);

	glTexImage2D(	GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED, GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return m_pTexture;
}
