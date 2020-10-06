// (c) 2020 Alexander Jenkins
//
// File Name   : TextLabel.h
// Description : TextLabel declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "Utils.h"

class CGameManager;

struct FontChar
{
	GLuint TextureID;	// texture ID
	ivec2 Size;			// size of the glyph
	ivec2 Bearing;		// offset of the glyph (top left) from the baseline
	GLuint Advance;		// how far to move for the next character
};

class CTextLabel
{

public:
	CTextLabel();
	~CTextLabel();

	void SetLabel(string text, string font, vec2 pos, vec3 color, float scale);
	void Render();
	void SetText(string newText) { text = newText; };
	void SetColor(vec3 newColor) { color = newColor; };
	void SetScale(GLfloat newScale) { scale = newScale; };
	void SetPosition(vec2 newPosition) { position = newPosition; };

private:

	GLuint GenerateTexture(FT_Face face);

	string text;
	GLfloat scale;
	vec3 color;
	vec2 position;

	GLuint VAO, VBO, textProgram;
	mat4 proj;
	map<GLchar, FontChar> Characters;
};