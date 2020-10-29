// (c) 2020 Media Design School
//
// File Name   : button.cpp
// Description : button implementation file
// Author      : alexander jenkins, Raven Clancey-Peetz
// Mail        : alexander.jenkins@mds.ac.nz, Raven-Clancey-Peetz@mediadesign.school.nz
//

#include "button.h"
#include "input.h"
#include "TextLabel.h"

//check if mouse is over button
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

//change state of button
void CButton::SetButton(bool _state)
{
    m_bIsBtn = _state;
}

//render buttons
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

//set the text of the button
void CButton::SetText(const char* _text)
{
    m_pText->SetLabel(_text,"Resources/Fonts/BRLNSR.ttf", vec2(GetObjPosition().x + GetObjSize().x/2 + 5.0f, -(GetObjPosition().y + GetObjSize().y / 4)), vec3(0.0f, 0.0f, 0.0f), 0.5f);
    m_bText = true;
}

