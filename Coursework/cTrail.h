#pragma once
#include "GameConstants.h"

#define MAX_NODES 100

struct sTrailNode
{
	float x, y, life;
};

class cTrail
{
private:
	sTrailNode nodes[MAX_NODES];
	GLuint vbo;
	GLuint vao;

	static PFNGLGENBUFFERSPROC glGenBuffers;
	static PFNGLBINDBUFFERPROC glBindBuffer;
	static PFNGLBUFFERDATAPROC glBufferData;
	static PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
	static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	static PFNGLSHADERSOURCEPROC glShaderSource;
	static PFNGLCREATEPROGRAMPROC glCreateProgram;
	static PFNGLUSEPROGRAMPROC glUseProgram;
	static PFNGLLINKPROGRAMPROC glLinkProgram;
	static PFNGLCREATESHADERPROC glCreateShader;
	static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	static PFNGLCOMPILESHADERPROC glCompileShader;
	static PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	static PFNGLATTACHSHADERPROC glAttachShader;
	static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	static GLuint shaderProgram;

public:
	cTrail();
	~cTrail() {}

	void Update(float deltaTime);
	void Render();

	void Add(glm::vec2 pos);

	static void GetFunctions();
};

