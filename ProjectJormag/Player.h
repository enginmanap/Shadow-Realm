#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include "Jormag.h"
#include "GameEntity.h"

class Player : public GameEntity {
private:
	InputManager* mInput;
	Controls::Scheme* mControls;

	int mPlayerID;
private:
	void GetInput(unsigned int deltaTime);
public:
	enum Keys { MOVE_LEFT, MOVE_RIGHT, JUMP, NEXT_PLANE, LAST_PLANE };
public:
	Player(AnimatedTexture* texturePath, Level* level, int playerID);
	~Player();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	int GetID();
};

#endif