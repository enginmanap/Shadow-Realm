#pragma once

#ifndef POWBLOCK_H
#define POWBLOCK_H

#include "Jormag.h"

class POWBlock : public GameObject {
private:
	Texture* mTexture;
	int mHits;
public:
	POWBlock();
	~POWBlock();

	void Tick(unsigned int deltaTime);
	void Render(SDL_Renderer* renderer);

	void RegisterHit();
	bool IsDepleted();

	int GetCollisionRadius();
	SDL_Rect GetCollisionBox();
};

#endif