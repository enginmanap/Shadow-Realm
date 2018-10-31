#pragma once

#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Jormag.h"

#include "Level.h"
#include "Player.h"
#include "Shadow.h"

class LevelManager : public Jormag::StateManager {
private:	
	Graphics* mGraphics = nullptr;

	// Game Objects
	Camera* mCamera = nullptr;
	Level* mCurrLevel = nullptr;
	Player* mPlayer = nullptr;

	// Flavour
	unsigned int mLastCamPan;
	Texture* mStartLabel = nullptr;
	Texture* mScoreLabel = nullptr;
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