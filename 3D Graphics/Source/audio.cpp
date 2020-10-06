// (c) 2020 Alexander Jenkins
//
// File Name   : audio.cpp
// Description : audio implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "audio.h"

CAudio::CAudio()
	:m_pAudioSystem(0)
{
}
CAudio::~CAudio()
{
}
//iniitalise audio sound system
bool CAudio::InitialiseSoundSystem()
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&m_pAudioSystem);
	if (result != FMOD_OK)
		return false;

	result = m_pAudioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK)
		return false;

	return true;
}
//play sounds
void CAudio::PlaySounds(Sound* holder, bool bLoop)
{

	FMOD_RESULT result;
	result = m_pAudioSystem->playSound(holder, 0, false, 0);

	//if loop is true, loop sound
	if (bLoop)
	{
		holder->setMode(FMOD_LOOP_NORMAL);
	}
}
//return audiosystem for update
System* CAudio::GetAudioSystem()
{
	return m_pAudioSystem;
}


