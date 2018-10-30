#include <ctime>

#include "GameManager.h"

GameManager* GameManager::sInstance = NULL;

GameManager* GameManager::Instance() {
	if (sInstance == NULL)
		sInstance = new GameManager();

	return sInstance;
}

void GameManager::Free() {
	delete sInstance;
	sInstance = NULL;
}

GameManager::GameManager() {
	mEngine = EngineManager::Instance();

	SetGameState(MENU);
}

GameManager::~GameManager() {
	if (mMario != NULL) {
		delete mMario;
		mMario = NULL;
	}
	
	delete mGame;
	mGame = NULL;
	
	delete mMenu;
	mMenu = NULL;

	mEngine->Free();
	mEngine = NULL;
}

MenuManager* GameManager::GetMenuState() {
	if (mMenu == NULL)
		mMenu = new MenuManager();
	return mMenu;
}

LevelManager* GameManager::GetLevelState() {
	if (mGame == NULL)
		mGame = new LevelManager();
	return mGame;
}

void GameManager::SetGameState(States state) {
	mMario = NULL;
	switch (state) {
	case MENU:
		if (mMenu == NULL)
			mMenu = new MenuManager();
		mEngine->SetCurrentMode(mMenu);
		break;
	case GAME:
		if (mGame == NULL)
			mGame = new LevelManager();
		mEngine->SetCurrentMode(mGame);
		break;
	case MARIO:
		mMario = new MarioManager();
		mEngine->SetCurrentMode(mMario);
	default:
		break;
	}
}

std::string GameManager::GetDate() {
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);

	char buffer[24];
	strftime(buffer, sizeof(buffer), "%d-%m-%y %H:%M:%S", tm);
	std::string date(buffer);

	std::cout << date << std::endl;

	return date;
}

void GameManager::NewSave(int slot) {
	std::stringstream path;
	path << SDL_GetBasePath() << "Assets/Data/Save" << std::to_string(slot) << ".dat";
	std::ofstream saveFile(path.str());

	saveFile << GetDate() << std::endl;
	saveFile << "-1" << std::endl;
	saveFile << "0" << std::endl;
	saveFile.close();

	LoadSave(slot);
}

void GameManager::LoadSave(int slot) {
	std::stringstream path;
	path << SDL_GetBasePath() << "Assets/Data/Save" << std::to_string(slot) << ".dat";
	std::ifstream saveFile(path.str());
	if (!saveFile) {
		saveFile.close();
		NewSave(slot);
	} else {
		if (!saveFile.good()) {
			std::cerr << "Could not load save." << std::endl;
			return;
		}
		std::string lastLevelCompleted;
		std::string totalDeaths;

		std::getline(saveFile, mSaveDate);
		std::getline(saveFile, lastLevelCompleted);
		std::getline(saveFile, totalDeaths);

		if (mGame == NULL)
			mGame = new LevelManager();
		mGame->SetSaveSlot(slot);
		mGame->SetNextLevel(stoi(lastLevelCompleted) + 1);
		mTotalDeaths = stoi(totalDeaths);

		saveFile.close();
	}
}

void GameManager::SaveGame(int slot) {
	std::stringstream path;
	path << SDL_GetBasePath() << "Assets/Data/Save" << std::to_string(slot) << ".dat";
	std::ofstream saveFile(path.str());

	saveFile << GetDate() << std::endl;
	if (mGame == NULL)
		saveFile << -1 << std::endl;
	else 
		saveFile << std::to_string(mGame->GetLevel()->GetLevelNum()) << std::endl;
	saveFile << std::to_string(mTotalDeaths) << std::endl;
	saveFile.close();
}

std::string GameManager::GetLastSaveDate() {
	return mSaveDate;
}

int GameManager::GetTotalDeaths() {
	return mTotalDeaths;
}

void GameManager::AddDeathToTotal() {
	mTotalDeaths++;
}