#pragma once
class cSettings
{
private:
	bool drawBackground = true;
	float volume = 0.5f;

	cSettings() {}
	static cSettings *singleton;

public:
	~cSettings() {}

	void ToggleDrawBackground() { drawBackground = !drawBackground; }
	bool GetDrawBackground() { return drawBackground; }

	void SetVolume(float pVolume) { volume = pVolume; }
	float GetVolume() { return volume; }

	static cSettings* Get() { if (!singleton) singleton = new cSettings(); return singleton; }
};

