// (c) 2020 Alexander Jenkins
//
// File Name   : button.h
// Description : button implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "slider.h"
#include "input.h"

bool CSlider::CheckHover()
{
    if (Input->GetMouseX() > (Background->GetObjPosition().x - (Background->GetObjSize().x / 2)) && Input->GetMouseX() < (Background->GetObjPosition().x + (Background->GetObjSize().x / 2)) &&
        Input->GetMouseY() > (Background->GetObjPosition().y - (Background->GetObjSize().y / 2)) && Input->GetMouseY() < (Background->GetObjPosition().y + (Background->GetObjSize().y / 2)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CSlider::Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, const char* path, float frameCount, vec3 _scale, vec3 _rotate, vec3 _translate, bool isAnchor)
{
    Background->Initialise(camera, timer, input, type, "", frameCount, _scale, _rotate, _translate);
    Arrow->Initialise(camera, timer, input, type, "", frameCount, vec3(_scale.x/10.0f,(_scale.y/2.0f),_scale.z), _rotate, vec3(_translate.x + 90.0f,_translate.y+10.0f,_translate.z));
    Background->InitialiseTextures(path, 1);
    Arrow->InitialiseTextures("Resources/Textures/arrow.png", 1);
    Input = input;
    m_bisAnchor = isAnchor;
}

void CSlider::Render(GLuint program)
{
    Arrow->RenderShapes(program);
    Background->RenderShapes(program);
  
}

void CSlider::Update()
{
    Background->UpdateShapes();
    Arrow->UpdateShapes();
    Slide();

    if (m_bisAnchor) {
        SetAnchorSize();
    }
    else {
        SetClothSize();
    }
   
  
}

void CSlider::Slide()
{
    if (CheckHover() && Input->GetMouseState()[0] == InputState::INPUT_DOWN)
    {
        if (Arrow->GetObjPosition().x + Arrow->GetObjSize().x < Background->GetObjPosition().x + Background->GetObjSize().x &&
            Arrow->GetObjPosition().x - Arrow->GetObjSize().x > Background->GetObjPosition().x - Background->GetObjSize().x)
        {
            Arrow->SetObjPosition(vec3(Input->GetMouseX(), Arrow->GetObjPosition().y, Arrow->GetObjPosition().z));
            mouseDown = 1;
        }
    }
    if (mouseDown == 1 && Input->GetMouseState()[0] == InputState::INPUT_UP)
    {
        mouseDown = 2;
    }
}

void CSlider::ResetMouse()
{
    mouseDown = 0;
}

int CSlider::GetMouse()
{
    return mouseDown;
}

int CSlider::GetClothSize()
{
    return size;
}

int CSlider::GetAnchorSize()
{
    return anchors;
}

void CSlider::SetClothSize()
{
    int distance = Distance(vec3(Background->GetObjPosition().x - Background->GetObjSize().x/2, Background->GetObjPosition().y, Background->GetObjPosition().z), vec3(Arrow->GetObjPosition().x, Background->GetObjPosition().y, Background->GetObjPosition().z))/2;
    if (distance <= 9)
    {
        distance = 3;
    }
    else if (distance > 9 && distance <= 16)
    {
        distance = 6;
    }
    else if (distance > 16 && distance <= 25)
    {
        distance = 9;
    }
    else if (distance > 25 && distance <= 36)
    {
        distance = 12;
    }
    else if (distance > 36 && distance <= 49)
    {
        distance = 15;
    }
    else if (distance > 49 && distance <= 72)
    {
        distance = 17;
    }
    else if (distance > 72 && distance <= 81)
    {
        distance = 19;
    }
    else if (distance > 81)
    {
        distance = 20;
    }
    /*if (distance <= 9)
    {
        distance = 9;
    }
    else if (distance > 9 && distance <= 16)
    {
        distance = 16;
    }
    else if (distance > 16 && distance <= 25)
    {
        distance = 25;
    }
    else if (distance > 25 && distance <= 36)
    {
        distance = 36;
    }
    else if (distance > 36 && distance <= 49)
    {
        distance = 49;
    }
    else if (distance > 49 && distance <= 72)
    {
        distance = 72;
    }
    else if (distance > 72 && distance <= 81)
    {
        distance = 81;
    }
    else if (distance > 81)
    {
        distance = 100;
    }*/
    size = distance;
}

void CSlider::SetClothSizeNumber(int num)
{
    
    size = num;
}

void CSlider::SetAnchorSize()
{
    int distance = Distance(vec3(Background->GetObjPosition().x - Background->GetObjSize().x / 2, Background->GetObjPosition().y, Background->GetObjPosition().z), vec3(Arrow->GetObjPosition().x, Background->GetObjPosition().y, Background->GetObjPosition().z)) / 2;
    if (distance <= 9)
    {
        distance = 3;
    }
    else if (distance > 9 && distance <= 16)
    {
        distance = 6;
    }
    else if (distance > 16 && distance <= 25)
    {
        distance = 9;
    }
    else if (distance > 25 && distance <= 36)
    {
        distance = 12;
    }
    else if (distance > 36 && distance <= 49)
    {
        distance = 15;
    }
    else if (distance > 49 && distance <= 72)
    {
        distance = 17;
    }
    else if (distance > 72 && distance <= 81)
    {
        distance = 19;
    }
    else if (distance > 81)
    {
        distance = 20;
    }

    if (distance > size)
    {
        distance = size;
    }
    anchors = distance;
}
