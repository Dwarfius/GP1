#include "cSettings.h"
#include <iostream>
#include <fstream>
#include <string>

#pragma warning(disable : 4244)

using namespace std;

cSettings* cSettings::singleton = 0;

cSettings::cSettings()
{
	//basic settings
	drawBackground = true; 
	volume = 0.5f;
	memset(scores, 0, sizeof(scores));
	Deserialize(); //attempt to get up to date settings
}

void cSettings::Serialize()
{
	fstream file("settings.txt", ios::out | ios::trunc); //open up the file for clean write

	file << to_string(drawBackground) << endl; //write down the settings
	file << to_string(volume) << endl;
	for (int i = 0; i < 10 && scores[i] != 0; i++) //don't write out end zeroes
		file << to_string(scores[i]) << endl;
	file.close();
}

void cSettings::Deserialize()
{
	fstream file("settings.txt", ios::in); //open up for reading
	if (!file.is_open()) //if doesn't exist, fall back to basic settings
		return;

	string line; //otherwise, parse it
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
}

void cSettings::AddScore(int score) //add score while still keeping it sorted
{
	for (int i = 0; i < 10; i++)
	{
		if (score > scores[i])
		{
			SwapDown(scores[i], i + 1);
			scores[i] = score;
			Serialize();
			return;
		}
	}
}

//utility - recursion to push scores down
void cSettings::SwapDown(int val, int index)
{
	if (index == 10 || val == 0)
		return;

	SwapDown(scores[index], index + 1);
	scores[index] = val;
}

//resetting of scores and writing them down
void cSettings::ClearScores()
{
	memset(scores, 0, sizeof(scores));
	Serialize();
}