// (c) 2020 Alexander Jenkins
//
// File Name   : main.cpp
// Description : main implementaion file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "gameManager.h"
//#include "vld.h"

//Global class calls
CGameManager* GameManager;

//Prototype functions
void KeyboardDown(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void MouseClick(int button, int state, int x, int y);
void MouseMove(int x, int y);
void Render();
void Update();

int main(int argc, char **argv)
{
	GameManager = new CGameManager();

	//Initialise gamewindow
	GameManager->InitialiseWindow(argc, argv);

	//initialise class pointers, vertecies and textures
	GameManager->InitialiseMenu();

	//Register callbacks
	glutDisplayFunc(Render);
	glutIdleFunc(Update);
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutMotionFunc(MouseMove);
	glutPassiveMotionFunc(MouseMove);
	glutMouseFunc(MouseClick);
	glutMainLoop();

	delete GameManager;
	GameManager = 0;
	
	return 0;
}

//Protorype callbacks
void Render()
{
		GameManager->Render();
}
void Update()
{
	GameManager->Update();	
}
void KeyboardDown(unsigned char key, int x, int y)
{
	GameManager->KeyboardDown(key, x, y);
}
void KeyboardUp(unsigned char key, int x, int y)
{
	GameManager->KeyboardUp(key, x, y);
}
void MouseClick(int button, int state, int x, int y)
{
	GameManager->MouseClick(button, state, x, y);
}
void MouseMove(int x, int y)
{
	GameManager->MouseMove(x, y);
}



