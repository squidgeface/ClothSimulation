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
class CTextLabel;

class CButton : public CPrefab
{
public:
	CButton() : CPrefab(), m_bIsBtn(false){};

	bool CheckHover(CInput* _object);
	void SetButton(bool state);
	void RenderShapes(GLuint program);
	void SetText(char* _text);
	bool GetShowing();

private:
	bool m_bIsBtn = false, m_bText = false;
	CTextLabel* m_pText = 0;
};

