#include "GUIButton.h"

namespace Jormag {

	GUIButton::GUIButton() : GUIObject("Textures/GUI/ButtonBackground.png", 0, 0, 256, 128) {
		mFocusedTexture = new AnimatedTexture("Textures/GUI/ButtonBackground.png", 0, 0, 256, 128, 5, 500, AnimatedTexture::VERTICAL);
		mFocusedTexture->SetParent(this);
		mFocusedTexture->SetCentered(true);
	}
	GUIButton::GUIButton(std::string backTexturePath, std::string focusTexturePath) : GUIObject(backTexturePath) {
		mFocusedTexture = new Texture(focusTexturePath);
		mFocusedTexture->SetParent(this);
	}

	GUIButton::~GUIButton() {
		return;
	}

	void GUIButton::Tick(unsigned int deltaTime) {
		mFocusedTexture->Tick(deltaTime);
	}
}

