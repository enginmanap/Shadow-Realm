#pragma once

#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "Jormag.h"

#include "GameManager.h"
#include "MainMenu.h"

namespace ButtonScripts {

	// Main Menu
	void SwitchMenuFocus(MainMenu* menu, MainMenu::MenuID menuID);
	void NewGame(MainMenu* menu);
	void LoadGame(MainMenu* menu);
	void SetSaveSlot(int saveSlot, bool* isOverwriting);
	void CycleResolution(GUIButton* resSelectButton, std::stringstream* resSelectText, const Direction::Facing cycleDir);
	void ConfirmResolution(MainMenu* menu, std::stringstream* resSelectText);
	void CycleFullscreen(GUIButton* fsSelectButton, std::stringstream* fsSelectText, const Direction::Facing cycleDir);
	void ConfirmFullscreen(MainMenu* menu, std::stringstream* fsSelectText);
	void CycleRatio(GUIButton* ratioSelectButton, std::stringstream* ratioSelectText, const Direction::Facing cycleDir);
	void ConfirmRatio(MainMenu* menu, std::stringstream* ratioSelectText);
	void UpdateMusicVolume(GUISlider* musicSlider, float change);
	void ConfirmMusicVolume(GUISlider* musicSlider);
	void UpdateSFXVolume(GUISlider* sfxSlider, float change);
	void ConfirmSFXVolume(GUISlider* sfxSlider);
	void QuitGame();

	// Game Win
	void NextLevel();
	void RetryLevel();
	void QuitToMenu();

}

#endif