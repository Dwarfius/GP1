#pragma once

#include "GameConstants.h"
#include "cSound.h"

class cSoundMgr
{
private:
	ALCcontext *m_OALContext;
	ALCdevice *m_OALDevice;

	cSoundMgr();
	~cSoundMgr();
	
	map <string, cSound*> sounds;
	void CreateContext();

public:
	void Add(string sndName, string fileName);
	cSound* GetSnd(string sndName);
	void CleanUp();
};