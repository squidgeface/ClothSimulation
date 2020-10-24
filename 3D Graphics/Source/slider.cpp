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

void CSlider::Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, const char* path, float frameCount, vec3 _scale, vec3 _rotate, vec3 _translate)
{
    Background->Initialise(camera, timer, input, type, "", frameCount, _scale, _rotate, _translate);
    Arrow->Initialise(camera, timer, input, type, "", frameCount, vec3(_scale.x/10.0f,(_scale.y/2.0f),_scale.z), _rotate, vec3(_translate.x,_translate.y+10.0f,_translate.z));
    Background->InitialiseTextures(path, 1);
    Arrow->InitialiseTextures("Resources/Textures/arrow.png", 1);
    Input = input;
}

void CSlider::Render(GLuint program)
{
    Background->RenderShapes(program);
    Arrow->RenderShapes(program);
}

void CSlider::Update()
{
    Background->UpdateShapes();
    Arrow->UpdateShapes();
    Slide();
    SetClothSize();
    SetAnchorSize();
}

void CSlider::Slide()
{
    if (CheckHover() && Input->GetMouseState()[0] == InputState::INPUT_DOWN)
    {
        if (Arrow->GetObjPosition().x + Arrow->GetObjSize().x < Background->GetObjPosition().x + Background->GetObjSize().x &&
            Arrow->GetObjPosition().x - Arrow->GetObjSize().x > Background->GetObjPosition().x - Background->GetObjSize().x)
        {
            Arrow->SetObjPosition(vec3(Input->GetMouseX(), Arrow->GetObjPosition().y, Arrow->GetObjPosition().z));
        }
    }
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
    }
    size = distance;
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
        distance = 4;
    }
    else if (distance > 16 && distance <= 25)
    {
        distance = 5;
    }
    else if (distance > 25 && distance <= 36)
    {
        distance = 6;
    }
    else if (distance > 36 && distance <= 49)
    {
        distance = 7;
    }
    else if (distance > 49 && distance <= 72)
    {
        distance = 8;
    }
    else if (distance > 72 && distance <= 81)
    {
        distance = 9;
    }
    else if (distance > 81)
    {
        distance = 10;
    }

    if (distance > sqrtf(size))
    {
        distance = sqrtf(size);
    }
    anchors = distance;
}
