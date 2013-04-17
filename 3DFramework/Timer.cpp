#include "Timer.hpp"

Timer::Timer(void) :
	m_freq(0),
	m_invFreq(0),
	m_startCycles(0),
	m_threadMask(0),
	m_totalNumFrames(0),
	m_oldFrameTime(0)
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_freq));
	m_invFreq = 1.0 / static_cast<double>(m_freq);

	m_updateFPS = 0.0;
}

Timer::~Timer(void)
{
	//
}

void Timer::update(void) 
{
	double currentTime	= getSeconds();
	m_deltaTime			= currentTime - m_oldFrameTime;
	m_oldFrameTime		= currentTime;

	int FrameIndex	= m_totalNumFrames % NUM_FRAMES;
	m_frameTimes[FrameIndex] = (1.0f / m_deltaTime);
	m_totalNumFrames++;	

	//Update the fps counter, we don't need to do this each frame since that would affect the FPS in a bad way
	if( m_updateFPS > 0.05f )
	{
		m_updateFPS = 0.0f;
		//Update the fps counter
		int count = 0;
		if (m_totalNumFrames < NUM_FRAMES) {
			count = m_totalNumFrames;
		} else {
			count = NUM_FRAMES;
		}

        m_FPS = 0;
		for( int i = 0; i < count; i++ ) {
           m_FPS += m_frameTimes[i];
		}

        m_FPS/=count;
	}

	m_updateFPS += m_deltaTime;
}

void Timer::reset(void)
{
	unsigned long procMask, sysMask;
	GetProcessAffinityMask(GetCurrentProcess(), &procMask, &sysMask);

	m_threadMask = 1;
	while( ( m_threadMask & procMask ) == 0 )
	{
		m_threadMask <<= 1;
	}

	HANDLE currentThread = GetCurrentThread();
	unsigned long oldMask = SetThreadAffinityMask(currentThread, m_threadMask);
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_startCycles));
	SetThreadAffinityMask(currentThread, oldMask);
}

uint64_t Timer::getCPUCycles(void)
{
	uint64_t curCycles;
	HANDLE currentThread = GetCurrentThread();
	unsigned long oldMask = SetThreadAffinityMask(currentThread, m_threadMask);
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curCycles));
	SetThreadAffinityMask(currentThread, oldMask);

	return curCycles - m_startCycles;
}