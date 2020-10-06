// (c) 2020 Alexander Jenkins
//
// File Name   : prefab.cpp
// Description : prefab implementaion file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "prefab.h"
#include "mesh.h"
#include "Texture.h"
#include "camera.h"
#include "time.h"
#include "input.h"
#include "cubemap.h"
#include "Model.h"


CPrefab::CPrefab()
	: m_fTexPos(0)
	, m_bRotations(false)
	, m_iMx(0)
	, m_iMy(0)
	, m_pCamera(0)
	, m_pInput(0)
	, m_pTime(0)
	, m_fRotationAngle(0)
	, m_iX(0)
	, m_iY(0)
	, m_pObjMesh(0)
	, m_pCubeMap(0)
	, m_eMeshType(MeshType::EMPTY)
	, m_v3LightPosition(vec3(0.0f, 60.0f, 0.0f))
	, m_v3LightColour(vec3(1.0f, 1.0f, 1.0f))
{
	m_pMesh = new CMesh();
	m_pTexture = new CTexture();
}

CPrefab::CPrefab(int mx, int my)
	: m_fTexPos(0)
	, m_bRotations(false)
	, m_pCamera(0)
	, m_pInput(0)
	, m_pTime(0)
	, m_pObjMesh(0)
	, m_fRotationAngle(0.0f)
	, m_iX(0)
	, m_iY(0)
	, m_iMx(mx)
	, m_iMy(my)
	, m_pCubeMap(0)
	, m_eMeshType(MeshType::EMPTY)
	, m_v3LightPosition(vec3(0.0f, 60.0f, 0.0f))
	, m_v3LightColour(vec3(1.0f, 1.0f, 1.0f))
{
	m_pMesh = new CMesh();
	m_pTexture = new CTexture();
}

CPrefab::~CPrefab()
{
	delete m_pMesh;
	m_pMesh = 0;
	delete m_pTexture;
	m_pTexture = 0;
	delete m_pObjMesh;
	m_pObjMesh = 0;
}

void CPrefab::Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, string path, float frameCount, vec3 _scale, vec3 _rotate, vec3 _translate)
{

	m_pInput = input;
	m_pTime = timer;
	m_pCamera = camera;
	m_eMeshType = type;
	

	switch (type)
	{
	case MeshType::QUAD:
		m_pMesh->CreateQuad();
		break;
	case MeshType::CUBE:
		m_pMesh->CreateCube();
		break;
	case MeshType::SPHERE:
		m_pMesh->CreateSphere();
		break;
	case MeshType::MODEL:
		m_pObjMesh = new CModel(path, camera);
		break;
	default:
		break;
	}

	//position
	m_v3ObjPosition = _translate;
	m_m4TranslationMatrix = translate(mat4(), m_v3ObjPosition);

	//Rotation
	m_v3RotationAxisX = vec3(1.0f, 0.0f, 0.0f);
	m_v3RotationAxisY = vec3(0.0f, 1.0f, 0.0f);
	m_v3RotationAxisZ = vec3(0.0f, 0.0f, 1.0f);
	m_fRotationAngle = _rotate.x;
	m_m4RotationX = rotate(glm::mat4(), radians(m_fRotationAngle), m_v3RotationAxisX);
	m_fRotationAngle = _rotate.y;
	m_m4RotationZ = rotate(glm::mat4(), radians(m_fRotationAngle), m_v3RotationAxisY);
	m_fRotationAngle = _rotate.z;
	m_m4RotationY = rotate(glm::mat4(), radians(m_fRotationAngle), m_v3RotationAxisZ);

	//Scale
	m_v3ObjScale = _scale;
	m_m4ScaleMatrix = scale(mat4(), m_v3ObjScale);

	//get this object start position relative to constructor input x,y position
	m_iX = (int)m_v3ObjPosition.x - m_iMx;
	m_iY = (int)m_v3ObjPosition.z - m_iMy;
}

//Initialise Texture as static animated
void CPrefab::InitialiseTextures(const char* data, int slot)
{
	m_pTexture->Generate(data, slot);
}
//Render the Shape
void CPrefab::RenderShapes(GLuint program, int slot)
{
	//enable blending and alpha channels
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (m_eMeshType == MeshType::CUBE || m_eMeshType == MeshType::SPHERE || m_eMeshType == MeshType::MODEL)
	{
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
	}

	glUseProgram(program);

	//objects local properties and matrix transformation rendering
	GLuint comboLoc = glGetUniformLocation(program, "MVP");
	glUniformMatrix4fv(comboLoc, 1, GL_FALSE, value_ptr(MVP));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_m4Model));

	//Texture position
	GLint texPosLoc = glGetUniformLocation(program, "texPos");
	glUniform1f(texPosLoc, m_fTexPos);	

	//Camera Position
	GLint camPosLoc = glGetUniformLocation(program, "camPos");
	glUniform3fv(camPosLoc, 1, glm::value_ptr(m_pCamera->GetCamPos()));
	
	//Lighting position and colour
	GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(m_v3LightPosition));

	GLint lightColLoc = glGetUniformLocation(program, "lightColor");
	glUniform3fv(lightColLoc, 1, glm::value_ptr(m_v3LightColour));

	//switch based on mesh type
	switch (m_eMeshType)
	{

	case MeshType::QUAD:
		
		if (slot == 1)
		{
			//regular render
			m_pTexture->Activate(program, 1);
		}
		else if (slot == 2)
		{
			//regular render
			m_pTexture->Activate(program, 2);
		}
		//Draw the shapes
		m_pMesh->Draw();

		break;

	case MeshType::CUBE:
		//regular render
		m_pTexture->Activate(program, 1);

		if (m_pCubeMap != NULL)
		{
			//cubeMap
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(glGetUniformLocation(program, "cubeMap"), 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_pCubeMap->GetTextureID());
		}
		//Draw the shapes
		m_pMesh->Draw();
		break;
	
	case MeshType::SPHERE:
		//regular render
		m_pTexture->Activate(program, 1);
		if (m_pCubeMap != NULL)
		{
			//cubeMap
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(glGetUniformLocation(program, "cubeMap"), 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_pCubeMap->GetTextureID());
		}
		//Draw the shapes
		m_pMesh->Draw();

		break;

	case MeshType::MODEL:
		m_pObjMesh->Render();
		m_pTexture->Activate(program, 1);
		if (m_pCubeMap != NULL)
		{
			//cubeMap
			glActiveTexture(GL_TEXTURE1);
			glUniform1i(glGetUniformLocation(program, "cubeMap"), 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_pCubeMap->GetTextureID());
		}
		break;

	default:
		break;
	}

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
}
//Update shape and time variables
void CPrefab::UpdateShapes(CCubemap* _cubeMap, CPrefab* _Object)
{
	//load in cubemap
	m_pCubeMap = _cubeMap;

	//Rotate player towards the mouse
	if (m_bRotations)
	{
		RotateToObject(_Object);
	}

	//Model matrix calculation
	m_m4Model = m_m4TranslationMatrix * m_m4RotationZ * m_m4RotationX * m_m4RotationY *  m_m4ScaleMatrix;

	if (m_eMeshType == MeshType::MODEL)
	{
		m_pObjMesh->Update(m_m4Model);
	}

	//Combo matrix
	MVP = m_pCamera->GetCamera() * m_pCamera->GetView() * m_m4Model;

}


//set the object x,y position
void CPrefab::SetObjPosition(vec3 _position)
{
	m_v3ObjPosition = _position;
	m_m4TranslationMatrix = translate(mat4(), m_v3ObjPosition);
}
//set the object moving in a particular x, y direction (based on constructor input)
void CPrefab::SetObjectVector()
{
	//move object based on input object position captured in constructor
	double direction = atan2(m_iY, m_iX);
	double dx = cos(direction);
	double dy = sin(direction);

	m_v3ObjPosition += vec3(-dx * m_pTime->GetDelta() * 200, 0.0f, -dy * m_pTime->GetDelta() * 200);
	m_m4TranslationMatrix = translate(mat4(), m_v3ObjPosition);
}

//return object size
vec3 CPrefab::GetObjSize()
{
	return m_v3ObjScale;
}
//return object position
vec3 CPrefab::GetObjPosition()
{
	return m_v3ObjPosition;
}

//Math for rotating player toward an object
void CPrefab::RotateToObject(CPrefab* _object)
{
	float x;
	float z;

	x = m_v3ObjPosition.x - _object->GetObjPosition().x;
	z = m_v3ObjPosition.z - _object->GetObjPosition().z;

	m_fRotationAngle = atan2f(-x, -z);

	m_v3RotationAxisY = vec3(0.0f, 1.0f, 0.0f);

	m_m4RotationZ = rotate(mat4(), m_fRotationAngle, m_v3RotationAxisY);
}

//shrink an object x and y
void CPrefab::Shrink(float xScale, float yScale, float zScale)
{
	//Scale
	m_v3ObjScale -= vec3(xScale, yScale, zScale) * (m_pTime->GetDelta() * 50);
	m_m4ScaleMatrix = scale(mat4(), m_v3ObjScale);
}
//Set whether this prefab is going to rotate

//math for magnitude of a vector
float CPrefab::Magnitude(vec2 _source)
{
	return sqrtf((_source.x * _source.x) + (_source.y * _source.y));
}
//math for the distance between two vectors
float CPrefab::Distance(vec3 _source, vec3 _target)
{
	return sqrtf((_source.x - _target.x) * (_source.x - _target.x) + (_source.y - _target.y) * (_source.y - _target.y) + (_source.z - _target.z) * (_source.z - _target.z));
}

//set light position for prefabs
void CPrefab::SetLightPosition(vec3 _lightPos)
{
	m_v3LightPosition = _lightPos;
}
//set light colour for prefabs
void CPrefab::SetLightColour(vec3 _lightCol)
{
	m_v3LightColour = _lightCol;
}
//load in pre existing model for multiple prefabs
void CPrefab::SetModel(CModel* _model)
{
	m_pObjMesh = _model;
	m_eMeshType = MeshType::MODEL;
}
//Set object rotating to mouse
void CPrefab::SetRotating()
{
	m_bRotations = !m_bRotations;
}