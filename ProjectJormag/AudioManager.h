#pragma once

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <iostream>

#include "ResourceManager.h"

namespace Jormag {

	class AudioManager {
	private:
		// Singleton Instance
		static AudioManager* sInstance;

		// File Handler
		ResourceManager* mResourceMgr;

		// Volume
		float mMusicVolume;
		float mSFXVolume;
	private:
		AudioManager();
		~AudioManager();
	public:
		static AudioManager* Instance();
		static void Free();

		void PlayMusic(std::string fileName, int loops);
		void PauseMusic();
		void ResumeMusic();
		void StopMusic();

		void PlaySFX(std::string fileName, int loops, int channel);

		void SetMusicVolume(float musicVolume);
		void SetSFXVolume(float sfxVolume);
	};
}

#endif