// (c) 2020 Media Design School
//
// File Name   : button.h
// Description : button declaration file
// Author      : alexander jenkins, Raven Clancey-Peetz
// Mail        : alexander.jenkins@mds.ac.nz, Raven-Clancey-Peetz@mediadesign.school.nz
//

#pragma once

#include "prefab.h"
#include "TextLabel.h"

class CInput;
class CTextLabel;

class CButton : public CPrefab
{
public:
	CButton() : CPrefab() { m_pText = new CTextLabel(); };

	bool CheckHover(CInput* _object);
	void SetButton(bool state);
	void RenderShapes(GLuint program);
	void SetText(const char* _text);
	bool GetShowing();

private:
	bool m_bIsBtn = false, m_bText = false;
	CTextLabel* m_pText = 0;
};

