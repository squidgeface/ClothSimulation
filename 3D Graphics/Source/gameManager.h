// (c) 2020 Alexander Jenkins
//
// File Name   : gameManager.h
// Description : gameManager declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once


//local includes
#include "Utils.h"

//class prototypes
class CPrefab;
class CInput;
class CTime;
class CCamera;
class CTextLabel;
class CButton;
class CParticle;
class CSlider;

//enum class
enum class GameState
{
	MENU,
	GAME,
	LOSE,
};

class CGameManager
{
public:

	CGameManager();
	~CGameManager();
	//functions
	void InitialiseWindow(int argc, char **argv);
	void SetUpCloth();
	void InitialiseMenu();

	void Clear();

	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	void RenderLines();
	vec3 GetRayFromMouse();
	bool CheckMouseSphereIntersect(CPrefab* _object);
	void Render();
	void Update();


	void ProcessInput(InputState* KeyState, InputState* MouseState);

	void RipCloth();
	void RipClothClick();
	void SetClothWidth(int _size);

	void SetClothHeight(int _size);

	void SetAnchorSize(int _size);

protected:
	//Variables

	//shader programs
	GLuint m_giTextProgram = 0;
	GLuint m_giStaticProgram = 0;
	GLuint m_giPhongProgram = 0;

	//Game objects
	vector<CParticle*> m_pSpheres;
	vector<CParticle*> m_Cloth;
	vector<CParticle*> m_pAnchorSpheres;
	CPrefab* m_pBall = 0;
	CPrefab* m_pTri = 0;
	CPrefab* m_pFloor = 0;
	CSlider* widthSlider = 0;
	CSlider* heightSlider = 0;
	CSlider* anchorSlider = 0;

	//Game management objects
	CInput* m_pInput = 0;
	CTime* m_pTime = 0;
	CCamera* m_pOrthoCamera = 0;
	CCamera* m_pProjCamera = 0;

	//counters and rates
	float m_fcounter = 0.0f;
	//bools and switches
	bool m_bRipToggle = false;
	bool m_bPaused = false;
	vec3 m_v3RayDirection = vec3();
	float previousX = 0;
	float previousY = 0;
	bool isClicking = false;
	int shape = 0;
	int width = 10;
	int height = 10;
	int anchors = 10;
	//enum
	GameState m_eGameState = GameState::MENU;

};