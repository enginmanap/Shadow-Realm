#pragma once

#ifndef SHADOW_H
#define SHADOW_H

#define NUM_SHADOW_TYPES 4

#include "GameEntity.h"

class Shadow : public GameEntity {
private:
	void (*behaviourFunc)(unsigned int, Shadow*);
public:
	Shadow(Vector2 startPos, Level* level, void (*behaviourFunc)(unsigned int, Shadow*));
	~Shadow();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);
};

#endif