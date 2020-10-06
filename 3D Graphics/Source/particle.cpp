#include "particle.h"
#include "time.h"

void CParticle::Update()
{
	if (!isAnchor)
	{
		ApplyForce(Gravity);
		Accel -= Velocity * (Damping / Mass);
		Velocity += Accel * m_pTime->GetDelta();
		SetObjPosition(GetObjPosition() + Velocity * m_pTime->GetDelta());
		Accel = vec3();
		//other particle links
		for (size_t i = 0; i < OtherParts.size(); i++)
		{
			vec3 Delta = OtherParts[i]->GetObjPosition() - GetObjPosition();
			float deltaLength = Distance(GetObjPosition(), OtherParts[i]->GetObjPosition());
			float difference = (deltaLength - RestDist) / deltaLength / m_pTime->GetDelta();
			float Im1 = 1 / Mass;
			float Im2 = 1 / Mass;
			SetObjPosition(GetObjPosition() + Delta * (Im1 / (Im1 + Im2)) * Stiffness * difference * m_pTime->GetDelta());
			if (!OtherParts[i]->GetAnchor())
			{
				OtherParts[i]->SetObjPosition(OtherParts[i]->GetObjPosition() - Delta * (Im1 / (Im1 + Im2)) * Stiffness * difference * m_pTime->GetDelta());
			}
		}
		float seedx = (rand() % 1000) - 500;
		float seedz = (rand() % 1000) - 500;
		Wind = vec3(seedx, 0.0f, seedz);
		ApplyForce(Wind);
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