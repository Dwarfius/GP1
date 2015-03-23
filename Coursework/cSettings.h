#pragma once
class cSettings
{
private:
	bool drawBackground = true;
	float volume = 0.5f;
	int scores[10];

	cSettings();
	static cSettings *singleton;

	bool Deserialize();

public:
	~cSettings() {}

	void ToggleDrawBackground() { drawBackground = !drawBackground; }
	bool GetDrawBackground() { return drawBackground; }

	void SetVolume(float pVolume) { volume = pVolume; }
	float GetVolume() { return volume; }

	void Serialize();

	static cSettings* Get() { if (!singleton) singleton = new cSettings(); return singleton; }
};

