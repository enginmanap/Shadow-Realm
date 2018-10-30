#include "Scripts.h"

namespace ButtonScripts {
	// ------------------------------------------------
	// MAIN MENU SECTION
	// ------------------------------------------------

	void SwitchMenuFocus(MainMenu* menu, MainMenu::MenuID menuID) {
		menu->SetMenuFocus(menuID);
	}

	void NewGame(MainMenu* menu) {
		menu->SetOverwritingSave(true);
		menu->SetMenuFocus(MainMenu::SAVESELECT);
	}

	void LoadGame(MainMenu* menu) {
		menu->SetOverwritingSave(false);
		menu->SetMenuFocus(MainMenu::SAVESELECT);
	}

	void SetSaveSlot(int saveSlot, bool* isOverwriting) {
		GameManager* game = GameManager::Instance();
		if (isOverwriting)
			game->NewSave(saveSlot);
		else 
			game->LoadSave(saveSlot);

		game->GetLevelState()->LoadNextLevel();
		game->SetGameState(GameManager::GAME);
	}

	void CycleResolution(GUIButton* resSelectButton, std::stringstream* resSelectText, const Direction::Facing cycleDir) {
		SettingsManager* settings = SettingsManager::Instance();

		// Limit resolutions to desktop compatible
		SDL_DisplayMode desktopDefault;
		if (SDL_GetDesktopDisplayMode(0, &desktopDefault) != 0) {
			std::cerr << "Could not read desktop display settings: " << SDL_GetError() << std::endl;
			return;
		}
		int maxResIndex;
		for (int i = 0; i < 6; i++) {
			if (desktopDefault.h < settings->resolutionHeightList[i]) {
				maxResIndex = i - 1;
				break;
			}
		}

		// Parse & update current resolution
		int resSplitterIndex = resSelectText->str().find_last_of('x');
		int prefHeight = atoi(resSelectText->str().substr(resSplitterIndex + 1, resSelectText->str().length() - resSplitterIndex).c_str());
		for (int i = 0; i <= maxResIndex; i++) {
			if (prefHeight == settings->resolutionHeightList[i]) {
				if (cycleDir == Direction::RIGHT) {
					i++;
					if (i > maxResIndex) i -= maxResIndex + 1;
				} else {
					i--;
					if (i < 0) i += maxResIndex;
				}
				prefHeight = settings->resolutionHeightList[i];
				break;
			}
		}

		// Update display
		resSelectText->str("");
		*resSelectText << "Resolution: " << settings->HeightToWidth(settings->GetPrefAspectRatio(), prefHeight) << "x" << prefHeight;
		resSelectButton->SetLabel(resSelectText->str(), "Main.ttf", 21, { 255,255,255 });
	}

	void ConfirmResolution(MainMenu* menu, std::stringstream* resSelectText) {
		SettingsManager* settings = SettingsManager::Instance();

		int resSplitterIndex = resSelectText->str().find_last_of('x');
		int prefHeight = atoi(resSelectText->str().substr(resSplitterIndex + 1, resSelectText->str().length() - resSplitterIndex).c_str());
		for (int i = 0; i < 6; i++) {
			if (prefHeight == settings->resolutionHeightList[i]) {
				settings->SetPrefResolution(settings->GetPrefAspectRatio(), ((Resolution::Screen)i));
			}
		}
		menu->Reset();
		menu->Init();
		menu->SetMenuFocus(MainMenu::VIDEO);
		Camera::Instance()->SetPanningTime(0);
	}

	void CycleFullscreen(GUIButton* fsSelectButton, std::stringstream* fsSelectText, const Direction::Facing cycleDir) {
		SettingsManager* settings = SettingsManager::Instance();

		// Parse & update current fullscreen mode
		int assignmentOpIndex = fsSelectText->str().find_first_of(':');
		int fsValue = atoi(fsSelectText->str().substr(assignmentOpIndex + 2, fsSelectText->str().length() - assignmentOpIndex - 1).c_str());
		if (cycleDir == Direction::RIGHT)
			fsValue++;
		else
			fsValue--;

		if (fsValue >= 3) fsValue -= 3;
		if (fsValue < 0) fsValue += 3;

		// Update display
		fsSelectText->str("");
		*fsSelectText << "Fullscreen: " << fsValue;
		fsSelectButton->SetLabel(fsSelectText->str(), "Main.ttf", 21, { 255,255,255 });
	}

	void ConfirmFullscreen(MainMenu* menu, std::stringstream* fsSelectText) {
		SettingsManager* settings = SettingsManager::Instance();

		int assignmentOpIndex = fsSelectText->str().find_first_of(':');
		int fsValue = atoi(fsSelectText->str().substr(assignmentOpIndex + 2, fsSelectText->str().length() - assignmentOpIndex - 1).c_str());
		settings->SetPrefFullScreenMode((FullscreenMode::Mode)fsValue);
	}

	void CycleRatio(GUIButton* ratioSelectButton, std::stringstream* ratioSelectText, const Direction::Facing cycleDir) {
		SettingsManager* settings = SettingsManager::Instance();

		// Parse & update current ratio
		int assignmentOpIndex = ratioSelectText->str().find_first_of(':');
		int ratioValue = atoi(ratioSelectText->str().substr(assignmentOpIndex + 2, ratioSelectText->str().length() - assignmentOpIndex - 1).c_str());
		if (cycleDir == Direction::RIGHT)
			ratioValue++;
		else
			ratioValue--;

		if (ratioValue >= 3) ratioValue -= 3;
		if (ratioValue < 0) ratioValue += 3;

		// Update display
		ratioSelectText->str("");
		*ratioSelectText << "Aspect Ratio: " << ratioValue;
		ratioSelectButton->SetLabel(ratioSelectText->str(), "Main.ttf", 21, { 255,255,255 });
	}

	void ConfirmRatio(MainMenu* menu, std::stringstream* ratioSelectText) {
		SettingsManager* settings = SettingsManager::Instance();

		int assignmentOpIndex = ratioSelectText->str().find_first_of(':');
		int ratioValue = atoi(ratioSelectText->str().substr(assignmentOpIndex + 2, ratioSelectText->str().length() - assignmentOpIndex - 1).c_str());
		settings->SetPrefAspectRatio((Ratio::Aspect)ratioValue);
	}

	void UpdateMusicVolume(GUISlider* musicSlider, float change) {
		musicSlider->ModifyValue(change);
		musicSlider->SetLabel("Music Volume: " + std::to_string(musicSlider->GetCurrentValue()), "Main.ttf", 21, { 255, 255, 255 });

		float newVolume = musicSlider->GetCurrentValue();
		SettingsManager::Instance()->SetPrefMusicVolume(newVolume);
		AudioManager::Instance()->SetMusicVolume(newVolume);
	}

	void ConfirmMusicVolume(GUISlider* musicSlider) {
		float newVolume = musicSlider->GetCurrentValue();
		SettingsManager::Instance()->SetPrefMusicVolume(newVolume);
		AudioManager::Instance()->SetMusicVolume(newVolume);
	}

	void UpdateSFXVolume(GUISlider* sfxSlider, float change) {
		sfxSlider->ModifyValue(change);
		sfxSlider->SetLabel("SFX Volume: " + std::to_string(sfxSlider->GetCurrentValue()), "Main.ttf", 21, { 255, 255, 255 });

		float newVolume = sfxSlider->GetCurrentValue();
		SettingsManager::Instance()->SetPrefSFXVolume(newVolume);
		AudioManager::Instance()->SetSFXVolume(newVolume);
	}

	void ConfirmSFXVolume(GUISlider* sfxSlider) {
		float newVolume = sfxSlider->GetCurrentValue();
		SettingsManager::Instance()->SetPrefSFXVolume(newVolume);
		AudioManager::Instance()->SetSFXVolume(newVolume);
	}

	void QuitGame() {
		EngineManager::Instance()->QuitGame();
	}

	// ------------------------------------------------
	// GAME WIN SECTION
	// ------------------------------------------------

	void NextLevel() {
		GameManager::Instance()->SetGameState(GameManager::GAME);
		GameManager::Instance()->GetLevelState()->LoadNextLevel();
	}

	void RetryLevel() {
		GameManager::Instance()->GetLevelState()->ReloadLevel();
		GameManager::Instance()->SetGameState(GameManager::GAME);
	}

	void QuitToMenu() {
		GameManager::Instance()->GetMenuState()->SetMenu(MenuManager::MAIN);
		GameManager::Instance()->SetGameState(GameManager::MENU);
	}
}