#include "cSettings.h"
#include <string>
#include <iostream>
#include <fstream>

#pragma warning(disable : 4244)

using namespace std;

cSettings* cSettings::singleton = 0;

cSettings::cSettings()
{
	if (!Deserialize())
	{
		drawBackground = true;
		volume = 0.5f;
		memset(scores, 0, sizeof(scores));
	}
}

void cSettings::Serialize()
{
	ofstream file("settings.txt", ios::trunc | ios::app);
	if (!file.is_open())
		return;

	file << to_string(drawBackground) << endl;
	file << to_string(volume) << endl;
	for (int i = 0; i < 10 && scores[i] != 0; i++) //don't write out end zeroes
		file << to_string(scores[i]) << endl;
	file.close();
}

bool cSettings::Deserialize()
{
	ifstream file("settings.txt");
	if (!file.is_open())
		return false;

	string line;
	int i = 0;
	while (getline(file, line))
	{
		if (i == 0)
			drawBackground = line == "1";
		else if (i == 1)
			volume = atof(line.c_str());
		else
			scores[i - 2] = atoi(line.c_str());
		i++;
	}
	file.close();
	return true;
}