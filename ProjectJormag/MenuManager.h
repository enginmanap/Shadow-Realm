#pragma once

#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include "Jormag.h"

#include "TitleScreen.h"
#include "MainMenu.h"
#include "GameWinScreen.h"
#include "GameLossScreen.h"
#include "GameEndScreen.h"

class MenuManager : public Jormag::StateManager {
public:
	enum ScreenID { TITLE, MAIN, GAMEWIN, GAMELOSS, GAMEEND };
private:
	TitleScreen* mTitleScreen;
	MainMenu* mMainMenu;
	GameWinScreen* mGameWinScreen;
	GameLossScreen* mGameLossScreen;
	GameEndScreen* mGameEndScreen;

	GameObject* mFocusedMenu;
public:
	MenuManager();
	~MenuManager();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	TitleScreen* GetTitleScreen();
	MainMenu* GetMainMenu();
	GameWinScreen* GetGameWinScreen();
	GameLossScreen* GetGameLossScreen();
	GameEndScreen* GetGameEndScreen();

	void SetMenu(ScreenID screen);
};

#endif