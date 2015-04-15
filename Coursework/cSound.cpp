#include "cSound.h"
#include "cSettings.h"

#pragma warning ( disable : 4996 )

void cSound::LoadWAVFile(LPCSTR filename)
{
	char *buffer;
	alutLoadWAVFile((ALbyte *)filename, &m_OALFormat, (void **)&buffer, (ALsizei *)&m_OALBufferLen, &m_OALFrequency, &m_OALLoop);

	alGenSources(1, &m_OALSource); 
	alGenBuffers(1, &m_OALBuffer); //create a buffer for the sound
	alBufferData(m_OALBuffer, m_OALFormat, buffer, m_OALBufferLen, m_OALFrequency); //load it up
	alSourcei(m_OALSource, AL_BUFFER, m_OALBuffer);

	alutUnloadWAV(m_OALFormat, buffer, m_OALBufferLen, m_OALFrequency); //free the memory since it's cached in buffer
}

void cSound::Play(ALboolean loop)
{
	float volume = cSettings::Get()->GetVolume(); //no need to call to play the sound if volume is 0
	if (volume > 0.05f) //floating point issues
	{
		alSourcei(m_OALSource, AL_LOOPING, loop); //setting the looping
		alSourcef(m_OALSource, AL_GAIN, volume); //setting the value
		alSourcePlay(m_OALSource);
	}
}

void cSound::Stop()
{
	alSourceStop(m_OALSource);
}

void cSound::CleanUp()
{
	Stop(); //stop if already running

	alDeleteSources(1, &m_OALSource);
	alDeleteBuffers(1, &m_OALBuffer);
	alcMakeContextCurrent(NULL);
}
