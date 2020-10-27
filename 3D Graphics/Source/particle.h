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
    CParticle():CPrefab() { geo = new CPrefab(); };
    CParticle(float _mass) :CPrefab(), Mass(_mass) { geo = new CPrefab(); };

    void InitialiseGeo(CCamera* camera, CTime* timer, CInput* input, MeshType type, string path, float frameCount, vec3 _scale, vec3 _rotate, vec3 _translate);

    void Update();
    void ApplyForce(vec3 _force);
    void SetAsAnchor();
    bool GetAnchor();
    void LinkParticles(CParticle* _other);
	void CheckObstacle(CPrefab* _obj);
	void CheckCapsule(CPrefab* _obj);
    void CheckFloor(CPrefab* _obj);
    void UnLinkParticles();
    void Draw();
    void DrawGeo(vec3 _right, vec3 _botLeft, vec3 _botRight);
    void DrawGeo2(vec3 _botLeft, vec3 _Right, vec3 _Left);
	void CollidePyramid(CPrefab *pyramid);
    void SetWind();
    float GetMass();

protected:
    float Mass = 1.0f;
    vec3 Accel = vec3();
    float Damping = 0.5f;
    vec3 Velocity = vec3();
    vec3 Gravity = vec3(0.0f, Mass* -9.8f, 0.0f);
    vec3 Wind = vec3();
    float Stiffness = 0.8f;
    float RestDist = 10.0f;
    vector<CParticle*> OtherParts;
    bool isLinked = false;
    bool isWind = false;
    bool isAnchor = false;
    float maxDistance = Mass * 80.0f;
    CPrefab* geo = 0;
    GLuint clothProgram = 0;
};

