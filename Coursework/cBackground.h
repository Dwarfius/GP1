#pragma once
#include "GameConstants.h"
#include "cGUIElement.h"

#define BG_LAYER_COUNT 3
#define BG_PART_COUNT 10000

class cBackground
{
private:
	cGUIElement *background;
	GLuint buffers[BG_LAYER_COUNT];
	glm::vec2 translation[BG_LAYER_COUNT];

	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

public:
	cBackground(cTexture *pTexture, int width, int height);
	~cBackground();

	void Update(glm::vec2 deltaMove);
	void Render();
	void UpdateSize(int width, int height);

	void GetFunctions();
};
