#include "ResourceManager.h"

#include <iostream>

namespace Jormag {

	ResourceManager* ResourceManager::sInstance = NULL;

	ResourceManager* ResourceManager::Instance() {
		if (sInstance == NULL)
			sInstance = new ResourceManager();

		return sInstance;
	}

	void ResourceManager::Free() {
		delete sInstance;
		sInstance = NULL;
	}

	ResourceManager::ResourceManager() {
	}

	ResourceManager::~ResourceManager() {
		for (auto texture : mTextures) {
			if (texture.second != nullptr) {
				SDL_DestroyTexture(texture.second);
			}
		}
		mTextures.clear();

		for (auto text : mText) {
			if (text.second != nullptr) {
				SDL_DestroyTexture(text.second);
			}
		}
		mText.clear();

		for (auto font : mFonts) {
			if (font.second != nullptr) {
				TTF_CloseFont(font.second);
			}
		}
		mTextures.clear();

		Mix_CloseAudio();
		for (auto music : mMusic) {
			if (music.second != NULL) {
				Mix_FreeMusic(music.second);
			}
		}
		mMusic.clear();

		for (auto sfx : mSFX) {
			if (sfx.second != NULL) {
				Mix_FreeChunk(sfx.second);
			}
		}
		mSFX.clear();
	}

	SDL_Texture* ResourceManager::GetTexture(std::string fileName) {
		std::string path = SDL_GetBasePath();
		path.append("Assets/" + fileName);

		if (mTextures[path] == nullptr)
			mTextures[path] = Graphics::Instance()->LoadImageTexture(path);
		
		return mTextures[path];
	}

	SDL_Texture* ResourceManager::GetText(std::string text, std::string fileName, int size, SDL_Color color) {
		TTF_Font* font = GetFont(fileName, size);

		std::string key = text + fileName + (char)size + (char)color.r + (char)color.g + (char)color.b;
		if (mText[key] == nullptr)
			mText[key] = Graphics::Instance()->LoadTextTexture(font, text, color);

		return mText[key];
	}

	TTF_Font* ResourceManager::GetFont(std::string fileName, int size) {
		std::string path = SDL_GetBasePath();
		path.append("Assets/Fonts/" + fileName);

		std::string key = path + (char)size;
		if (mFonts[key] == nullptr) {
			mFonts[key] = TTF_OpenFont(path.c_str(), size);
			if (mFonts[key] == nullptr)
				std::cerr << "Font could not initialise: " << SDL_GetError() << std::endl;
		}

		return mFonts[key];
	}

	Mix_Music* ResourceManager::GetMusic(std::string fileName) {
		std::string path = SDL_GetBasePath();
		path.append("Assets/" + fileName);

		if (mMusic[path] == nullptr) {
			mMusic[path] = Mix_LoadMUS(path.c_str());
			if (mMusic[path] == NULL)
				std::cerr << "Music could not initialise: " << Mix_GetError() << std::endl;
		}

		return mMusic[path];
	}

	Mix_Chunk* ResourceManager::GetSFX(std::string fileName) {
		std::string path = SDL_GetBasePath();
		path.append("Assets/" + fileName);

		if (mSFX[path] == nullptr) {
			mSFX[path] = Mix_LoadWAV(path.c_str());
			if (mSFX[path] == NULL)
				std::cerr << "Music could not initialise: " << Mix_GetError() << std::endl;
		}

		return mSFX[path];
	}

}