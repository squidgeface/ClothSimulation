#include "particle.h"
#include "time.h"

void CParticle::Update()
{
	if (!isAnchor)
	{
		ApplyForce(Gravity);
		Accel -= Velocity * Damping / Mass;
		Velocity += Accel * m_pTime->GetDelta();
		if (Velocity.y <= Gravity.y)
		{
			Velocity.y = Gravity.y;
		}

		SetObjPosition(GetObjPosition() + Velocity * m_pTime->GetDelta());
		
		
		//other particle links
		for (size_t i = 0; i < OtherParts.size(); i++)
		{
			if (OtherParts[i] != this)
			{
				vec3 Delta = OtherParts[i]->GetObjPosition() - GetObjPosition();
				float deltaLength = Distance(OtherParts[i]->GetObjPosition(), GetObjPosition());
				float difference = (deltaLength - RestDist) / deltaLength;
				float Im1 = 1 / Mass;
				float Im2 = 1 / OtherParts[i]->GetMass();
				vec3 force = Delta * (Im1 / (Im1 + Im2)) * Stiffness * difference;
				vec3 force2 = Delta * (Im2 / (Im1 + Im2)) * Stiffness * difference;
				SetObjPosition(GetObjPosition() + force);

				if (!OtherParts[i]->GetAnchor())
				{
					OtherParts[i]->SetObjPosition(OtherParts[i]->GetObjPosition() - force2);
				}
			}
		}
		float seedx = float(rand() % 100) - 20.0f;
		float seedz = float(rand() % 100) - 20.0f;
		Wind = vec3(seedx, 0.0f, seedz);
		if (isWind)
		{
			ApplyForce(Wind * Mass);
		}
		Accel = vec3();
	}
	else
	{
		SetObjPosition(vec3(GetObjPosition().x,0.0f,GetObjPosition().z));
	}
}

void CParticle::ApplyForce(vec3 _force)
{
	Accel += _force / Mass;
}

void CParticle::SetAsAnchor()
{
	isAnchor = true;
}

bool CParticle::GetAnchor()
{
	return isAnchor;
}

void CParticle::LinkParticles(CParticle* _other)
{
	OtherParts.push_back(_other);
	isLinked = true;
}

void CParticle::SetWind()
{
	isWind = !isWind;
}

float CParticle::GetMass()
{
	return Mass;
}