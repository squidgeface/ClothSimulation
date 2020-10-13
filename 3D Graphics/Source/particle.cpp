// (c) 2020 Media Design School
//
// File Name   : particle.cpp
// Description : particle implementation file
// Author      : alexander jenkins, Raven Clancey-Peetz
// Mail        : alexander.jenkins@mds.ac.nz, Raven-Clancey-Peetz@mediadesign.school.nz
//

#include "particle.h"
#include "time.h"
//Particle update
void CParticle::Update()
{
	//Check if particle is one of the static points
	if (!isAnchor)
	{

		//apply forces
		ApplyForce(Gravity);
		//nice gentle breeze
		float seedx = float(rand() % 100) - 40.0f;
		float seedz = float(rand() % 100) - 40.0f;
		

		//Seed with with values
		Wind = vec3(-seedx, 0.0f, -seedz);
		//Togle wind on and off qith 'Q'
		if (isWind)
		{
			ApplyForce(Wind * Mass);
		}

		
		//Use those forces to move object 
		Accel -= Velocity * Damping / Mass;
		Velocity += Accel * m_pTime->GetDelta();
		//limit velocity to graviy force
		if (Velocity.y <= Gravity.y)
		{
			Velocity.y = Gravity.y;
		}
		//add velocity to current position
		SetObjPosition(GetObjPosition() + Velocity * m_pTime->GetDelta());
		
		
		//other particle links
		//for all the particles that have been linked
		for (size_t i = 0; i < OtherParts.size(); i++)
		{	
			//if the particle in the vector is not this one
			if (OtherParts[i] != this)
			{
			//math for calculating forces
				vec3 Delta = OtherParts[i]->GetObjPosition() - GetObjPosition();
				float deltaLength = Distance(OtherParts[i]->GetObjPosition(), GetObjPosition());
				float difference = (deltaLength - RestDist) / deltaLength;
				float Im1 = 1 / Mass;
				float Im2 = 1 / OtherParts[i]->GetMass();

				vec3 force = Delta * (Im1 / (Im1 + Im2)) * Stiffness * difference;
				vec3 force2 = Delta * (Im2 / (Im1 + Im2)) * Stiffness * difference;
				//set forces for this object
				SetObjPosition(GetObjPosition() + force);
				//if the other object is not an anchor object
				if (!OtherParts[i]->GetAnchor())
				{
				//Apply forces to other object
					OtherParts[i]->SetObjPosition(OtherParts[i]->GetObjPosition() - force2);
				}
			}
		}
	//reset acceleration
		Accel = vec3();
	}
	else
	{
		//set static position
		SetObjPosition(vec3(GetObjPosition().x,0.0f,GetObjPosition().z));
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
	if (distance(_obj->GetObjPosition(), GetObjPosition()) <= _obj->GetObjSize().x/2)
	{
		vec3 force = GetObjPosition() - _obj->GetObjPosition();
		force = normalize(force) * -(_obj->GetObjSize().x / 2.0f);
		SetObjPosition(GetObjPosition() + force);
	}
}