#include "EngineManager.h"
#include "StateManager.h"

#include "MenuManager.h"

namespace Jormag {

	EngineManager* EngineManager::sInstance = NULL;

	EngineManager* EngineManager::Instance() {
		if (sInstance == NULL)
			sInstance = new EngineManager();

		return sInstance;
	}

	void EngineManager::Free() {
		delete sInstance;
		sInstance = NULL;
	}

	EngineManager::EngineManager() {
		mFramesPerSec = 0;
		mQuit = false;
		mPrefInterval = 16;
		mDeltaTime = 0;
		mAlphaTime = 0;
		mFramesThisSec = 0;

		mGraphics = Graphics::Instance();
		if (!Graphics::IsInitialised())
			mQuit = true;
	
		mSettingsMgr = SettingsManager::Instance();
		mResourceMgr = ResourceManager::Instance();
		mInputMgr = InputManager::Instance();
		mAudioMgr = AudioManager::Instance();
		mCollisionMgr = CollisionManager::Instance();
		mCamera = Camera::Instance();
	}

	EngineManager::~EngineManager() {
		mCurrentGameState = NULL;

		Camera::Free();
		mCamera = NULL;

		CollisionManager::Free();
		mCollisionMgr = NULL;

		ResourceManager::Free();
		mResourceMgr = NULL;

		AudioManager::Free();
		mAudioMgr = NULL;

		InputManager::Free();
		mInputMgr = NULL;

		SettingsManager::Free();
		mSettingsMgr = NULL;

		Graphics::Free();
		mGraphics = NULL;
	}

	void EngineManager::Tick(unsigned int deltaTime) {
		SDL_Event events;
		SDL_PollEvent(&events);
		if (events.type == SDL_QUIT) {
			QuitGame();
			return;
		}
		mInputMgr->Tick();
		if (mCurrentGameState != NULL)
			mCurrentGameState->Tick(deltaTime);

		mCamera->Tick(deltaTime);
		mInputMgr->FinaliseTick();
	}

	void EngineManager::Render() {
		mGraphics->Render(mCurrentGameState);
	}

	StateManager* EngineManager::GetCurrentMode() {
		return mCurrentGameState;
	}

	void EngineManager::SetCurrentMode(StateManager* newState) {
		mCurrentGameState = newState;
	}

	void EngineManager::InitGameLoop() {
		mLastTime = SDL_GetTicks();
		while (!mQuit) {
			Uint32 thisTime = SDL_GetTicks();
			int diffTime = thisTime - mLastTime;

			mAlphaTime += diffTime;
			if (mAlphaTime >= 1000) {
				mAlphaTime -= 1000;

				mFramesPerSec = mFramesThisSec;
				mFramesThisSec = 0;
			}

			mDeltaTime += diffTime;
			if (mDeltaTime >= mPrefInterval) {
				mFramesThisSec++;

				Tick(mDeltaTime);
				Render();

				mDeltaTime -= mPrefInterval;
			}
			mLastTime = thisTime;
		}
	}

	void EngineManager::QuitGame() {
		AudioManager::Instance()->StopMusic();
		mSettingsMgr->SaveToFile();
		mQuit = true;
	}
}