#include "cTrail.h"

PFNGLGENBUFFERSPROC cTrail::glGenBuffers;
PFNGLBINDBUFFERPROC cTrail::glBindBuffer;
PFNGLBUFFERDATAPROC cTrail::glBufferData;
PFNGLDELETEBUFFERSPROC cTrail::glDeleteBuffers;
PFNGLENABLEVERTEXATTRIBARRAYPROC cTrail::glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC cTrail::glVertexAttribPointer;
PFNGLDISABLEVERTEXATTRIBARRAYPROC cTrail::glDisableVertexAttribArray;
PFNGLSHADERSOURCEPROC cTrail::glShaderSource;
PFNGLCREATEPROGRAMPROC cTrail::glCreateProgram;
PFNGLUSEPROGRAMPROC cTrail::glUseProgram;
PFNGLLINKPROGRAMPROC cTrail::glLinkProgram;
PFNGLCREATESHADERPROC cTrail::glCreateShader;
PFNGLBINDVERTEXARRAYPROC cTrail::glBindVertexArray;
PFNGLCOMPILESHADERPROC cTrail::glCompileShader;
PFNGLGETATTRIBLOCATIONPROC cTrail::glGetAttribLocation;
PFNGLATTACHSHADERPROC cTrail::glAttachShader;
PFNGLGENVERTEXARRAYSPROC cTrail::glGenVertexArrays;
GLuint cTrail::shaderProgram;

cTrail::cTrail()
{
	memset(nodes, 0, sizeof(nodes));

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
}

void cTrail::Add(glm::vec2 pos)
{
	for (int i = MAX_NODES - 1; i > 0; i--) //reverse swap, so that the last one becomes first and everything else is shifted
	{
		sTrailNode tmp = nodes[i];
		nodes[i] = nodes[i - 1];
		nodes[i - 1] = tmp;
	}
	//now we can freely overwire it
	nodes[0].x = pos.x;
	nodes[0].y = pos.y;
	nodes[0].life = 5;
}

void cTrail::Update(float deltaTime)
{
	for (int i = 0; i < MAX_NODES; i++)
		nodes[i].life -= deltaTime;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(nodes), nodes, GL_DYNAMIC_DRAW);
}

void cTrail::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(shaderProgram);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);

	//enabling the position for vertex shader
	GLuint posAttrib = glGetAttribLocation(shaderProgram, "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	//enabling the life timer for frag shader
	GLuint lifeAttrib = glGetAttribLocation(shaderProgram, "life");
	glEnableVertexAttribArray(lifeAttrib);
	glVertexAttribPointer(lifeAttrib, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));

	glDrawArrays(GL_POINTS, 0, MAX_NODES);

	glDisableVertexAttribArray(posAttrib);
	glDisableVertexAttribArray(lifeAttrib);

	glUseProgram(0);

	glPopMatrix();
}

#define GLSL(src) "#version 150 core\n" #src
void cTrail::GetFunctions()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");

	//https://open.gl/geometry
	//first of all, the shader code
	const char *vertShaderSrc = GLSL(
		in vec2 pos;
		in float life;

		out float vLife;
		
		void main()
		{
			gl_Position = vec4(pos, 0.0, 1.0);
			vLife = life;
		}
	);

	const char *fragShaderSrc = GLSL(
		in float vLife;
		
		out vec4 outColor;
		
		void main()
		{
			outColor = vec4(0.0, 1.0, 0.0, vLife / 5.0); //just blue trail
		}
	);

	const char *geomShaderSrc = GLSL(
		layout(points) in;
		layout(triangle_strip, max_vertices = 6) out;

		in float vLife[]; //per vertex life

		void main()
		{
			float size = vLife[0];
			if (size > 0) //if the node is still alive
			{
				//first triangle
				//top left
				gl_Position = gl_in[0].gl_Position + vec4(-size, -size, 0, 0);
				EmitVertex();

				//top right
				gl_Position = gl_in[0].gl_Position + vec4(size, -size, 0, 0);
				EmitVertex();

				//bottom left
				gl_Position = gl_in[0].gl_Position + vec4(-size, size, 0, 0);
				EmitVertex();
				EndPrimitive();

				//second triangle
				//top right
				gl_Position = gl_in[0].gl_Position + vec4(size, -size, 0, 0);
				EmitVertex();

				//bottom right
				gl_Position = gl_in[0].gl_Position + vec4(size, size, 0, 0);
				EmitVertex();

				//bottom left
				gl_Position = gl_in[0].gl_Position + vec4(-size, size, 0, 0);
				EmitVertex();
				EndPrimitive();
			}
		}
	);

	//compiling them
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertShaderSrc, nullptr);
	glCompileShader(vertShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSrc, nullptr);
	glCompileShader(fragShader);

	GLuint geomShader = glCreateShader(0x8DD9); //for some reason I have GL_GEOMETRY_SHADER undefined, so
	glShaderSource(geomShader, 1, &geomShaderSrc, nullptr); //used this from https://www.opengl.org/registry/oldspecs/glext.h
	glCompileShader(geomShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glAttachShader(shaderProgram, geomShader);
	glLinkProgram(shaderProgram);
}
