#pragma once

#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Jormag.h"

#include "Level.h"
#include "Player.h"
#include "Shadow.h"

class LevelManager : public Jormag::StateManager {
private:	
	Graphics* mGraphics;

	// Game Objects
	Camera* mCamera;
	Level* mCurrLevel;
	Player* mPlayer;

	// Flavour
	unsigned int mLastCamPan;
	Texture* mStartLabel;
	Texture* mScoreLabel;
	bool mLevelFinishing;
	int mScreenOpacity;

	// Preload flags
	int mLevelToLoad;
	int mSaveSlotNum;
private:
	void GetInput();
	bool CheckWin();
	bool CheckLoss();
public:
	LevelManager();
	~LevelManager();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	void LoadNextLevel();
	void ReloadLevel();
	void SetNextLevel(int levelNum);
	void SetSaveSlot(int saveSlot);

	Level* GetLevel();
	Player* GetPlayer();
};

#endif