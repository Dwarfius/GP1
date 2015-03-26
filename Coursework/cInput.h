#pragma once

#include <Windows.h>
#include "glm\glm.hpp"
#include <Xinput.h>

#pragma comment(lib, "Xinput9_1_0.lib")

class cInput
{
private:
	static bool keys[2][256]; //keyboard
	static bool buttons[2][2]; //mouse
	static glm::vec2 mPos;

	//controller
	static float lX, lY, rX, rY; //left/right sticks
	static float lT, rT; //left/right triggers
	static bool controller[10]; //remaining buttons
	static float resetPollingTimer;
	static int msgInd;

	cInput() {}
	~cInput() {}
public:
	static void Update(float delta);
	static void Reset();

	static void SetKey(short key, bool state) { keys[0][key] = state; }
	static void SetButton(short btn, bool state) { buttons[0][btn] = state; }
	static void SetMousePos(int x, int y) { mPos = glm::vec2(x, y); }

	//keyboard
	static bool GetKey(short key) { return keys[0][key]; };
	static bool GetKeyDown(short key) { return keys[0][key] && !keys[1][key]; };
	static bool GetKeyUp(short key) { return !keys[0][key] && keys[1][key]; };

	static bool GetAnyKey();
	static bool GetAnyKeyDown();

	//mouse
	static bool GetButton(short btn) { return buttons[0][btn]; }
	static bool GetButtonDown(short btn) { return buttons[0][btn] && !buttons[1][btn]; }
	static bool GetButtonUp(short btn) { return !buttons[0][btn] && buttons[1][btn]; }
	static glm::vec2 GetMousePos() { return mPos; }

	//controller

};

