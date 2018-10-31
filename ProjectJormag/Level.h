#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#define TILESIZE 64
#define TOTAL_TILES 10

#include "Jormag.h"
#include "Player.h"
#include "Shadow.h"

struct CollisionResult {
	bool up;
	bool down;
	bool left;
	bool right;
};

struct Tile : public GameObject {
	Texture* mTexture;

	Tile(std::string tileset, int x, int y, Collisions::Type collisionType) : GameObject() {
		mTexture = new Texture(tileset, x, y, TILESIZE, TILESIZE);
		mTexture->SetParent(this);
		mCollisionType = collisionType;
	}

	~Tile() {
		delete mTexture;
		mTexture = NULL;
	}

	int GetCollisionRadius() {
		return TILESIZE;
	}

	SDL_Rect GetCollisionBox() {
		Vector2 pos = GetPos(WORLD);
		return { (int)pos.x, (int)pos.y, TILESIZE, TILESIZE };
	}
};

class Level : public GameObject {
private:
	// Tileset
	Tile* mTiles[TOTAL_TILES];

	int mScreenWidthInTiles, mScreenHeightInTiles;

	// Level data
	int mLevelNum;
	int mWidth, mHeight;
	int mNumPlanes;
	int mNumShadows;
	int mCurrentScore;
	Vector2 mStartPos;
	Vector2 mEndPos;

	Texture* mBackdrops[3] = {0};
	int*** mLevelTileData;
	Shadow** mShadows;
private:
	void LoadTiles(std::string setPath);
	void LoadHeader(std::string rootPath);
	void LoadBackdrops(std::string rootPath);
	void LoadLevelTileData(std::string rootPath);
	void LoadShadows(std::string rootPath);
	int ParseInt(std::string& buffer, int& lastIndexParsed);
public:
	Level(std::string tilesetName);
	~Level();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer, int plane);

	void LoadLevel(int level);
	void LoadLevel(std::string rootPath);
	void ClearLevel();

	int GetLevelNum();
	int GetWidth();
	int GetHeight();

	Vector2 GetStartPos();
	Vector2 GetEndPos();
	Vector2 GetStartTile();
	Vector2 GetEndTile();

	Tile* GetTileAt(float x, float y, unsigned int plane);
	bool CheckCollisions(GameObject* o, unsigned int plane);

	int GetTotalPlanes();
	int GetCurrentScore();

	std::vector<GameEntity*>* GetEntitiesByRange(Vector2 point, float radius);
};

#endif