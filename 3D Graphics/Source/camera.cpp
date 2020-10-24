// (c) 2020 Alexander Jenkins
//
// File Name   : camera.cpp
// Description : camera implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "camera.h"
#include "time.h"

CCamera::CCamera()
	: m_pTime(0)
	, m_fTimeElapsed(0)
	, m_v3CamPos(vec3(0.0f, 0.0f, 3.0f))
	, m_v3CamLookDir(vec3(0.0f, 0.0f, -1.0f))
	, m_v3CamUpDir(vec3(0.0f, 1.0f, 0.0f))
	, m_m4View(lookAt(m_v3CamPos, m_v3CamPos + m_v3CamLookDir, m_v3CamUpDir))
{
}


//Get camera variables
mat4 CCamera::GetCamera()
{
	return m_m4Proj;
}
//get view variable
mat4 CCamera::GetView()
{
	return m_m4View;
}

mat4 CCamera::GetVPMatrix()
{
	return (m_m4Proj * m_m4View);
}

//Sets the camera to the input variables
void CCamera::SetCamera(vec3 camPos, vec3 lookDir, vec3 camUpDir)
{
	m_v3CamPos = camPos;
	m_v3CamLookDir = lookDir;
	m_v3CamUpDir = camUpDir;
	m_m4View = lookAt(m_v3CamPos, m_v3CamPos + m_v3CamLookDir, m_v3CamUpDir);
}

//Takes imput variables and sets camera to Projection
void CCamera::SetProjection()
{
	m_m4Proj = perspective(45.0f, (float)Utils::ScreenWidth / (float)Utils::ScreenHeight, 0.1f, 10000.0f);
}

//Takes imput variables and sets camera to orthographic (upper left display)
void CCamera::SetOrthoUL()
{
	m_m4Proj = ortho(0.0f, (float)Utils::ScreenWidth, (float)Utils::ScreenHeight, 0.0f, 0.1f, 100.0f);
}

void CCamera::SetOrtho()
{
	m_m4Proj = ortho(-(float)Utils::HalfScreenW, (float)Utils::HalfScreenW, (float)Utils::HalfScreenH, -(float)Utils::HalfScreenH, 0.1f, 100.0f);

}

void CCamera::LookAtObject(vec3 _objPositions)
{
	m_v3CamPos = vec3(0.0f, _objPositions.y - 100, 300.0f);
	m_v3CamLookDir = vec3(0.0f, _objPositions.y - 100, 0.0f);
	m_m4View = lookAt(m_v3CamPos, m_v3CamLookDir, m_v3CamUpDir);
}

vec3 CCamera::GetCamPos()
{
	return m_v3CamPos;
}

vec3 CCamera::GetCamLookDir()
{
	return m_v3CamLookDir;
}

void CCamera::CameraRotate(CTime* _time)
{
	m_fTimeElapsed += _time->GetDelta()/2;
	GLfloat radius = 100.0f;

	SetCamera(vec3(sin(m_fTimeElapsed) * radius, 0.0f, cos(m_fTimeElapsed) * radius), m_v3CamLookDir, m_v3CamUpDir);

	m_m4View = lookAt(m_v3CamPos, vec3(0.0f, 0.0f, 0.0f), m_v3CamUpDir);
}

void CCamera::MoveCamera(vec3 _move, CTime* _time)
{
	m_v3CamLookDir += _move * (_time->GetDelta() * 100);
	m_v3CamPos += _move * (_time->GetDelta() * 100);
	m_m4View = lookAt(m_v3CamPos, m_v3CamLookDir, m_v3CamUpDir);
}

