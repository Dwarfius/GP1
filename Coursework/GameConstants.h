#pragma once

// Windows & OpenGL 
#include <stdlib.h>
#include <Windows.h>
#include "GL\glut.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

// Image Texture loading library
#include "IL\il.h"
#include "IL\ilu.h"
#include "IL\ilut.h"

// Audio
#include "OpenAL\al.h"
#include "OpenAL\alc.h"
#include "OpenAL\alut.h"

// Maths functions
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\geometric.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\rotate_vector.hpp"

// STL Container & Algorithms
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "Space Arcade Game"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

struct RECTF 
{
	float left, top, right, bottom;

	static bool Intersects(RECTF r1, RECTF r2)
	{
		return r2.left < r1.right &&
			r2.right > r1.left &&
			r2.top < r1.bottom &&
			r2.bottom > r1.top;
	}

	static bool InRect(RECTF r, glm::vec2 p)
	{
		return p.x > r.left && p.x < r.right && p.y > r.top && p.y < r.bottom;
	}

	string ToString()
	{
		return "(" + to_string((int)left) + ", " + to_string((int)top) + ", " + to_string((int)right) + ", " + to_string((int)bottom) + ")";
	}
};