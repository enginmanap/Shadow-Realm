#pragma once

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL.h>

#include "MathHelper.h"

// *NOTE* USED INTERNET RELATIVELY HEAVILY
namespace Jormag {

	class InputManager {
	private:
		// Singleton Instance
		static InputManager* sInstance;

		// Keyboard
		Uint8* mLastKeyboardState;
		const Uint8* mKeyboardState;
		int mKeyLength;

		// Mouse
		Uint32 mLastMouseState;
		Uint32 mMouseState;
		int mMousePosX;
		int mMousePosY;
	public:
		enum MouseButton { LEFT, RIGHT, MIDDLE, BACK, FORWARD };
	private:
		InputManager();
		~InputManager();
	public:
		static InputManager* Instance();
		static void Free();

		void Tick();
		void FinaliseTick();

		bool KeyDown(SDL_Scancode scanCode);
		bool KeyPressed(SDL_Scancode scanCode);
		bool KeyReleased(SDL_Scancode scanCode);

		bool AnyKeyHeld();
		bool AnyKeyPressed();
		bool AnyKeyReleased();

		bool MouseButtonDown(MouseButton button);
		bool MouseButtonPressed(MouseButton button);
		bool MouseButtonReleased(MouseButton button);
		Vector2 GetMousePos();
	};
}

#endif