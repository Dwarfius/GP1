#pragma once

#include "GameConstants.h"
#include <string>
#include <iostream>
#include <fstream>

class cSound
{
private:

	char*        m_OALData = NULL;           //data for the buffer
	ALenum       m_OALFormat;         //buffer format
	ALsizei      m_OALFrequency = 0;     //frequency
	long         m_OALBufferLen;      //bit depth
	float        m_OALLength = 0;		  // Length
	ALboolean    m_OALLoop;           //loop
	unsigned int m_OALSource;         //source
	ALuint       m_OALBuffer = 0;         // Buffer

public:
	cSound() {}
	~cSound() { CleanUp(); }

	void LoadWAVFile(LPCSTR filename);
	void Play(ALboolean loop);
	void Stop();
	void CleanUp();
};