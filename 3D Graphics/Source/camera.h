// (c) 2020 Alexander Jenkins
//
// File Name   : camera.h
// Description : camera declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//


#pragma once

#include "Utils.h"

class CTime;

class CCamera
{

public:

	CCamera();
	//~CCamera();
	//functions
	mat4 GetCamera();
	mat4 GetView();
	mat4 GetVPMatrix();
	void SetCamera(vec3 camPos, vec3 lookDir, vec3 camUpDir);
	void SetProjection();
	void SetOrthoUL();
	void SetOrtho();
	void LookAtObject(vec3 _objPositions);
	vec3 GetCamPos();
	vec3 GetCamLookDir();
	void CameraRotate(CTime* _time);
	void MoveCamera(vec3 _move, CTime* _time);

protected:
	//variables
	vec3 m_v3CamPos, m_v3CamLookDir, m_v3CamUpDir;
	mat4 m_m4View, m_m4Proj;
	CTime* m_pTime;
	float m_fTimeElapsed;
};
