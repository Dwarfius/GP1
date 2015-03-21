#include "cSound.h"
#include "cSettings.h"

#pragma warning ( disable : 4996 )

void cSound::LoadWAVFile(LPCSTR filename)
{
	alutLoadWAVFile((ALbyte *)filename, &m_OALFormat, (void **)&m_OALData, (ALsizei *)&m_OALBufferLen, &m_OALFrequency, &m_OALLoop);

	alGenSources(1, &m_OALSource);
	alGenBuffers(1, &m_OALBuffer);
	alBufferData(m_OALBuffer, m_OALFormat, m_OALData, m_OALBufferLen, m_OALFrequency);
	alSourcei(m_OALSource, AL_BUFFER, m_OALBuffer);

	alutUnloadWAV(m_OALFormat, m_OALData, m_OALBufferLen, m_OALFrequency);
}

void cSound::Play(ALboolean loop)
{
	alSourcef(m_OALSource, loop, cSettings::Get()->GetVolume());
	alSourcePlay(m_OALSource);
}

void cSound::Stop()
{
	alSourceStop(m_OALSource);
}

void cSound::CleanUp()
{
	int state;
	alGetSourcei(m_OALSource, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING)
		Stop();

	alDeleteSources(1, &m_OALSource);
	alDeleteBuffers(1, &m_OALBuffer);
	alcMakeContextCurrent(NULL);

	delete m_OALData;
}
