#include "cGUI.h"
#include "GameConstants.h"
#include "cGUILabel.h"
#include "cGUIProgressBar.h"
#include "cGame.h"
#include "cInput.h"
#include "cSettings.h"

#pragma warning(disable: 4018 4244)

cGUI::cGUI(glm::vec2 pWindowSize, cTexture *pTutorialTexture)
{
	windowSize = pWindowSize;
	tutorialTexture = pTutorialTexture;
	SetUp();
}

cGUI::~cGUI()
{
	CleanUp();
}

void cGUI::UpdateSize(glm::vec2 newSize)
{
	windowSize = newSize;
	CleanUp();
	SetUp();
}

void cGUI::SetUp()
{
	SetUpMain();
	SetUpInstructions();
	SetUpOptions();
	SetUpGameOverlay();
	SetUpPause();
	SetUpPauseOptions();
	SetUpGlobalDataOverlay();
	SetMenu(currentMenu);
}

void cGUI::Update(float delta)
{
	//managing keyboard-gamepad gui navigation
	if (btns[(int)currentMenu].size() > 0)
	{
		bool updBtn = false;
		if (cInput::GetControllerKeyDown(GamepadKeys::Up) || cInput::GetKeyDown('W'))
		{
			btns[(int)currentMenu][activeBtn]->SetActive(false);
			activeBtn--;
			updBtn = true;
		}
		else if (cInput::GetControllerKeyDown(GamepadKeys::Down) || cInput::GetKeyDown('S'))
		{
			btns[(int)currentMenu][activeBtn]->SetActive(false);
			activeBtn++;
			updBtn = true;
		}
		if (updBtn)
		{
			if (activeBtn < 0)
				activeBtn = btns[(int)currentMenu].size() - 1;
			else if (activeBtn == btns[(int)currentMenu].size())
				activeBtn = 0;
			btns[(int)currentMenu][activeBtn]->SetActive(true);
		}
	}

	//menu-specific updates
	switch (currentMenu)
	{
	case Screen::GameOverlay:
	{
		//updating the bottom health bar
		cPlayer *player = cGame::Get()->GetPlayer();
		float health = player->GetHealth();
		float max = player->GetMaxHealth();
		cGUIProgressBar *bar = (cGUIProgressBar*)menus[(int)Screen::GameOverlay][0];
		bar->SetPercentage(health / max);
		bar->SetText(to_string((int)health) + "/" + to_string((int)max));

		//update the score - not efficient, but simple
		cGUILabel *lbl = (cGUILabel*)menus[(int)Screen::GameOverlay][1];
		lbl->SetText("Score: " + to_string(cGame::Get()->GetScore()));

		if (cInput::GetKeyDown(27) || cInput::GetControllerKeyDown(GamepadKeys::Start))
		{
			SetMenu(Screen::Pause);
			cGame::Get()->SetPaused(true);
		}
	}
	break;
	case Screen::Pause:
		if (cInput::GetKeyDown(27) || cInput::GetControllerKeyDown(GamepadKeys::B) || 
			cInput::GetControllerKeyDown(GamepadKeys::Start))
		{
			cGame::Get()->SetPaused(false);
			SetMenu(Screen::GameOverlay);
		}
		break;
	case Screen::Options:
		if (cInput::GetKeyDown(27) || cInput::GetControllerKeyDown(GamepadKeys::B))
		{
			SetMenu(Screen::Main);
			cSettings::Get()->Serialize();
		}
		break;
	case Screen::PauseOptions:
		if (cInput::GetKeyDown(27) || cInput::GetControllerKeyDown(GamepadKeys::B))
		{
			SetMenu(Screen::Pause);
			cSettings::Get()->Serialize();
		}
		break;
	case Screen::Instructions:
		if (cInput::GetAnyKeyDown())
			SetMenu(Screen::Main);
		break;
	default:
		break;
	}

	//fps calculation implementation - simplest stuff and smoothed
	timer += delta;
	if (timer > 1)
	{
		fps = frames;
		timer = 0;
		frames = 0;
	}

	//Global elements
	((cGUILabel*)menus[(int)Screen::Global][0])->SetText("FPS: " + to_string(fps));

	int count = cGame::Get()->GetGameObjetsCount();
	((cGUILabel*)menus[(int)Screen::Global][1])->SetText("Objects: " + to_string(count));

	//updating current menu items
	vector<cGUIElement *> menu = menus[(int)currentMenu];
	for (auto iter = menu.begin(); iter != menu.end(); iter++)
		(*iter)->Update();
}

void cGUI::Render(glm::vec2 offest)
{
	frames++; //counting the rendered frame
	
	//drawing the overlay in case we're in pause
	if (currentMenu == Screen::Pause || currentMenu == Screen::PauseOptions) //yeah, dirty, I know
		for (cGUIElement *elem : menus[(int)Screen::GameOverlay])
			elem->Render();

	//rendering the current menu
	vector<cGUIElement *> menu = menus[(int)currentMenu];
	for (cGUIElement *elem : menu)
		elem->Render();

	//always rendering performance counters
	for (cGUIElement *elem : menus[(int)Screen::Global])
		elem->Render();
}

void cGUI::ToggleBackground()
{
	cSettings::Get()->ToggleDrawBackground();
	UpdateBackgroundBtn();
}

void cGUI::ToggleVolume()
{
	//the actual toggling of sound level
	float volume = cSettings::Get()->GetVolume() + 0.1f;
	if (volume >= 1.1f)
		volume = 0;
		
	cSettings::Get()->SetVolume(volume);
	
	if (volume == 0) //resetting the theme music
		cGame::Get()->GetSnd("theme")->Stop();
	else //I have to restart it everytime for changes to take effect
		cGame::Get()->PlaySnd("theme", true);

	UpdateVolumeBtn();
}

void cGUI::CleanUp()
{
	for (int i = 0; i < GUI_SCREENS; i++)
		CleanUp((Screen)i);
}

void cGUI::CleanUp(Screen menu)
{
	for (int i = 0; i < menus[(int)menu].size(); i++)
		delete menus[(int)menu][i];
	menus[(int)menu].clear();
	btns[(int)menu].clear();
}

void cGUI::UpdateVolumeBtn()
{
	float volume = cSettings::Get()->GetVolume();
	int offset = currentMenu == Screen::Options ? 0 : 1;
	cGUIButton *btn = (cGUIButton*)menus[(int)currentMenu][offset + 1];
	btn->SetText("Volume: " + to_string((int)(volume * 100)) + "%");
}

void cGUI::UpdateBackgroundBtn()
{
	int offset = currentMenu == Screen::Options ? 0 : 1;
	cGUIButton *btn = (cGUIButton*)menus[(int)currentMenu][offset];
	btn->SetText(cSettings::Get()->GetDrawBackground() ? "Background: On" : "Background: Off");
}

void cGUI::SetMenu(Screen menu)
{
	cInput::Reset(); //removes annoying bugs, which happen because after menu is changed the buttons get updated,
					 //but since it's the same frame, the Get...Down is still true, forcing another trigger
	if (btns[(int)currentMenu].size() > 0)
		btns[(int)currentMenu][activeBtn]->SetActive(false);
	activeBtn = 0;
	currentMenu = menu;
	switch (currentMenu)
	{
	case Screen::Death:
		CleanUp(Screen::Death);
		SetUpDeath();
		break;
	case Screen::PauseOptions:
	case Screen::Options:
		UpdateBackgroundBtn();
		UpdateVolumeBtn();
		break;
	case Screen::Highscores:
		CleanUp(Screen::Highscores);
		SetUpHightscores();
		break;
	case Screen::Upgrade:
		CleanUp(Screen::Upgrade);
		SetUpUpgrade();
	default:
		break;
	}
	if (btns[(int)currentMenu].size() > 0)
		btns[(int)currentMenu][activeBtn]->SetActive(true);
}

void cGUI::SetUpMain()
{
	//basic sizes
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	//starting points
	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight }; //rect an element occupies

	//building the menu one element at a time
	cGUIButton *btn = new cGUIButton(NULL, r, "Play Game");
	btn->SetCallback([this]() { cGame::Get()->StartLevel(0); SetMenu(Screen::GameOverlay); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Main].push_back(btn);
	btns[(int)Screen::Main].push_back(btn);

	//now that button was created, the rect has to be moved to place another one
	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Instructions");
	btn->SetCallback([this]() { SetMenu(Screen::Instructions); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Main].push_back(btn);
	btns[(int)Screen::Main].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Highscores");
	btn->SetCallback([this]() { SetMenu(Screen::Highscores); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Main].push_back(btn);
	btns[(int)Screen::Main].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Options");
	btn->SetCallback([this]() { SetMenu(Screen::Options); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Main].push_back(btn);
	btns[(int)Screen::Main].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Quit");
	btn->SetCallback([]() { exit(0); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Main].push_back(btn);
	btns[(int)Screen::Main].push_back(btn);
}

void cGUI::SetUpInstructions()
{
	cGUIElement *elem = new cGUIElement(tutorialTexture, { 0, 0, windowSize.x, windowSize.y });
	menus[(int)Screen::Instructions].push_back(elem);
}

void cGUI::SetUpHightscores()
{
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };

	int *scores = cSettings::Get()->GetScores();
	for (int i = 0; i < 10 && scores[i] != 0; i++)
	{
		r.top += emptySpace + 5; r.bottom += emptySpace + 5;
		cGUILabel *lbl = new cGUILabel(NULL, r, to_string(i + 1) + ". " + to_string(scores[i]));
		menus[(int)Screen::Highscores].push_back(lbl);
	}

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	cGUIButton *btn = new cGUIButton(NULL, r, "Back");
	btn->SetCallback([this](){ SetMenu(Screen::Main); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Highscores].push_back(btn);
	btns[(int)Screen::Highscores].push_back(btn);
}

void cGUI::SetUpOptions()
{
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };
	string state = cSettings::Get()->GetDrawBackground() ? "On" : "Off";
	cGUIButton *btn = new cGUIButton(NULL, r, "Background: " + state);
	btn->SetCallback([this]() { ToggleBackground(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Options].push_back(btn);
	btns[(int)Screen::Options].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	state = to_string((int)(cSettings::Get()->GetVolume() * 100)) + "%";
	btn = new cGUIButton(NULL, r, "Volume: " + state);
	btn->SetCallback([this]() { ToggleVolume(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Options].push_back(btn);
	btns[(int)Screen::Options].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	state = to_string((int)(cSettings::Get()->GetVolume() * 100)) + "%";
	btn = new cGUIButton(NULL, r, "Clear Scores");
	btn->SetCallback([]() { cSettings::Get()->ClearScores(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Options].push_back(btn);
	btns[(int)Screen::Options].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Back");
	btn->SetCallback([this]() { SetMenu(Screen::Main); cSettings::Get()->Serialize(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Options].push_back(btn);
	btns[(int)Screen::Options].push_back(btn);
}

void cGUI::SetUpGameOverlay()
{
	RECTF r = { windowSize.x / 2 - 100, windowSize.y - 40, windowSize.x / 2 + 100, windowSize.y };
	cGUIProgressBar *bar = new cGUIProgressBar(NULL, r, "", glm::vec4(1, 0, 0, 1));
	menus[(int)Screen::GameOverlay].push_back(bar);

	r.bottom -= 40; r.top -= 40;
	cGUILabel *lbl = new cGUILabel(NULL, r, "");
	menus[(int)Screen::GameOverlay].push_back(lbl);
}

void cGUI::SetUpUpgrade()
{
	cGUIElement *elem = new cGUIElement(NULL, { 0, 0, windowSize.x, windowSize.y });
	elem->SetBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	menus[(int)Screen::Upgrade].push_back(elem);

	cPlayer *p = cGame::Get()->GetPlayer();
	float btnWidth = 150;
	float btnHeight = 38;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };

	r.top -= btnHeight; r.bottom -= btnHeight;
	cGUILabel *lbl = new cGUILabel(NULL, r, "Upgrade Screen");
	menus[(int)Screen::Upgrade].push_back(lbl);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	r.left -= windowSize.x / 4; r.right -= windowSize.x / 4;
	bool canUpgrade = cGame::Get()->GetPlayer()->GetShipType() != ShipType::Cruiser;
	string text = !canUpgrade ? "MAX" : "Cost : " + to_string(p->GetShipUpCost());
	cGUIButton *btn = new cGUIButton(NULL, r, "Upgrade Ship\n" + text);
	if (canUpgrade) //there are 4 ship types in game - can't let to go upgrade more than that
	{
		btn->SetCallback([this, btn](){ //passing this to update GUI and btn to update it's label
			int score = cGame::Get()->GetScore();
			cPlayer *p = cGame::Get()->GetPlayer();
			int level = (int)p->GetShipType();
			int cost = p->GetShipUpCost();
			if (cost <= score) //if the score allows it
			{
				p->SetShipType((ShipType)(level + 1)); //upgrade
				cGame::Get()->AddScore(-cost);
				UpdateUpgradeLabelText(); //update the text

				bool canUpgrade = p->GetShipType() != ShipType::Cruiser; //updating the label
				string text = !canUpgrade ? "MAX" : "Cost : " + to_string(p->GetShipUpCost());
				btn->SetText("Upgrade Ship\n" + text);
				if (!canUpgrade)
					btn->SetCallback([](){}); //remove the callback so it's impossible to do anything
			}
		}); //everything else is basically the same
	} //other buttons are done in similar fashion
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(canUpgrade ? COLOR_HIGHL : COLOR_NORMAL);
	menus[(int)Screen::Upgrade].push_back(btn);
	btns[(int)Screen::Upgrade].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Upgrade Hull\nCost:"+to_string(p->GetHullUpCost()));
	btn->SetCallback([this, btn](){
		int score = cGame::Get()->GetScore();
		cPlayer *p = cGame::Get()->GetPlayer();
		int level = p->GetHullLevel() + 1;
		int cost = p->GetHullUpCost();
		if (cost <= score)
		{
			cGame::Get()->GetPlayer()->SetHullLevel(level);
			cGame::Get()->AddScore(-cost);
			btn->SetText("Upgrade Hull\nCost:" + to_string(p->GetHullUpCost()));
			UpdateUpgradeLabelText();
		}
	});
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Upgrade].push_back(btn);
	btns[(int)Screen::Upgrade].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Upgrade Guns\nCost:"+to_string(p->GetBulletUpCost()));
	btn->SetCallback([this, btn](){
		int score = cGame::Get()->GetScore();
		cPlayer *p = cGame::Get()->GetPlayer();
		int level = p->GetBulletLevel() + 1;
		int cost = p->GetBulletUpCost();
		if (cost <= score)
		{
			cGame::Get()->GetPlayer()->SetBulletLevel(level);
			cGame::Get()->AddScore(-cost);
			btn->SetText("Upgrade Guns\nCost:" + to_string(p->GetBulletUpCost()));
			UpdateUpgradeLabelText();
		}
	});
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Upgrade].push_back(btn);
	btns[(int)Screen::Upgrade].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Upgrade Missiles\nCost:"+to_string(p->GetMissileUpCost()));
	btn->SetCallback([this, btn](){
		int score = cGame::Get()->GetScore();
		cPlayer *p = cGame::Get()->GetPlayer();
		int level = p->GetMissileLevel() + 1;
		int cost = p->GetMissileUpCost();
		if (cost <= score)
		{
			cGame::Get()->GetPlayer()->SetMissileLevel(level);
			cGame::Get()->AddScore(-cost);
			btn->SetText("Upgrade Missiles\nCost:" + to_string(p->GetMissileUpCost()));
			UpdateUpgradeLabelText();
		}
	});
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Upgrade].push_back(btn);
	btns[(int)Screen::Upgrade].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Upgrade Engine\nCost:"+to_string(p->GetEngineUpCost()));
	btn->SetCallback([this, btn](){
		int score = cGame::Get()->GetScore();
		cPlayer *p = cGame::Get()->GetPlayer();
		int level = p->GetEngineLevel() + 1;
		int cost = p->GetEngineUpCost();
		if (cost <= score)
		{
			cGame::Get()->GetPlayer()->SetEngineLevel(level);
			cGame::Get()->AddScore(-cost);
			btn->SetText("Upgrade Engine\nCost:" + to_string(p->GetEngineUpCost()));
			UpdateUpgradeLabelText();
		}
	});
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Upgrade].push_back(btn);
	btns[(int)Screen::Upgrade].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Fix Ship\nCost:"+to_string(p->GetFixCost()));
	btn->SetCallback([this, btn](){
		int score = cGame::Get()->GetScore();
		int cost = cGame::Get()->GetPlayer()->GetFixCost();
		if (cost <= score)
		{
			cGame::Get()->GetPlayer()->Repair();
			cGame::Get()->AddScore(-cost);
			btn->SetText("Upgrade Engine\nCost:0");
			UpdateUpgradeLabelText();
		}
	});
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Upgrade].push_back(btn);
	btns[(int)Screen::Upgrade].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace - 13;
	r.left = x; r.right = x + btnWidth;
	btn = new cGUIButton(NULL, r, "Continue");
	btn->SetCallback([this]() { SetMenu(Screen::GameOverlay); cGame::Get()->StartNextLevel(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Upgrade].push_back(btn);
	btns[(int)Screen::Upgrade].push_back(btn);

	x = windowSize.x / 4 + btnWidth + emptySpace;
	y = windowSize.y / 4;
	r = { x, y, x + windowSize.x / 4, y + btnHeight };
	lbl = new cGUILabel(NULL, r, GetUpgradeLabelText(), false);
	menus[(int)Screen::Upgrade].push_back(lbl);
}

void cGUI::SetUpPause()
{
	//for pause and pauseOptions I overlay a semi-transparent black texture, to darken up everything except buttons
	cGUIElement *elem = new cGUIElement(NULL, { 0, 0, windowSize.x, windowSize.y });
	elem->SetBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	menus[(int)Screen::Pause].push_back(elem);

	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };
	cGUIButton *btn = new cGUIButton(NULL, r, "Resume");
	btn->SetCallback([this]() { cGame::Get()->SetPaused(false); SetMenu(Screen::GameOverlay); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Pause].push_back(btn);
	btns[(int)Screen::Pause].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Options");
	btn->SetCallback([this]() { SetMenu(Screen::PauseOptions); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Pause].push_back(btn);
	btns[(int)Screen::Pause].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Main Menu");
	btn->SetCallback([this]() { cGame::Get()->Clear(); SetMenu(Screen::Main); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Pause].push_back(btn);
	btns[(int)Screen::Pause].push_back(btn);
}

void cGUI::SetUpPauseOptions()
{
	cGUIElement *elem = new cGUIElement(NULL, { 0, 0, windowSize.x, windowSize.y });
	elem->SetBackgroundColor(glm::vec4(0, 0, 0, 0.3f));
	menus[(int)Screen::PauseOptions].push_back(elem);
	
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };
	string state = cSettings::Get()->GetDrawBackground() ? "On" : "Off";
	cGUIButton *btn = new cGUIButton(NULL, r, "Background: " + state);
	btn->SetCallback([this]() { ToggleBackground(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::PauseOptions].push_back(btn);
	btns[(int)Screen::PauseOptions].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	state = to_string((int)(cSettings::Get()->GetVolume() * 100)) + "%";
	btn = new cGUIButton(NULL, r, "Volume: " + state);
	btn->SetCallback([this]() { ToggleVolume(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::PauseOptions].push_back(btn);
	btns[(int)Screen::PauseOptions].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	state = to_string((int)(cSettings::Get()->GetVolume() * 100)) + "%";
	btn = new cGUIButton(NULL, r, "Clear Scores");
	btn->SetCallback([]() { cSettings::Get()->ClearScores(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::PauseOptions].push_back(btn);
	btns[(int)Screen::PauseOptions].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Back");
	btn->SetCallback([this]() { SetMenu(Screen::Pause); cSettings::Get()->Serialize(); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::PauseOptions].push_back(btn);
	btns[(int)Screen::PauseOptions].push_back(btn);
}

void cGUI::SetUpDeath()
{
	float btnWidth = 150;
	float btnHeight = 25;
	float emptySpace = 10;

	float x = (windowSize.x - btnWidth) / 2;
	float y = windowSize.y / 4;
	RECTF r = { x, y, x + btnWidth, y + btnHeight };
	cGUILabel *lbl = new cGUILabel(NULL, r, "The final score is: " + to_string(finalScore));
	menus[(int)Screen::Death].push_back(lbl);

	int *scores = cSettings::Get()->GetScores();
	bool wasFound = false;
	for (int i = 0; i < 10 && scores[i] != 0; i++)
	{
		r.top += emptySpace + 5; r.bottom += emptySpace + 5;
		lbl = new cGUILabel(NULL, r, to_string(i + 1) + ". " + to_string(scores[i]));
		if (!wasFound && scores[i] == finalScore)
		{
			lbl->SetColor(glm::vec3(1, 0, 0));
			wasFound = true;
		}
		menus[(int)Screen::Death].push_back(lbl);
	}

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	cGUIButton *btn = new cGUIButton(NULL, r, "Try Again");
	btn->SetCallback([this]() { cGame::Get()->Clear(); cGame::Get()->StartLevel(0); SetMenu(Screen::GameOverlay); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Death].push_back(btn);
	btns[(int)Screen::Death].push_back(btn);

	r.top += btnHeight + emptySpace; r.bottom += btnHeight + emptySpace;
	btn = new cGUIButton(NULL, r, "Back To Main");
	btn->SetCallback([this]() { SetMenu(Screen::Main); });
	btn->SetBackgroundColor(COLOR_NORMAL);
	btn->SetHighlightColor(COLOR_HIGHL);
	menus[(int)Screen::Death].push_back(btn);
	btns[(int)Screen::Death].push_back(btn);
}

void cGUI::SetUpGlobalDataOverlay()
{
	cGUILabel *lbl = new cGUILabel(NULL, { windowSize.x - 80, 0, windowSize.x, 20 }, "");
	menus[(int)Screen::Global].push_back(lbl);
	lbl = new cGUILabel(NULL, { windowSize.x - 80, 30, windowSize.x, 20 }, "");
	menus[(int)Screen::Global].push_back(lbl);
}

string cGUI::GetUpgradeLabelText() //method which returns the big wall-o-text
{
	cPlayer *p = cGame::Get()->GetPlayer();
	ShipType type = p->GetShipType();
	int t = cGame::Get()->GetScore();
	string s = "Welcome to the Upgrade screen.\n";
	s += "Current score: " + to_string(cGame::Get()->GetScore()) + "\n\n";
	string tmp = type == ShipType::Scout ? "Scout" : type == ShipType::Fighter ? "Fighter" : type == ShipType::Corvette ? "Corvette" : "Cruiser";
	s += "Your ship is a " + tmp + " type. It has ";
	tmp = type == ShipType::Scout ? "1 gun" : type == ShipType::Fighter ? "1 gun and 1 missile launcher" : type == ShipType::Corvette ? "2 guns and 1 missile launchers" : "2 guns, 1 missile launcher and 1 turret";
	s += tmp + ".\n";
	s += "Upgrading the ship will also change your hull and engine values - the bigger the ship, the more armor.\n";
	s += "it has, yet the more slower it becomes.\n\n";
	t = p->GetHullLevel();
	s += "Your hull is level " + to_string(t) + ". Your max health is " + to_string(p->GetMaxHealth()) + ". Your armor is " + to_string(p->GetArmor()) + ".\n";
	s += "Each level gives 100 health and 2 armor.\n\n";
	t = p->GetBulletLevel();
	s += "Your guns are at level " + to_string(t) + ". They do " + to_string(p->GetBulletDamage()) + " damage per hit, firing every " + to_string(p->GetBulletCooldown()) + "s.\n";
	s += "Each level in guns gives +5 damage and -0.02s cooldown.\n\n";
	t = p->GetMissileLevel();
	s += "Your missiles are at level " + to_string(t) + ". They do " + to_string(p->GetMissileDamage()) + " damage per hit, firing every " + to_string(p->GetMissileCooldown()) + "s.\n";
	s += "Each level in missiles gives you +10 damage and -0.1s cooldown.\n\n";
	t = p->GetEngineLevel();
	s += "Your engines are at level " + to_string(t) + ". The maximum speed of the speed is " + to_string((int)p->GetMaxVel()) + "m/s.\n";
	s += "The acceleration rate is " + to_string((int)p->GetAccelRate()) + "m/s^2.\n";
	s += "The rotation speed is " + to_string((int)p->GetRotSpeed()) + "deg/s.\n\n";
	t = p->GetMaxHealth() - p->GetHealth();
	s += "Your ship sustained " + to_string(t) + " damage. You're at " + to_string(p->GetHealth()) + " health.";
 	return s;
}

void cGUI::UpdateUpgradeLabelText()
{
	auto m = menus[(int)Screen::Upgrade];
	((cGUILabel*)m[m.size() - 1])->SetText(GetUpgradeLabelText());
}