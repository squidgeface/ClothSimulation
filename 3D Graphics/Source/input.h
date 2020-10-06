// (c) 2020 Alexander Jenkins
//
// File Name   : input.h
// Description : input declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "gameManager.h"

//Prototype classes
class CCamera;

class CInput
{

public:
	CInput();
	~CInput();

	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	InputState* GetKeyState();
	InputState* GetMouseState();
	int GetMouseX();
	int GetMouseY();

protected:

	int m_imouseX,  m_imouseY, m_icounter;

	InputState KeyState[255];
	InputState MouseState[3];

};