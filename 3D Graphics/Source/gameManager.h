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
	void SpecialDown(int key, int x, int y);
	void SpecialUp(int key, int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	void RenderLines();
	vec3 GetRayFromMouse();
	bool CheckMouseSphereIntersect(CPrefab* _object);
	void Render();
	void Update();

	void MoveCamera();


	void ProcessInput(InputState* KeyState, InputState* MouseState);

	void RipCloth();
	void RipClothClick();
	void BurnClothClick();
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
	CPrefab* m_pCapsule = 0;
	CPrefab* m_pFloor = 0;
	CSlider* m_pWidthSlider = 0;
	CSlider* m_pHeightSlider = 0;
	CSlider* m_pAnchorSlider = 0;
	CButton* m_pUpCam = 0;
	CButton* m_pDownCam = 0;
	CButton* m_pLeftCam = 0;
	CButton* m_pRightCam = 0;
	CButton* m_pInCam = 0;
	CButton* m_pOutCam = 0;
	CButton* m_pRLeftCam = 0;
	CButton* m_pRRightCam = 0;

	CButton* m_pSetCutting = 0;
	CButton* m_pSetRipping = 0;
	CButton* m_pSetBurning = 0;

	CTextLabel* m_pInstructions = 0;
	

	//Game management objects
	CInput* m_pInput = 0;
	CTime* m_pTime = 0;
	CCamera* m_pOrthoCamera = 0;
	CCamera* m_pProjCamera = 0;

	//counters and rates
	float m_fcounter = 0.0f;
	//bools and switches
	bool m_bRipToggle = true, m_bCutToggle = false, m_bBurnToggle = false;
	bool m_bPaused = false;
	vec3 m_v3RayDirection = vec3();
	float previousX = 0;
	float previousY = 0;
	float m_fHorz = 0;
	float m_fVert = 0;
	float m_fLat = 0;
	float m_fSpin = 0;
	bool isClicking = false;
	int shape = 0;
	int width = 20;
	int height = 20;
	int anchors = 20;

	//raven camera stuff
	float zoom = 100;
	float CamRotx = 0;

	//enum
	GameState m_eGameState = GameState::MENU;

};