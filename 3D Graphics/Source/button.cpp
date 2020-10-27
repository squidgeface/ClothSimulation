// (c) 2020 Alexander Jenkins
//
// File Name   : button.h
// Description : button implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "button.h"
#include "input.h"
#include "TextLabel.h"

bool CButton::CheckHover(CInput* _object)
{
    if (_object->GetMouseX() > (this->GetObjPosition().x - (this->GetObjSize().x / 2)) && _object->GetMouseX() < (this->GetObjPosition().x + (this->GetObjSize().x / 2)) &&
        _object->GetMouseY() > (this->GetObjPosition().y - (this->GetObjSize().y / 2)) && _object->GetMouseY() < (this->GetObjPosition().y + (this->GetObjSize().y / 2)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CButton::SetButton(bool _state)
{
    m_bIsBtn = _state;
}

void CButton::RenderShapes(GLuint program)
{
	if (m_bIsBtn)
	{
		//on button hover
		CPrefab::RenderShapes(program, 2);
	}
	else
	{
        CPrefab::RenderShapes(program, 1);
	}

    if (m_bText)
    {
        m_pText->Render();
    }
}

//return if it is a button
bool CButton::GetShowing()
{
    return m_bIsBtn;
}

void CButton::SetText(const char* _text)
{
    m_pText->SetLabel(_text,"Resources/Fonts/BRLNSR.ttf", vec2(GetObjPosition().x + GetObjSize().x/2 + 5.0f, -(GetObjPosition().y + GetObjSize().y / 4)), vec3(0.0f, 0.0f, 0.0f), 0.5f);
    m_bText = true;
}

