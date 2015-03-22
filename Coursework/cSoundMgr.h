#pragma once

#include "GameConstants.h"
#include "cSound.h"

class cSoundMgr
{
private:
	ALCcontext *m_OALContext;
	ALCdevice *m_OALDevice;

	map <string, cSound*> sounds;
	void CreateContext();

	void CleanUp();

public:
	cSoundMgr();
	~cSoundMgr();

	void Add(string sndName, string fileName);
	cSound* GetSnd(string sndName);
};