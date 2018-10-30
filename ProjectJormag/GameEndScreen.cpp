#include "GameEndScreen.h"
#include "Scripts.h"

GameEndScreen::GameEndScreen() : GameObject() {
	Camera::Instance()->SetCentre(GetPos(WORLD));
	mScreenWidth = Graphics::Instance()->mScreenWidth;
	mScreenHeight = Graphics::Instance()->mScreenHeight;

	// Content
	mBackground = new Texture("Textures/GUI/TitleBackground.png");
	mBackground->SetParent(this);
	mBackground->SetCentered(true);

	mEndLabel = new Texture("GAME COMPLETE!", "Title.ttf", 201, { 255,255,255 });
	mEndLabel->SetParent(this);
	mEndLabel->SetPos(Vector2(0.0f, mScreenHeight * -0.3f));
	mEndLabel->SetCentered(true);

	mDeathsLabel = nullptr;

	mQuitButton = new GUIButton();
	mQuitButton->SetParent(this);
	mQuitButton->SetPos(V2_ZERO);
	mQuitButton->SetLabel("Exit to Menu", "Main.ttf", 21, { 255,255,255 });
	mQuitButton->SetOnConfirmAction(std::bind(ButtonScripts::QuitToMenu));

	// Selection
	mInput = InputManager::Instance();
}

GameEndScreen::~GameEndScreen() {
	mInput = NULL;

	delete mBackground;
	mBackground = NULL;

	delete mEndLabel;
	mEndLabel = NULL;

	delete mDeathsLabel;
	mDeathsLabel = NULL;

	delete mQuitButton;
	mQuitButton = NULL;
}

void GameEndScreen::Tick(unsigned int deltaTime) {
	mQuitButton->SetFocused(true);

	// Interact
	if (mInput->KeyPressed(SDL_SCANCODE_RETURN))
		mQuitButton->DoConfirmAction();
	else if (mInput->KeyPressed(SDL_SCANCODE_RIGHT))
		mQuitButton->DoIncrementAction();
	else if (mInput->KeyPressed(SDL_SCANCODE_LEFT))
		mQuitButton->DoDecrementAction();

	mQuitButton->Tick(deltaTime);
}

void GameEndScreen::Render(SDL_Renderer* renderer) {
	mBackground->Render(renderer);
	mEndLabel->Render(renderer);
	mQuitButton->Render(renderer);
}

void GameEndScreen::SetDeaths(int totalDeaths) {
	mDeathsLabel = new Texture("Total Deaths: " + std::to_string(totalDeaths), "Main.ttf", 32, { 255,255,255 });
	mDeathsLabel->SetParent(this);
	mDeathsLabel->SetPos(Vector2(0.0f, mScreenHeight * -0.125f));
	mDeathsLabel->SetCentered(true);
}