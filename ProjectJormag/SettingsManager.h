#pragma once

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <fstream>
#include <sstream>
#include <vector>

namespace Jormag {
	
	namespace Ratio {
		enum Aspect { SQUARE, WIDE, ULTRAWIDE };
	}

	namespace Resolution {
		enum Screen { SD, HD, HDP, FHD, QHD, UHD };
	}

	namespace FullscreenMode {
		enum Mode { WINDOWED, BORDERLESS, FULLSCREEN };
	}
	
	namespace Controls {
		struct Scheme {
			int* mKeyCodes;

			Scheme(int controlCount) {
				mKeyCodes = new int[controlCount];
			}

			~Scheme() {
				delete[] mKeyCodes;
				mKeyCodes = NULL;
			}
		};
	}

	class SettingsManager {
	private:
		// Singleton Instance
		static SettingsManager* sInstance;

		// Settings
		Ratio::Aspect mPrefAspectRatio;
		Resolution::Screen mPrefResolution;
		FullscreenMode::Mode mPrefFullscreenMode;
		bool mPrefVSync;
		float mPrefMusicVolume;
		float mPrefSFXVolume;
	public:
		// Available Resolutions
		const int resolutionHeightList[6] = { 480, 720, 900, 1080, 1440, 2160 };
	private:
		SettingsManager();
		~SettingsManager();

		void InitDefaults();
		void InitFromFile();
	public:
		static SettingsManager* Instance();
		static void Free();

		static int HeightToWidth(Ratio::Aspect ratio, int screenHeight);

		void SaveToFile();

		int GetPrefScreenWidth();
		int GetPrefScreenHeight();
		Ratio::Aspect GetPrefAspectRatio();
		void SetPrefAspectRatio(Ratio::Aspect ratio);
		void SetPrefResolution(Ratio::Aspect ratio, Resolution::Screen res);

		FullscreenMode::Mode GetPrefFullscreenMode();
		void SetPrefFullScreenMode(FullscreenMode::Mode mode);

		bool GetPrefVSync();
		void SetPrefVSync(bool vsync);

		float GetPrefMusicVolume();
		void SetPrefMusicVolume(float volume);

		float GetPrefSFXVolume();
		void SetPrefSFXVolume(float volume);

		Controls::Scheme* GetControls(int schemeID);
	};
}

#endif