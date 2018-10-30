#include "AudioManager.h"

namespace Jormag {

	AudioManager* AudioManager::sInstance = NULL;

	AudioManager* AudioManager::Instance() {
		if (sInstance == NULL)
			sInstance = new AudioManager();

		return sInstance;
	}

	void AudioManager::Free() {
		delete sInstance;
		sInstance = NULL;
	}

	AudioManager::AudioManager() {
		mResourceMgr = ResourceManager::Instance();

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
			std::cerr << "Mixer could not initialise: " <<  Mix_GetError() << std::endl;
	}

	AudioManager::~AudioManager() {
		mResourceMgr = NULL;
		
		Mix_Quit();
	}

	void AudioManager::PlayMusic(std::string fileName, int loops) {
		Mix_PlayMusic(mResourceMgr->GetMusic(fileName), loops);
	}

	void AudioManager::PauseMusic() {
		if (Mix_PlayingMusic() != 0)
			Mix_PausedMusic();
	}

	void AudioManager::ResumeMusic() {
		if (Mix_PausedMusic() != 0)
			Mix_ResumeMusic();
	}

	void AudioManager::StopMusic() {
		Mix_HaltMusic();
	}

	void AudioManager::PlaySFX(std::string fileName, int loops, int channel) {
		Mix_PlayChannel(channel, mResourceMgr->GetSFX(fileName), loops);
	}

	void AudioManager::SetMusicVolume(float musicVolume) {
		Mix_VolumeMusic((int)(musicVolume * 128.0f));
	}

	void AudioManager::SetSFXVolume(float sfxVolume) {
		Mix_Volume(-1,(int)(sfxVolume * 128.0f));
	}
}