#include "GameWinScreen.h"
#include "Scripts.h"

GameWinScreen::GameWinScreen() : GameObject() {
	Camera::Instance()->SetCentre(GetPos(WORLD));
	mScreenWidth = Graphics::Instance()->mScreenWidth;
	mScreenHeight = Graphics::Instance()->mScreenHeight;

	// Content
	mBackground = new Texture("Textures/GUI/TitleBackground.png");
	mBackground->SetParent(this);
	mBackground->SetCentered(true);

	mWinLabel = new Texture("LEVEL WIN!", "Title.ttf", 169, { 255,255,255 });
	mWinLabel->SetParent(this);
	mWinLabel->SetPos(Vector2(0.0f, mScreenHeight * -0.35f));
	mWinLabel->SetCentered(true);

	mScoreLabel = nullptr;

	mButtons = InitButtons();
	mButtons->mContentPane->SetParent(this);
	mButtons->mContentPane->SetPos(Vector2(0.0f, mScreenHeight * 0.05f));

	// Selection
	mInput = InputManager::Instance();
	mIndexFocused = 0;
}

GameWinScreen::~GameWinScreen() {
	mInput = NULL;

	delete mBackground;
	mBackground = NULL;

	delete mWinLabel;
	mWinLabel = NULL;

	delete mScoreLabel;
	mScoreLabel = NULL;

	delete mButtons;
	mButtons = NULL;
}

void GameWinScreen::Tick(unsigned int deltaTime) {
	// Input
	if (mInput->KeyPressed(SDL_SCANCODE_UP)) {
		mIndexFocused--;
		AudioManager::Instance()->PlaySFX("Sounds/SFX/UI/Rollover.ogg", 0, -1);
	}
	if (mInput->KeyPressed(SDL_SCANCODE_DOWN)) {
		mIndexFocused++;
		AudioManager::Instance()->PlaySFX("Sounds/SFX/UI/Rollover.ogg", 0, -1);
	}

	// Wrap selection
	if (mIndexFocused < 0)
		mIndexFocused += mButtons->mNumChildren;
	else if (mIndexFocused >= mButtons->mNumChildren)
		mIndexFocused -= mButtons->mNumChildren;

	mButtons->SetObjectFocus(mIndexFocused);

	// Interact
	if (mInput->KeyPressed(SDL_SCANCODE_RETURN))
		mButtons->mChildren[mIndexFocused]->DoConfirmAction();
	else if (mInput->KeyPressed(SDL_SCANCODE_RIGHT))
		mButtons->mChildren[mIndexFocused]->DoIncrementAction();
	else if (mInput->KeyPressed(SDL_SCANCODE_LEFT))
		mButtons->mChildren[mIndexFocused]->DoDecrementAction();

	// Tick Menu
	for (int i = 0; i < mButtons->mNumChildren; i++) {
		mButtons->mChildren[i]->Tick(deltaTime);
	}
}

void GameWinScreen::Render(SDL_Renderer* renderer) {
	mBackground->Render(renderer);
	mWinLabel->Render(renderer);
	mScoreLabel->Render(renderer);

	for (int i = 0; i < mButtons->mNumChildren; i++) {
		mButtons->mChildren[i]->Render(renderer);
	}
}

Menu* GameWinScreen::InitButtons() {
	Menu* menu = new Menu(3);

	GameObject* contentPane = new GameObject();
	contentPane->SetParent(this);

	GUIButton* continueButton = new GUIButton();
	continueButton->SetPos(Vector2(0.0f, mScreenHeight * -0.125f));
	continueButton->SetLabel("Continue", "Main.ttf", 21, { 255,255,255 });
	continueButton->SetOnConfirmAction(std::bind(ButtonScripts::NextLevel));
	continueButton->SetParent(contentPane);

	GUIButton* retryButton = new GUIButton();
	retryButton->SetPos(Vector2(0.0f, 0.0f));
	retryButton->SetLabel("Retry", "Main.ttf", 21, { 255,255,255 });
	retryButton->SetOnConfirmAction(std::bind(ButtonScripts::RetryLevel));
	retryButton->SetParent(contentPane);

	GUIButton* menuButton = new GUIButton();
	menuButton->SetPos(Vector2(0.0f, mScreenHeight * 0.125f));
	menuButton->SetLabel("Return to Menu", "Main.ttf", 21, { 255,255,255 });
	menuButton->SetOnConfirmAction(std::bind(ButtonScripts::QuitToMenu));
	menuButton->SetParent(contentPane);

	menu->mContentPane = contentPane;
	menu->mChildren[0] = continueButton;
	menu->mChildren[1] = retryButton;
	menu->mChildren[2] = menuButton;

	return menu;
}

void GameWinScreen::SetFinalScore(int finalScore) {
	mScoreLabel = new Texture("Score: " + std::to_string(finalScore), "Main.ttf", 32, { 255,255,255 });
	mScoreLabel->SetParent(this);
	mScoreLabel->SetPos(Vector2(0.0f, mScreenHeight * -0.225f));
	mScoreLabel->SetCentered(true);
}