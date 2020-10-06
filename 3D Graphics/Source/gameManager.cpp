// (c) 2020 Alexander Jenkins
//
// File Name   : gameManager.cpp
// Description : gameManager implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "gameManager.h"

#include "prefab.h"
#include "input.h"
#include "time.h"
#include "camera.h"
#include "TextLabel.h"
#include "button.h"
#include "particle.h"

CGameManager::CGameManager()
{
	m_pOrthoCamera = new CCamera();
	m_pProjCamera = new CCamera();
	m_pTime = new CTime();
	m_pInput = new CInput();
}

CGameManager::~CGameManager()
{
	
}

//Initialise Glut window
void CGameManager::InitialiseWindow(int argc, char **argv)
{
	//Setup and create glut controlled window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((int)Utils::HalfScreenW/2, 0);
	glutInitWindowSize(Utils::ScreenWidth, Utils::ScreenHeight);
	glutCreateWindow("OpenGL First Window");

	//Set up all gl function callbacks based on p hardware
	if (glewInit() != GLEW_OK)
	{
		//if glew setup failed then applicaion will not run graphics correctly
		std::cout << "Glew Initialization Failed. Aborting App." << std::endl;
		system("pause");
	}

	//Sets the clear color when calling glclear()
	glClearColor(1.0, 0.0, 0.0, 1.0);

	//Load shader variables
	m_giStaticProgram = ShaderLoader::CreateProgram("Resources/Shaders/vertexshader.txt", "Resources/Shaders/fragshader.txt");
	m_giPhongProgram = ShaderLoader::CreateProgram("Resources/Shaders/3Dvertexshader.txt", "Resources/Shaders/phongFshader.txt");
	m_giBlinnProgram = ShaderLoader::CreateProgram("Resources/Shaders/3Dvertexshader.txt", "Resources/Shaders/blinnFshader.txt");
	m_giRimProgram = ShaderLoader::CreateProgram("Resources/Shaders/3Dvertexshader.txt", "Resources/Shaders/rimFshader.txt");
	m_giCubeMapProgram = ShaderLoader::CreateProgram("Resources/Shaders/CMvertexshader.txt", "Resources/Shaders/CMFshader.txt");
	m_giReflectCubeMapProgram = ShaderLoader::CreateProgram("Resources/Shaders/CMRvertexshader.txt", "Resources/Shaders/CMRFshader.txt");

	
	//set culling on
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//Initialise time
	m_pTime->InitiateTime();

	//Set Up Camera from class
	m_pOrthoCamera->SetOrtho();
	m_pProjCamera->SetProjection();
}

//Initialise Menu items
void CGameManager::InitialiseMenu()
{
	int gridSize = 49;
	//Load shapes
	for (size_t y = 0; y < sqrt(gridSize); y++)
	{
		for (size_t x = 0; x < sqrt(gridSize); x++)
		{
			CParticle* m_pSphere = new CParticle();
			m_pSphere->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::CUBE, "", 0, vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(-(sqrt(gridSize)/2 * 15) + (x * 15), 0.0f + y * 3, 0.0f));
			m_pSphere->InitialiseTextures("Resources/Textures/top.jpg", 1);
			m_pSpheres.push_back(m_pSphere);
		}
	}

	int counter = 0;
	//Link Particles
	for (size_t y = 0; y < sqrt(m_pSpheres.size()); y++)
	{
		for (size_t x = 0; x < sqrt(m_pSpheres.size()); x++)
		{
			if (x != 0)
			{
				m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - 1]);

				if (y != 0)
				{
					m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - sqrt(m_pSpheres.size())]);
				}
			}
			else
			{
				if (y != 0)
				{
					m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - sqrt(m_pSpheres.size())]);
				}
			}
			counter++;
		}
	}
	
	int anchors = sqrt(gridSize);
	for (size_t i = 0; i < anchors; i++)
	{
		CParticle* m_pSphere = new CParticle();
		m_pSphere->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::PYRAMID, "", 0, vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15) + float(i * 10), 10.0f, 0.0f));
		m_pSphere->InitialiseTextures("Resources/Textures/top.jpg", 1);
		m_pSphere->SetAsAnchor();
		m_pAnchorSpheres.push_back(m_pSphere);
		m_pSpheres[i]->LinkParticles(m_pAnchorSpheres[i]);
	}

	m_pProjCamera->LookAtObject(vec3(m_pAnchorSpheres[4]->GetObjPosition().x,m_pAnchorSpheres[4]->GetObjPosition().y,m_pAnchorSpheres[4]->GetObjPosition().z));

	m_pBall = new CParticle(10.0f);
	m_pBall->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(10.0f, 10.0f, 10.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15)/2, 10.0f, 0.0f));
	m_pBall->InitialiseTextures("Resources/Textures/top.jpg", 1);
	m_pBall->SetAsAnchor();

	m_pFloor = new CPrefab();
	//load floor
	m_pFloor->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::CUBE, "", 0, vec3(200.0f, 1.0f, 200.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15)/2, -50.0f, 0.0f));
	//load textures 
		//game textures

	
	
}



//render function
void CGameManager::Render()
{
	//Clear buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	int counter = 0;
	//switch for game state
	switch (m_eGameState)
	{
	case GameState::MENU:
		for (size_t i = 0; i < m_pSpheres.size(); i++)
		{
			m_pSpheres[i]->RenderShapes(m_giBlinnProgram);
		}
		for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
		{
			m_pAnchorSpheres[i]->RenderShapes(m_giBlinnProgram);
		}
		
		m_pBall->RenderShapes(m_giBlinnProgram);
	/*	for (size_t y = 0; y < sqrt(m_pSpheres.size()); y++)
		{
			for (size_t x = 0; x < sqrt(m_pSpheres.size()); x++)
			{
				if (x != 0)
				{
					
					glVertex3f(m_pSpheres[counter]->GetObjPosition().x, m_pSpheres[counter]->GetObjPosition().y, m_pSpheres[counter]->GetObjPosition().z);
					glVertex3f(m_pSpheres[counter - 1]->GetObjPosition().x, m_pSpheres[counter - 1]->GetObjPosition().y, m_pSpheres[counter - 1]->GetObjPosition().z);
					glEnd();
					
					if (y != 0)
					{
						glBegin(GL_LINES);
						glVertex3f(m_pSpheres[counter]->GetObjPosition().x, m_pSpheres[counter]->GetObjPosition().y, m_pSpheres[counter]->GetObjPosition().z);
						glVertex3f(m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().x, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().y, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().z);
						glEnd();
					}
				}
				else
				{
					if (y != 0)
					{
						glBegin(GL_LINES);
						glVertex3f(m_pSpheres[counter]->GetObjPosition().x, m_pSpheres[counter]->GetObjPosition().y, m_pSpheres[counter]->GetObjPosition().z);
						glVertex3f(m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().x, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().y, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().z);
						glEnd();
					}
				}
				counter++;
			}
		}*/
		
	
		m_pFloor->RenderShapes(m_giBlinnProgram);

		break;

	default:
		break;
	}


	glutSwapBuffers();
	//glFinish();
}
//update function
void CGameManager::Update()
{
//Update Time components
	m_pTime->UpdateTime();
	
//Get user input into proces input function
	ProcessInput(m_pInput->GetKeyState(), m_pInput->GetMouseState());

//switch for game state
	switch (m_eGameState)
	{
	case GameState::MENU:
	//menu component updates
		for (size_t i = 0; i < m_pSpheres.size(); i++)
		{
			m_pSpheres[i]->UpdateShapes();
			m_pSpheres[i]->Update();

			if (m_pBall->Distance(m_pBall->GetObjPosition(), m_pSpheres[i]->GetObjPosition()) < m_pBall->GetObjSize().x / 2)
			{
				m_pSpheres[i]->LinkParticles(m_pBall);
			}
		}
		for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
		{
			m_pAnchorSpheres[i]->UpdateShapes();
			m_pSpheres[i]->Update();
		}

		m_pBall->UpdateShapes();
		m_pBall->Update();

		//m_pProjCamera->CameraRotate(m_pTime);
		m_pFloor->UpdateShapes();
		break;	
	default:
		break;
	}

	

	//update game information
	glutPostRedisplay();
}



//input functions
void CGameManager::ProcessInput(InputState* KeyState, InputState* MouseState)
{
								/// *** MAIN MENU INPUT *** ///
	switch (m_eGameState)
	{
	case GameState::MENU:

		//if mouse click counter is 0 (one click at a time)
		if (m_fcounter == 0.0f)
		{
			if (KeyState['w'] == InputState::INPUT_DOWN)
			{
				m_pAnchorSpheres[0]->SetObjPosition(vec3(m_pAnchorSpheres[0]->GetObjPosition().x - 1.0f, m_pAnchorSpheres[0]->GetObjPosition().y, m_pAnchorSpheres[0]->GetObjPosition().z));
				m_pAnchorSpheres[1]->SetObjPosition(vec3(m_pAnchorSpheres[1]->GetObjPosition().x - 0.75f, m_pAnchorSpheres[1]->GetObjPosition().y, m_pAnchorSpheres[1]->GetObjPosition().z));
				m_pAnchorSpheres[2]->SetObjPosition(vec3(m_pAnchorSpheres[2]->GetObjPosition().x - 0.5f, m_pAnchorSpheres[2]->GetObjPosition().y, m_pAnchorSpheres[2]->GetObjPosition().z));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().x + 1.0f, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().z));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().x + 0.75f, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().z));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().x + 0.5f, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().z));
			}
			else if (KeyState['s'] == InputState::INPUT_DOWN)
			{
				m_pAnchorSpheres[0]->SetObjPosition(vec3(m_pAnchorSpheres[0]->GetObjPosition().x + 1.0f, m_pAnchorSpheres[0]->GetObjPosition().y, m_pAnchorSpheres[0]->GetObjPosition().z));
				m_pAnchorSpheres[1]->SetObjPosition(vec3(m_pAnchorSpheres[1]->GetObjPosition().x + 0.75f, m_pAnchorSpheres[1]->GetObjPosition().y, m_pAnchorSpheres[1]->GetObjPosition().z));
				m_pAnchorSpheres[2]->SetObjPosition(vec3(m_pAnchorSpheres[2]->GetObjPosition().x + 0.5f, m_pAnchorSpheres[2]->GetObjPosition().y, m_pAnchorSpheres[2]->GetObjPosition().z));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().x - 1.0f, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().z));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().x - 0.75f, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().z));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().x - 0.5f, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().z));

			}

			if (KeyState['a'] == InputState::INPUT_DOWN)
			{
				m_pAnchorSpheres[0]->SetObjPosition(vec3(m_pAnchorSpheres[0]->GetObjPosition().x, m_pAnchorSpheres[0]->GetObjPosition().y, m_pAnchorSpheres[0]->GetObjPosition().z - 1.0f));
				m_pAnchorSpheres[1]->SetObjPosition(vec3(m_pAnchorSpheres[1]->GetObjPosition().x, m_pAnchorSpheres[1]->GetObjPosition().y, m_pAnchorSpheres[1]->GetObjPosition().z - 1.0f));
				m_pAnchorSpheres[2]->SetObjPosition(vec3(m_pAnchorSpheres[2]->GetObjPosition().x, m_pAnchorSpheres[2]->GetObjPosition().y, m_pAnchorSpheres[2]->GetObjPosition().z - 1.0f));
				m_pAnchorSpheres[3]->SetObjPosition(vec3(m_pAnchorSpheres[3]->GetObjPosition().x, m_pAnchorSpheres[3]->GetObjPosition().y, m_pAnchorSpheres[3]->GetObjPosition().z - 1.0f));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().z - 1.0f));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().z - 1.0f));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().z - 1.0f));
			}
			else if (KeyState['d'] == InputState::INPUT_DOWN)
			{
				m_pAnchorSpheres[0]->SetObjPosition(vec3(m_pAnchorSpheres[0]->GetObjPosition().x, m_pAnchorSpheres[0]->GetObjPosition().y, m_pAnchorSpheres[0]->GetObjPosition().z + 1.0f));
				m_pAnchorSpheres[1]->SetObjPosition(vec3(m_pAnchorSpheres[1]->GetObjPosition().x, m_pAnchorSpheres[1]->GetObjPosition().y, m_pAnchorSpheres[1]->GetObjPosition().z + 1.0f));
				m_pAnchorSpheres[2]->SetObjPosition(vec3(m_pAnchorSpheres[2]->GetObjPosition().x, m_pAnchorSpheres[2]->GetObjPosition().y, m_pAnchorSpheres[2]->GetObjPosition().z + 1.0f));
				m_pAnchorSpheres[3]->SetObjPosition(vec3(m_pAnchorSpheres[3]->GetObjPosition().x, m_pAnchorSpheres[3]->GetObjPosition().y, m_pAnchorSpheres[3]->GetObjPosition().z + 1.0f));

				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().z + 1.0f));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().z + 1.0f));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().z + 1.0f));
			}
			/*else if (KeyState['a'] == InputState::INPUT_UP && KeyState['d'] == InputState::INPUT_UP)
			{
				m_pAnchorSpheres[0]->SetObjPosition(vec3(m_pAnchorSpheres[0]->GetObjPosition().x, m_pAnchorSpheres[0]->GetObjPosition().y, m_pAnchorSpheres[0]->GetObjPosition().z));
				m_pAnchorSpheres[1]->SetObjPosition(vec3(m_pAnchorSpheres[1]->GetObjPosition().x, m_pAnchorSpheres[1]->GetObjPosition().y, m_pAnchorSpheres[1]->GetObjPosition().z));
				m_pAnchorSpheres[2]->SetObjPosition(vec3(m_pAnchorSpheres[2]->GetObjPosition().x, m_pAnchorSpheres[2]->GetObjPosition().y, m_pAnchorSpheres[2]->GetObjPosition().z));

				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 1]->GetObjPosition().z));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 2]->GetObjPosition().z));
				m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->SetObjPosition(vec3(m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().x, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().y, m_pAnchorSpheres[sqrt(m_pSpheres.size()) - 3]->GetObjPosition().z ));
			}*/
			
		}
		else if (KeyState['w'] == InputState::INPUT_UP && KeyState['s'] == InputState::INPUT_UP)
		{
		//reset counter on mouse up
			m_fcounter = 0.0f;
		}
		break;

													// **** GAME INPUT **** //

	case GameState::GAME:

		//Menu


		//Bring up menu on escape key
		if (m_fcounter == 0.0f)
		{	//escape key pressed
			if (KeyState[27] == InputState::INPUT_DOWN)
			{
				
			}
		}
		else if (m_bPaused == true)
		{
			m_fcounter = 0.0f;
		}

		if (!m_bPaused)
		{
			if (MouseState[0] == InputState::INPUT_UP)
			{


				m_fcounter += (m_pTime->GetDelta() * 100);
				//simulate delay on fire


			}
			else if (MouseState[0] == InputState::INPUT_UP)
			{
				m_fcounter = 0.0f;
			}
		}
		else 
		{
		
			if (m_fcounter == 0.0f)
			{
				
				
				
				
			}
		}

		break;

	
	default:
		break;
	}

}



//keyboard input functions
void CGameManager::KeyboardDown(unsigned char key, int x, int y)
{
	m_pInput->KeyboardDown(key, x, y);
}
void CGameManager::KeyboardUp(unsigned char key, int x, int y)
{
	m_pInput->KeyboardUp(key, x, y);
}

//mouse input functions
void CGameManager::MouseClick(int button, int state, int x, int y)
{
	m_pInput->MouseClick(button, state, x, y);
}
void CGameManager::MouseMove(int x, int y)
{
	m_pInput->MouseMove(x, y);
}
