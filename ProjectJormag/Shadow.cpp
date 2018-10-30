#include "Shadow.h"

#include "GameManager.h"
#include "Level.h"

Shadow::Shadow(Vector2 startPos, Level* level, void (*behaviourFunc)(unsigned int, Shadow*)) : GameEntity(level, new AnimatedTexture("Textures/Entities/Slimer.png", 0, 0, 32, 32, 10, 1000, AnimatedTexture::AnimDir::HORIZONTAL)) {
	Player* player = GameManager::Instance()->GetLevelState()->GetPlayer();
	mMoveSpeed = player->GetMoveSpeed() * ((rand() % 1) + 0.5);
	mJumpSpeed = player->GetJumpSpeed() * ((rand() % 1) + 0.5);
	mJumpAcceleration = player->GetJumpAcceleration() * ((rand() % 1) + 0.5);

	SetPos(startPos * TILESIZE);

	this->behaviourFunc = behaviourFunc;
}

Shadow::~Shadow() {
	behaviourFunc = NULL;
}

void Shadow::Tick(unsigned int deltaTime) {
	if (!mActive) return;

	mVelocity = V2_ZERO;

	(*behaviourFunc)(deltaTime, this);

	ApplyMovement(deltaTime);
	ApplyJump(deltaTime);
	ApplyGravity(deltaTime);
	ApplyCollisions();

	Vector2 moveThisTick = mVelocity + mAcceleration;
	if (mIsFloored && mTexture->GetAnimationRow() == 1) {
		if (mTexture->GetAnimationRow() != 2) {
			mTexture->SetAnimationRow(2);
			mTexture->SetWrapMode(AnimatedTexture::ONCE);
		}
	} else if (!mIsFloored) {
		if (mTexture->GetAnimationRow() != 1) {
			mTexture->SetAnimationRow(1);
			mTexture->SetWrapMode(AnimatedTexture::ONCE);
		}
	} else if (mIsFloored && mTexture->AnimationDone()) {
		if (mTexture->GetAnimationRow() != 0) {
			mTexture->SetAnimationRow(0);
			mTexture->SetWrapMode(AnimatedTexture::LOOP);
		}
	}
	mTexture->Tick(deltaTime);

	SetPos(GetPos(LOCAL) + moveThisTick);
}

void Shadow::Render(SDL_Renderer* renderer) {
	mTexture->Render(renderer);
}