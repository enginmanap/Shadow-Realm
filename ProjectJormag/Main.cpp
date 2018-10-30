#include "GameManager.h"

int main(int argc, char* argv[]) {
	GameManager* game = GameManager::Instance();
	EngineManager::Instance()->InitGameLoop();

	GameManager::Free();
	game = NULL;

	return 0;
}