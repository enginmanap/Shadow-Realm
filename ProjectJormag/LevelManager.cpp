#include "LevelManager.h"

#include "GameManager.h"

LevelManager::LevelManager() {
	mGraphics = Graphics::Instance();
	mCamera = Camera::Instance();

	mCurrLevel = new Level("Textures/Tileset.png");
	mPlayer = new Player(new AnimatedTexture("Textures/Entities/Slimer.png", 0, 0, 32, 32, 10, 1000, AnimatedTexture::AnimDir::HORIZONTAL), mCurrLevel, 1);
	mCurrLevel->SetActive(true);

	mLastCamPan = 5000;
	mScoreLabel = nullptr;
	mLevelFinishing = false;
	mScreenOpacity = 255;
}

LevelManager::~LevelManager() {
	mGraphics = NULL;

	delete mStartLabel;
	mStartLabel = NULL;

	delete mScoreLabel;
	mScoreLabel = NULL;

	delete mPlayer;
	mPlayer = NULL;

	delete mCurrLevel;
	mCurrLevel = NULL;

	mCamera = NULL;
}

void LevelManager::GetInput() {
	if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_ESCAPE)) {
		mCamera->SetCentre(GameManager::Instance()->GetMenuState()->GetMainMenu()->GetPos(GameObject::WORLD));
		GameManager::Instance()->GetMenuState()->SetMenu(MenuManager::MAIN);
		GameManager::Instance()->SetGameState(GameManager::MENU);
	}
}

void LevelManager::Tick(unsigned int deltaTime) {
	mCurrLevel->Tick(deltaTime);
	mPlayer->Tick(deltaTime);

	if (!mPlayer->IsDead()) {
		GetInput();

		mLastCamPan += deltaTime;
		if (mLastCamPan >= 5000) {
			mLastCamPan = 0;
			std::vector<GameEntity*>* entities = mCurrLevel->GetEntitiesByRange(mPlayer->GetPos(GameObject::WORLD), mGraphics->mScreenHeight * 0.4f);
			mCamera->SetPanDestination(entities->at((int)(rand() % entities->size()))->GetPos(GameObject::WORLD), 5000);
		}
	}

	if (CheckWin()) {
		GameManager* game = GameManager::Instance();
		game->GetMenuState()->GetGameWinScreen()->SetFinalScore(mCurrLevel->GetCurrentScore());
		game->GetMenuState()->SetMenu(MenuManager::GAMEWIN);
		game->SaveGame(mSaveSlotNum);
		game->SetGameState(GameManager::MENU);
		return;
	}
		
	if (CheckLoss()) {
		int changeThisTick = (int)((510 / (5000 / 1000.0f)) * (deltaTime / 1000.0f));
		mScreenOpacity -= changeThisTick;

		if (mScreenOpacity <= 0) {
			GameManager* game = GameManager::Instance();
			game->GetMenuState()->GetGameLossScreen()->SetCauseOfDeath(mPlayer->GetCauseOfDeath());
			game->GetMenuState()->SetMenu(MenuManager::GAMELOSS);

			game->AddDeathToTotal();
			game->SaveGame(mSaveSlotNum);
			game->SetGameState(GameManager::MENU);

			mScreenOpacity = 255;
			mLevelFinishing = false;
		}
		mCurrLevel->SetOpacity(mScreenOpacity);
		mPlayer->SetOpacity(mScreenOpacity);

		return;
	}
		

	if (mScoreLabel != nullptr) {
		delete mScoreLabel;
		mScoreLabel = nullptr;
	}
	mScoreLabel = new Texture("Score: " + std::to_string(mCurrLevel->GetCurrentScore()), "Main.ttf", 28, { 255,255,255 });
	mScoreLabel->SetPos(Vector2(20.0f, 20.0f) + mCamera->GetOffset());
}

void LevelManager::Render(SDL_Renderer* renderer) {
	mCurrLevel->Render(renderer, mPlayer->GetCurrentPlane());
	mPlayer->Render(renderer);

	if (mScoreLabel != nullptr)
		mScoreLabel->Render(renderer);
}

bool LevelManager::CheckWin() {
	return floor(mPlayer->GetPos(GameObject::WORLD).x / TILESIZE) == mCurrLevel->GetEndTile().x &&
		floor(mPlayer->GetPos(GameObject::WORLD).y / TILESIZE) == mCurrLevel->GetEndTile().y;
}

bool LevelManager::CheckLoss() {
	if (mPlayer->IsDead()) {
		if (!mLevelFinishing) {
			mCurrLevel->SetActive(false);
			mLevelFinishing = true;
			switch (mPlayer->GetCauseOfDeath()) {
			case NONE:
				break;
			case ATTACK:
				break;
			case CRUSH:
				// TODO: Splat - Find blood texture and replace player texture?
				break;
			case FALL:
				// TODO: Jump?
				break;
			}
		}
		return true;
	}
	return false;
}

void LevelManager::LoadNextLevel() {
	int attempts = 0;

	while (attempts < 1) {
		if (mLevelToLoad == mCurrLevel->GetLevelNum())
			mLevelToLoad++;

		std::stringstream path;
		path << SDL_GetBasePath() << "Assets/Data/Level" << mLevelToLoad << "/Header.dat";
		std::fstream headerStream(path.str());
		if (!headerStream.good()) {
			attempts++;
			mLevelToLoad++;
			continue;
		} else {
			mCurrLevel->LoadLevel(mLevelToLoad);
			mPlayer->SetPos(mCurrLevel->GetStartPos());
			return;
		}
	}
	
	mLevelToLoad = 0;
	GameManager* game = GameManager::Instance();
	game->SetGameState(GameManager::MENU);
	game->GetMenuState()->SetMenu(MenuManager::GAMEEND);
}

void LevelManager::ReloadLevel() {
	mPlayer->SetPos(mCurrLevel->GetStartPos());
	mPlayer->SetDead(false, NONE);
	mCurrLevel->LoadLevel(mLevelToLoad);
	mCurrLevel->SetActive(true);
}

void LevelManager::SetNextLevel(int levelNum) {
	mLevelToLoad = levelNum;
}

void LevelManager::SetSaveSlot(int saveSlot) {
	mSaveSlotNum = saveSlot;
}

Level* LevelManager::GetLevel() {
	return mCurrLevel;
}

Player* LevelManager::GetPlayer() {
	return mPlayer;
}