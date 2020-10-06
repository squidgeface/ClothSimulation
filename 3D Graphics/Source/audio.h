// (c) 2020 Alexander Jenkins
//
// File Name   : audio.h
// Description : audio declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "Utils.h"

class CAudio
{
public:
	CAudio();
	~CAudio();
	bool InitialiseSoundSystem();
	void PlaySounds(Sound* holder, bool bLoop = false);
	System* GetAudioSystem();

protected:
	//audio system
	System* m_pAudioSystem;

};