#pragma once
#include "prefab.h"

class CParticle :
    public CPrefab
{

public:
    CParticle():CPrefab() {  };
    CParticle(float _mass):CPrefab(), Mass(_mass){  };

    void Update();
    void ApplyForce(vec3 _force);
    void SetAsAnchor();
    bool GetAnchor();
    void LinkParticles(CParticle* _other);

protected:
    float Mass = 5.0f;
    vec3 Accel = vec3();
    float Damping = 5.0f;
    vec3 Velocity = vec3();
    vec3 Gravity = vec3(0.0f, Mass*-9.8f, 0.0f);
    vec3 Wind = vec3();
    bool isAnchor = false;
    float Stiffness = 1.5f;
    float RestDist = 5.0f;
    vector<CParticle*> OtherParts;
    bool isLinked = false;
};

