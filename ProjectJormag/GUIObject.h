#pragma once

#ifndef GUIOBJECT_H
#define GUIOBJECT_H

#include "AudioManager.h"
#include "AnimatedTexture.h"

#include <functional>

namespace Jormag {
	class GUIObject : public GameObject {
	protected:
		Texture* mBackgroundTexture;
		Texture* mFocusedTexture;
		Texture* mLabel;

		std::function<void()> mIncrementFunc;
		std::function<void()> mDecrementFunc;
		std::function<void()> mConfirmFunc;

		bool mIsFocused;
	public:
		GUIObject(std::string backTexturePath);
		GUIObject(std::string backTexturePath, int x, int y, int w, int h);
		~GUIObject();

		virtual void Tick(unsigned int deltaTime) = 0;
		virtual void Render(SDL_Renderer* renderer);

		virtual void DoIncrementAction();
		virtual void DoDecrementAction();
		virtual void DoConfirmAction();

		virtual void SetLabel(std::string text, std::string fontPath, int size, SDL_Color color);

		void SetOnIncrementAction(std::function<void()> incrementFunc);
		void SetOnDecrementAction(std::function<void()> decrementFunc);
		void SetOnConfirmAction(std::function<void()> confirmFunc);

		bool IsFocused();
		void SetFocused(bool isFocused);

		virtual int GetCollisionRadius();
		virtual SDL_Rect GetCollisionBox();
	};
}

#endif