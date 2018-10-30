#include "GUIObject.h"

namespace Jormag {

	GUIObject::GUIObject(std::string backTexturePath) : GameObject() {
		mBackgroundTexture = new Texture(backTexturePath);
		mBackgroundTexture->SetParent(this);
		mBackgroundTexture->SetCentered(true);
		mFocusedTexture = nullptr;
		mLabel = nullptr;

		mIncrementFunc = nullptr;
		mDecrementFunc = nullptr;
		mConfirmFunc = nullptr;

		mIsFocused = false;
	}
	GUIObject::GUIObject(std::string backTexturePath, int x, int y, int w, int h) : GameObject() {
		mBackgroundTexture = new Texture(backTexturePath, x, y, w, h);
		mBackgroundTexture->SetParent(this);
		mBackgroundTexture->SetCentered(true);
		mFocusedTexture = nullptr;
		mLabel = nullptr;

		mIncrementFunc = nullptr;
		mDecrementFunc = nullptr;
		mConfirmFunc = nullptr;

		mIsFocused = false;
	}

	GUIObject::~GUIObject() {
		delete mBackgroundTexture;
		mBackgroundTexture = NULL;

		delete mFocusedTexture;
		mFocusedTexture = NULL;

		delete mLabel;
		mLabel = NULL;

		mIncrementFunc = NULL;
		mDecrementFunc = NULL;
		mConfirmFunc = NULL;
	}

	void GUIObject::Render(SDL_Renderer* renderer) {
		if (mIsFocused)
			mFocusedTexture->Render(renderer);
		else
			mBackgroundTexture->Render(renderer);

		if (mLabel != nullptr)
			mLabel->Render(renderer);
	}

	void GUIObject::DoIncrementAction() {
		if (mIncrementFunc == nullptr) return;

		AudioManager::Instance()->PlaySFX("Sounds/SFX/UI/Click.ogg", 0, -1);
		mIncrementFunc();
	}
	void GUIObject::DoDecrementAction() {
		if (mDecrementFunc == nullptr) return;

		AudioManager::Instance()->PlaySFX("Sounds/SFX/UI/Click.ogg", 0, -1);
		mDecrementFunc();
	}
	void GUIObject::DoConfirmAction() {
		if (mConfirmFunc == nullptr) return;

		AudioManager::Instance()->PlaySFX("Sounds/SFX/UI/Click.ogg", 0, -1);
		mConfirmFunc();
	}

	void GUIObject::SetLabel(std::string text, std::string fontPath, int size, SDL_Color color) {
		mLabel = new Texture(text, fontPath, size, color);
		mLabel->SetParent(this);
		mLabel->SetCentered(true);
	}

	void GUIObject::SetOnIncrementAction(std::function<void()> incrementFunc) {
		mIncrementFunc = incrementFunc;
	}
	void GUIObject::SetOnDecrementAction(std::function<void()> decrementFunc) {
		mDecrementFunc = decrementFunc;
	}
	void GUIObject::SetOnConfirmAction(std::function<void()> confirmFunc) {
		mConfirmFunc = confirmFunc;
	}

	bool GUIObject::IsFocused() {
		return mIsFocused;
	}
	void GUIObject::SetFocused(bool isFocused) {
		mIsFocused = isFocused;
	}

	int GUIObject::GetCollisionRadius() {
		int width = mBackgroundTexture->GetWidth();
		int height = mBackgroundTexture->GetHeight();
		if (width > height)
			return width;
		else
			return height;
	}

	SDL_Rect GUIObject::GetCollisionBox() {
		Vector2 pos = GetPos(WORLD);
		return { (int)pos.x, (int)pos.y, mBackgroundTexture->GetWidth(), mBackgroundTexture->GetHeight() };
	}
}
