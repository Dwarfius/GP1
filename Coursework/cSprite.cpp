#include "cSprite.h"

cSprite::cSprite()
{
	pos.x = 0;
	pos.y = 0;
	scale.x = scale.y = 1;
	rotation = 0;
}

cSprite::~cSprite()
{
}

void cSprite::render()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextureID); // Binding of GLtexture name 

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(transform));

	glBegin(GL_QUADS);
		glColor3f(255.0f, 255.0f, 255.0f); //flush with white
		glTexCoord2i(0, 0); //providing uv mapping
		glVertex2i(-textureWidth / 2,  -textureHeight / 2);
		glTexCoord2i(1, 0);
		glVertex2i(textureWidth / 2, -textureHeight / 2);
		glTexCoord2i(1, 1);
		glVertex2i(textureWidth / 2, textureHeight / 2);
		glTexCoord2i(0, 1);
		glVertex2i(-textureWidth / 2, textureHeight/2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cSprite::setTextureDimensions(int texWidth, int textHeight)
{
	textureWidth = texWidth;
	textureHeight = textHeight;
}

//call at the start of the update
void cSprite::updateMatrix()
{
	transform = glm::mat4x4(1);
	transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0));
	transform = glm::rotate(transform, rotation, glm::vec3(0, 0, 1));
	transform = glm::scale(transform, glm::vec3(scale.x, scale.y, 0));
}

//call after the updateMatrix
void cSprite::updateBoundingRect()
{
	glm::vec4 points[4] = { glm::vec4(0, 0, 0, 0), glm::vec4(textureWidth, 0, 0, 0),
		glm::vec4(textureWidth, textureHeight, 0, 0), glm::vec4(0, textureHeight, 0, 0) };
	for (int i = 0; i < 4; i++)
		points[i] = transform * points[i];

	boundingRect.left = (int)points[0].x;
	boundingRect.bottom = (int)points[0].y;
	boundingRect.right = (int)points[0].x;
	boundingRect.top = (int)points[0].y;

	for (int i = 1; i < 4; i++)
	{
		if (boundingRect.left >(int)points[i].x)
			boundingRect.left = (int)points[i].x;
		if (boundingRect.top < (int)points[i].y)
			boundingRect.top = (int)points[i].y;

		if (boundingRect.bottom >(int)points[i].y)
			boundingRect.bottom = (int)points[i].y;
		if (boundingRect.right < (int)points[i].x)
			boundingRect.right = (int)points[i].x;
	}
}
