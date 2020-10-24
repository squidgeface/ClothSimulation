// (c) 2020 Alexander Jenkins
//
// File Name   : button.h
// Description : button declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "prefab.h"

class CInput;

class CSlider : public CPrefab
{
public:
	CSlider():CPrefab() { Arrow = new CPrefab(); Background = new CPrefab();};

	bool CheckHover();

	void Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, const char* path, float frameCount, vec3 _scale, vec3 _rotate, vec3 _translate);

	void Render(GLuint program);

	void Update();

	void Slide();
	int GetClothSize();
	int GetAnchorSize();

	void SetClothSize();
	void SetAnchorSize();
		

private:
	CPrefab* Arrow = 0;
	CPrefab* Background = 0;
	CInput* Input = 0;
	int size = 100;
	int anchors = 10;
};

