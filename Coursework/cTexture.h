#pragma once

#include "GameConstants.h"

class cTexture
{
private:
	char* txData;
	GLuint GLTextureID;
	GLuint aTextureInfo;
	GLsizei textureWidth;
	GLsizei textureHeight;
	ILuint ilTextureID;

public:
	cTexture();
	cTexture(LPCSTR theFilename);
	~cTexture();
	bool createTexture(LPCSTR theFilename);		// create the texture for use.
	GLuint getTexture() { return GLTextureID; }
	GLsizei getTWidth() { return textureWidth; }
	GLsizei getTHeight() { return textureHeight; }
	char* GetData() { return txData; }

	void PrintOut(int channel);
};