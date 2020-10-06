// (c) 2020 Alexander Jenkins
//
// File Name   : time.cpp
// Description : Time implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "time.h"

CTime::CTime()
	: m_fTimeElapsed(0.0)
	, m_fDeltaTime(0.0)
	, m_fLastTime(0.0)
	, m_fCurrentTime(0.0)
{	
}

CTime::~CTime()
{
}
//initialise previous timestamp
void CTime::InitiateTime()
{
	__int64 _TimerFrequency, _currTime;
	QueryPerformanceFrequency((LARGE_INTEGER*)&_TimerFrequency);
	m_fSecondsPerCount = 1.0 / static_cast<double>(_TimerFrequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&_currTime);
	m_fCurrentTime = static_cast<double>(_currTime);
	m_fLastTime = static_cast<double>(_currTime);
}
//update time variables
void CTime::UpdateTime()
{
	//Get the time this frame.
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_fCurrentTime = static_cast<double>(currTime);
	//Time difference between this frame and the previous frame
	m_fDeltaTime = (m_fCurrentTime - m_fLastTime) * m_fSecondsPerCount;
	//Prepare for the next frame
	m_fLastTime = m_fCurrentTime;

	//Force non-negative
	if (m_fDeltaTime < 0.0)
	{
		m_fDeltaTime = 0.0;
	}
	//increment time elapsed
	m_fTimeElapsed += m_fDeltaTime;
}
//get delta time
float CTime::GetDelta()
{
	return static_cast<float>(m_fDeltaTime);
}
//get time elapsed
float CTime::GetTime()
{
	return static_cast<float>(m_fTimeElapsed);
}
