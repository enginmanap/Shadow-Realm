#include "GameLossScreen.h"
#include "GameEntity.h"
#include "Scripts.h"

GameLossScreen::GameLossScreen() : GameObject() {
	Camera::Instance()->SetCentre(GetPos(WORLD));
	mScreenWidth = Graphics::Instance()->mScreenWidth;
	mScreenHeight = Graphics::Instance()->mScreenHeight;

	// Content
	mBackground = new Texture("Textures/GUI/TitleBackground.png");
	mBackground->SetParent(this);
	mBackground->SetCentered(true);

	mLossLabel = new Texture("YOU DIED!", "Title.ttf", 169, { 255,255,255 });
	mLossLabel->SetParent(this);
	mLossLabel->SetPos(Vector2(0.0f, mScreenHeight * -0.35f));
	mLossLabel->SetCentered(true);

	mDeathLabel = nullptr;

	mButtons = InitButtons();
	mButtons->mContentPane->SetParent(this);
	mButtons->mContentPane->SetPos(Vector2(0.0f, mScreenHeight * 0.05f));

	// Selection
	mInput = InputManager::Instance();
	mIndexFocused = 0;
}

GameLossScreen::~GameLossScreen() {
	mInput = NULL;

	delete mBackground;
	mBackground = NULL;

	delete mLossLabel;
	mLossLabel = NULL;

	delete mDeathLabel;
	mDeathLabel = NULL;

	delete mButtons;
	mButtons = NULL;
}

void GameLossScreen::Tick(unsigned int deltaTime) {
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

void GameLossScreen::Render(SDL_Renderer* renderer) {
	mBackground->Render(renderer);
	mLossLabel->Render(renderer);
	mDeathLabel->Render(renderer);

	for (int i = 0; i < mButtons->mNumChildren; i++) {
		mButtons->mChildren[i]->Render(renderer);
	}
}

Menu* GameLossScreen::InitButtons() {
	Menu* menu = new Menu(2);

	GameObject* contentPane = new GameObject();
	contentPane->SetParent(this);

	GUIButton* retryButton = new GUIButton();
	retryButton->SetPos(Vector2(0.0f, mScreenHeight * -0.125f));
	retryButton->SetLabel("Retry", "Main.ttf", 21, { 255,255,255 });
	retryButton->SetOnConfirmAction(std::bind(ButtonScripts::RetryLevel));
	retryButton->SetParent(contentPane);

	GUIButton* menuButton = new GUIButton();
	menuButton->SetPos(Vector2(0.0f, 0.0f));
	menuButton->SetLabel("Return to Menu", "Main.ttf", 21, { 255,255,255 });
	menuButton->SetOnConfirmAction(std::bind(ButtonScripts::QuitToMenu));
	menuButton->SetParent(contentPane);

	menu->mContentPane = contentPane;
	menu->mChildren[0] = retryButton;
	menu->mChildren[1] = menuButton;

	return menu;
}

void GameLossScreen::SetCauseOfDeath(DeathType causeOfDeath) {
	std::string deathMsg = "Cause of Death: ";
	switch (causeOfDeath) {
	case NONE:
		deathMsg.append("ERR_NOT_DEAD");
		break;
	case ATTACK:
		deathMsg.append("Overly friendy to a rather unfriendly being.");
		break;
	case CRUSH:
		deathMsg.append("Plane switching is not an overpowered superpower.");
		break;
	case FALL:
		deathMsg.append("Slipped through the fabric of reality.");
		break;
	default:
		deathMsg.append("ERR_UNKNOWN_TYPE");
		break;
	}

	mDeathLabel = new Texture(deathMsg, "Main.ttf", 32, { 255,255,255 });
	mDeathLabel->SetParent(this);
	mDeathLabel->SetPos(Vector2(0.0f, mScreenHeight * -0.225f));
	mDeathLabel->SetCentered(true);
}