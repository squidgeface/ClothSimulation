// (c) 2020 Alexander Jenkins
//
// File Name   : input.cpp
// Description : input implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "input.h"


CInput::CInput()
	: m_icounter(0)
	, m_imouseX(0)
	, m_imouseY(0)
{
	//start with all keys set to up
	KeyState['w'] = InputState::INPUT_UP;
	KeyState['s'] = InputState::INPUT_UP;
	KeyState['a'] = InputState::INPUT_UP;
	KeyState['d'] = InputState::INPUT_UP;
	MouseState[0] = InputState::INPUT_UP;
}

CInput::~CInput()
{
}
//keyboard down and up functions
void CInput::KeyboardDown(unsigned char key, int x, int y)
{
	KeyState[key] = InputState::INPUT_DOWN;
}
void CInput::KeyboardUp(unsigned char key, int x, int y)
{
	KeyState[key] = InputState::INPUT_UP;
}
//mouse click and move functions
void CInput::MouseClick(int button, int state, int x, int y)
{
	if (button >= 3)
		return;

	CInput::MouseState[button] = (state == GLUT_DOWN) ? InputState::INPUT_DOWN : InputState::INPUT_UP;
}
void CInput::MouseMove(int _x, int _y)
{

	m_imouseX = _x - (int)Utils::HalfScreenW;
	m_imouseY = _y - (int)Utils::HalfScreenH;
}
//get the current keystate
InputState* CInput::GetKeyState()
{
	return KeyState;
}
//get the current mouse state
InputState* CInput::GetMouseState()
{
	return MouseState;
}
//get mouse x position
int CInput::GetMouseX()
{
	return m_imouseX;
}
//get mouse y position
int CInput::GetMouseY()
{
	return m_imouseY;
}
