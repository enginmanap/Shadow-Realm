#include "MainMenu.h"
#include "GameManager.h"
#include "Scripts.h"

#define PAN_TIME 750

MainMenu::MainMenu() : GameObject() {
	mCamera = Camera::Instance();
	
	// Settings
	mResSelectText = new std::stringstream();
	mFullscreenSelectText = new std::stringstream();
	mRatioSelectText = new std::stringstream();

	Init();

	// Selection
	mInput = InputManager::Instance();
	mFocusedMenu = mTopMenu;
	mIndexFocused = 0;
}

MainMenu::~MainMenu() {
	mCamera = NULL;
	mInput = NULL;

	Reset();

	delete mResSelectText;
	mResSelectText = NULL;

	delete mFullscreenSelectText;
	mFullscreenSelectText = NULL;

	delete mRatioSelectText;
	mRatioSelectText = NULL;
}

void MainMenu::Tick(unsigned int deltaTime) {
	// Skip animation if necessary
	if (!mCamera->IsAtDestination()) {
		if (mInput->KeyPressed(SDL_SCANCODE_RETURN) || mInput->KeyPressed(SDL_SCANCODE_LEFT) ||
			mInput->KeyPressed(SDL_SCANCODE_RIGHT) || mInput->KeyPressed(SDL_SCANCODE_UP) ||
			mInput->KeyPressed(SDL_SCANCODE_DOWN)) // TODO WORKING any key pressed method
			mCamera->SetPanningTime(0);
	} else {
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
			mIndexFocused += mFocusedMenu->mNumChildren;
		else if (mIndexFocused >= mFocusedMenu->mNumChildren)
			mIndexFocused -= mFocusedMenu->mNumChildren;
		
		mFocusedMenu->SetObjectFocus(mIndexFocused);

		// Interact
		if (mInput->KeyPressed(SDL_SCANCODE_RETURN))
			mFocusedMenu->mChildren[mIndexFocused]->DoConfirmAction();
		else if (mInput->KeyPressed(SDL_SCANCODE_RIGHT))
			mFocusedMenu->mChildren[mIndexFocused]->DoIncrementAction();
		else if (mInput->KeyPressed(SDL_SCANCODE_LEFT))
			mFocusedMenu->mChildren[mIndexFocused]->DoDecrementAction();
	}
	
	// Tick menu
	for (int i = 0; i < mTopMenu->mNumChildren; i++) {
		mTopMenu->mChildren[i]->Tick(deltaTime);
	}
	for (int i = 0; i < mSaveSelectMenu->mNumChildren; i++) {
		mSaveSelectMenu->mChildren[i]->Tick(deltaTime);
	}
	for (int i = 0; i < mSettingsMenu->mNumChildren; i++) {
		mSettingsMenu->mChildren[i]->Tick(deltaTime);
	}
	for (int i = 0; i < mVideoSettingsMenu->mNumChildren; i++) {
		mVideoSettingsMenu->mChildren[i]->Tick(deltaTime);
	}
	for (int i = 0; i < mAudioSettingsMenu->mNumChildren; i++) {
		mAudioSettingsMenu->mChildren[i]->Tick(deltaTime);
	}
}

void MainMenu::Render(SDL_Renderer* renderer) {
	mBackground->Render(renderer);
	mLogo->Render(renderer);

	for (int i = 0; i < mTopMenu->mNumChildren; i++) {
		mTopMenu->mChildren[i]->Render(renderer);
	}
	for (int i = 0; i < mSaveSelectMenu->mNumChildren; i++) {
		mSaveSelectMenu->mChildren[i]->Render(renderer);
	}
	for (int i = 0; i < mSettingsMenu->mNumChildren; i++) {
		mSettingsMenu->mChildren[i]->Render(renderer);
	}
	for (int i = 0; i < mVideoSettingsMenu->mNumChildren; i++) {
		mVideoSettingsMenu->mChildren[i]->Render(renderer);
	}
	for (int i = 0; i < mAudioSettingsMenu->mNumChildren; i++) {
		mAudioSettingsMenu->mChildren[i]->Render(renderer);
	}
}

void MainMenu::Init() {
	mScreenWidth = Graphics::Instance()->mScreenWidth;
	mScreenHeight = Graphics::Instance()->mScreenHeight;

	// Background
	mBackground = new Texture("Textures/GUI/TitleBackground.png");
	mBackground->SetScale(Vector2(0.3f, 0.3f));
	mBackground->SetParent(this);
	mBackground->SetCentered(true);

	// Logo
	mLogo = new Texture("Textures/Logo.png");
	mLogo->SetParent(this);
	mLogo->SetCentered(true);
	mLogo->SetPos(Vector2(0.0f, mScreenHeight * -0.4f));
	mLogo->SetScale(Vector2(1 / (mLogo->GetWidth() / (mScreenWidth / 2.0f)), 1 / (mLogo->GetWidth() / (mScreenWidth / 2.0f))));

	// Settings
	mResSelectText->str("");
	mFullscreenSelectText->str("");
	mRatioSelectText->str("");

	// Menus
	mTopMenu = InitTopMenu();
	mTopMenu->mContentPane->SetPos(Vector2(0.0f, 0.0f));
	mSaveSelectMenu = InitSaveSelectMenu();
	mSaveSelectMenu->mContentPane->SetPos(Vector2(mScreenWidth * 1.0f, 0.0f));
	mSettingsMenu = InitMainSettingsMenu();
	mSettingsMenu->mContentPane->SetPos(Vector2(mScreenWidth * -1.0f, 0.0f));
	mVideoSettingsMenu = InitVideoSettingsMenu();
	mVideoSettingsMenu->mContentPane->SetPos(Vector2(mScreenWidth * -1.0f, mScreenHeight * -1.0f));
	mAudioSettingsMenu = InitAudioSettingsMenu();
	mAudioSettingsMenu->mContentPane->SetPos(Vector2(mScreenWidth * -2.0f, 0.0f));
}

void MainMenu::Reset() {
	delete mBackground;
	mBackground = NULL;

	delete mTopMenu;
	mTopMenu = NULL;

	delete mSaveSelectMenu;
	mSaveSelectMenu = NULL;

	delete mSettingsMenu;
	mSettingsMenu = NULL;

	delete mVideoSettingsMenu;
	mVideoSettingsMenu = NULL;

	delete mAudioSettingsMenu;
	mAudioSettingsMenu = NULL;

	mFocusedMenu = NULL;
}

void MainMenu::SetMenuFocus(MenuID menuID) {
	switch (menuID) {
	default:
	case TOP:
		mCamera->SetPanDestination(mTopMenu->mContentPane->GetPos(WORLD), PAN_TIME);
		mFocusedMenu = mTopMenu;
		break;
	case SAVESELECT:
		mCamera->SetPanDestination(mSaveSelectMenu->mContentPane->GetPos(WORLD), PAN_TIME);
		mFocusedMenu = mSaveSelectMenu;
		break;
	case SETTINGS:
		mCamera->SetPanDestination(mSettingsMenu->mContentPane->GetPos(WORLD), PAN_TIME);
		mFocusedMenu = mSettingsMenu;
		break;
	case VIDEO:
		mCamera->SetPanDestination(mVideoSettingsMenu->mContentPane->GetPos(WORLD), PAN_TIME);
		mFocusedMenu = mVideoSettingsMenu;
		break;
	case AUDIO:
		mCamera->SetPanDestination(mAudioSettingsMenu->mContentPane->GetPos(WORLD), PAN_TIME);
		mFocusedMenu = mAudioSettingsMenu;
		break;
	}
}

Menu* MainMenu::InitTopMenu() {
	Menu* menu = new Menu(4);
	
	GameObject* contentPane = new GameObject();
	contentPane->SetParent(this);
	
	GUIButton* newGameButton = new GUIButton();
	newGameButton->SetPos(Vector2(0.0f, mScreenHeight * -0.125f));
	newGameButton->SetLabel("New Game", "Main.ttf", 21, { 255,255,255 });
	newGameButton->SetOnConfirmAction(std::bind(&ButtonScripts::NewGame, this));
	newGameButton->SetParent(contentPane);

	GUIButton* loadGameButton = new GUIButton();
	loadGameButton->SetPos(Vector2(0.0f, 0.0f));
	loadGameButton->SetLabel("Load Game", "Main.ttf", 21, { 255,255,255 });
	loadGameButton->SetOnConfirmAction(std::bind(&ButtonScripts::LoadGame, this));
	loadGameButton->SetParent(contentPane);

	GUIButton* settingsButton = new GUIButton();
	settingsButton->SetPos(Vector2(0.0f, mScreenHeight * 0.125f));
	settingsButton->SetLabel("Settings", "Main.ttf", 21, { 255,255,255 });
	settingsButton->SetOnConfirmAction(std::bind(&ButtonScripts::SwitchMenuFocus, this, SETTINGS));
	settingsButton->SetParent(contentPane);

	GUIButton* quitButton = new GUIButton();
	quitButton->SetPos(Vector2(0.0f, mScreenHeight * 0.25f));
	quitButton->SetLabel("Quit", "Main.ttf", 21, { 255,255,255 });
	quitButton->SetOnConfirmAction(std::bind(&ButtonScripts::QuitGame));
	quitButton->SetParent(contentPane);

	menu->mContentPane = contentPane;
	menu->mChildren[0] = newGameButton;
	menu->mChildren[1] = loadGameButton;
	menu->mChildren[2] = settingsButton;
	menu->mChildren[3] = quitButton;

	return menu;
}

Menu* MainMenu::InitSaveSelectMenu() {
	Menu* menu = new Menu(5);

	std::string saveDates[4];

	for (int i = 0; i < 4; i++) {
		std::stringstream path;
		path << SDL_GetBasePath() << "Assets/Data/Save" << std::to_string(i) << ".dat";
		std::ifstream saveFile(path.str());
		if (!saveFile) {
			saveDates[i] = "null";
		} else {
			std::getline(saveFile, saveDates[i]);
		}
		saveFile.close();
	}

	GameObject* contentPane = new GameObject();
	contentPane->SetParent(this);

	GUIButton* selectOneButton = new GUIButton();
	selectOneButton->SetPos(Vector2(mScreenWidth * -0.1f, mScreenHeight * -0.1f));
	if (saveDates[0] == "null")
		selectOneButton->SetLabel("Slot One", "Main.ttf", 21, { 255,255,255 });
	else
		selectOneButton->SetLabel(saveDates[0], "Main.ttf", 21, { 255,255,255 });
	selectOneButton->SetOnConfirmAction(std::bind(&ButtonScripts::SetSaveSlot, 0, &mOverwriting));
	selectOneButton->SetParent(contentPane);

	GUIButton* selectTwoButton = new GUIButton();
	selectTwoButton->SetPos(Vector2(mScreenWidth * 0.1f, mScreenHeight * -0.1f));
	if (saveDates[1] == "null")
		selectTwoButton->SetLabel("Slot Two", "Main.ttf", 21, { 255,255,255 });
	else
		selectTwoButton->SetLabel(saveDates[1], "Main.ttf", 21, { 255,255,255 });
	selectTwoButton->SetOnConfirmAction(std::bind(&ButtonScripts::SetSaveSlot, 1, &mOverwriting));
	selectTwoButton->SetParent(contentPane);

	GUIButton* selectThreeButton = new GUIButton();
	selectThreeButton->SetPos(Vector2(mScreenWidth * -0.1f, mScreenHeight * 0.1f));
	if (saveDates[2] == "null")
		selectThreeButton->SetLabel("Slot Three", "Main.ttf", 21, { 255,255,255 });
	else
		selectThreeButton->SetLabel(saveDates[2], "Main.ttf", 21, { 255,255,255 });
	selectThreeButton->SetOnConfirmAction(std::bind(&ButtonScripts::SetSaveSlot, 2, &mOverwriting));
	selectThreeButton->SetParent(contentPane);

	GUIButton* selectFourButton = new GUIButton();
	selectFourButton->SetPos(Vector2(mScreenWidth * 0.1f, mScreenHeight * 0.1f));
	if (saveDates[3] == "null")
		selectFourButton->SetLabel("Slot Four", "Main.ttf", 21, { 255,255,255 });
	else
		selectFourButton->SetLabel(saveDates[3], "Main.ttf", 21, { 255,255,255 });
	selectFourButton->SetOnConfirmAction(std::bind(&ButtonScripts::SetSaveSlot, 3, &mOverwriting));
	selectFourButton->SetParent(contentPane);

	GUIButton* backButton = new GUIButton();
	backButton->SetPos(Vector2(0.0f, mScreenHeight * 0.3f));
	backButton->SetLabel("Back", "Main.ttf", 21, { 255,255,255 });
	backButton->SetOnConfirmAction(std::bind(&ButtonScripts::SwitchMenuFocus, this, TOP));
	backButton->SetParent(contentPane);

	menu->mContentPane = contentPane;
	menu->mChildren[0] = selectOneButton;
	menu->mChildren[1] = selectTwoButton;
	menu->mChildren[2] = selectThreeButton;
	menu->mChildren[3] = selectFourButton;
	menu->mChildren[4] = backButton;

	return menu;
}

Menu* MainMenu::InitMainSettingsMenu() {
	Menu* menu = new Menu(3);

	GameObject* contentPane = new GameObject();
	contentPane->SetParent(this);

	GUIButton* videoSettingsButton = new GUIButton();
	videoSettingsButton->SetPos(Vector2(0.0f, mScreenHeight * -0.125f));
	videoSettingsButton->SetLabel("Video", "Main.ttf", 21, { 255,255,255 });
	videoSettingsButton->SetOnConfirmAction(std::bind(&ButtonScripts::SwitchMenuFocus, this, VIDEO));
	videoSettingsButton->SetParent(contentPane);

	GUIButton* audioSettingsButton = new GUIButton();
	audioSettingsButton->SetPos(Vector2(0.0f, mScreenHeight * 0.0f));
	audioSettingsButton->SetLabel("Audio", "Main.ttf", 21, { 255,255,255 });
	audioSettingsButton->SetOnConfirmAction(std::bind(&ButtonScripts::SwitchMenuFocus, this, AUDIO));
	audioSettingsButton->SetParent(contentPane);

	GUIButton* backButton = new GUIButton();
	backButton->SetPos(Vector2(0.0f, mScreenHeight * 0.125f));
	backButton->SetLabel("Back", "Main.ttf", 21, { 255,255,255 });
	backButton->SetOnConfirmAction(std::bind(&ButtonScripts::SwitchMenuFocus, this, TOP));
	backButton->SetParent(contentPane);

	menu->mContentPane = contentPane;
	menu->mChildren[0] = videoSettingsButton;
	menu->mChildren[1] = audioSettingsButton;
	menu->mChildren[2] = backButton;

	return menu;
}

Menu* MainMenu::InitVideoSettingsMenu() {
	Menu* menu = new Menu(5);
	
	GameObject* contentPane = new GameObject();
	contentPane->SetParent(this);

	GUIButton* resSelectButton = new GUIButton();
	resSelectButton->SetPos(Vector2(0.0f, mScreenHeight * -0.125f));
	SettingsManager* settings = SettingsManager::Instance();
	*mResSelectText << "Resolution: " << settings->GetPrefScreenWidth() << "x" << settings->GetPrefScreenHeight();
	resSelectButton->SetLabel(mResSelectText->str(), "Main.ttf", 21, { 255,255,255 });
	resSelectButton->SetOnIncrementAction(std::bind(&ButtonScripts::CycleResolution, resSelectButton, mResSelectText, Direction::RIGHT));
	resSelectButton->SetOnDecrementAction(std::bind(&ButtonScripts::CycleResolution, resSelectButton, mResSelectText, Direction::LEFT));
	resSelectButton->SetOnConfirmAction(std::bind(&ButtonScripts::ConfirmResolution, this, mResSelectText));
	resSelectButton->SetParent(contentPane);

	GUIButton* fullscreenSelectButton = new GUIButton();
	fullscreenSelectButton->SetPos(Vector2(0.0f, 0.0f));
	*mFullscreenSelectText << "Fullscreen: " << SettingsManager::Instance()->GetPrefFullscreenMode();
	fullscreenSelectButton->SetLabel(mFullscreenSelectText->str(), "Main.ttf", 21, { 255,255,255 });
	fullscreenSelectButton->SetOnIncrementAction(std::bind(&ButtonScripts::CycleFullscreen, fullscreenSelectButton, mFullscreenSelectText, Direction::RIGHT));
	fullscreenSelectButton->SetOnDecrementAction(std::bind(&ButtonScripts::CycleFullscreen, fullscreenSelectButton, mFullscreenSelectText, Direction::LEFT));
	fullscreenSelectButton->SetOnConfirmAction(std::bind(&ButtonScripts::ConfirmFullscreen, this, mFullscreenSelectText));
	fullscreenSelectButton->SetParent(contentPane);

	GUIButton* ratioSelectButton = new GUIButton();
	ratioSelectButton->SetPos(Vector2(0.0f, mScreenHeight * 0.125f));
	*mRatioSelectText << "Aspect Ratio: " << SettingsManager::Instance()->GetPrefAspectRatio();
	ratioSelectButton->SetLabel(mRatioSelectText->str(), "Main.ttf", 21, { 255,255,255 });
	ratioSelectButton->SetOnIncrementAction(std::bind(&ButtonScripts::CycleRatio, ratioSelectButton, mRatioSelectText, Direction::RIGHT));
	ratioSelectButton->SetOnDecrementAction(std::bind(&ButtonScripts::CycleRatio, ratioSelectButton, mRatioSelectText, Direction::LEFT));
	ratioSelectButton->SetOnConfirmAction(std::bind(&ButtonScripts::ConfirmRatio, this, mRatioSelectText));
	ratioSelectButton->SetParent(contentPane);

	GUIButton* vSyncSelectButton = new GUIButton();
	vSyncSelectButton->SetPos(Vector2(0.0f, mScreenHeight * 0.25f));
	vSyncSelectButton->SetLabel("VSync: Off", "Main.ttf", 21, { 255,255,255 });
	// TODO Add event handlers
	vSyncSelectButton->SetParent(contentPane);

	GUIButton* backButton = new GUIButton();
	backButton->SetPos(Vector2(0.0f, mScreenHeight * 0.375f));
	backButton->SetLabel("Back", "Main.ttf", 21, {255, 255, 255});
	backButton->SetOnConfirmAction(std::bind(&ButtonScripts::SwitchMenuFocus, this, SETTINGS));
	backButton->SetParent(contentPane);

	menu->mContentPane = contentPane;
	menu->mChildren[0] = resSelectButton;
	menu->mChildren[1] = fullscreenSelectButton;
	menu->mChildren[2] = ratioSelectButton;
	menu->mChildren[3] = vSyncSelectButton;
	menu->mChildren[4] = backButton;

	return menu;
}

Menu* MainMenu::InitAudioSettingsMenu() {
	Menu* menu = new Menu(3);

	GameObject* contentPane = new GameObject();
	contentPane->SetParent(this);

	SettingsManager* settings = SettingsManager::Instance();

	GUISlider* musicSlider = new GUISlider(0.0f, 1.0f, settings->GetPrefMusicVolume());
	musicSlider->SetPos(Vector2(0.0f, mScreenHeight * -0.125f));
	musicSlider->SetLabel("Music Volume: " + std::to_string(musicSlider->GetCurrentValue()), "Main.ttf", 21, {255, 255, 255});
	musicSlider->SetOnDecrementAction(std::bind(&ButtonScripts::UpdateMusicVolume, musicSlider, -0.05));
	musicSlider->SetOnIncrementAction(std::bind(&ButtonScripts::UpdateMusicVolume, musicSlider, 0.05));
	musicSlider->SetOnConfirmAction(std::bind(&ButtonScripts::ConfirmMusicVolume, musicSlider));
	musicSlider->SetParent(contentPane);

	GUISlider* sfxSlider = new GUISlider(0.0f, 1.0f, settings->GetPrefSFXVolume());
	sfxSlider->SetPos(Vector2(0.0f, 0.0f));
	sfxSlider->SetLabel("SFX Volume: " + std::to_string(sfxSlider->GetCurrentValue()), "Main.ttf", 21, {255, 255, 255});
	sfxSlider->SetOnDecrementAction(std::bind(&ButtonScripts::UpdateSFXVolume, sfxSlider, -0.05));
	sfxSlider->SetOnIncrementAction(std::bind(&ButtonScripts::UpdateSFXVolume, sfxSlider, 0.05));
	sfxSlider->SetOnConfirmAction(std::bind(&ButtonScripts::ConfirmSFXVolume, sfxSlider));
	sfxSlider->SetParent(contentPane);

	GUIButton* backButton = new GUIButton();
	backButton->SetPos(Vector2(0.0f, mScreenHeight * 0.125f));
	backButton->SetLabel("Back", "Main.ttf", 21, { 255,255,255 });
	backButton->SetOnConfirmAction(std::bind(&ButtonScripts::SwitchMenuFocus, this, SETTINGS));
	backButton->SetParent(contentPane);

	menu->mContentPane = contentPane;
	menu->mChildren[0] = musicSlider;
	menu->mChildren[1] = sfxSlider;
	menu->mChildren[2] = backButton;

	return menu;
}

void MainMenu::SetOverwritingSave(bool overwriting) {
	mOverwriting = overwriting;
}
