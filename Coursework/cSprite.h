/*
=================
cSprite.h
- Header file for class definition - SPECIFICATION
- Header file for the Sprite class
=================
*/
#pragma once

#include "GameConstants.h"

class cSprite
{
private:
	// position, rotation, scalling
	glm::vec2 pos;
	glm::vec2 scale;
	float rotation;
	glm::mat4x4 transform;
	RECT boundingRect;

	// Texture width & Height
	int textureWidth;
	int textureHeight;
	GLuint GLTextureID;

public:
	cSprite();
	~cSprite();

	glm::vec2 getSpritePos() { return pos; }
	void setSpritePos(glm::vec2 sPosition) { pos = sPosition; }

	glm::vec2 getSpriteScale() { return scale; }
	void setSpriteScale(glm::vec2 sScale) { scale = sScale; }

	float getSpriteRotation() { return rotation; }
	void setSpriteRotation(float sRotation) { rotation = sRotation; }

	RECT getSpriteBoundingRect() { return boundingRect; }

	void setTexture(GLuint GLtexID) { GLTextureID = GLtexID; }
	void setTextureDimensions(int texWidth, int textHeight);
	void updateMatrix();
	void updateBoundingRect();
	void render();
};