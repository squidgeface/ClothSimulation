// (c) 2020 Media Design School
//
// File Name   : gameManager.cpp
// Description : gameManager implementation file
// Author      : alexander jenkins, Raven Clancey-Peetz
// Mail        : alexander.jenkins@mds.ac.nz, Raven-Clancey-Peetz@mediadesign.school.nz
//


#include "gameManager.h"
#include "prefab.h"
#include "input.h"
#include "time.h"
#include "camera.h"
#include "TextLabel.h"
#include "button.h"
#include "particle.h"

//constructor
CGameManager::CGameManager()
{
	m_pOrthoCamera = new CCamera();
	m_pProjCamera = new CCamera();
	m_pTime = new CTime();
	m_pInput = new CInput();
}
//destructor
CGameManager::~CGameManager()
{
	delete m_pOrthoCamera;
	delete m_pProjCamera;
	delete m_pTime;
	delete m_pInput;
	m_pOrthoCamera = 0;
	m_pProjCamera = 0;
	m_pTime = 0;
	m_pInput = 0;
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
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//Load shader variables
	m_giPhongProgram = ShaderLoader::CreateProgram("Resources/Shaders/3Dvertexshader.txt", "Resources/Shaders/phongFshader.txt");

	//set culling on
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//Initialise time
	m_pTime->InitiateTime();

	//Seed random
	srand(time(NULL));

	//Set Up Camera from class
	m_pOrthoCamera->SetOrtho();
	m_pProjCamera->SetProjection();
}

//Initialise Menu items
void CGameManager::InitialiseMenu()
{
	//create a 12x12 particle grid
	int gridSize = 100;
	//Load shapes
	for (size_t y = 0; y < sqrt(gridSize); y++)
	{
		for (size_t x = 0; x < sqrt(gridSize); x++)
		{
			//offset each x and y to form a grid shape
			float _x = (sqrt(gridSize)) + (x * 5);
			float _y = 0.0f - y;
			CParticle* m_pSphere = new CParticle();
			m_pSphere->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::CUBE, "", 0, vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(_x, _y, 0.0f));
			m_pSphere->InitialiseTextures("Resources/Textures/green.bmp", 1);
			m_pSpheres.push_back(m_pSphere);
		}
	}

	int counter = 0;
	//Link Particles
	for (size_t y = 0; y < sqrt(m_pSpheres.size()); y++)
	{
		for (size_t x = 0; x < sqrt(m_pSpheres.size()); x++)
		{
			//if not the first column
			if (x != 0)
			{
				m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - 1]);
				//if not the first row
				if (y != 0)
				{
					m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - sqrt(m_pSpheres.size())]);
				}
			}
			else
			{
				//if not the first row
				if (y != 0)
				{
					m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - sqrt(m_pSpheres.size())]);
				}
			}
			counter++;
		}
	}
	//set 5 anchors
	int anchors = 10;
	for (size_t i = 0; i < anchors; i++)
	{
		CParticle* m_pSphere = new CParticle();
		m_pSphere->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15) + float(i * 10), 10.0f, 0.0f));
		m_pSphere->InitialiseTextures("Resources/Textures/green.bmp", 1);
		m_pSphere->SetAsAnchor();
		m_pAnchorSpheres.push_back(m_pSphere);
	}
	//Link top row of cloth with anchors with a gap in the center
	for (int i = 0; i < anchors; i++)
	{
		m_pSpheres[i]->LinkParticles(m_pAnchorSpheres[i]);
	}
	
	
	//create a ball obstacle
	m_pBall = new CPrefab;
	m_pBall->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(10.0f, 10.0f, 10.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15)/2, -20.0f, 0.0f));
	m_pBall->InitialiseTextures("Resources/Textures/green.bmp", 1);


	
	//load floor
	m_pFloor = new CPrefab();
	m_pFloor->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::CUBE, "", 0, vec3(200.0f, 1.0f, 200.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15)/2, -50.0f, 0.0f));
	//load textures 
		//game textures

}

//render function
void CGameManager::Render()
{
	//Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	

		for (size_t i = 0; i < m_pSpheres.size(); i++)
		{
			m_pSpheres[i]->RenderShapes(m_giPhongProgram);
		}
		for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
		{
			m_pAnchorSpheres[i]->RenderShapes(m_giPhongProgram);
		}
		
		//m_pBall->RenderShapes(m_giBlinnProgram);
		RenderLines();

		
		//render floor and obstacle
		//m_pBall->RenderShapes(m_giBlinnProgram);
		//m_pFloor->RenderShapes(m_giBlinnProgram);

	


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
	
	//menu component updates
		for (size_t i = 0; i < m_pSpheres.size(); i++)
		{
			m_pSpheres[i]->UpdateShapes();
			m_pSpheres[i]->Update();
			//check for obstacle collision
			//m_pSpheres[i]->CheckObstacle(m_pBall);
		}
		for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
		{
			m_pAnchorSpheres[i]->UpdateShapes();
			m_pSpheres[i]->Update();
		}

		//m_pBall->UpdateShapes();

		//m_pFloor->UpdateShapes();
		//set camera looking at anchor sphere
		m_pProjCamera->LookAtObject(m_pAnchorSpheres[(m_pAnchorSpheres.size() / 2) - 1]->GetObjPosition());

	

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
				for (size_t i = 2; i <= (m_pAnchorSpheres.size() / 2) + 1; i++)
				{
					float move = 1.0f * float((1.0f / i) * 2.0f);
					m_pAnchorSpheres[i - 2]->SetObjPosition(vec3(m_pAnchorSpheres[i - 2]->GetObjPosition().x - move, m_pAnchorSpheres[i - 2]->GetObjPosition().y, m_pAnchorSpheres[i - 2]->GetObjPosition().z));
					m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->SetObjPosition(vec3(m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().x + move, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().y, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().z));
				}
			}
			else if (KeyState['s'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 2; i <= (m_pAnchorSpheres.size() / 2) + 1; i++)
				{
					float move = 1.0f * float((1.0f / i) * 2.0f);
					m_pAnchorSpheres[i - 2]->SetObjPosition(vec3(m_pAnchorSpheres[i - 2]->GetObjPosition().x + move, m_pAnchorSpheres[i - 2]->GetObjPosition().y, m_pAnchorSpheres[i - 2]->GetObjPosition().z));
					m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->SetObjPosition(vec3(m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().x - move, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().y, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().z));
				}
			}

			if (KeyState['a'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
				{
					m_pAnchorSpheres[i]->SetObjPosition(vec3(m_pAnchorSpheres[i]->GetObjPosition().x, m_pAnchorSpheres[i]->GetObjPosition().y, m_pAnchorSpheres[i]->GetObjPosition().z - 1.0f));
				}
			}
			else if (KeyState['d'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
				{
					m_pAnchorSpheres[i]->SetObjPosition(vec3(m_pAnchorSpheres[i]->GetObjPosition().x, m_pAnchorSpheres[i]->GetObjPosition().y, m_pAnchorSpheres[i]->GetObjPosition().z + 1.0f));
				}
				

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
			if (KeyState['q'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 0; i < m_pSpheres.size(); i++)
				{
					m_pSpheres[i]->SetWind();
					
					m_fcounter++;
				}
				cout << "Setting Wind: " << endl;
			}
			
		}
		else if (KeyState['q'] == InputState::INPUT_UP && KeyState['w'] == InputState::INPUT_UP && KeyState['s'] == InputState::INPUT_UP)
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

void CGameManager::RenderLines()
{
	int counter = 0;
	float SCALE = 90.0f;
	float upBoost = 0.5f;

	for (size_t y = 0; y < sqrt(m_pSpheres.size()); y++)
	{
		for (size_t x = 0; x < sqrt(m_pSpheres.size()); x++)
		{
			if (x != 0)
			{
				glBegin(GL_LINES);
				glVertex3f(m_pSpheres[counter]->GetObjPosition().x / SCALE, m_pSpheres[counter]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter]->GetObjPosition().z / SCALE);
				glVertex3f(m_pSpheres[counter - 1]->GetObjPosition().x / SCALE, m_pSpheres[counter - 1]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter - 1]->GetObjPosition().z / SCALE);
				glEnd();

				if (y != 0)
				{
					glBegin(GL_LINES);
					glVertex3f(m_pSpheres[counter]->GetObjPosition().x / SCALE, m_pSpheres[counter]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter]->GetObjPosition().z / SCALE);
					glVertex3f(m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().x / SCALE, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().z / SCALE);
					glEnd();
				}
			}
			else
			{
				if (y != 0)
				{
					glBegin(GL_LINES);
					glVertex3f(m_pSpheres[counter]->GetObjPosition().x / SCALE, m_pSpheres[counter]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter]->GetObjPosition().z / SCALE);
					glVertex3f(m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().x / SCALE, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().z / SCALE);
					glEnd();
				}
			}
			counter++;
		}
	}
}