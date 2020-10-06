// (c) 2020 Alexander Jenkins
//
// File Name   : button.h
// Description : button implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "button.h"
#include "input.h"

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
}

//return if it is a button
bool CButton::GetShowing()
{
    return m_bIsBtn;
}

