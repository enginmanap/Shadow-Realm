#pragma once

#ifndef KOOPA_H
#define KOOPA_H

#include "GameEntity.h"

class Koopa : public GameEntity {
private:
	bool mFlipped;
	unsigned int mFlipTimer;
	unsigned int mFlipTimeMillis;
private:
public:
	Koopa(Level* level);
	~Koopa();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	bool IsFlipped();
	void SetFlipped(bool flipped);
};

#endif