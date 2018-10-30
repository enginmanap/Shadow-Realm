#pragma once

#ifndef GAMEWIN_H
#define GAMEWIN_H

#include "Jormag.h"

class GameWinScreen : public GameObject {
private:
	int mScreenWidth;
	int mScreenHeight;

	// Content
	Texture* mBackground;
	Texture* mWinLabel;
	Texture* mScoreLabel;
	Menu* mButtons;

	// Selection
	InputManager* mInput;
	int mIndexFocused;
private:
	Menu* InitButtons();
public:
	GameWinScreen();
	~GameWinScreen();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	void SetFinalScore(int finalScore);
};

#endif