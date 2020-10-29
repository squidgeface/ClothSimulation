// (c) 2020 Media Design School
//
// File Name   : slider.h
// Description : slider declaration file
// Author      : alexander jenkins, Raven Clancey-Peetz
// Mail        : alexander.jenkins@mds.ac.nz, Raven-Clancey-Peetz@mediadesign.school.nz
//


#pragma once

#include "prefab.h"

class CInput;

class CSlider : public CPrefab
{
public:
	CSlider():CPrefab() { Arrow = new CPrefab(); Background = new CPrefab();};

	bool CheckHover();

	void Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, const char* path, float frameCount, vec3 _scale, vec3 _rotate, vec3 _translate, bool isAnchor);

	void Render(GLuint program);

	void Update();

	void Slide();
	void ResetMouse();
	int GetMouse();
	int GetClothSize();
	int GetAnchorSize();

	void SetClothSize();
	void SetClothSizeNumber(int num);
	void SetAnchorSize();
		

private:
	CPrefab* Arrow = 0;
	CPrefab* Background = 0;
	CInput* Input = 0;
	int size = 100;
	int anchors = 10;
	bool m_bisAnchor = false;
	int mouseDown = 0;
};

