#pragma once

#ifndef GAMELOSS_H
#define GAMELOSS_H

#include "Jormag.h"

enum DeathType: char;

class GameLossScreen : public GameObject {
private:
	int mScreenWidth;
	int mScreenHeight;

	// Content
	Texture* mBackground;
	Texture* mLossLabel;
	Texture* mDeathLabel;
	Menu* mButtons;

	// Selection
	InputManager* mInput;
	int mIndexFocused;
private:
	Menu* InitButtons();
public:
	GameLossScreen();
	~GameLossScreen();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	void SetCauseOfDeath(DeathType causeOfDeath);
};

#endif