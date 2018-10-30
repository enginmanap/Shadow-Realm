#include "Character.h"

Character::Character(std::string texturePath, int playerID) : GameEntity(new AnimatedTexture(texturePath, 0, 0, 32, 48, 1, 1, AnimatedTexture::AnimDir::HORIZONTAL)) {
	mInput = InputManager::Instance();
	mPlayerID = playerID;
	mControls = SettingsManager::Instance()->GetControls(playerID);

	mMoveSpeed = 200.0f;
	mJumpSpeed = -8.4f;
	mJumpAcceleration = 1.8f;
}

Character::~Character() {
	mInput = NULL;

	delete mTexture;
	mTexture = NULL;
}

void Character::GetInput(unsigned int deltaTime) {
	if (mInput->KeyDown((SDL_Scancode)mControls->mKeyCodes[MOVE_LEFT])) {
		mIsMovingLeft = true;
		SetFlip(SDL_FLIP_NONE);
		mDir = Direction::LEFT;
	} else {
		mIsMovingLeft = false;
	}
	if (mInput->KeyDown((SDL_Scancode)mControls->mKeyCodes[MOVE_RIGHT])) {
		mIsMovingRight = true;
		SetFlip(SDL_FLIP_HORIZONTAL);
		mDir = Direction::RIGHT;
	} else {
		mIsMovingRight = false;
	}
	if (mInput->KeyDown((SDL_Scancode)mControls->mKeyCodes[JUMP])) {
		if (mCanJump && mIsFloored) {
			mCanJump = false;
			mIsJumping = true;
			mIsFloored = false;
			mAcceleration.y = mJumpSpeed;
			AudioManager::Instance()->PlaySFX("Sounds/SFX/Player/Jump.wav", 0, -1);
		}
	} else {
		mCanJump = true;
		mIsJumping = false;
	}
}

void Character::Tick(unsigned int deltaTime) {
	mVelocity = V2_ZERO;

	if (!mIsDead)
		GetInput(deltaTime);

	ApplyMovement(deltaTime);
	ApplyJump(deltaTime);
	ApplyGravity(deltaTime);
}

void Character::Render(SDL_Renderer* renderer) {
}

int Character::GetID() {
	return mPlayerID;
}