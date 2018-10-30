#include "MenuManager.h"
#include "GameManager.h"

MenuManager::MenuManager() {
	AudioManager::Instance()->PlayMusic("Sounds/Music/Title.mp3", -1);
	mTitleScreen = new TitleScreen();
	mMainMenu = new MainMenu();
	mGameWinScreen = new GameWinScreen();
	mGameLossScreen = new GameLossScreen();
	mGameEndScreen = new GameEndScreen();

	mFocusedMenu = mMainMenu;
}

MenuManager::~MenuManager() {
	delete mTitleScreen;
	mTitleScreen = NULL;

	delete mMainMenu;
	mMainMenu = NULL;

	delete mGameWinScreen;
	mGameWinScreen = NULL;

	delete mGameLossScreen;
	mGameLossScreen = NULL;

	delete mGameEndScreen;
	mGameEndScreen = NULL;

	mFocusedMenu = NULL;
}

void MenuManager::Tick(unsigned int deltaTime) {
	InputManager* input = InputManager::Instance();
	if (input->KeyDown(SDL_SCANCODE_LSHIFT) &&
		input->KeyDown(SDL_SCANCODE_LALT) &&
		input->KeyPressed(SDL_SCANCODE_M))
		GameManager::Instance()->SetGameState(GameManager::MARIO);

	mFocusedMenu->Tick(deltaTime);
}

void MenuManager::Render(SDL_Renderer* renderer) {
	mFocusedMenu->Render(renderer);
}

TitleScreen* MenuManager::GetTitleScreen() {
	return mTitleScreen;
}

MainMenu* MenuManager::GetMainMenu() {
	return mMainMenu;
}

GameWinScreen* MenuManager::GetGameWinScreen() {
	return mGameWinScreen;
}

GameLossScreen* MenuManager::GetGameLossScreen() {
	return mGameLossScreen;
}

GameEndScreen* MenuManager::GetGameEndScreen() {
	return mGameEndScreen;
}

void MenuManager::SetMenu(ScreenID screen) {
	switch (screen) {
	case TITLE:
		mFocusedMenu = mTitleScreen;
		Camera::Instance()->SetCentre(V2_ZERO);
		break;
	case MAIN:
		delete mMainMenu;
		mMainMenu = new MainMenu();
		mFocusedMenu = mMainMenu;
		Camera::Instance()->SetPanningTime(0);
		break;
	case GAMEWIN:
		mFocusedMenu = mGameWinScreen;
		Camera::Instance()->SetCentre(V2_ZERO);
		break;
	case GAMELOSS:
		mFocusedMenu = mGameLossScreen;
		Camera::Instance()->SetCentre(V2_ZERO);
		break;
	case GAMEEND:
		mFocusedMenu = mGameEndScreen;
		Camera::Instance()->SetCentre(V2_ZERO);
		break;
	default:
		break;
	}
}
