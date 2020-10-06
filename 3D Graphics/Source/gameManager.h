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
	void InitialiseMenu();

	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	void Render();
	void Update();


	void ProcessInput(InputState* KeyState, InputState* MouseState);


protected:
	//Variables
	//sound FX
		//sound FX
	Sound* m_pTrackBackground;


	//shader programs
	GLuint m_giStaticProgram = 0, m_giTextProgram = 0;
	GLuint m_giPhongProgram = 0;
	GLuint m_giBlinnProgram = 0;
	GLuint m_giRimProgram = 0;
	GLuint m_giCubeMapProgram = 0;
	GLuint m_giReflectCubeMapProgram = 0;



	//Game objects

	vector<CParticle*> m_pSpheres;
	vector<CParticle*> m_pAnchorSpheres;
	CParticle* m_pBall = 0;
	CPrefab* m_pFloor = 0;


	//Game management objects
	CInput* m_pInput = 0;
	CTime* m_pTime = 0;
	CCamera* m_pOrthoCamera = 0;
	CCamera* m_pProjCamera = 0;

	//counters and rates
	float m_fcounter = 0.0f;
	//bools and switches
	bool m_bPaused = false;
	//enum
	GameState m_eGameState;

	//particle values

};