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
	delete m_pBall;
	delete m_pCapsule;
	delete m_pFloor;
	m_pOrthoCamera = 0;
	m_pProjCamera = 0;
	m_pTime = 0;
	m_pInput = 0;
	m_pBall = 0;
	m_pBall = 0;
	m_pFloor = 0;
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
	glEnable(GL_DEPTH_TEST);

	//Initialise time
	m_pTime->InitiateTime();

	//Seed random
	srand(int(time(NULL)));

	//Set Up Camera from class
	m_pOrthoCamera->SetOrtho();
	m_pProjCamera->SetProjection();
}
//Set up the cloth - used in reset
void CGameManager::SetUpCloth()
{
	//counter for cloth
	int count = 0;
	//Load shapes
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//offset each x and y to form a grid shape
			float _x = -((width / 2) * 15) + float(x * 5) + width; //(sqrt(gridSize)) + (x * 5);
			float _y =  -20 + (-5.0f * y);
			//initialise a new particle
			CParticle* m_pSphere = new CParticle();
			m_pSphere->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::CUBE, "", 0, vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(_x, _y, 0.0f));
			m_pSphere->InitialiseTextures("Resources/Textures/green.bmp", 1);
			//Initialise Geometry
			m_pSphere->InitialiseGeo(m_pProjCamera, m_pTime, m_pInput, MeshType::GEOMETRY, "", 0, vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(_x, _y, 0.0f));
			//Push particle to vector
			m_pSpheres.push_back(m_pSphere);
			count++;
		}
	}

	count = 0;
	//Link Particles Up and Left and Diagonal
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//if not the first column
			if (x != 0)
			{
				m_pSpheres[count]->LinkParticles(m_pSpheres[count - 1]);
				m_pSpheres[count - 1]->LinkParticles(m_pSpheres[count]);
				//if not the first row
				if (y != 0)
				{
					m_pSpheres[count]->LinkParticles(m_pSpheres[count - width]);
					m_pSpheres[count - width]->LinkParticles(m_pSpheres[count]);
					m_pSpheres[count]->LinkParticles(m_pSpheres[count - (width + 1)]);
				}

				if (y != height - 1)
				{
					m_pSpheres[count]->LinkParticles(m_pSpheres[count + (width - 1)]);
				}
			}
			else
			{
				//if not the first row
				if (y != 0)
				{
					m_pSpheres[count]->LinkParticles(m_pSpheres[count - width]);
					m_pSpheres[count - width]->LinkParticles(m_pSpheres[count]);
				}
			}
			count++;
		}
	}
	//Create Anchors
	for (int i = 0; i < anchors; i++)
	{
		//Create a particle as an anchor
		CParticle* m_pSphere = new CParticle();
		float _x = -((width / 2) * 15) + float(i * 5) + width;
		m_pSphere->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(1.0f, 1.0f, 1.0f), vec3(), vec3(_x, 10.0f, 0.0f));
		m_pSphere->InitialiseTextures("Resources/Textures/green.bmp", 1);
		m_pSphere->SetAsAnchor();
		//Push to Anchor Vector
		m_pAnchorSpheres.push_back(m_pSphere);
	}

	//Link top row of cloth with anchors with a gap in the center for even number of anchors
	int d = anchors % 2;
	//If even number of anchors
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
			m_pAnchorSpheres[i]->LinkParticles(m_pSpheres[width - count]);
			m_pSpheres[width - count]->LinkParticles(m_pAnchorSpheres[i]);
			count++;
		}
		m_pAnchorSpheres[anchors / 2]->LinkParticles(m_pSpheres[width / 2]);
	}
	//If odd number of anchors
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
		
			m_pAnchorSpheres[i]->LinkParticles(m_pSpheres[width - count]);
			m_pSpheres[width - count]->LinkParticles(m_pAnchorSpheres[i]);
			count++;
		}
		
	}
}

//Initialise Menu items
void CGameManager::InitialiseMenu()
{
	//Set up the cloth
	SetUpCloth();

	//create a ball obstacle
	m_pBall = new CPrefab;
	m_pBall->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(15.0f, 15.0f, 15.0f), vec3(), vec3(-((width / 2) * 15) + float(width/2 * 5) + width, -100.0f, 20.0f));
	m_pBall->InitialiseTextures("Resources/Textures/green.bmp", 1);
	//create a capsule obstacle
	m_pCapsule = new CPrefab;
	m_pCapsule->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::CAPSULE, "", 0, vec3(10.0f, 10.0f, 10.0f), vec3(), vec3(-((width / 2) * 15) / 2.0f, -100.0f, 20.0f));
	m_pCapsule->InitialiseTextures("Resources/Textures/green.bmp", 1);


	//load floor object
	m_pFloor = new CPrefab();
	m_pFloor->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::CUBE, "", 0, vec3(500.0f, 1.0f, 500.0f), vec3(), vec3(-((width / 2) * 15) + float(width / 2 * 5) + width, -200.0f, 0.0f));
	m_pFloor->InitialiseTextures("Resources/Textures/green.bmp", 1);

	//create slider for Cloth Width
	m_pWidthSlider = new CSlider();
	m_pWidthSlider->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "Resources/Textures/widthSlider.png", 0, vec3(200.0f, 40.0f, 1.0f), vec3(0.0f,0.0f,0.0f), vec3(-Utils::HalfScreenW + 110, -Utils::HalfScreenH + 60, 0.0f), false);
	//create slider for Cloth Height
	m_pHeightSlider = new CSlider();
	m_pHeightSlider->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "Resources/Textures/heightSlider.png", 0, vec3(200.0f, 40.0f, 1.0f), vec3(0.0f,0.0f,0.0f), vec3(-Utils::HalfScreenW + 110, -Utils::HalfScreenH + 110, 0.0f), false);
	//create slider for Anchors
	m_pAnchorSlider = new CSlider();
	m_pAnchorSlider->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "Resources/Textures/anchorSlider.png", 0, vec3(200.0f, 40.0f, 1.0f), vec3(0.0f,0.0f,0.0f), vec3(-Utils::HalfScreenW + 110, -Utils::HalfScreenH + 160, 0.0f), true);
	//create slider for Wind Strength
	m_pWindStSlider = new CSlider();
	m_pWindStSlider->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "Resources/Textures/wStrengthSlider.png", 0, vec3(200.0f, 40.0f, 1.0f), vec3(0.0f,0.0f,0.0f), vec3(-Utils::HalfScreenW + 110, -Utils::HalfScreenH + 210, 0.0f), true);
	//create slider for Wind Size
	m_pWindSiSlider = new CSlider();
	m_pWindSiSlider->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "Resources/Textures/wSizeSlider.png", 0, vec3(200.0f, 40.0f, 1.0f), vec3(0.0f,0.0f,0.0f), vec3(-Utils::HalfScreenW + 110, -Utils::HalfScreenH + 260, 0.0f), true);
	//Set camera looking at where objects spawn
	m_pProjCamera->SetPosition(vec3(m_pBall->GetObjPosition().x, -100.0f, 200.0f), m_pBall->GetObjPosition());

	//Set Up on screen buttons and UI
	m_pUpCam = new CButton();
	m_pUpCam->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(Utils::HalfScreenW - 80.0f, -Utils::HalfScreenH + 80, 0.0f));
	m_pUpCam->InitialiseTextures("Resources/Textures/upCam.png", 1);
	m_pDownCam = new CButton();
	m_pDownCam->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(Utils::HalfScreenW - 80.0f, -Utils::HalfScreenH + 120, 0.0f));
	m_pDownCam->InitialiseTextures("Resources/Textures/downCam.png", 1);
	m_pLeftCam = new CButton();
	m_pLeftCam->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(Utils::HalfScreenW - 120.0f, -Utils::HalfScreenH + 120, 0.0f));
	m_pLeftCam->InitialiseTextures("Resources/Textures/leftCam.png", 1);
	m_pRightCam = new CButton();
	m_pRightCam->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(Utils::HalfScreenW - 40.0f, -Utils::HalfScreenH + 120, 0.0f));
	m_pRightCam->InitialiseTextures("Resources/Textures/rightCam.png", 1);
	m_pInCam = new CButton();
	m_pInCam->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(Utils::HalfScreenW - 120.0f, -Utils::HalfScreenH + 80, 0.0f));
	m_pInCam->InitialiseTextures("Resources/Textures/inCam.png", 1);
	m_pOutCam = new CButton();
	m_pOutCam->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(Utils::HalfScreenW - 40.0f, -Utils::HalfScreenH + 80, 0.0f));
	m_pOutCam->InitialiseTextures("Resources/Textures/outCam.png", 1);
	m_pRLeftCam = new CButton();
	m_pRLeftCam->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(Utils::HalfScreenW - 100.0f, -Utils::HalfScreenH + 170, 0.0f));
	m_pRLeftCam->InitialiseTextures("Resources/Textures/rLeftCam.png", 1);
	m_pRRightCam = new CButton();
	m_pRRightCam->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(Utils::HalfScreenW - 60.0f, -Utils::HalfScreenH + 170, 0.0f));
	m_pRRightCam->InitialiseTextures("Resources/Textures/rRightCam.png", 1);
	//Cut button
	m_pSetCutting = new CButton();
	m_pSetCutting->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(-Utils::HalfScreenW + 60.0f, Utils::HalfScreenH - 230, 0.0f));
	m_pSetCutting->InitialiseTextures("Resources/Textures/chkBox.png", 1);
	m_pSetCutting->InitialiseTextures("Resources/Textures/chkBoxT.png", 2);
	m_pSetCutting->SetText("Cut");
	//Rip button
	m_pSetRipping = new CButton();
	m_pSetRipping->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(-Utils::HalfScreenW + 60.0f, Utils::HalfScreenH - 190, 0.0f));
	m_pSetRipping->InitialiseTextures("Resources/Textures/chkBox.png", 1);
	m_pSetRipping->InitialiseTextures("Resources/Textures/chkBoxT.png", 2);
	m_pSetRipping->SetText("Rip");
	m_pSetRipping->SetButton(true);
	//Burn button
	m_pSetBurning = new CButton();
	m_pSetBurning->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0f, 40.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(-Utils::HalfScreenW + 60.0f, Utils::HalfScreenH - 270, 0.0f));
	m_pSetBurning->InitialiseTextures("Resources/Textures/chkBox.png", 1);
	m_pSetBurning->InitialiseTextures("Resources/Textures/chkBoxT.png", 2);
	m_pSetBurning->SetText("Ignight");
	//Instructions
	m_pInstructions = new CTextLabel();
	m_pInstructions->SetLabel(" Press 'R' to Reset \n Press 'Q' to toggle wind \n Shape Selection: '1' Sphere, '2' Capsule, '~' None \n 'W' and 'S' - Move Cloth \n 'A' and 'D' - Stretch/Squash Hooks \n 'X' Drop Cloth", "Resources/Fonts/BRLNSR.ttf", vec2(-Utils::HalfScreenW + 30.0f, -Utils::HalfScreenH + 140), vec3(0.0f, 0.0f, 0.0f), 0.5f);
}
//Reset cloth
void CGameManager::Clear()
{
	//clear vectors
	m_pSpheres.clear();
	m_pAnchorSpheres.clear();
	//Check slider changes
	SetClothWidth(m_pWidthSlider->GetClothSize());
	SetClothHeight(m_pHeightSlider->GetClothSize());
	SetAnchorSize(m_pAnchorSlider->GetAnchorSize());
	//Set up the cloth
	SetUpCloth();
	//Move objects to line up with cloth
	m_pBall->SetObjPosition(vec3(-((width / 2) * 15) + float(width / 2 * 5) + width, m_pBall->GetObjPosition().y, m_pBall->GetObjPosition().z));
	m_pCapsule->SetObjPosition(vec3(-((width / 2) * 15) + float(width / 2 * 5) + width, m_pCapsule->GetObjPosition().y, m_pCapsule->GetObjPosition().z));
	m_pFloor->SetObjPosition(vec3(-((width / 2) * 15) + float(width / 2 * 5) + width, m_pFloor->GetObjPosition().y, m_pFloor->GetObjPosition().z));
	//Reset to default camera position
	m_pProjCamera->m_fTimeElapsed = 0.0f;
	m_pProjCamera->SetPosition(vec3(m_pBall->GetObjPosition().x, -100.0f, 200.0f), m_pBall->GetObjPosition());
}
//render function
void CGameManager::Render()
{
	//Clear buffers
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//Render floor
	m_pFloor->RenderShapes(m_giPhongProgram);
		
	//Render Cloth
		for (size_t i = 0; i < m_pSpheres.size(); i++)
		{
			//If the particle is not burned, render it
			if (m_pSpheres[i]->bisBurned != true)
			{
				m_pSpheres[i]->RenderShapes(m_giPhongProgram);
				m_pSpheres[i]->Draw();
			}
		}
		//Render Anchor points
		for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
		{
			m_pAnchorSpheres[i]->RenderShapes(m_giPhongProgram);
		}
		int counter = 0;

		//Draw Geometry
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (x != 0)
				{
					if (y != 0)
					{
						m_pSpheres[counter]->DrawGeo(m_pSpheres[counter - 1 ]->GetObjPosition(), m_pSpheres[counter - width]->GetObjPosition(), m_pSpheres[counter - width - 1]->GetObjPosition());
					}
				}
				counter++;
			}
		}
		//draw shapes
		if (shape == 1)
		{
			m_pBall->RenderShapes(m_giPhongProgram);
		}
		else if (shape == 2)
		{
			m_pCapsule->RenderShapes(m_giPhongProgram);
		}

		//draw UI
		m_pWidthSlider->Render(m_giStaticProgram);
		m_pHeightSlider->Render(m_giStaticProgram);
		m_pAnchorSlider->Render(m_giStaticProgram);
		m_pWindStSlider->Render(m_giStaticProgram);
		m_pWindSiSlider->Render(m_giStaticProgram);
		m_pUpCam->RenderShapes(m_giStaticProgram);
		m_pDownCam->RenderShapes(m_giStaticProgram);
		m_pLeftCam->RenderShapes(m_giStaticProgram);
		m_pRightCam->RenderShapes(m_giStaticProgram);	
		m_pInCam->RenderShapes(m_giStaticProgram);
		m_pOutCam->RenderShapes(m_giStaticProgram);
		m_pRLeftCam->RenderShapes(m_giStaticProgram);
		m_pRRightCam->RenderShapes(m_giStaticProgram);
		m_pSetCutting->RenderShapes(m_giStaticProgram);
		m_pSetRipping->RenderShapes(m_giStaticProgram);
		m_pSetBurning->RenderShapes(m_giStaticProgram);
		m_pInstructions->Render();

	glutSwapBuffers();
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
			//check for obstacle collision when shape is active
			if (shape == 1)
			{
				m_pSpheres[i]->CheckObstacle(m_pBall);
			}
			else if (shape == 2)
			{
				m_pSpheres[i]->CheckCapsule(m_pCapsule);
			}
		
			//check floor collision
			m_pSpheres[i]->CheckFloor(m_pFloor);
		}
	//update anchor points
		for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
		{
			m_pAnchorSpheres[i]->UpdateShapes();
			m_pAnchorSpheres[i]->Update();
		}
	//Set cloth functions when selected
		if (m_bRipToggle)
		{
			RipCloth();
		}
		else if (m_bCutToggle)
		{
			RipClothClick();
		}
		else if (m_bBurnToggle)
		{
			BurnClothClick();
		}
		if (m_bWindToggle)
		{
			for (size_t i = 0; i < m_pSpheres.size(); i++)
			{
				m_pSpheres[i]->SetBreakable(false);
			}
			ApplyWind();
		}
		else
		{
			for (size_t i = 0; i < m_pSpheres.size(); i++)
			{
				m_pSpheres[i]->SetBreakable(true);
			}
		}
	//Updated shapes when active	
		if (shape == 1)
		{
			m_pBall->UpdateShapes();
		}
		else if (shape == 2)
		{
			m_pCapsule->UpdateShapes();
		}
	//Update Floor
		m_pFloor->UpdateShapes();

	//Update UI elements
		m_pWidthSlider->Update();
		m_pHeightSlider->Update();
		m_pAnchorSlider->Update();
		m_pWindStSlider->Update();
		m_pWindSiSlider->Update();
		m_pWindStSlider->SetWindStrength();
		m_pWindSiSlider->SetWindStrength();
		m_pAnchorSlider->SetClothSizeNumber(m_pWidthSlider->GetClothSize());
		m_pUpCam->UpdateShapes();
		m_pDownCam->UpdateShapes();
		m_pLeftCam->UpdateShapes();
		m_pRightCam->UpdateShapes();
		m_pInCam->UpdateShapes();
		m_pOutCam->UpdateShapes();	
		m_pRLeftCam->UpdateShapes();
		m_pRRightCam->UpdateShapes();
		m_pSetCutting->UpdateShapes();
		m_pSetBurning->UpdateShapes();
		m_pSetRipping->UpdateShapes();
		
	//Reset cloth when sliders are moved	
		if (m_pAnchorSlider->GetMouse() == 2)
		{
			m_pAnchorSlider->ResetMouse();
			Clear();
		}
		if (m_pWidthSlider->GetMouse() == 2)
		{
			m_pWidthSlider->ResetMouse();
			Clear();
		}	
		if (m_pHeightSlider->GetMouse() == 2)
		{
			m_pHeightSlider->ResetMouse();
			Clear();
		}
	//Move the camera function
		MoveCamera();
	//update game information
	glutPostRedisplay();
}

// MOPve Camera Controls
void CGameManager::MoveCamera()
{
	//Move direction baed on button presses
	vec3 moveDir = vec3(m_fHorz, m_fVert, m_fLat);
	m_pProjCamera->MoveCamera(moveDir, m_pTime);
	//Rotation based on button presses and object position
	m_pProjCamera->CameraRotate(m_pBall->GetObjPosition(), m_pTime, m_fSpin);
}

//input functions
void CGameManager::ProcessInput(InputState* KeyState, InputState* MouseState)
{
								/// *** INPUT *** ///

		//if counter is 0 (one click/press at a time)
		if (m_fcounter == 0.0f)
		{
			//If mouse is down set isClicking
			if (MouseState[0] == InputState::INPUT_DOWN)
			{
				isClicking = true;
			}
			else
			{
				isClicking = false;
			}

			//Pressing A moves anchors away from each other
			if (KeyState['a'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 2; i <= (m_pAnchorSpheres.size() / 2) + 1; i++)
				{
					//Half the anchors in one direction and half in another
					float move = float((1.0f / i) * 2.0f);
					m_pAnchorSpheres[i - 2]->SetObjPosition(vec3(m_pAnchorSpheres[i - 2]->GetObjPosition().x - move, m_pAnchorSpheres[i - 2]->GetObjPosition().y, m_pAnchorSpheres[i - 2]->GetObjPosition().z));
					m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->SetObjPosition(vec3(m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().x + move, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().y, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().z));
				}
			}
			//Pressing D moves anchors away from each other
			else if (KeyState['d'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 2; i <= (m_pAnchorSpheres.size() / 2) + 1; i++)
				{
					//Half the anchors in one direction and half in another
					float move = float((1.0f / i) * 2.0f);
					m_pAnchorSpheres[i - 2]->SetObjPosition(vec3(m_pAnchorSpheres[i - 2]->GetObjPosition().x + move, m_pAnchorSpheres[i - 2]->GetObjPosition().y, m_pAnchorSpheres[i - 2]->GetObjPosition().z));
					m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->SetObjPosition(vec3(m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().x - move, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().y, m_pAnchorSpheres[m_pAnchorSpheres.size() + 1 - i]->GetObjPosition().z));
				}
			}
			//Pressing S moves anchors towards the front
			if (KeyState['s'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
				{
					m_pAnchorSpheres[i]->SetObjPosition(vec3(m_pAnchorSpheres[i]->GetObjPosition().x, m_pAnchorSpheres[i]->GetObjPosition().y, m_pAnchorSpheres[i]->GetObjPosition().z - 1.0f));
				}
			}
			//Pressing W moves anchors towards the back
			else if (KeyState['w'] == InputState::INPUT_DOWN)
			{
				for (size_t i = 0; i < m_pAnchorSpheres.size(); i++)
				{
					m_pAnchorSpheres[i]->SetObjPosition(vec3(m_pAnchorSpheres[i]->GetObjPosition().x, m_pAnchorSpheres[i]->GetObjPosition().y, m_pAnchorSpheres[i]->GetObjPosition().z + 1.0f));
				}
			}
			
			//Initialise wind with Q
			if (KeyState['q'] == InputState::INPUT_DOWN)
			{
				//disable other features
				m_pSetCutting->SetButton(false);
				m_pSetRipping->SetButton(false);
				m_pSetBurning->SetButton(false);
				m_bRipToggle = false;
				m_bCutToggle = false;
				m_bBurnToggle = false;
				//toggle wind
				m_bWindToggle = !m_bWindToggle;
				m_fcounter++;
				
				cout << "Setting Wind: " << endl;
			}

			//Reset Scene with R
			if (KeyState['r'] == InputState::INPUT_DOWN)
			{
				Clear();
				m_fcounter++;

				cout << "Resetting Scene" << endl;
			}

			//Remove anchors with X
			if (KeyState['x'] == InputState::INPUT_DOWN)
			{
				//Unlink all anchors
				for (int i = 0; i < anchors; i++)
				{
					m_pAnchorSpheres[i]->UnLinkParticles();
				}
				//Unlink top row of cloth
				for (int i = 0; i < width; i++)
				{
					m_pSpheres[i]->UnLinkParticles();
				}
				
				m_fcounter++;

				cout << "Dropping Cloth" << endl;
			}
			//Check button collisions
			if (m_pSetCutting->CheckHover(m_pInput) && isClicking && !m_pSetCutting->GetShowing())
			{
				//Sets button to checked
				m_pSetCutting->SetButton(true);
				//Sets other buttons to un checked
				m_pSetRipping->SetButton(false);
				m_pSetBurning->SetButton(false);
				m_bRipToggle = false;
				m_bCutToggle = true;
				m_bBurnToggle = false;
				//disable wind
				m_bWindToggle = false;
				m_fWindSize = 1.0f;
				m_fcounter++;
			}
			else if (m_pSetCutting->CheckHover(m_pInput) && isClicking && m_pSetCutting->GetShowing())
			{
				//Sets button to unchecked
				m_pSetCutting->SetButton(false);
				m_bCutToggle = false;
				m_fcounter++;
			}
			
			if (m_pSetRipping->CheckHover(m_pInput) && isClicking && !m_pSetRipping->GetShowing())
			{
				m_pSetCutting->SetButton(false);
				m_pSetRipping->SetButton(true);
				m_pSetBurning->SetButton(false);
				m_bRipToggle = true;
				m_bCutToggle = false;
				m_bBurnToggle = false;
				//disable wind
				m_bWindToggle = false;
				m_fWindSize = 1.0f;
				m_fcounter++;
			}
			else if (m_pSetRipping->CheckHover(m_pInput) && isClicking && m_pSetRipping->GetShowing())
			{
				m_pSetRipping->SetButton(false);
				m_bRipToggle = false;
				m_fcounter++;
			}

			if (m_pSetBurning->CheckHover(m_pInput) && isClicking && !m_pSetBurning->GetShowing())
			{
				m_pSetCutting->SetButton(false);
				m_pSetRipping->SetButton(false);
				m_pSetBurning->SetButton(true);
				m_bRipToggle = false;
				m_bCutToggle = false;
				m_bBurnToggle = true;
				//disable wind
				m_bWindToggle = false;
				m_fWindSize = 1.0f;
				m_fcounter++;
			}
			else if (m_pSetBurning->CheckHover(m_pInput) && isClicking && m_pSetBurning->GetShowing())
			{
				m_pSetBurning->SetButton(false);
				m_bBurnToggle = false;
				m_fcounter++;
			}
			//Sets shapes
			if (KeyState['1'] == InputState::INPUT_DOWN)
			{
				shape = 1;
			}
			if (KeyState['2'] == InputState::INPUT_DOWN)
			{
				shape = 2;
			}
		
			if (KeyState['`'] == InputState::INPUT_DOWN)
			{
				shape = 0;
			}

			
			//Camera control buttons
			if (m_pLeftCam->CheckHover(m_pInput) && isClicking)
			{
				m_fHorz = -1.0f;
			}
			else if (m_pRightCam->CheckHover(m_pInput) && isClicking)
			{
				m_fHorz = 1.0f;
			}
			else
			{
				m_fHorz = 0.0f;
			}
			
			if (m_pUpCam->CheckHover(m_pInput) && isClicking)
			{
				m_fVert = 1.0f;
			}
			else if (m_pDownCam->CheckHover(m_pInput) && isClicking)
			{
				m_fVert = -1.0f;
			}
			else
			{
				m_fVert = 0.0f;
			}	
			
			if (m_pOutCam->CheckHover(m_pInput) && isClicking)
			{
				m_fLat = 1.0f;
			}
			else if (m_pInCam->CheckHover(m_pInput) && isClicking)
			{
				m_fLat = -1.0f;
			}
			else
			{
				m_fLat = 0.0f;
			}
			
			if (m_pRLeftCam->CheckHover(m_pInput) && isClicking)
			{
				m_fSpin = -1.0f;
			}
			else if (m_pRRightCam->CheckHover(m_pInput) && isClicking)
			{
				m_fSpin = 1.0f;
			}
			else
			{
				m_fSpin = 0.0f;
			}

			
		}
		//If all keys are up, reset counter
		else if (KeyState['q'] == InputState::INPUT_UP && KeyState['r'] == InputState::INPUT_UP && KeyState['x'] == InputState::INPUT_UP && KeyState['f'] == InputState::INPUT_UP && MouseState[0] == InputState::INPUT_UP)
		{
		//reset counter on mouse up
			m_fcounter = 0.0f;
		}
}

//Rip cloth function
void CGameManager::RipCloth()
{
	//Get a force based on mouse movement
	vec2 force = vec2(m_pInput->GetMouseX() - previousX, m_pInput->GetMouseY() - previousY);
	//for all the points in the cloth
	for (size_t i = 0; i < m_pSpheres.size(); i++)
	{
		//Check if mouse is intersecting the point and clicking
		if (CheckMouseSphereIntersect(m_pSpheres[i]) && isClicking)
		{
			//changed y force based on direction (tweaking for visual effect)
			if (force.y > 0)
			{
				force.y *= 2.0f;
			}
			else
			{
				force.y *= 1.25f;
			}
			//Apply force to the point
			m_pSpheres[i]->ApplyForce(vec3(force.x * 50.0f * m_pSpheres[i]->GetMass(), -force.y * m_pSpheres[i]->GetMass(), 0.0f));
		}
	}
	//update previous mouse position
	previousX = float(m_pInput->GetMouseX());
	previousY = float(m_pInput->GetMouseY());
}
//Cut cloth function
void CGameManager::RipClothClick()
{
	//For all the points in the cloth
	for (size_t i = 0; i < m_pSpheres.size(); i++)
	{
		//Check if mouse is intersecting and mouse click
		if (CheckMouseSphereIntersect(m_pSpheres[i]) && isClicking)
		{
			//Unlink all other particles from this particle
			m_pSpheres[i]->UnLinkParticles();
		}
	}

}
//Burn cloth functions
void CGameManager::BurnClothClick()
{
	//For all the points in the cloth
	for (size_t i = 0; i < m_pSpheres.size(); i++)
	{
		//Check if mouse is intersecting and mouse click
		if (CheckMouseSphereIntersect(m_pSpheres[i]) && isClicking)
		{
			//Set point to burning
			m_pSpheres[i]->Burn();
		}
	}

}
//Apply Wind force to points
void CGameManager::ApplyWind()
{

	//nice gentle breeze
	m_fWindStrength = (rand() % m_pWindStSlider->GetClothSize()/4) - m_pWindStSlider->GetClothSize()/10;
	m_fWindSize = m_pWindSiSlider->GetClothSize() / 2;
	//Seed with with values
	float randWind = (rand() % 50) - 20;
	float randOffSet = (rand() % 10) - 5;
	float randOffSet2 = (rand() % 10) - 5;
	vec3 Wind = vec3(randOffSet, randOffSet2, randWind);
	//For all the points in the cloth
	for (size_t i = 0; i < m_pSpheres.size(); i++)
	{
		//Check if mouse is intersecting and mouse click
		if (CheckMouseSphereIntersect(m_pSpheres[i]) && isClicking)
		{
			//Set point to burning
			m_pSpheres[i]->ApplyForce(Wind * m_pSpheres[i]->GetMass() * m_fWindStrength);
		}
	}
	
	
	
}
//Set the width of the cloth
void CGameManager::SetClothWidth(int _size)
{
	width = _size;
}
//Set the height of the cloth
void CGameManager::SetClothHeight(int _size)
{
	height = _size;
}
//Set anchor size
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
void CGameManager::SpecialDown(int key, int x, int y)
{
	m_pInput->SpecialDown(key, x, y);
}
void CGameManager::SpecialUp(int key, int x, int y)
{
	m_pInput->SpecialUp(key, x, y);
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
//Get ray from mouse
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
	float radius = _object->GetObjSize().x *10 * m_fWindSize;

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
	else
	{
		return false;
	}
}
