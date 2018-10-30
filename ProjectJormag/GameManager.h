#pragma once

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Jormag.h"

#include "LevelManager.h"
#include "MenuManager.h"
#include "MarioManager.h"

class GameManager {
private:
	// Singleton Instance
	static GameManager* sInstance;

	EngineManager* mEngine;
	MenuManager* mMenu;
	LevelManager* mGame;
	MarioManager* mMario;

	std::string mSaveDate;
	int mTotalDeaths;
public:
	enum States { MENU, GAME, MARIO };
private:
	GameManager();
	~GameManager();

	std::string GetDate();
public:
	static GameManager* Instance();
	static void Free();

	MenuManager* GetMenuState();
	LevelManager* GetLevelState();
	void SetGameState(States state);

	void NewSave(int slot);
	void LoadSave(int slot);
	void SaveGame(int slot);

	std::string GetLastSaveDate();

	int GetTotalDeaths();
	void AddDeathToTotal();
};

#endif