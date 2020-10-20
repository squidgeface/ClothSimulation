// (c) 2020 Media Design School
//
// File Name   : particle.h
// Description : particle declaration file
// Author      : alexander jenkins, Raven Clancey-Peetz
// Mail        : alexander.jenkins@mds.ac.nz, Raven-Clancey-Peetz@mediadesign.school.nz
//

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
	void CheckObstacle(CPrefab* _obj);
    void CheckFloor(CPrefab* _obj);
    void UnLinkParticles();
    void Draw();
    void SetWind();
    float GetMass();

protected:
    float Mass = 1.0f;
    vec3 Accel = vec3();
    float Damping = 0.5f;
    vec3 Velocity = vec3();
    vec3 Gravity = vec3(0.0f, Mass* -9.8f, 0.0f);
    vec3 Wind = vec3();
    float Stiffness = 0.3f;
    float RestDist = 10.0f;
    vector<CParticle*> OtherParts;
    bool isLinked = false;
    bool isWind = false;
    bool isAnchor = false;
    float maxDistance = Mass * 50.0f;
    
};

