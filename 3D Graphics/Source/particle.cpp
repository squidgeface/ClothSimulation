// (c) 2020 Media Design School
//
// File Name   : particle.cpp
// Description : particle implementation file
// Author      : alexander jenkins, Raven Clancey-Peetz
// Mail        : alexander.jenkins@mds.ac.nz, Raven-Clancey-Peetz@mediadesign.school.nz
//

#include "particle.h"
#include "time.h"
#include "camera.h"

void CParticle::InitialiseGeo(CCamera* camera, CTime* timer, CInput* input, MeshType type, string path, float frameCount, vec3 _scale, vec3 _rotate, vec3 _translate)
{
	clothProgram = ShaderLoader::CreateProgram("Resources/Shaders/GeometryVS.txt", "Resources/Shaders/GeometryFS.txt", "Resources/Shaders/GeoGrassGS.txt");
	geo->Initialise(camera, timer, input, MeshType::GEOMETRY, path,frameCount, _scale, _rotate,_translate);
	geo->InitialiseTextures("Resources/Textures/green.bmp", 1);
}

//Particle update
void CParticle::Update()
{
	//Check if particle is one of the static points
	
		//nice gentle breeze
		float seedx = float(rand() % 80) - 37.0f;
		float seedz = float(rand() % 80) - 37.0f;
		
		//Seed with with values
		Wind = vec3(-seedx, 0.0f, -seedz);
		//Togle wind on and off qith 'Q'
		if (isWind)
		{
			ApplyForce(Wind * Mass);
		}
		//Eulers method
		//apply forces
		//ApplyForce(Gravity);
		////Use those forces to move object 
		//Accel -= Velocity * Damping / Mass;
		//Velocity += Accel * m_pTime->GetDelta();
		////limit velocity to graviy force
		//if (Velocity.y <= Gravity.y)
		//{
		//	Velocity.y = Gravity.y;
		//}
		////add velocity to current position
		//SetObjPosition(GetObjPosition() + Velocity * m_pTime->GetDelta());

		////varlet integration
		vec3 last_acceleration = Accel;
		SetObjPosition(GetObjPosition() + Velocity * m_pTime->GetDelta() + (0.5f * last_acceleration * pow(m_pTime->GetDelta(), 2)));
		//apply forces
		ApplyForce(Gravity);
		Accel -= Velocity * Damping / Mass;
		vec3 avg_acceleration = (last_acceleration + Accel) / 2.0f;
		Velocity += avg_acceleration * m_pTime->GetDelta();
		
		if (Velocity.y <= Gravity.y)
		{
			Velocity.y = Gravity.y;
		}


		
		//other particle links
		//for all the particles that have been linked
		for (size_t i = 0; i < OtherParts.size(); i++)
		{	
			//if the particle in the vector is not this one
			if (OtherParts[i] != this)
			{
			////math for calculating forces
				vec3 Delta = OtherParts[i]->GetObjPosition() - GetObjPosition();
				float deltaLength = Distance(OtherParts[i]->GetObjPosition(), GetObjPosition());
				float difference = (deltaLength - RestDist) / deltaLength;
				float Im1 = 1 / Mass;
				float Im2 = 1 / OtherParts[i]->GetMass();
				//calculate forces
				vec3 force = Delta * (Im1 / (Im1 + Im2)) * Stiffness * difference;
				vec3 force2 = Delta * (Im2 / (Im1 + Im2)) * Stiffness * difference;
				
				//if this object is not an anchor object
				if (!isAnchor)
				{
					//set forces for this object
					SetObjPosition(GetObjPosition() + force);
				}
			
				if (!OtherParts[i]->GetAnchor())
				{
					//Apply forces to other object
					OtherParts[i]->SetObjPosition(OtherParts[i]->GetObjPosition() - force2);
				}
		
				if (deltaLength > maxDistance)
				{
					OtherParts.erase(OtherParts.begin() + i);
				}
			}
		}
	//reset acceleration
		Accel = vec3();
		//Velocity = vec3();
		
	
	if (isAnchor)
	{
		//set static position
		SetObjPosition(vec3(GetObjPosition().x,0.0f,GetObjPosition().z));
	}
	else if (geo->GetObjPosition() != vec3())
	{
		geo->UpdateShapes();
		geo->SetObjPosition(GetObjPosition());
	}

}
//Apply force
void CParticle::ApplyForce(vec3 _force)
{
	Accel += _force / Mass;
}
//Set this particle as an anchor
void CParticle::SetAsAnchor()
{
	isAnchor = true;
}
//Get if this particle is an anchor
bool CParticle::GetAnchor()
{
	return isAnchor;
}
//Add a particle to the linked particles vector
void CParticle::LinkParticles(CParticle* _other)
{
	OtherParts.push_back(_other);
	isLinked = true;
}
//Turn wind on and off
void CParticle::SetWind()
{
	isWind = !isWind;
}
//Get this particles mass
float CParticle::GetMass()
{
	return Mass;
}
//Check for obstacles
void CParticle::CheckObstacle(CPrefab* _obj)
{
	//float dist = distance(_obj->GetObjPosition(), GetObjPosition());
	//if (dist <= _obj->GetObjSize().x)
	//{
		vec3 force = GetObjPosition() - _obj->GetObjPosition();
		float dist = length(force);
		force = normalize(force);
		if (dist < _obj->GetObjSize().x + 3)
		{
			SetObjPosition(GetObjPosition() + force * (_obj->GetObjSize().x + 3 - dist));
			ApplyForce(force * 100.0f * (_obj->GetObjSize().x + 3 - dist));
		}
	//}
}
//Check for obstacles
void CParticle::CheckFloor(CPrefab* _obj)
{
	if (GetObjPosition().y <= _obj->GetObjPosition().y + _obj->GetObjSize().y / 2 
		&& GetObjPosition().x < _obj->GetObjPosition().x + _obj->GetObjSize().x / 2 
		&& GetObjPosition().x > _obj->GetObjPosition().x - _obj->GetObjSize().x / 2
		&& GetObjPosition().z < _obj->GetObjPosition().z + _obj->GetObjSize().z / 2
		&& GetObjPosition().z > _obj->GetObjPosition().z - _obj->GetObjSize().z / 2)
	{
		
		SetObjPosition(vec3(GetObjPosition().x, _obj->GetObjPosition().y + _obj->GetObjSize().y/2, GetObjPosition().z));
	}
}

void CParticle::UnLinkParticles()
{
	OtherParts.clear();
}

void CParticle::Draw()
{
	/*float SCALEx = 275.0 - GetObjPosition().z;
	float SCALEy = 170.0 - GetObjPosition().z;
	
	float booster = 400.0f;
	float upBoost = 0.6f + GetObjPosition().z/ booster;*/
	
	/*float SCALEx = 1;
	float SCALEy = 1;
	
	float booster = 0;
	float upBoost = 0;*/
	vec4 point1 = -m_pCamera->GetVPMatrix() * vec4(GetObjPosition(), 1.0f);

	for (size_t i = 0; i < OtherParts.size(); i++)
	{
		if (!isAnchor && !OtherParts[i]->GetAnchor())
		{
			/*float SCALE2x = 275.0 - OtherParts[i]->GetObjPosition().z;
			float SCALE2y = 170.0 - OtherParts[i]->GetObjPosition().z;
			float upBoost2 = 0.6f + OtherParts[i]->GetObjPosition().z / booster;*/
			/*glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(GetObjPosition().x / (SCALEx), (GetObjPosition().y / (SCALEy)) + upBoost);
			glVertex2f(OtherParts[i]->GetObjPosition().x / (SCALE2x), (OtherParts[i]->GetObjPosition().y / (SCALE2y)) + upBoost2);
			glEnd();*/

			vec4 point2 = -m_pCamera->GetVPMatrix() * vec4(OtherParts[i]->GetObjPosition(), 1.0f);
			
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f(point1.x / point1.w, point1.y / point1.w, point1.z / point1.w);
			glVertex3f(point2.x / point2.w, point2.y / point2.w, point2.z / point2.w);
			glEnd();
		}

	}


	
}
//unused geometry drawing alternative
void CParticle::DrawGeo(vec3 _botLeft, vec3 _Right, vec3 _Left)
{
	glUseProgram(clothProgram);
	bool bRight = false;
	bool bLeft = false;
	bool bBotLeft = false;

	//Alternate method
	// Left  ----  right
	//	|			  |
	//	|			  |
	//botLeft ----  this
	//topLeft
	glUniform3fv(glGetUniformLocation(clothProgram, "Left"), 1, glm::value_ptr(_Left));
	//Right
	glUniform3fv(glGetUniformLocation(clothProgram, "Right"), 1, glm::value_ptr(_Right));
	//botRight
	glUniform3fv(glGetUniformLocation(clothProgram, "botRight"), 1, glm::value_ptr(GetObjPosition()));
	//botLeft
	glUniform3fv(glGetUniformLocation(clothProgram, "botLeft"), 1, glm::value_ptr(_botLeft));

	geo->RenderShapes(clothProgram);
}
//geometry drawing
void CParticle::DrawGeo2(vec3 _Right, vec3 _botLeft, vec3 _botRight)
{
	glUseProgram(clothProgram);
	bool bRight = false;
	bool bBotRight = false;
	bool bBotLeft = false;
	//check if the links coming in are in the list of linked particles
	for (int i = 0; i < OtherParts.size(); i++) {

		if (OtherParts[i]->GetObjPosition() == _Right)
		{
			bRight = true;
		}

		if (OtherParts[i]->GetObjPosition() == _botLeft)
		{
			bBotLeft = true;
		}

		/*if (OtherParts[i]->GetObjPosition() == _botRight)
		{
			bBotRight = true;
		}*/

	}

	//Justin's method
	// this   ----  right
	//	|			  |
	//	|			  |
	//botLeft ---- botRight
	//topLeft
	glUniform3fv(glGetUniformLocation(clothProgram, "Left"), 1, glm::value_ptr(GetObjPosition()));
	
	if (bRight)
	{
		//Right
		glUniform3fv(glGetUniformLocation(clothProgram, "Right"), 1, glm::value_ptr(_Right));
	}
	else
	{
		glUniform3fv(glGetUniformLocation(clothProgram, "Right"), 1, glm::value_ptr(GetObjPosition()));
	}

	//botRight
	glUniform3fv(glGetUniformLocation(clothProgram, "botRight"), 1, glm::value_ptr(_botRight));

	if (bBotLeft)
	{
		//botLeft
		glUniform3fv(glGetUniformLocation(clothProgram, "botLeft"), 1, glm::value_ptr(_botLeft));
	}
	else
	{
		glUniform3fv(glGetUniformLocation(clothProgram, "botLeft"), 1, glm::value_ptr(GetObjPosition()));
	}


	geo->RenderShapes(clothProgram);
}