#include "GUISlider.h"

namespace Jormag {

	GUISlider::GUISlider(float min, float max, float initValue) : GUIObject("Textures/GUI/ButtonBackground.png", 0, 0, 256, 128) {
		mFocusedTexture = new AnimatedTexture("Textures/GUI/ButtonBackground.png", 0, 0, 256, 128, 5, 500, AnimatedTexture::VERTICAL);
		mFocusedTexture->SetParent(this);
		mFocusedTexture->SetCentered(true);

		mMin = min;
		mMax = max;
		mCurrent = initValue;
	}

	GUISlider::GUISlider(float min, float max, float initValue, std::string backTexturePath, std::string focusTexturePath) : GUIObject(backTexturePath) {
		mFocusedTexture = new Texture(focusTexturePath);
		mFocusedTexture->SetParent(this);

		mMin = min;
		mMax = max;
		mCurrent = initValue;
	}

	GUISlider::~GUISlider() {
		return;
	}

	void GUISlider::Tick(unsigned int deltaTime) {
		mFocusedTexture->Tick(deltaTime);
	}

	void GUISlider::Render(SDL_Renderer* renderer) {
		if (mIsFocused)
			mFocusedTexture->Render(renderer);
		else
			mBackgroundTexture->Render(renderer);

		if (mLabel != nullptr)
			mLabel->Render(renderer);
	}

	float GUISlider::GetMinValue() {
		return mMin;
	}

	float GUISlider::GetMaxValue() {
		return mMax;
	}

	float GUISlider::GetCurrentValue() {
		return mCurrent;
	}

	void GUISlider::ModifyValue(float change) {
		mCurrent += change;
		if (mCurrent < mMin)
			mCurrent = mMin;
		if (mCurrent > mMax)
			mCurrent = mMax;
	}
}