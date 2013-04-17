#ifndef __Timer_HPP__
#define __Timer_HPP__

#include <iostream>
#include <Windows.h>

#define NUM_FRAMES 10
typedef unsigned long long uint64_t;

class Timer
{
public:

	Timer(void);
	~Timer(void);

	void reset(void);
	void update(void);

	uint64_t getCPUCycles(void);
	uint64_t getCPUFrequency(void)	{ return m_freq; }

	double getDeltaTime(void)		{ return m_deltaTime; }
	double getSeconds(void)			{ return static_cast<double>(getCPUCycles()) * m_invFreq; }
	double getMilliseconds(void)	{ return static_cast<double>(getCPUCycles()) * (m_invFreq * 1000.0); }
	double getMicroseconds(void)	{ return static_cast<double>(getCPUCycles()) * (m_invFreq * 1000000.0); }
	double getNanoseconds(void)		{ return static_cast<double>(getCPUCycles()) * (m_invFreq * 1000000000.0); }

	int getFPS(void)	{ return m_FPS; }

private:

	uint64_t m_freq;
	double m_invFreq;
	uint64_t m_startCycles;
	unsigned long m_threadMask;

	double m_deltaTime;
	double m_frameTimes[ NUM_FRAMES ];
	double m_oldFrameTime;
	int m_totalNumFrames;
	int m_FPS;

	double m_updateFPS;
};

#endif // __Timer_HPP__