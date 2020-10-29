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
		}/marker 


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

				//rip cloth if distance too far
				if (deltaLength > maxDistance)
				{
					OtherParts.erase(OtherParts.begin() + i);
				}
			}
		}
	//reset acceleration
		Accel = vec3();
	


	if (isAnchor)
	{
		//set static position
		SetObjPosition(vec3(GetObjPosition().x,0.0f,GetObjPosition().z));
	}
	else if (geo->GetObjPosition() != vec3())
	{
		//calculate geometry shader
		geo->UpdateShapes();
		geo->SetObjPosition(GetObjPosition());
	}

	//check if particle is on fire
	if (m_bOnFire) {
		//add random burn amount
		m_fBurnTime += ((rand() % 10)/ 10.000f) * m_pTime->GetDelta();

		ApplyForce(vec3(0.0f, 0.8f * m_fBurnTime, 0.0f));
	}

	//spread fire
	if (m_fBurnTime > 3) {
		for (size_t i = 0; i < OtherParts.size(); i++)
		{
			OtherParts[i]->Burn();
		}
	}
	//burned out
	 if (m_fBurnTime > 8)
	{
		UnLinkParticles();
		bisBurned = true;
		m_bOnFire = false;
	}

	 glUseProgram(clothProgram);
	 GLint colorLoc = glGetUniformLocation(clothProgram, "colour");
	 glUniform1f(colorLoc, (m_fBurnTime / 8.00f));

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
	//sphere collision
		vec3 force = GetObjPosition() - _obj->GetObjPosition();
		float dist = length(force);
		force = normalize(force);
		if (dist < _obj->GetObjSize().x + 2)
		{
			SetObjPosition(GetObjPosition() + force * (_obj->GetObjSize().x + 2 - dist));
			ApplyForce(force * 100.0f * (_obj->GetObjSize().x + 2 - dist));
		}

}

void CParticle::CheckCapsule(CPrefab* _obj)
{

	//capsule collision

	float offset = 1.0f;
	for (int i = 0; i < 30; i++)
	{
		vec3 force = GetObjPosition() - _obj->GetObjPosition() + vec3(0, i * offset, 0);
		float dist = length(force);
		force = normalize(force);
		if (dist < _obj->GetObjSize().x + 5)
		{
			SetObjPosition(GetObjPosition() + force * (_obj->GetObjSize().x + 5 - dist));
			ApplyForce(force * 100.0f * (_obj->GetObjSize().x + 5 - dist));
		}
	}

}
//Check for obstacles
void CParticle::CheckFloor(CPrefab* _obj)
{
	//floor collision
	if (GetObjPosition().y <= _obj->GetObjPosition().y + _obj->GetObjSize().y / 2
		&& GetObjPosition().x < _obj->GetObjPosition().x + _obj->GetObjSize().x / 2
		&& GetObjPosition().x > _obj->GetObjPosition().x - _obj->GetObjSize().x / 2
		&& GetObjPosition().z < _obj->GetObjPosition().z + _obj->GetObjSize().z / 2
		&& GetObjPosition().z > _obj->GetObjPosition().z - _obj->GetObjSize().z / 2)
	{

		SetObjPosition(vec3(GetObjPosition().x, _obj->GetObjPosition().y + _obj->GetObjSize().y/2, GetObjPosition().z));
	}
}

//break all connections to a particle
void CParticle::UnLinkParticles()
{
	OtherParts.clear();
}

//draw the connections between particles
void CParticle::Draw()
{

	vec4 point1 = -m_pCamera->GetVPMatrix() * vec4(GetObjPosition(), 1.0f);

	for (size_t i = 0; i < OtherParts.size(); i++)
	{
		if (!isAnchor && !OtherParts[i]->GetAnchor())
		{

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
	bool bTopLeft = false;
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

		if (OtherParts[i]->GetObjPosition() == _Left)
		{
			bTopLeft = true;
		}

	}
	//Alternate method
	// Left  ----  right
	//	|			  |
	//	|			  |
	//botLeft ----  this

		//botRight
	glUniform3fv(glGetUniformLocation(clothProgram, "botRight"), 1, glm::value_ptr(_Left));

	//topLeft
	if (bTopLeft)
	{
		glUniform3fv(glGetUniformLocation(clothProgram, "Left"), 1, glm::value_ptr(GetObjPosition()));
	}
	else {
		glUniform3fv(glGetUniformLocation(clothProgram, "Left"), 1, glm::value_ptr(_Left));
	}

	//Right
	if (bRight)
	{
		glUniform3fv(glGetUniformLocation(clothProgram, "Right"), 1, glm::value_ptr(_botLeft));
	}
	else {
		glUniform3fv(glGetUniformLocation(clothProgram, "Right"), 1, glm::value_ptr(_Left));
	}

	//botLeft
	if (bBotLeft)
	{
		glUniform3fv(glGetUniformLocation(clothProgram, "botLeft"), 1, glm::value_ptr(_Right));
	}
	else
	{
		glUniform3fv(glGetUniformLocation(clothProgram, "botLeft"), 1, glm::value_ptr(_Left));
	}

	geo->RenderShapes(clothProgram);
}

//unused pyrmid collision attempts
void CParticle::CollidePyramid(CPrefab *pyramid)
{

	float size = pyramid->GetObjSize().x / 2;
	vec3 PyramidCenter = pyramid->GetObjPosition() + vec3(0, 0.25f * size * 2, 0);
	//T1
	vec3 Top = pyramid->GetObjPosition() + vec3(0, size, 0);
	vec3 t1p1 = pyramid->GetObjPosition() + vec3(-size, 0, -size);
	vec3 t1p2 = pyramid->GetObjPosition() + vec3(size, 0, -size);
	vec3 t1Center((Top + t1p1 + t1p2) / 3.0f);

	//T2
	vec3 t2p1 = pyramid->GetObjPosition() + vec3(size, 0, -size);
	vec3 t2p2 = pyramid->GetObjPosition() + vec3(size, 0, size);
	vec3 t2Center((Top + t2p1 + t2p2) / 3.0f);

	//T3
	vec3 t3p1 = pyramid->GetObjPosition() + vec3(size, 0, size);
	vec3 t3p2 = pyramid->GetObjPosition() + vec3(-size, 0, size);
	vec3 t3Center((Top + t3p1 + t3p2) / 3.0f);
	//T4
	vec3 t4p1 = pyramid->GetObjPosition() + vec3(-size, 0, size);
	vec3 t4p2 = pyramid->GetObjPosition() + vec3(-size, 0, -size);
	vec3 t4Center((Top + t4p1 + t4p2) / 3.0f);

	//t1 forces
	vec3 forcet1(t1Center - PyramidCenter);
	//forcet1 = normalize(forcet1);

	float minDistancet1 = distance(Top, t1Center) + distance(t1p1, t1Center) + distance(t1p2, t1Center);
	float currentDistancet1 = distance(Top, GetObjPosition()) + distance(t1p1, GetObjPosition()) + distance(t1p2, GetObjPosition());

	if (currentDistancet1 - minDistancet1 < 1)
	{
		//ApplyForce(forcet1  * ((size * 2) - distance(GetObjPosition(), PyramidCenter)));
		ApplyForce(forcet1 * ( (size * 2) - distance(t1p1, GetObjPosition())));
		ApplyForce(forcet1 * ((size * 2) - distance(t1p2, GetObjPosition())));
		ApplyForce(forcet1 * ((size * 2) - distance(Top, GetObjPosition())));

		SetObjPosition(GetObjPosition() + forcet1);
	}


	//t2 forces
	vec3 forcet2(t2Center - PyramidCenter);


	float minDistancet2 = distance(Top, t2Center) + distance(t2p1, t2Center) + distance(t2p2, t2Center);
	float currentDistancet2 = distance(Top, GetObjPosition()) + distance(t2p1, GetObjPosition()) + distance(t2p2, GetObjPosition());

	if (currentDistancet2 - minDistancet2 < 1)
	{
		
		ApplyForce(forcet2 * ((size * 2) - distance(t2p1, GetObjPosition())));
		ApplyForce(forcet2 * ((size * 2) - distance(t2p2, GetObjPosition())));
		ApplyForce(forcet2 * ((size * 2) - distance(Top, GetObjPosition())));


		SetObjPosition(GetObjPosition() + forcet2);
	}

	//t3 forces
	vec3 forcet3(t3Center - PyramidCenter);


	float minDistancet3 = distance(Top, t3Center) + distance(t3p1, t3Center) + distance(t3p2, t3Center);
	float currentDistancet3 = distance(Top, GetObjPosition()) + distance(t3p1, GetObjPosition()) + distance(t3p2, GetObjPosition());

	if (currentDistancet3 - minDistancet3 < 1)
	{
		//ApplyForce(forcet3  * ((size * 2) - distance(GetObjPosition(), PyramidCenter)));
		ApplyForce(forcet3 * ((size * 2) - distance(t3p1, GetObjPosition())));
		ApplyForce(forcet3 * ((size * 2) - distance(t3p2, GetObjPosition())));
		ApplyForce(forcet3 * ((size * 2) - distance(Top, GetObjPosition())));


		SetObjPosition(GetObjPosition() + forcet3);
	}


	//t4 forces
	vec3 forcet4(t4Center - PyramidCenter);
	//forcet4 = normalize(forcet4);

	float minDistancet4 = distance(Top, t4Center) + distance(t4p1, t4Center) + distance(t4p2, t4Center);
	float currentDistancet4 = distance(Top, GetObjPosition()) + distance(t4p1, GetObjPosition()) + distance(t4p2, GetObjPosition());

	if (currentDistancet4 - minDistancet4 < 1)
	{
		//ApplyForce(forcet4  * ((size * 2) - distance(GetObjPosition(), PyramidCenter)) );
		ApplyForce(forcet4 * ((size * 2) - distance(t4p1, GetObjPosition())));
		ApplyForce(forcet4 * ((size * 2) - distance(t4p2, GetObjPosition())));
		ApplyForce(forcet4 * ((size * 2) - distance(Top, GetObjPosition())));


		SetObjPosition(GetObjPosition() + forcet4);
	}

}

//function for starting fire on cloth
void CParticle::Burn()
{
	m_bOnFire = true;
}
