#include "TitleScreen.h"

TitleScreen::TitleScreen() : GameObject(Vector2(Graphics::Instance()->mScreenWidth, Graphics::Instance()->mScreenHeight) * 0.5f) {
	mBackground = new Texture("Textures/GUI/TitleBackground.png");
	mBackground->SetParent(this);
	mBackground->SetCentered(true);

	mLogo = new Texture("Textures/Logo.png");
	mLogo->SetParent(mBackground);
	mLogo->SetCentered(true);

	mPressText = new Texture("Press any key to begin...", "Title.ttf", 72, { 255,255,255 });
	mPressText->SetParent(mBackground);

	mPressTextOpacity = 255;
	mPressTextFadingOut = true;
}

TitleScreen::~TitleScreen() {
	delete mBackground;
	mBackground = NULL;

	delete mLogo;
	mLogo = NULL;

	delete mPressText;
	mPressText = NULL;
}

void TitleScreen::Tick(unsigned int deltaTime) {
	if (!mActive) { return; }

	int changeThisTick = (int)((510 / (5000 / 1000.0f)) * (deltaTime / 1000.0f));
	if(mPressTextFadingOut == true)
		mPressTextOpacity -= changeThisTick;
	else
		mPressTextOpacity += changeThisTick;
	
	mPressText->SetOpacity(mPressTextOpacity);

	if (mPressTextOpacity <= 0) 
		mPressTextFadingOut = false;
	else if (mPressTextOpacity >= 255) 
		mPressTextFadingOut = true;
}

void TitleScreen::Render(SDL_Renderer* renderer) {
	mBackground->Render(renderer);

	mLogo->Render(renderer);
	mPressText->Render(renderer);
}
