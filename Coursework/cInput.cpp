#include "cInput.h"
#include "GameConstants.h"

bool cInput::keys[2][256];
bool cInput::buttons[2][2];
glm::vec2 cInput::mPos;
float cInput::lXN = 0, cInput::lYN = 0, cInput::rXN = 0, cInput::rYN = 0;
bool cInput::lT = false, cInput::rT = false;
bool cInput::controller[12] = {};
float cInput::resetPollingTimer = 0;
int cInput::msgInd = -1;

void cInput::Update(float delta)
{
	memcpy(keys[1], keys[0], sizeof(keys[0]));
	memcpy(buttons[1], buttons[0], sizeof(buttons[0]));

	//https://msdn.microsoft.com/en-us/library/windows/desktop/ee417001%28v=vs.85%29.aspx
	if (resetPollingTimer -= delta <= 0)
	{
		DWORD res;
		XINPUT_STATE state;
		memset(&state, 0, sizeof(state));
		res = XInputGetState(0, &state);
		if (res == ERROR_SUCCESS)
		{
			if (msgInd != state.dwPacketNumber)
				Parse(state.Gamepad);
			msgInd = state.dwPacketNumber;
		}
		else
		{
			resetPollingTimer = 2;
		}
	}
}

void cInput::Reset()
{
	memset(keys, 0, sizeof(keys));
	memset(buttons, 0, sizeof(buttons));
}

bool cInput::GetAnyKey()
{
	for (int i = 0; i < 256; i++)
	{
		if (GetKey(i))
			return true;
	}
	return false;
}

bool cInput::GetAnyKeyDown()
{
	for (int i = 0; i < 256; i++)
	{
		if (GetKeyDown(i))
			return true;
	}
	return false;
}

void cInput::Parse(XINPUT_GAMEPAD &pad)
{
	//left stick movement
	float x = pad.sThumbLX;
	float y = pad.sThumbLY;
	
	float mag = sqrt(x * x + y * y);
	if (mag > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		lXN = x / 32767;
		lYN = y / 32767;
	}
	else
		lXN = lYN = 0;

	//now the right stick
	x = pad.sThumbRX;
	y = pad.sThumbRY;

	mag = sqrt(x*x + y*y);
	if (mag > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		rXN = x / 32767;
		rYN = y / 32767;
	}
	else
		rXN = rYN = 0;

	//triggers
	byte triggerPressure = pad.bLeftTrigger;
	if (triggerPressure > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		triggerPressure -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		lT = triggerPressure > 60;
	}
	else
		lT = false;

	triggerPressure = pad.bRightTrigger;
	if (triggerPressure > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		triggerPressure -= XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
		rT = triggerPressure > 60;
	}
	else
		rT = false;

	//key pressses
	controller[(int)GamepadKeys::Up] = ((pad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0);
	controller[(int)GamepadKeys::Down] = ((pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0);
	controller[(int)GamepadKeys::Left] = ((pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0);
	controller[(int)GamepadKeys::Right] = ((pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0);
	controller[(int)GamepadKeys::X] = ((pad.wButtons & XINPUT_GAMEPAD_X) != 0);
	controller[(int)GamepadKeys::Y] = ((pad.wButtons & XINPUT_GAMEPAD_Y) != 0);
	controller[(int)GamepadKeys::A] = ((pad.wButtons & XINPUT_GAMEPAD_A) != 0);
	controller[(int)GamepadKeys::B] = ((pad.wButtons & XINPUT_GAMEPAD_B) != 0);
	controller[(int)GamepadKeys::Start] = ((pad.wButtons & XINPUT_GAMEPAD_START) != 0);
	controller[(int)GamepadKeys::Back] = ((pad.wButtons & XINPUT_GAMEPAD_BACK) != 0);
	controller[(int)GamepadKeys::LBumper] = ((pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0);
	controller[(int)GamepadKeys::RBumper] = ((pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0);
}