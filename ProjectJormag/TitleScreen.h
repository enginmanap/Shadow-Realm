#pragma once

#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "Jormag.h"

class TitleScreen : public GameObject {
private:
	// Content
	Texture* mBackground;
	Texture* mLogo;
	Texture* mPressText;

	// Flashing text variables
	int mPressTextOpacity;
	bool mPressTextFadingOut;
public:
	TitleScreen();
	~TitleScreen();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);
};

#endif