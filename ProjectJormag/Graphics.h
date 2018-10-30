#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SettingsManager.h"

namespace Jormag {

	class StateManager;

	class Graphics {
	private:
		// Singleton Instance
		static Graphics* sInstance;
		static bool sInitialised;

		// SDL Window Variables
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		SettingsManager* mSettings;
	public:
		// Primitive Window variables
		int mScreenWidth;
		int mScreenHeight;
		const char* mScreenTitle;
	private:
		Graphics();
		~Graphics();

		bool Init();
	public:
		static Graphics* Instance();
		static void Free();
		static bool IsInitialised();

		void Update();
		void Render(StateManager* currentGameState);

		SDL_Texture* LoadImageTexture(std::string path);
		SDL_Texture* LoadTextTexture(TTF_Font* font, std::string text, SDL_Color color);

		void SetScreenSize(int screenWidth, int screenHeight);
	};

}

#endif