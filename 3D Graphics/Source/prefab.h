// (c) 2020 Alexander Jenkins
//
// File Name   : prefab.h
// Description : prefab declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//


#pragma once

#include "Utils.h"

class CMesh;
class CTexture;
class CCamera;
class CTime;
class CInput;
class CCubemap;


class CPrefab
{

public:

	CPrefab();
	~CPrefab();

	//Game Functions
	virtual void Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, string path, float frameCount = 0.0f, vec3 scale = vec3(0.0, 0.0, 0.0), vec3 rotate = vec3(0.0, 0.0, 0.0), vec3 translate = vec3(0.0, 0.0, 0.0));
	void InitialiseTextures(const char* data, int slot);
	virtual void RenderShapes(GLuint program, int slot = 1);
	virtual void UpdateShapes(CCubemap* cubeMap = NULL, CPrefab* _Object = NULL, CCamera* = NULL);

	//math functions
	float Magnitude(vec3 _source);
	float Distance(vec3 _source, vec3 _target);

	//set functions
	void SetRotation(vec3 _rotation);
	float GetRoataionY();
	void SetBlend(BlendType _blend);
	void SetObjPosition(vec3 _position);

	//get functions
	vec3 GetObjSize();
	vec3 GetObjPosition();


	vec3 GetLightPos();

	void EnableShadows(const GLuint& _texture);

	void DisableShadows();

	void RenderShadows(GLuint program);



protected:

	//class pointers
	CMesh* m_pMesh = 0;
	CTexture* m_pTexture = 0;
	CCamera* m_pCamera = 0;
	CTime* m_pTime = 0;
	CInput* m_pInput = 0;
	CCubemap* m_pCubeMap = 0;
	//Matrix manipulation variables
	vec3 m_v3ObjPosition, m_v3RotationAxisX, m_v3RotationAxisY, m_v3RotationAxisZ, m_v3ObjScale;
	mat4 m_m4TranslationMatrix, m_m4RotationX, m_m4RotationZ, m_m4RotationY, m_m4ScaleMatrix, m_m4Model, MVP;
	float m_fRotationAngle = 0, m_fTexPos = 0;
	bool m_bRotations, m_bStencil, m_bShadows = false;
	MeshType m_eMeshType;
	vec3 m_v3LightPosition;
	vec3 m_v3LightColour;
	BlendType blendType = BlendType::ADD;
	GLuint shadowTexture = 0;
	
};