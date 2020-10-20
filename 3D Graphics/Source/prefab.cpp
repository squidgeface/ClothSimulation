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
#include "ShaderLoader.h"


CPrefab::CPrefab()
	: m_bRotations(false)
	, m_bStencil(false)
	, m_eMeshType(MeshType::EMPTY)
	, m_v3LightPosition(vec3(10.0f, 500.0f, 0.0f))
	, m_v3LightColour(vec3(1.0f, 1.0f, 1.0f))
{
	m_pMesh = new CMesh();
	m_pTexture = new CTexture();
}

CPrefab::~CPrefab()
{
	delete m_pMesh;
	m_pMesh = 0;
	delete m_pCamera;
	m_pCamera = 0;
	delete m_pTime;
	m_pTime = 0;
	delete m_pInput;
	m_pInput = 0;
	delete m_pCubeMap;
	m_pCubeMap = 0;
	delete m_pTexture;
	m_pTexture = 0;
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

		break;
	case MeshType::GEOMETRY:


		m_pMesh->CreateGrometry();
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

	switch (blendType)
	{
	case BlendType::ADD:
		glBlendFunc(GL_SRC_ALPHA, GL_FUNC_ADD);
		break;
	case BlendType::SUB:
		glBlendFunc(GL_SRC_ALPHA, GL_FUNC_SUBTRACT);
		break;
	case BlendType::MULTI:
		glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
		break;
	case BlendType::TRANS:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	default:
		break;
	}

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	if ( m_eMeshType == MeshType::CUBE || m_eMeshType == MeshType::SPHERE || m_eMeshType == MeshType::MODEL)
	{
		//set CCW winding
		glFrontFace(GL_CCW);
	}

	if (m_eMeshType == MeshType::TERRAIN || m_eMeshType == MeshType::TESSELATED)
	{
		//set CW winding
		glFrontFace(GL_CCW);
	}

	glUseProgram(program);

	RenderShadows(program);

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
	
	//Camera proj
	GLint camProjLoc = glGetUniformLocation(program, "proj");
	glUniformMatrix4fv(camProjLoc, 1, GL_FALSE, glm::value_ptr(m_pCamera->GetCamera()));
	
	//Camera view
	GLint camViewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(camViewLoc, 1, GL_FALSE, glm::value_ptr(m_pCamera->GetView()));

	//Lighting position and colour
	GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(m_v3LightPosition));

	GLint lightColLoc = glGetUniformLocation(program, "lightColor");
	glUniform3fv(lightColLoc, 1, glm::value_ptr(m_v3LightColour));

	//switch based on mesh type
	switch (m_eMeshType)
	{
	case MeshType::QUAD:
	{
		//regular render
		m_pTexture->Activate(program, 1);
		//Draw the shapes
		m_pMesh->Draw();

		break;
	}
	case MeshType::CUBE:
	{
		

		//regular render
		m_pTexture->Activate(program, 1);
		if (m_bShadows)
		{
			RenderShadows(program);
		}
		m_pMesh->Draw();

		
		break;
	}
	case MeshType::SPHERE:
	{	
		
		//regular render
		m_pTexture->Activate(program, 1);
		if (m_bShadows)
		{
			RenderShadows(program);
		}
		//Draw the shapes
		m_pMesh->Draw();

		break;
	}
	case MeshType::MODEL:
	{
		//render model mesh
		
		m_pTexture->Activate(program, 1);
		if (m_bShadows)
		{
			RenderShadows(program);
		}
		//m_pAniModel->render(m_pTime->GetDelta(), m_pNoise);
		break;
	}
	case MeshType::GEOMETRY:
	{

		//add justine goe code
		m_pTexture->Activate(program, 1);
		m_pMesh->RenderGeometry();

		break;
	}
	case MeshType::TERRAIN:
	{
		

		float inner = 0;
		float outer = 0;
		inner = (256 / Distance(m_pCamera->GetCamPos(), GetObjPosition())) * 100;
		outer = (256 / Distance(m_pCamera->GetCamPos(), GetObjPosition())) * 100;
		if (inner <= 256.0f)
		{
			inner = 256.0f;
			outer = 256.0f;
		}
		glUniform1f(glGetUniformLocation(program, "inner"), inner);
		glUniform1f(glGetUniformLocation(program, "outer"), outer);

		m_pTexture->Activate(program, 1);

		if (m_bShadows)
		{
			RenderShadows(program);
		}


		
		

		m_pMesh->RenderTesselated();
		break;
	}
	case MeshType::TESSELATED:
	{
		float inner = 0;
		float outer = 0;
		inner = (5/Distance(m_pCamera->GetCamPos(), GetObjPosition())) * 100;
		outer = (3/Distance(m_pCamera->GetCamPos(), GetObjPosition())) * 100;

		glUniform1f(glGetUniformLocation(program, "inner"), inner);
		glUniform1f(glGetUniformLocation(program, "outer"), outer);
		m_pTexture->Activate(program, 1);
		m_pMesh->RenderTesselated();
		break;
	}
	default:
		break;
	}

	

	//disable, bind and clear buffers, textures and disable stencil, blend, culling and depth
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}
//Update shape and time variables
void CPrefab::UpdateShapes(CCubemap* _cubeMap, CPrefab* _Object, CCamera* _Camera)
{
	if (_Camera != NULL)
	{
		m_pCamera = _Camera;
	}

	//load in cubemap
	m_pCubeMap = _cubeMap;

	//Model matrix calculation
	m_m4Model = m_m4TranslationMatrix * m_m4RotationZ * m_m4RotationX * m_m4RotationY *  m_m4ScaleMatrix;

	if (m_eMeshType == MeshType::MODEL)
	{
		//m_pAniModel->Update(m_m4Model);
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


//math for magnitude of a vector
float CPrefab::Magnitude(vec3 _source)
{
	return sqrtf((_source.x * _source.x) + (_source.y * _source.y)+ (_source.z * _source.z));
}
//math for the distance between two vectors
float CPrefab::Distance(vec3 _source, vec3 _target)
{
	return sqrtf((_source.x - _target.x) * (_source.x - _target.x) + (_source.y - _target.y) * (_source.y - _target.y) + (_source.z - _target.z) * (_source.z - _target.z));
}

//Set object rotating to mouse
void CPrefab::SetRotation(vec3 _rotation)
{
	if (_rotation.x != 0)
	{
		m_fRotationAngle = _rotation.x;
		m_m4RotationX = rotate(glm::mat4(), radians(m_fRotationAngle), m_v3RotationAxisX);
	}
	if (_rotation.y != 0)
	{
		m_fRotationAngle = _rotation.y;
		m_m4RotationZ = rotate(glm::mat4(), radians(m_fRotationAngle), m_v3RotationAxisY);
	}
	if (_rotation.z != 0)
	{
		m_fRotationAngle = _rotation.z;
		m_m4RotationY = rotate(glm::mat4(), radians(m_fRotationAngle), m_v3RotationAxisZ);
	}
}

float CPrefab::GetRoataionY()
{
	return m_fRotationAngle;
}

void CPrefab::SetBlend(BlendType _blend)
{
	blendType = _blend;
}



vec3 CPrefab::GetLightPos()
{
	return m_v3LightPosition;
}

void CPrefab::EnableShadows(const GLuint& _texture)
{
	m_bShadows = true;
	shadowTexture = _texture;
}

void CPrefab::DisableShadows()
{
	m_bShadows = false;
}

void CPrefab::RenderShadows(GLuint program)
{
	mat4 lightViewMatrix = lookAt(m_v3LightPosition, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4 lightVPMatrix = m_pCamera->GetCamera() * lightViewMatrix;
	GLint vpLoc = glGetUniformLocation(program, "lightVPMatrix");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, value_ptr(lightVPMatrix));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowTexture);
	glUniform1i(glGetUniformLocation(program, "shadowMap"), 1);
	
}