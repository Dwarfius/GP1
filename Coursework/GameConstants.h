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
//#include "OpenAL\al.h"
//#include "OpenAL\alc.h"
//#include "OpenAL\alut.h"

// Maths functions
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\geometric.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtx\quaternion.hpp"

// STL Container & Algorithms
#include <vector>
#include <map>
#include <algorithm>

// Textures & Sprites
#include "cTexture.h"
#include "cSprite.h"
#include "cGameObject.h"

using namespace std;

//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "Space Arcade Game"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768