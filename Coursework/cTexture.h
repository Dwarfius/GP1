/*
=================
cTexture.h
- Header file for class definition - SPECIFICATION
- Header file for the cD3DXTexture class
=================
*/
#pragma once

#include "GameConstants.h"

class cTexture
{
private:
	GLuint GLTextureID;
	GLuint aTextureInfo;
	GLsizei textureWidth;
	GLsizei textureHeight;
	ILuint ilTextureID;
	byte *txData;

public:
	cTexture();
	cTexture(LPCSTR theFilename);
	~cTexture();
	bool createTexture(LPCSTR theFilename);		// create the texture for use.
	GLuint getTexture() { return GLTextureID; }
	GLsizei getTWidth() { return textureWidth; }
	GLsizei getTHeight() { return textureHeight; }
	ILubyte* GetData() { return txData; }
};