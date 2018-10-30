#include "SettingsManager.h"
#include "AudioManager.h"
#include "Graphics.h"

namespace Jormag {

	SettingsManager* SettingsManager::sInstance = NULL;

	SettingsManager* SettingsManager::Instance() {
		if (sInstance == NULL)
			sInstance = new SettingsManager();

		return sInstance;
	}

	void SettingsManager::Free() {
		delete sInstance;
		sInstance = NULL;
	}

	SettingsManager::SettingsManager() {
		std::string path = SDL_GetBasePath();
		path.append("Assets/Data/Settings.cfg");
		std::fstream settingsFile(path);
		if (!settingsFile) {
			settingsFile.close();
			InitDefaults();
		} else {
			if (!settingsFile.good()) {
				std::cerr << "Could not load preferences" << std::endl;
				return;
			}
			settingsFile.close();
			InitFromFile();
		}
	}

	SettingsManager::~SettingsManager() {
		return;
	}

	void SettingsManager::InitDefaults() {
		std::string path = SDL_GetBasePath();
		path.append("Assets/Data/Settings.cfg");
		std::ofstream settingsFile(path);

		settingsFile << "AspectRatio=" << std::to_string(Ratio::SQUARE) << std::endl;
		settingsFile << "Resolution=" << std::to_string(Resolution::SD) << std::endl;
		settingsFile << "Fullscreen=" << std::to_string(FullscreenMode::WINDOWED) << std::endl;
		settingsFile << "VSync=" << std::to_string(true) << std::endl;
		settingsFile << "Music=" << std::to_string(0.6f) << std::endl;
		settingsFile << "SFX=" << std::to_string(0.75f) << std::endl;

		settingsFile.close();

		InitFromFile();
	}

	void SettingsManager::InitFromFile() {
		std::string path = SDL_GetBasePath();
		path.append("Assets/Data/Settings.cfg");
		std::ifstream settingsFile(path);

		std::string line;
		std::stringstream unparsedData[6];
		int lineIndex = 0;
		while (settingsFile >> line) {
			unparsedData[lineIndex] << line.substr(line.find_last_of('=') + 1, line.length() - line.find_last_of('='));
			lineIndex++;
		}
		settingsFile.close();

		mPrefAspectRatio = (Ratio::Aspect)std::atoi(unparsedData[0].str().c_str());
		mPrefResolution = (Resolution::Screen)std::atoi(unparsedData[1].str().c_str());
		mPrefFullscreenMode = (FullscreenMode::Mode)std::atoi(unparsedData[2].str().c_str());
		unparsedData[3] >> std::boolalpha >> mPrefVSync;
		mPrefMusicVolume = stof(unparsedData[4].str());
		mPrefSFXVolume = stof(unparsedData[5].str());
	}

	void SettingsManager::SaveToFile() {
		std::string path = SDL_GetBasePath();
		path.append("Assets/Data/Settings.cfg");
		std::ofstream settingsFile(path);
		
		settingsFile << "AspectRatio=" << std::to_string(mPrefAspectRatio) << std::endl;
		settingsFile << "Resolution=" << std::to_string(mPrefResolution) << std::endl;
		settingsFile << "Fullscreen=" << std::to_string(mPrefFullscreenMode) << std::endl;
		settingsFile << "VSync=" << std::to_string(mPrefVSync) << std::endl;
		settingsFile << "Music=" << std::to_string(mPrefMusicVolume) << std::endl;
		settingsFile << "SFX=" << std::to_string(mPrefSFXVolume) << std::endl;

		settingsFile.close();
	}

	int SettingsManager::HeightToWidth(Ratio::Aspect ratio, int screenHeight) {
		float screenWidth;
		switch (ratio) {
		default:
		case Ratio::SQUARE:
			screenWidth = 4.0f * screenHeight / 3.0f;
			return (int)screenWidth;
		case Ratio::WIDE:
			screenWidth = 16.0f * screenHeight / 9.0f;
			return (int)screenWidth;
		case Ratio::ULTRAWIDE:
			screenWidth = 21.0f * screenHeight / 9.0f;
			return (int)screenWidth;
		}
	}

	int SettingsManager::GetPrefScreenWidth() {
		return HeightToWidth(mPrefAspectRatio, resolutionHeightList[mPrefResolution]);
	}
	int SettingsManager::GetPrefScreenHeight() {
		return resolutionHeightList[mPrefResolution];
	}
	Ratio::Aspect SettingsManager::GetPrefAspectRatio() {
		return mPrefAspectRatio;
	}
	void SettingsManager::SetPrefAspectRatio(Ratio::Aspect ratio) {
		mPrefAspectRatio = ratio;
		Graphics::Instance()->SetScreenSize(GetPrefScreenWidth(), GetPrefScreenHeight());
		Graphics::Instance()->Update();
	}
	void SettingsManager::SetPrefResolution(Ratio::Aspect ratio, Resolution::Screen res) {
		mPrefAspectRatio = ratio;
		mPrefResolution = res;
		Graphics::Instance()->SetScreenSize(GetPrefScreenWidth(), GetPrefScreenHeight());
		Graphics::Instance()->Update();
	}

	FullscreenMode::Mode SettingsManager::GetPrefFullscreenMode() {
		return mPrefFullscreenMode;
	}
	void SettingsManager::SetPrefFullScreenMode(FullscreenMode::Mode mode) {
		mPrefFullscreenMode = mode;
		Graphics::Instance()->Update();
	}

	bool SettingsManager::GetPrefVSync() {
		return mPrefVSync;
	}
	void SettingsManager::SetPrefVSync(bool prefVSync) {
		mPrefVSync = prefVSync;
	}

	float SettingsManager::GetPrefMusicVolume() {
		return mPrefMusicVolume;
	}
	void SettingsManager::SetPrefMusicVolume(float volume) {
		mPrefMusicVolume = volume;
		AudioManager::Instance()->SetMusicVolume(volume);
	}

	float SettingsManager::GetPrefSFXVolume() {
		return mPrefSFXVolume;
	}
	void SettingsManager::SetPrefSFXVolume(float volume) {
		mPrefSFXVolume = volume;
		AudioManager::Instance()->SetSFXVolume(volume);
	}

	Controls::Scheme* SettingsManager::GetControls(int schemeID) {
		std::stringstream path;
		path << SDL_GetBasePath() << "Assets/Data/Controls/Scheme" << std::to_string(schemeID) << ".dat";

		std::vector<int> loadedCtrls;
		std::ifstream inFile(path.str());
		while (!inFile.eof()) {
			std::string line;
			inFile >> line;
			loadedCtrls.push_back(std::stoi(line));
		}

		Controls::Scheme* controls = new Controls::Scheme(loadedCtrls.size());
		for (int i = 0; i < loadedCtrls.size(); i++)
			controls->mKeyCodes[i] = loadedCtrls[i];

		return controls;
	}
}