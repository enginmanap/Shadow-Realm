#include <string>

#include "InputManager.h"

namespace Jormag {

	InputManager* InputManager::sInstance = NULL;

	InputManager* InputManager::Instance() {
		if (sInstance == NULL)
			sInstance = new InputManager();

		return sInstance;
	}

	void InputManager::Free() {
		delete sInstance;
		sInstance = NULL;
	}

	InputManager::InputManager() {
		mKeyboardState = SDL_GetKeyboardState(&mKeyLength);
		mLastKeyboardState = new Uint8[mKeyLength];
		memcpy(mLastKeyboardState, mKeyboardState, mKeyLength);
	}

	InputManager::~InputManager() {
		delete[] mLastKeyboardState;
		mLastKeyboardState = NULL;
	}

	void InputManager::Tick() {
		mMouseState = SDL_GetMouseState(&mMousePosX, &mMousePosY);
	}

	void InputManager::FinaliseTick() {
		memcpy(mLastKeyboardState, mKeyboardState, mKeyLength);
		mLastMouseState = mMouseState;
	}

	bool InputManager::KeyDown(SDL_Scancode scanCode) {
		return (mKeyboardState[scanCode] != 0);
	}

	bool InputManager::KeyPressed(SDL_Scancode scanCode) {
		return (mLastKeyboardState[scanCode]) == 0 && (mKeyboardState[scanCode] != 0);
	}

	bool InputManager::KeyReleased(SDL_Scancode scanCode) {
		return (mLastKeyboardState[scanCode] != 0) && (mKeyboardState[scanCode] == 0);
	}

	bool InputManager::AnyKeyHeld() {
		for (int i = 0; i < mKeyLength; i++) {
			if (mKeyboardState[i] != 0)
				return true;
		}
		return false;
	}

	bool InputManager::AnyKeyPressed() {
		for (int i = 0; i < mKeyLength; i++) {
			if (mLastKeyboardState[i] == 0 && mKeyboardState[i] != 0)
				return true;
		}
		return false;
	}

	bool InputManager::AnyKeyReleased() {
		for (int i = 0; i < mKeyLength; i++) {
			if (mLastKeyboardState[i] != 0 && mKeyboardState[i] == 0)
				return true;
		}
		return false;
	}

	bool InputManager::MouseButtonDown(MouseButton button) {
		Uint32 mask = 0;

		switch (button) {
		case LEFT:
			mask = SDL_BUTTON_LMASK;
			break;
		case RIGHT:
			mask = SDL_BUTTON_RMASK;
			break;
		case MIDDLE:
			mask = SDL_BUTTON_MMASK;
			break;
		case BACK:
			mask = SDL_BUTTON_X1MASK;
			break;
		case FORWARD:
			mask = SDL_BUTTON_X2MASK;
			break;
		}

		return ((mMouseState & mask) != 0);
	}

	bool InputManager::MouseButtonPressed(MouseButton button) {
		Uint32 mask = 0;
		switch (button) {
		case LEFT:
			mask = SDL_BUTTON_LMASK;
			break;
		case RIGHT:
			mask = SDL_BUTTON_RMASK;
			break;
		case MIDDLE:
			mask = SDL_BUTTON_MMASK;
			break;
		case BACK:
			mask = SDL_BUTTON_X1MASK;
			break;
		case FORWARD:
			mask = SDL_BUTTON_X2MASK;
			break;
		}

		return ((mLastMouseState & mask) == 0) && ((mMouseState & mask) != 0);
	}

	bool InputManager::MouseButtonReleased(MouseButton button) {
		Uint32 mask = 0;
		switch (button) {
		case LEFT:
			mask = SDL_BUTTON_LMASK;
			break;
		case RIGHT:
			mask = SDL_BUTTON_RMASK;
			break;
		case MIDDLE:
			mask = SDL_BUTTON_MMASK;
			break;
		case BACK:
			mask = SDL_BUTTON_X1MASK;
			break;
		case FORWARD:
			mask = SDL_BUTTON_X2MASK;
			break;
		}

		return ((mLastMouseState & mask) != 0) && ((mMouseState & mask) == 0);
	}

	Vector2 InputManager::GetMousePos() {
		return Vector2((float)mMousePosX, (float)mMousePosY);
	}
}
