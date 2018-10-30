#include "Level.h"
#include "AIBehaviours.h"
#include "GameManager.h"
#include "Player.h"
#include "Shadow.h"

Level::Level(std::string tilesetName) : GameObject() {
	Graphics* g = Graphics::Instance();
	mScreenWidthInTiles = g->mScreenWidth / TILESIZE;
	mScreenHeightInTiles = g->mScreenHeight / TILESIZE;

	LoadTiles(tilesetName);
}

Level::~Level() {
	for (int i = 0; i < 3; i++) {
		if (mBackdrops[i] == NULL) continue;
		delete mBackdrops[i];
		mBackdrops[i] = NULL;
	}

	for (int i = 0; i < mNumShadows; i++) {
		delete mShadows[i];
		mShadows[i] = NULL;
	}
	delete[] mShadows;

	for (int i = 0; i < TOTAL_TILES; i++) {
		delete mTiles[i];
		mTiles[i] = NULL;
	}
	delete[] *mTiles;

	ClearLevel();
}

void Level::Tick(unsigned int deltaTime) {
	if (!mActive) return;

	mCurrentScore--;

	for (int i = 0; i < mNumShadows; i++)
		mShadows[i]->Tick(deltaTime);
}

void Level::Render(SDL_Renderer* renderer, int plane) {
	for (int b = 0; b < 3; b++) {
		if (mBackdrops[b] != nullptr)
			mBackdrops[b]->Render(renderer);
	}
	for (int j = 0; j < mHeight; j++) {
		for (int i = 0; i < mWidth; i++) {
			mTiles[mLevelTileData[plane][i][j]]->mTexture->Render(renderer, Vector2((float)i * TILESIZE, (float)j * TILESIZE));
		}
	}

	for (int i = 0; i < mNumShadows; i++) {
		mShadows[i]->Render(renderer);
	}
}

// File IO
void Level::LoadLevel(std::string rootPath) {
	ClearLevel();

	LoadHeader(rootPath);
	LoadLevelTileData(rootPath);
}

void Level::LoadLevel(int level) {
	ClearLevel();

	std::stringstream rootPath;
	rootPath << "Data/Level" << level;
	LoadHeader(rootPath.str());
	LoadBackdrops(rootPath.str());
	LoadLevelTileData(rootPath.str());
	LoadShadows(rootPath.str());

	Graphics* graphics = Graphics::Instance();
	Camera::Instance()->SetOffset(V2_ZERO);
	std::vector<GameEntity*>* entities = GetEntitiesByRange(GetStartPos(), graphics->mScreenHeight * 0.4f);
	Camera::Instance()->SetPanDestination(entities->at((int)(rand() % entities->size()))->GetPos(GameObject::WORLD), 5000);
}

void Level::ClearLevel() {
	mLevelNum = 0;
	mWidth = 0;
	mHeight = 0;

	for (int i = 0; i < mWidth * mHeight * mNumPlanes; i++) {
		delete[] mLevelTileData[i];
		mLevelTileData[i] = NULL;
	}
	delete[] mLevelTileData;
}

void Level::LoadTiles(std::string setPath) {
	std::string path = SDL_GetBasePath();
	path.append("Assets/Data/Collisions.dat");

	std::fstream fileStream(path);
	std::string buffer;
	fileStream.seekg(0, std::ios::end);
	buffer.resize((int)fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(&buffer[0], buffer.size());
	fileStream.close();

	for (int i = 0; i < TOTAL_TILES; i++) {
		mTiles[i] = new Tile(setPath, i * TILESIZE, 0, (Collisions::Type)(buffer[i] - '0'));
		mTiles[i]->SetParent(this);
	}
}

void Level::LoadHeader(std::string rootPath) {
	std::string buffer;
	int lastIndexParsed;

	std::stringstream headerPath;
	headerPath << SDL_GetBasePath() << "Assets/" << rootPath << "/Header.dat";

	std::fstream headerStream(headerPath.str().c_str(), std::ios::in | std::ios::binary);
	headerStream.seekg(0, std::ios::end);
	buffer.resize((int)headerStream.tellg());
	headerStream.seekg(0, std::ios::beg);
	headerStream.read(&buffer[0], buffer.size());
	headerStream.close();

	lastIndexParsed = 0;
	mLevelNum = ParseInt(buffer, lastIndexParsed);
	mWidth = ParseInt(buffer, lastIndexParsed);
	mHeight = ParseInt(buffer, lastIndexParsed);
	mNumPlanes = ParseInt(buffer, lastIndexParsed);
	mNumShadows = ParseInt(buffer, lastIndexParsed);
	mCurrentScore = ParseInt(buffer, lastIndexParsed);

	int startX = ParseInt(buffer, lastIndexParsed);
	int startY = ParseInt(buffer, lastIndexParsed);
	mStartPos = Vector2((float)startX, (float)startY);

	int endX = ParseInt(buffer, lastIndexParsed);
	int endY = ParseInt(buffer, lastIndexParsed);
	mEndPos = Vector2((float)endX, (float)endY);
}

void Level::LoadBackdrops(std::string rootPath) {
	int fillWidthScale, fillHeightScale;
	
	std::stringstream worldDropPath;
	worldDropPath << rootPath << "/Backdrop-1.png";
	mBackdrops[0] = new Texture(worldDropPath.str());
	fillWidthScale = (float)mWidth * TILESIZE / (float)mBackdrops[0]->GetWidth();
	fillHeightScale = (float)mHeight * TILESIZE / (float)mBackdrops[0]->GetHeight();
	mBackdrops[0]->SetScale(Vector2(fillWidthScale, fillHeightScale));
	mBackdrops[0]->SetParent(this);

	std::stringstream enviroDropPath;
	enviroDropPath << rootPath << "/Backdrop-2.png";
	mBackdrops[1] = new Texture(enviroDropPath.str());
	fillWidthScale = (float)mWidth * TILESIZE / (float)mBackdrops[1]->GetWidth();
	fillHeightScale = (float)mHeight * TILESIZE / (float)mBackdrops[1]->GetHeight();
	mBackdrops[1]->SetScale(Vector2(fillWidthScale, fillHeightScale));
	mBackdrops[1]->SetParent(this);

	std::stringstream decorDropPath;
	decorDropPath << rootPath << "/Backdrop-3.png";
	mBackdrops[2] = new Texture(decorDropPath.str());
	fillWidthScale = (float)mWidth * TILESIZE / (float)mBackdrops[2]->GetWidth();
	fillHeightScale = (float)mHeight * TILESIZE / (float)mBackdrops[2]->GetHeight();
	mBackdrops[2]->SetScale(Vector2(fillWidthScale, fillHeightScale));
	mBackdrops[2]->SetParent(this);
}

void Level::LoadLevelTileData(std::string rootPath) {
	std::string buffer;
	int lastIndexParsed;

	mLevelTileData = new int**[mNumPlanes];
	for (int k = 0; k < mNumPlanes; k++) {
		mLevelTileData[k] = new int*[mWidth];
		for (int j = 0; j < mWidth; j++)
			mLevelTileData[k][j] = new int[mHeight];
	}

	for (int plane = 0; plane < mNumPlanes; plane++) {
		std::stringstream levelPath;
		levelPath << SDL_GetBasePath() << "Assets/" << rootPath << "/Plane" << plane << ".csv";

		std::fstream levelStream(levelPath.str().c_str(), std::ios::in | std::ios::binary);
		levelStream.seekg(0, std::ios::end);
		buffer.resize((int)levelStream.tellg());
		levelStream.seekg(0, std::ios::beg);
		levelStream.read(&buffer[0], buffer.size());
		levelStream.close();

		lastIndexParsed = 0;
		for (int j = 0; j < mHeight; j++) {
			for (int i = 0; i < mWidth; i++) {
				mLevelTileData[plane][i][j] = ParseInt(buffer, lastIndexParsed);
			}
		}
	}
}

void Level::LoadShadows(std::string rootPath) {
	std::stringstream entitiesPath;
	entitiesPath << SDL_GetBasePath() << "Assets/" << rootPath << "/Entities.dat";

	int numShadowsSpawned = 0;
	mShadows = new Shadow*[mNumShadows];

	std::ifstream inFile(entitiesPath.str().c_str(), std::ios::binary);
	while (!inFile.eof()) {
		std::string line;
		inFile >> line;

		// Split point and spawns
		int assignmentOpIndex = line.find(':');
		std::string coords = line.substr(0, assignmentOpIndex);
		std::string spawns = line.substr(assignmentOpIndex + 1, line.length() - assignmentOpIndex);

		// Parse coords
		Vector2 spawnPoint;
		int coordSplitterIndex = line.find(',');
		spawnPoint.x = std::stof(line.substr(0, coordSplitterIndex).c_str());
		spawnPoint.y = std::stof(line.substr(coordSplitterIndex + 1, line.length() - coordSplitterIndex).c_str());

		// Parse spawn rates
		int spawnRates[NUM_SHADOW_TYPES];
		int lastIndexParsed = 0;
		for (int type = 0; type < NUM_SHADOW_TYPES; type++) {
			spawnRates[type] = ParseInt(spawns, lastIndexParsed);

			if (spawnRates[type] == 0) continue;
			for (int i = 0; i < spawnRates[type]; i++) {
				if (type == 0)
					mShadows[numShadowsSpawned] = new Shadow(spawnPoint, this, &Behaviours::CopyCat);
				else if (type == 1)
					mShadows[numShadowsSpawned] = new Shadow(spawnPoint, this, &Behaviours::Inverted);
				else if (type == 2)
					mShadows[numShadowsSpawned] = new Shadow(spawnPoint, this, &Behaviours::Random);
				else if (type == 3)
					mShadows[numShadowsSpawned] = new Shadow(spawnPoint, this, &Behaviours::Mario);
				else
					continue;
				numShadowsSpawned++;
			}
		}
	}
	if (numShadowsSpawned != mNumShadows) std::cerr << "File mismatch: Entity total not equal to spawn rates." << std::endl;
	for (int i = 0; i < mNumShadows; i++)
		mShadows[i]->SetParent(this);
}

int Level::ParseInt(std::string& buffer, int& lastIndexParsed) {
	unsigned int commaIndex = lastIndexParsed;
	while (commaIndex < buffer.length() && buffer[commaIndex] != ',') {
		if (buffer[commaIndex] == '\\' || buffer[commaIndex] == 'r' || buffer[commaIndex] == 'n')
			lastIndexParsed++;

		commaIndex++;
	}
	int out = atoi(buffer.substr(lastIndexParsed, commaIndex).c_str());
	lastIndexParsed = commaIndex + 1;
	return out;
}

// Getters & Setters
int Level::GetLevelNum() {
	return mLevelNum;

}
int Level::GetWidth() {
	return mWidth;
}
int Level::GetHeight() {
	return mHeight;
}

Vector2 Level::GetStartPos() {
	return mStartPos * TILESIZE;
}
Vector2 Level::GetEndPos() {
	return mEndPos * TILESIZE;
}
Vector2 Level::GetStartTile() {
	return mStartPos;
}
Vector2 Level::GetEndTile() {
	return mEndPos;
}

Tile* Level::GetTileAt(float x, float y, unsigned int plane) {
	int tileX = (int)(x / TILESIZE);
	int tileY = (int)(y / TILESIZE);
	if (tileX < 0 || tileY < 0 || tileX > mWidth - 1 || tileY > mHeight - 1) {
		return mTiles[0];
	} else {
		Tile* tile = mTiles[mLevelTileData[plane][tileX][tileY]];
		if (tile != NULL) {
			tile->SetPos(Vector2(tileX * TILESIZE, tileY * TILESIZE));
			return tile;
		} else {
			return mTiles[0];
		}
	}
}

bool Level::CheckCollisions(GameObject* o, unsigned int plane) {
	SDL_Rect box = o->GetCollisionBox();
	
	bool betweenX = box.x / TILESIZE != (box.x + box.w) / TILESIZE;
	bool betweenY = box.y / TILESIZE != (box.y + box.h) / TILESIZE;

	if (CollisionManager::Instance()->Check(o, GetTileAt(box.x + 1, box.y + 1, plane), CollisionManager::BOX))
		return true;

	if (betweenX) {
		if (CollisionManager::Instance()->Check(o, GetTileAt(box.x + box.w - 1, box.y + 1, plane), CollisionManager::BOX))
			return true;
	}

	if (betweenY) {
		if (CollisionManager::Instance()->Check(o, GetTileAt(box.x + 1, box.y + box.h - 1, plane), CollisionManager::BOX))
			return true;
	}

	if (betweenX && betweenY) {
		if (CollisionManager::Instance()->Check(o, GetTileAt(box.x + box.w - 1, box.y + box.h - 1, plane), CollisionManager::BOX))
			return true;
	}
	return false;
}

int Level::GetTotalPlanes() {
	return mNumPlanes;
}

int Level::GetCurrentScore() {
	return mCurrentScore;
}

std::vector<GameEntity*>* Level::GetEntitiesByRange(Vector2 point, float radius) {
	std::vector<GameEntity*>* entities = new std::vector<GameEntity*>();
	entities->push_back(GameManager::Instance()->GetLevelState()->GetPlayer());

	for (int i = 0; i < mNumShadows; i++) {
		if (mShadows[i] != nullptr) {
			Vector2 diff = point - mShadows[i]->GetPos(WORLD);
			if (sqrt(diff.x * diff.x + diff.y * diff.y) <= radius)
				entities->push_back(mShadows[i]);
		}
	}

	return entities;
}
