#pragma once
class cSettings
{
private:
	bool drawBackground;
	float volume;
	int scores[10];

	cSettings();
	static cSettings *singleton;

	void Deserialize();
	void SwapDown(int val, int index);

public:
	~cSettings() {}

	void ToggleDrawBackground() { drawBackground = !drawBackground; }
	bool GetDrawBackground() { return drawBackground; }

	void SetVolume(float pVolume) { volume = pVolume; }
	float GetVolume() { return volume; }

	void AddScore(int score);
	int* GetScores() { return scores; }
	void ClearScores();

	void Serialize();

	static cSettings* Get() { if (!singleton) singleton = new cSettings(); return singleton; }
};

