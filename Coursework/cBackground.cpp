#include "cBackground.h"
#include "cSettings.h"

#pragma warning(disable : 4244)

cBackground::cBackground(cTexture *pTexture, int width, int height)
{
	GetFunctions();

	background = new cGUIElement(pTexture, { 0, 0, width, height });

	glGenBuffers(BG_LAYER_COUNT, buffers);
	int buffer[BG_PART_COUNT * 3];
	for (int i = 0; i < BG_LAYER_COUNT; i++)
	{
		translation[i] = glm::vec2(0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);

		for (int j = 0; j < BG_PART_COUNT * 3; j++)
			buffer[j] = j % 3 == 2 ? 0 : (rand() % 50000 - 25000);

		glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);
	}
}

cBackground::~cBackground()
{
	glDeleteBuffers(3, buffers);
	delete background;
}

void cBackground::Update(glm::vec2 deltaMove)
{
	for (int i = 0; i < BG_LAYER_COUNT; i++)
		translation[i] += -deltaMove * (float)(i + 1);
}

void cBackground::Render()
{
	glMatrixMode(GL_MODELVIEW);

	//first of all, the background
	if (cSettings::Get()->GetDrawBackground())
		background->Render();

	glColor3f(1, 1, 1);
	//then all the stars
	for (int i = 0; i < BG_LAYER_COUNT; i++)
	{
		glPushMatrix();

		glTranslatef(translation[i].x, translation[i].y, 0);
		glPointSize(i + 1);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		//http://ogldev.atspace.co.uk/www/tutorial02/tutorial02.html
		glEnableVertexAttribArray(0); //turns out you have to bind a buffer before enabling attribs
		glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, 0, 0); //thanks documentation
		glDrawArrays(GL_POINTS, 0, BG_PART_COUNT);
		glDisableVertexAttribArray(0);
		glPopMatrix();
	}
}

void cBackground::UpdateSize(int width, int height)
{
	background->SetRect({ 0, 0, width, height });
}

//since for some reason I can't get VS to recognize the GLEW lib, I'll have to do it manually
//http://stackoverflow.com/questions/16390078/build-error-when-trying-to-run-an-opengl-example-in-vc
void cBackground::GetFunctions()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
}