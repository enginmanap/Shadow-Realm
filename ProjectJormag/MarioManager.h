#pragma once

#ifndef MARIOMANAGER_H
#define MARIOMANAGER_H

#include "Jormag.h"
#include "Koopa.h"
#include "Level.h"
#include "Player.h"
#include "POWBlock.h"

class MarioManager : public Jormag::StateManager {
private:
	Graphics* mGraphics;

	// Game Objects
	Camera* mCamera;
	Texture* mBackground;
	Level* mLevel;
	Player* mPlayers[2];
	Koopa* mKoopas[2];
	POWBlock* mPowBlock;

	int mKoopaRespawnTimer;
	int mKoopaRespawnTimeMillis;

	// Screenshake
	bool mScreenshaking;
	unsigned int mScreenshakeTimer;
	unsigned int mScreenshakeTimeMillis;
	float mWobble;
	Vector2 mCameraStartPos;
private:
	bool CheckLoss(Player* player);
	void CheckPowBlock(Player* player);
	void DoScreenshake(unsigned int deltaTime);
public:
	MarioManager();
	~MarioManager();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);
};

#endif