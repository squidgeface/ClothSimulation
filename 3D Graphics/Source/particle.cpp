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
	else
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
	float dist = distance(_obj->GetObjPosition(), GetObjPosition());
	if (distance(_obj->GetObjPosition(), GetObjPosition()) <= _obj->GetObjSize().x + 3)
	{
		vec3 force = GetObjPosition() - _obj->GetObjPosition();
		force = normalize(force) ;
		SetObjPosition(GetObjPosition() + force);
		ApplyForce(force * 100.0f * (_obj->GetObjSize().x + 4 - dist));
	}
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
	float SCALEx = 275.0 - GetObjPosition().z;
	float SCALEy = 170.0 - GetObjPosition().z;
	
	float booster = 400.0f;
	float upBoost = 0.6f + GetObjPosition().z/ booster;
	

	for (size_t i = 0; i < OtherParts.size(); i++)
	{
		if (!isAnchor && !OtherParts[i]->GetAnchor())
		{
			float SCALE2x = 275.0 - OtherParts[i]->GetObjPosition().z;
			float SCALE2y = 170.0 - OtherParts[i]->GetObjPosition().z;
			float upBoost2 = 0.6f + OtherParts[i]->GetObjPosition().z / booster;
			glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(GetObjPosition().x / (SCALEx), (GetObjPosition().y / (SCALEy)) + upBoost);
			glVertex2f(OtherParts[i]->GetObjPosition().x / (SCALE2x), (OtherParts[i]->GetObjPosition().y / (SCALE2y)) + upBoost2);
			glEnd();
		}

	}

	
}

void CParticle::DrawGeo(vec3 _left, vec3 _botLeft, vec3 _right)
{
	glUseProgram(clothProgram);
	bool bRight = false;
	bool bLeft = false;
	bool bBotLeft = false;

	// left ---- right
	//	|			|
	//	|			|
	//botLeft ---- this
		
	for (int i = 0; i < OtherParts.size(); i++) {

		if (OtherParts[i]->GetObjPosition() == _right)
		{
			bRight = true;
		}

		if (OtherParts[i]->GetObjPosition() == _botLeft)
		{
			bBotLeft = true;
		}

		if (OtherParts[i]->GetObjPosition() == _left)
		{
			bLeft = true;
		}

	}

	//bot right (this)
	glUniform3fv(glGetUniformLocation(clothProgram, "botRight"), 1, glm::value_ptr(GetObjPosition()));

	if (bLeft)
	{
		//left
		glUniform3fv(glGetUniformLocation(clothProgram, "Left"), 1, glm::value_ptr(_left));
	}
	else
	{
		glUniform3fv(glGetUniformLocation(clothProgram, "Left"), 1, glm::value_ptr(_right));
	}

		if (bRight)
		{
			//right
			glUniform3fv(glGetUniformLocation(clothProgram, "Right"), 1, glm::value_ptr(_right));
		}
		else
		{
			//right
			glUniform3fv(glGetUniformLocation(clothProgram, "Right"), 1, glm::value_ptr(GetObjPosition()));
		}
		
		if (bBotLeft)
		{
			//botLeft
			glUniform3fv(glGetUniformLocation(clothProgram, "botLeft"), 1, glm::value_ptr(_botLeft));
		}
		else
		{
			//botLeft
			glUniform3fv(glGetUniformLocation(clothProgram, "botLeft"), 1, glm::value_ptr(GetObjPosition()));
		}

			
	
		



	

	geo->RenderShapes(clothProgram);
}