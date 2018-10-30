#pragma once

#ifndef MAINMENU_H
#define MAINMENU_H

#include "Jormag.h"

class MainMenu : public GameObject {
public:
	enum MenuID { TOP, SAVESELECT, SETTINGS, VIDEO, AUDIO };
private:
	// Content
	Camera* mCamera;
	Texture* mBackground;
	Texture* mLogo;
	int mScreenWidth;
	int mScreenHeight;

	// Menus
	Menu* mTopMenu;
	Menu* mSaveSelectMenu;
	Menu* mSettingsMenu;
	Menu* mVideoSettingsMenu;
	Menu* mAudioSettingsMenu;

	// Selection
	InputManager* mInput;
	Menu* mFocusedMenu;
	int mIndexFocused;

	// Settings
	bool mOverwriting;
	std::stringstream* mResSelectText;
	std::stringstream* mFullscreenSelectText;
	std::stringstream* mRatioSelectText;
private:
	Menu* InitTopMenu();
	Menu* InitSaveSelectMenu();
	Menu* InitMainSettingsMenu();
	Menu* InitVideoSettingsMenu();
	Menu* InitAudioSettingsMenu();
public:
	MainMenu();
	~MainMenu();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	void Init();
	void Reset();
	void SetMenuFocus(MenuID menuID);
	void SetOverwritingSave(bool overwriting);
};

#endif