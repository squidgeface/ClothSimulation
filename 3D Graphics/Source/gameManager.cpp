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
#include "slider.h"

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
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//Load shader variables
	m_giStaticProgram = ShaderLoader::CreateProgram("Resources/Shaders/vertexshader.txt", "Resources/Shaders/fragshader.txt");
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

void CGameManager::SetUpCloth()
{
	//Load shapes
	for (size_t y = 0; y < sqrt(gridSize); y++)
	{
		for (size_t x = 0; x < sqrt(gridSize); x++)
		{
			//offset each x and y to form a grid shape
			float _x = -((sqrt(gridSize) / 2) * 15) + float(x * 10); //(sqrt(gridSize)) + (x * 5);
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
				m_pSpheres[counter - 1]->LinkParticles(m_pSpheres[counter]);
				//if not the first row
				if (y != 0)
				{
					m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - sqrt(m_pSpheres.size())]);
					m_pSpheres[counter - sqrt(m_pSpheres.size())]->LinkParticles(m_pSpheres[counter]);
					//m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - (sqrt(m_pSpheres.size()) + 1)]);
				}
			}
			else
			{
				//if not the first row
				if (y != 0)
				{
					m_pSpheres[counter]->LinkParticles(m_pSpheres[counter - sqrt(m_pSpheres.size())]);
					m_pSpheres[counter - sqrt(m_pSpheres.size())]->LinkParticles(m_pSpheres[counter]);
				}
			}
			counter++;
		}
	}
	//set anchors
	for (size_t i = 0; i < anchors; i++)
	{
		CParticle* m_pSphere = new CParticle();
		m_pSphere->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15) + float(i * 10), 10.0f, 0.0f));
		m_pSphere->InitialiseTextures("Resources/Textures/green.bmp", 1);
		m_pSphere->SetAsAnchor();
		m_pAnchorSpheres.push_back(m_pSphere);
	}
	//Link top row of cloth with anchors with a gap in the center

	int d = anchors % 2;
	if (d != 0)
	{
		for (int i = 0; i < (anchors / 2); i++)
		{
			m_pAnchorSpheres[i]->LinkParticles(m_pSpheres[i]);
			m_pSpheres[i]->LinkParticles(m_pAnchorSpheres[i]);
		}
		int count = 1;
		for (int i = anchors - 1; i > anchors / 2; i--)
		{
			m_pAnchorSpheres[i]->LinkParticles(m_pSpheres[sqrtf(m_pSpheres.size()) - count]);
			m_pSpheres[sqrtf(m_pSpheres.size()) - count]->LinkParticles(m_pAnchorSpheres[i]);
			count++;
		}
		m_pAnchorSpheres[anchors / 2]->LinkParticles(m_pSpheres[sqrtf(m_pSpheres.size()) / 2]);
	}
	else
	{
		for (int i = 0; i < (anchors / 2); i++)
		{
			m_pAnchorSpheres[i]->LinkParticles(m_pSpheres[i]);
			m_pSpheres[i]->LinkParticles(m_pAnchorSpheres[i]);
		}
		int count = 1;
		for (int i = anchors - 1; i > anchors / 2 - 1; i--)
		{
			m_pAnchorSpheres[i]->LinkParticles(m_pSpheres[sqrtf(m_pSpheres.size()) - count]);
			m_pSpheres[sqrtf(m_pSpheres.size()) - count]->LinkParticles(m_pAnchorSpheres[i]);
			count++;
		}
		
	}
}

//Initialise Menu items
void CGameManager::InitialiseMenu()
{
	SetUpCloth();

	//create a ball obstacle
	m_pBall = new CPrefab;
	m_pBall->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(10.0f, 10.0f, 10.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15)/2, -20.0f, 0.0f));
	m_pBall->InitialiseTextures("Resources/Textures/green.bmp", 1);


	//load floor
	m_pFloor = new CPrefab();
	m_pFloor->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::CUBE, "", 0, vec3(500.0f, 1.0f, 500.0f), vec3(), vec3(-((sqrt(gridSize) / 2) * 15)/2, -200.0f, 0.0f));
	m_pFloor->InitialiseTextures("Resources/Textures/green.bmp", 1);

	//create slider for Cloth size
	sizeSlider = new CSlider();
	sizeSlider->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "Resources/Textures/sizeSlider.png", 0, vec3(200.0f, 40.0f, 1.0f), vec3(0.0f,0.0f,0.0f), vec3(200.0f, 50.0f, 0.0f));
	//create slider for Anchors
	anchorSlider = new CSlider();
	anchorSlider->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "Resources/Textures/anchorSlider.png", 0, vec3(200.0f, 40.0f, 1.0f), vec3(0.0f,0.0f,0.0f), vec3(200.0f, 100.0f, 0.0f));

}
//clear menu
void CGameManager::Clear()
{
	m_pSpheres.clear();
	m_pAnchorSpheres.clear();
	SetUpCloth();
}
//render function
void CGameManager::Render()
{
	//Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	m_pFloor->RenderShapes(m_giPhongProgram);

		for (size_t i = 0; i < m_pSpheres.size(); i++)
		{
			m_pSpheres[i]->RenderShapes(m_giPhongProgram);
			m_pSpheres[i]->Draw();
		}
		for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
		{
			m_pAnchorSpheres[i]->RenderShapes(m_giPhongProgram);
		}
		
		//m_pBall->RenderShapes(m_giBlinnProgram);
		//RenderLines();

		
		//render floor and obstacle
		//m_pBall->RenderShapes(m_giPhongProgram);
	

		sizeSlider->Render(m_giStaticProgram);
		anchorSlider->Render(m_giStaticProgram);


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
	
	//Update Particle points
		for (size_t i = 0; i < m_pSpheres.size(); i++)
		{
			m_pSpheres[i]->UpdateShapes();
			m_pSpheres[i]->Update();
			//check for obstacle collision
			//m_pSpheres[i]->CheckObstacle(m_pBall);
			//check floor collision
			m_pSpheres[i]->CheckFloor(m_pFloor);
		}
	//update anchor points
		for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
		{
			m_pAnchorSpheres[i]->UpdateShapes();
			m_pAnchorSpheres[i]->Update();
		}
		RipCloth();
		//m_pBall->UpdateShapes();

		m_pFloor->UpdateShapes();

		//set camera looking at anchors
		m_pProjCamera->LookAtObject(m_pAnchorSpheres[(m_pAnchorSpheres.size() / 2) - 1]->GetObjPosition());

		sizeSlider->Update();
		anchorSlider->Update();
		SetClothSize(sizeSlider->GetClothSize());
		SetAnchorSize(anchorSlider->GetAnchorSize());

	//update game information
	glutPostRedisplay();
}



//input functions
void CGameManager::ProcessInput(InputState* KeyState, InputState* MouseState)
{
								/// *** INPUT *** ///

		//if mouse click counter is 0 (one click at a time)
		if (m_fcounter == 0.0f)
		{
			if (KeyState['a'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 2; i <= (m_pAnchorSpheres.size() / 2) + 1; i++)
				{
					float move = 1.0f * float((1.0f / i) * 2.0f);
					m_pAnchorSpheres[i - 2]->SetObjPosition(vec3(m_pAnchorSpheres[i - 2]->GetObjPosition().x - move, m_pAnchorSpheres[i - 2]->GetObjPosition().y, m_pAnchorSpheres[i - 2]->GetObjPosition().z));
					m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->SetObjPosition(vec3(m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().x + move, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().y, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().z));
				}
			}
			else if (KeyState['d'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 2; i <= (m_pAnchorSpheres.size() / 2) + 1; i++)
				{
					float move = 1.0f * float((1.0f / i) * 2.0f);
					m_pAnchorSpheres[i - 2]->SetObjPosition(vec3(m_pAnchorSpheres[i - 2]->GetObjPosition().x + move, m_pAnchorSpheres[i - 2]->GetObjPosition().y, m_pAnchorSpheres[i - 2]->GetObjPosition().z));
					m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->SetObjPosition(vec3(m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().x - move, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().y, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().z));
				}
			}

			if (KeyState['s'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
				{
					m_pAnchorSpheres[i]->SetObjPosition(vec3(m_pAnchorSpheres[i]->GetObjPosition().x, m_pAnchorSpheres[i]->GetObjPosition().y, m_pAnchorSpheres[i]->GetObjPosition().z - 1.0f));
				}
			}
			else if (KeyState['w'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
				{
					m_pAnchorSpheres[i]->SetObjPosition(vec3(m_pAnchorSpheres[i]->GetObjPosition().x, m_pAnchorSpheres[i]->GetObjPosition().y, m_pAnchorSpheres[i]->GetObjPosition().z + 1.0f));
				}
			}
			
			//Initialise wind
			if (KeyState['q'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 0; i < m_pSpheres.size(); i++)
				{
					m_pSpheres[i]->SetWind();
					
					m_fcounter++;
				}
				cout << "Setting Wind: " << endl;
			}

			//Initialise wind
			if (KeyState['r'] == InputState::INPUT_DOWN)
			{
				
				Clear();
				m_fcounter++;

				cout << "Resetting Scene" << endl;
			}

			//Remove anchors
			if (KeyState['x'] == InputState::INPUT_DOWN)
			{

				for (int i = 0; i < anchors; i++)
				{
					m_pAnchorSpheres[i]->UnLinkParticles();
				}
				m_fcounter++;

				cout << "Dropping Cloth" << endl;
			}

			if (MouseState[0] == InputState::INPUT_DOWN)
			{
				isClicking = true;
			}
			else
			{
				isClicking = false;
			}
			
		}
		else if (KeyState['q'] == InputState::INPUT_UP && KeyState['r'] == InputState::INPUT_UP && KeyState['x'] == InputState::INPUT_UP)
		{
		//reset counter on mouse up
			m_fcounter = 0.0f;
		}
		

													// **** GAME INPUT **** //

	

}


void CGameManager::RipCloth()
{
	vec3 mousePos = GetRayFromMouse();
	int diffx = m_pInput->GetMouseX() - previousX; //check the difference between the current x and the last x position
	int diffy = m_pInput->GetMouseY() - previousY; //check the difference between the current y and the last y position
	float m_imouseX = 2.0f * m_pInput->GetMouseX();// / Utils::ScreenWidth;
	float m_imouseY = 0.0f - (2.0f * m_pInput->GetMouseY());// / Utils::ScreenHeight);

	vec2 force = vec2(m_pInput->GetMouseX() - previousX, m_pInput->GetMouseY() - previousY);
	for (size_t i = 0; i < m_pSpheres.size(); i++)
	{
		if (CheckMouseSphereIntersect(m_pSpheres[i]) && isClicking)
		{
			m_pSpheres[i]->ApplyForce(vec3(force.x * 50.0f * m_pSpheres[i]->GetMass(), -force.y * 100.0f * m_pSpheres[i]->GetMass(), 0.0f));
		}
	}
	previousX = m_pInput->GetMouseX();
	previousY = m_pInput->GetMouseY();
}

void CGameManager::SetClothSize(int _size)
{
	gridSize = _size;
}

void CGameManager::SetAnchorSize(int _size)
{
	anchors = _size;
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
	float SCALE = 300.0f;
	float upBoost = 0.5f;
	float rightBoost = -0.5f;

	for (size_t y = 0; y < sqrt(m_pSpheres.size()); y++)
	{
		for (size_t x = 0; x < sqrt(m_pSpheres.size()); x++)
		{
			if (x != 0)
			{
				glBegin(GL_LINES);
				glVertex3f(m_pSpheres[counter]->GetObjPosition().x / SCALE + rightBoost, m_pSpheres[counter]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter]->GetObjPosition().z / SCALE);
				glVertex3f(m_pSpheres[counter - 1]->GetObjPosition().x / SCALE + rightBoost, m_pSpheres[counter - 1]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter - 1]->GetObjPosition().z / SCALE);
				glEnd();

				if (y != 0)
				{
					glBegin(GL_LINES);
					glVertex3f(m_pSpheres[counter]->GetObjPosition().x / SCALE + rightBoost, m_pSpheres[counter]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter]->GetObjPosition().z / SCALE);
					glVertex3f(m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().x / SCALE + rightBoost, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().z / SCALE);
					glVertex3f(m_pSpheres[counter]->GetObjPosition().x / SCALE + rightBoost, m_pSpheres[counter]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter]->GetObjPosition().z / SCALE);
					glVertex3f(m_pSpheres[counter - (sqrt(m_pSpheres.size())+1)]->GetObjPosition().x / SCALE + rightBoost, m_pSpheres[counter - (sqrt(m_pSpheres.size()) + 1)]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter - (sqrt(m_pSpheres.size()) + 1)]->GetObjPosition().z / SCALE);
					glEnd();

				}
			}
			else
			{
				if (y != 0)
				{
					glBegin(GL_LINES);
					glVertex3f(m_pSpheres[counter]->GetObjPosition().x / SCALE + rightBoost, m_pSpheres[counter]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter]->GetObjPosition().z / SCALE);
					glVertex3f(m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().x / SCALE + rightBoost, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().y / SCALE + upBoost, m_pSpheres[counter - sqrt(m_pSpheres.size())]->GetObjPosition().z / SCALE);
					glEnd();
				}
			}
			counter++;
		}
	}
}

vec3 CGameManager::GetRayFromMouse()
{
	//normalize mouse position
	float m_imouseX = 2.0f * m_pInput->GetMouseX() / Utils::ScreenWidth;
	float m_imouseY = 0.0f - (2.0f * m_pInput->GetMouseY() / Utils::ScreenHeight);

	//calculate ray direction vector
	vec2 ray_nds = vec2(m_imouseX, m_imouseY);
	vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
	mat4 invProjMat = inverse(m_pProjCamera->GetCamera());
	vec4 eyeCoords = invProjMat * ray_clip;
	eyeCoords = vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
	mat4 invViewMat = inverse(m_pProjCamera->GetView());
	vec4 rayWorld = invViewMat * eyeCoords;
	m_v3RayDirection = normalize(vec3(rayWorld));

	return m_v3RayDirection;
}

//Check mouse picking ray against a sphere object
bool CGameManager::CheckMouseSphereIntersect(CPrefab* _object)
{
	//radius is object x size
	float radius = _object->GetObjSize().x * 10;

	//get vector from camera position to objectposition
	vec3 v = _object->GetObjPosition() - m_pProjCamera->GetCamPos();

	//calculate difference of ray and object vector within radius
	float a = dot(GetRayFromMouse(), GetRayFromMouse());
	float b = 2 * dot(v, GetRayFromMouse());
	float c = dot(v, v) - radius * radius;
	float d = b * b - 4 * a * c;

	if (d > 0) {
		float x1 = (-b - sqrtf(d)) / 2.0f;
		float x2 = (-b + sqrtf(d)) / 2.0f;

		if (x1 >= 0 && x2 >= 0) return true; // intersects
		if (x1 < 0 && x2 >= 0) return true; // intersects
	}
	else if (d <= 0)
	{
		return false;// no intersection
	}

}