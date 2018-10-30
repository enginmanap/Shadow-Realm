#pragma once

#ifndef ENGINEMANAGER_H
#define ENGINEMANAGER_H

#include "InputManager.h"
#include "AudioManager.h"
#include "CollisionManager.h"
#include "Texture.h"

class StateManager;

namespace Jormag {

	class EngineManager {
	private:
		// Singleton instance
		static EngineManager* sInstance;

		// Game Loop Variables
		bool mQuit;
		unsigned int mPrefInterval;
		Uint32 mLastTime;
		unsigned int mDeltaTime;
		unsigned int mAlphaTime;
		unsigned int mFramesThisSec;
		unsigned int mFramesPerSec;

		// Internal Managers
		SettingsManager* mSettingsMgr;
		Graphics* mGraphics;
		ResourceManager* mResourceMgr;
		InputManager* mInputMgr;
		AudioManager* mAudioMgr;
		CollisionManager* mCollisionMgr;
		Camera* mCamera;

		// Runtime
		StateManager* mCurrentGameState;
	private:
		EngineManager();
		~EngineManager();

		void Tick(unsigned int deltaTime);
		void Render();
	public:
		static EngineManager* Instance();
		static void Free();

		StateManager* GetCurrentMode();
		void SetCurrentMode(StateManager* newMode);

		void InitGameLoop();

		void QuitGame();
	};
}

#endif;