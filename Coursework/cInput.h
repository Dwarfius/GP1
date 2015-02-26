#pragma once

#include <Windows.h>

class cInput
{
private:
	static bool keys[2][256];
	cInput() {}
	~cInput() {}
public:
	static void Setup() { memset(keys, 0, sizeof(keys)); }
	static void Update() { memcpy(keys[1], keys[0], sizeof(keys[0])); }
	static void SetKey(short key, bool state) { keys[0][key] = state; }

	static bool GetKey(short key) { return keys[0][key]; };
	static bool GetKeyDown(short key) { return keys[0][key] && !keys[1][key]; };
	static bool GetKeyUp(short key) { return !keys[0][key] && keys[1][key]; };
};

