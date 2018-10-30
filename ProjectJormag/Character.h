#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameEntity.h"

class Character : public GameEntity {
private:
	InputManager* mInput;
	Controls::Scheme* mControls;

	int mPlayerID;
private:
	void GetInput(unsigned int deltaTime);
public:
	enum Keys { MOVE_LEFT, MOVE_RIGHT, JUMP };
public:
	Character(std::string texturePath, int playerID);
	~Character();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	int GetID();
};

#endif