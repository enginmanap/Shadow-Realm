#include "POWBlock.h"

POWBlock::POWBlock() : GameObject() {
	mTexture = new Texture("Secret/PowBlock.png", 0, 0, 32, 32);
	mTexture->SetParent(this);
	mHits = 0;
	mCollisionType = Collisions::ALL;
}

POWBlock::~POWBlock() {
	delete mTexture;
	mTexture = NULL;
}

void POWBlock::Tick(unsigned int deltaTime) {
	if (!IsActive()) return;

	mTexture->UseSprite(2 - mHits);
	mTexture->Tick(deltaTime);
}

void POWBlock::Render(SDL_Renderer* renderer) {
	if (!IsActive()) return;

	mTexture->Render(renderer);
}

void POWBlock::RegisterHit() {
	mHits++;

	if (IsDepleted())
		SetActive(false);
}

bool POWBlock::IsDepleted() {
	return mHits >= 3;
}

int POWBlock::GetCollisionRadius() {
	return mTexture->GetHeight();
}

SDL_Rect POWBlock::GetCollisionBox() {
	Vector2 pos = GetPos(WORLD);
	return { (int)pos.x, (int)pos.y, mTexture->GetWidth(), mTexture->GetHeight() };
}
