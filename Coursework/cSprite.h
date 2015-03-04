#pragma once

#include "GameConstants.h"
#include "cTexture.h"

class cSprite
{
private:
	// position, rotation, scalling
	glm::vec2 pos;
	glm::vec2 scale;
	float rotation;
	glm::mat4x4 transform;
	RECTF boundingRect;

	// Texture width & Height
	int textureWidth;
	int textureHeight;
	cTexture *texture;

public:
	cSprite();
	~cSprite();

	glm::vec2 getSpritePos() { return pos; }
	void setSpritePos(glm::vec2 sPosition) { pos = sPosition; }

	glm::vec2 getSpriteScale() { return scale; }
	void setSpriteScale(glm::vec2 sScale) { scale = sScale; }

	float getSpriteRotation() { return rotation; }
	void setSpriteRotation(float sRotation) { rotation = sRotation; }

	RECTF getSpriteBoundingRect() { return boundingRect; }

	char* GetData() { return texture->GetData(); }

	glm::mat4x4 GetTransform() { return transform; }

	glm::vec2 GetSize() { return glm::vec2(textureWidth, textureHeight); } //untransformed

	void setTexture(cTexture *pText) { texture = pText; }
	void setTextureDimensions(int texWidth, int textHeight);
	void updateMatrix();
	void updateBoundingRect();
	void render();
};