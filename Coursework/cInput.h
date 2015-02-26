#pragma once

#include <Windows.h>
#include "glm\glm.hpp"

class cInput
{
private:
	static bool keys[2][256];
	static bool buttons[2][2];
	static glm::vec2 mPos;

	cInput() {}
	~cInput() {}
public:
	static void Update();
	static void SetKey(short key, bool state) { keys[0][key] = state; }
	static void SetButton(short btn, bool state) { buttons[0][btn] = state; }
	static void SetMousePos(int x, int y) { mPos = glm::vec2(x, y); }

	static bool GetKey(short key) { return keys[0][key]; };
	static bool GetKeyDown(short key) { return keys[0][key] && !keys[1][key]; };
	static bool GetKeyUp(short key) { return !keys[0][key] && keys[1][key]; };

	static bool GetButton(short btn) { return buttons[0][btn]; }
	static bool GetButtonDown(short btn) { return buttons[0][btn] && !buttons[1][btn]; }
	static bool GetButtonUp(short btn) { return !buttons[0][btn] && buttons[1][btn]; }

	static glm::vec2 GetMousePos() { return mPos; }
};

