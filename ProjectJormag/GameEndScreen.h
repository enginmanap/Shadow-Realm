#pragma once

#ifndef GAMEEND_H
#define GAMEEND_H

#include "Jormag.h"

class GameEndScreen : public GameObject {
private:
	int mScreenWidth;
	int mScreenHeight;

	// Content
	Texture* mBackground;
	Texture* mEndLabel;
	Texture* mDeathsLabel;
	GUIButton* mQuitButton;

	// Selection
	InputManager* mInput;
public:
	GameEndScreen();
	~GameEndScreen();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	void SetDeaths(int totalDeaths);
};

#endif