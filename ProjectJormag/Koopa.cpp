#include "Koopa.h"

Koopa::Koopa(Level* level) : GameEntity(level, new AnimatedTexture("Secret/Koopa.png", 0, 0, 32, 30, 1, 0, AnimatedTexture::AnimDir::VERTICAL)) {
	mFlipped = false;
	mFlipTimer = 0;
	mFlipTimeMillis = 4000;

	mMoveSpeed = 200.0f;
	mJumpSpeed = -8.4f;
	mJumpAcceleration = 1.8f;
}

Koopa::~Koopa() {
	return;
}

void Koopa::Tick(unsigned int deltaTime) {
	mVelocity = V2_ZERO;
	
	Vector2 pos = GetPos(WORLD);
	if (!mIsDead) {
		if (pos.x <= 0) {
			mIsMovingRight = true;
			mIsMovingLeft = false;
			SetFlip(SDL_FLIP_NONE);
			mDir = Direction::RIGHT;
		} 
		if (pos.x + mTexture->GetWidth() >= 16 * 64) {
			mIsMovingLeft = true;
			mIsMovingRight = false;
			SetFlip(SDL_FLIP_HORIZONTAL);
			mDir = Direction::LEFT;
		} 
	}

	if (!mFlipped) {
		ApplyMovement(deltaTime);
		ApplyJump(deltaTime);

		mTexture->UseSprite(0);
	} else {
		mFlipTimer += deltaTime;

		if (mFlipTimer >= mFlipTimeMillis) {
			mFlipped = false;
			mFlipTimer = 0;
		}

		mTexture->UseSprite(1);
	}
	ApplyGravity(deltaTime);
	ApplyCollisions();
	mTexture->Tick(deltaTime);

	SetPos(GetPos(LOCAL) + mVelocity + mAcceleration);
}

void Koopa::Render(SDL_Renderer* renderer) {
	if (mIsDead) return;
	mTexture->Render(renderer);
}

bool Koopa::IsFlipped() {
	return mFlipped;
}

void Koopa::SetFlipped(bool flipped) {
	mFlipped = flipped;
}