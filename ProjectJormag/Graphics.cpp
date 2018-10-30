#include "Graphics.h"
#include "StateManager.h"

namespace Jormag {

	Graphics* Graphics::sInstance = NULL;
	bool Graphics::sInitialised = false;

	Graphics* Graphics::Instance() {
		if (sInstance == NULL)
			sInstance = new Graphics();

		return sInstance;
	}

	void Graphics::Free() {
		delete sInstance;
		sInstance = NULL;
	}

	bool Graphics::IsInitialised() {
		return sInitialised;
	}

	Graphics::Graphics() : mScreenTitle("Project Jormag") {
		mSettings = SettingsManager::Instance();
		sInitialised = Init();
	}

	Graphics::~Graphics() {
		mSettings = NULL;

		SDL_DestroyWindow(mWindow);
		mWindow = NULL;

		SDL_DestroyRenderer(mRenderer);
		mRenderer = NULL;

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void Graphics::Render(StateManager* currentState) {
		SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(mRenderer);

		if (currentState != NULL)
			currentState->Render(mRenderer);

		SDL_RenderPresent(mRenderer);
	}

	bool Graphics::Init() {
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
			std::cerr << "SDL Video/Audio could not initialise: " << SDL_GetError() << std::endl;
			return false;
		}

		mWindow = SDL_CreateWindow(mScreenTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
		if (mWindow == NULL) {
			std::cerr << "Window could not initialise: " << SDL_GetError() << std::endl;
			return false;
		}
		Update();

		if (mSettings->GetPrefVSync())
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED); // Enable VSYNC
		else
			mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
		if (mRenderer == NULL) {
			std::cerr << "Renderer could not initialise: " << SDL_GetError() << std::endl;
			return false;
		} else {
			int imageFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imageFlags) &imageFlags)) {
				std::cerr << "SDL Image could not initialise: " << IMG_GetError() << std::endl;
				return false;
			}
		}

		if (TTF_Init() == -1) {
			std::cerr << "SDL TTF could not initialise: " << SDL_GetError() << std::endl;
			return false;
		}

		return true;
	}

	void Graphics::Update() {
		SDL_DisplayMode desktopDefault;
		if (SDL_GetDesktopDisplayMode(0, &desktopDefault) != 0) {
			std::cerr << "Could not read desktop display settings: " << SDL_GetError() << std::endl;
			return;
		}
		
		// Reset Window
		SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_SHOWN);
		SDL_SetWindowBordered(mWindow, SDL_TRUE);
		SDL_RestoreWindow(mWindow);
		SDL_SetWindowSize(mWindow, mSettings->GetPrefScreenWidth(), mSettings->GetPrefScreenHeight());

		// Apply Preferences
		switch (mSettings->GetPrefFullscreenMode()) {
		case FullscreenMode::BORDERLESS:
			SDL_SetWindowBordered(mWindow, SDL_FALSE);
			SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED_MASK, SDL_WINDOWPOS_CENTERED_MASK);
			break;
		case FullscreenMode::FULLSCREEN:
			SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);
			break;
		case FullscreenMode::WINDOWED:
			if (mSettings->GetPrefScreenWidth() == desktopDefault.w &&
				mSettings->GetPrefScreenHeight() == desktopDefault.h)
				SDL_MaximizeWindow(mWindow);
			else
				SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED_MASK, SDL_WINDOWPOS_CENTERED_MASK);
			break;
		}
		SDL_GetWindowSize(mWindow, &mScreenWidth, &mScreenHeight);
	}

	SDL_Texture* Graphics::LoadImageTexture(std::string path) {
		SDL_Texture* texture = NULL;

		SDL_Surface* surface = IMG_Load(path.c_str());
		if (surface == NULL) {
			std::cerr << "Could not create surface: " << SDL_GetError() << std::endl;
			return texture;
		}

		texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (texture == NULL) {
			std::cerr << "Could not create texture: " << SDL_GetError() << std::endl;
			return texture;
		}
		SDL_FreeSurface(surface);

		return texture;
	}

	SDL_Texture* Graphics::LoadTextTexture(TTF_Font* font, std::string text, SDL_Color color) {
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (surface == NULL) {
			std::cerr << "Could not create surface: " << TTF_GetError() << std::endl;
			return NULL;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (texture == NULL) {
			std::cerr << "Could not create textureL " << SDL_GetError() << std::endl;
			return NULL;
		}
		SDL_FreeSurface(surface);

		return texture;
	}

	void Graphics::SetScreenSize(int screenWidth, int screenHeight) {
		mScreenWidth = screenWidth;
		mScreenHeight = mScreenHeight;
	}
}