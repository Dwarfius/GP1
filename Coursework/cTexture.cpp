#include "cTexture.h"

cTexture::cTexture()
{
	cTexture::GLTextureID = NULL;
}

cTexture::cTexture(LPCSTR theFilename)
{
	cTexture::createTexture(theFilename);
}

cTexture::~cTexture()
{
	ilDeleteImages(1, &ilTextureID);
}

bool cTexture::createTexture(LPCSTR theFilename) 	// create the texture for use.
{
	ILboolean success = false;

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		return false;
	}

	ilInit();  /*Initialize the DevIL library*/
	ilGenImages(1, &ilTextureID); //Generate DevIL image objects
	ilBindImage(ilTextureID); /* Binding of image object */
	success = ilLoadImage((const ILstring)theFilename); /* Loading of image*/

	if (!success)
	{
		ilDeleteImages(1, &ilTextureID);
		return false;
	}

	success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); // Convert every colour component into unsigned byte.
	if (!success)
	{
		return false;
	}

	textureWidth = ilGetInteger(IL_IMAGE_WIDTH);
	textureHeight = ilGetInteger(IL_IMAGE_HEIGHT);
	txData = new byte[textureWidth * textureHeight * 4];
	memcpy(txData, ilGetData(), textureWidth * textureHeight * 4); //saving the pixels locally since we perform cleanup after this

	glGenTextures(1, &GLTextureID); // GLTexture name generation 
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear interpolation for magnification filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Use linear interpolation for minifying filter 
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	ilDeleteImages(1, &ilTextureID);

	return true;
}