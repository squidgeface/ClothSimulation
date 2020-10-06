// (c) 2020 Alexander Jenkins
//
// File Name   : time.h
// Description : Time declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "Utils.h"

class CTime
{

public:

	CTime();
	~CTime();

	void InitiateTime();

	void UpdateTime();

	float GetDelta();

	float GetTime();

private:
	CTime(const CTime& _kr);
	
protected:
	//variables
	double m_fTimeElapsed, m_fDeltaTime, m_fLastTime, m_fCurrentTime, m_fSecondsPerCount;
	int m_iFrameCount = 0;
};
