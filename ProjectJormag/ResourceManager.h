#pragma once

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>

#include <SDL_mixer.h>

#include "Graphics.h"

namespace Jormag {

	class ResourceManager {
	private:
		// Singleton Instance
		static ResourceManager* sInstance;

		// Cache
		std::map<std::string, SDL_Texture*> mTextures;
		std::map<std::string, SDL_Texture*> mText;
		std::map<std::string, TTF_Font*> mFonts;
		std::map<std::string, Mix_Music*> mMusic;
		std::map<std::string, Mix_Chunk*> mSFX;
	public:
		static ResourceManager* Instance();
		static void Free();

		SDL_Texture* GetTexture(std::string fileName);
		SDL_Texture* GetText(std::string text, std::string fileName, int size, SDL_Color color);
		Mix_Music* GetMusic(std::string fileName);
		Mix_Chunk* GetSFX(std::string fileName);
	private:
		ResourceManager();
		~ResourceManager();

		TTF_Font* GetFont(std::string fileName, int size);
	};

}

#endif
