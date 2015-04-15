#include "cSoundMgr.h"

//I've cleared the class
cSoundMgr::cSoundMgr()
{
	CreateContext();
}

cSoundMgr::~cSoundMgr()
{
	CleanUp();

	m_OALContext = alcGetCurrentContext();
	m_OALDevice = alcGetContextsDevice(m_OALContext);

	alcDestroyContext(m_OALContext);
	alcCloseDevice(m_OALDevice);
}

void cSoundMgr::CreateContext()
{
	m_OALDevice = alcOpenDevice(NULL);
	if (m_OALDevice)
	{
		m_OALContext = alcCreateContext(m_OALDevice, NULL);
		alcMakeContextCurrent(m_OALContext);
	}
}

void cSoundMgr::Add(string sndName, string fileName)
{
	cSound *newSnd = new cSound();
	newSnd->LoadWAVFile(fileName.c_str());
	sounds.insert(make_pair(sndName, newSnd));
}

cSound* cSoundMgr::GetSnd(string sndName)
{
	return sounds[sndName];
}

void cSoundMgr::CleanUp()
{
	for (auto snd = sounds.begin(); snd != sounds.end(); ++snd)
		delete snd->second;
	sounds.clear();
}
