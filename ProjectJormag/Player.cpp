#include "Player.h"
#include "Level.h"

Player::Player(AnimatedTexture* texture, Level* level, int playerID) : GameEntity(level, texture) {
	mInput = InputManager::Instance();
	mPlayerID = playerID;
	mControls = SettingsManager::Instance()->GetControls(playerID);

	mMoveSpeed = 200.0f;
	mJumpSpeed = -8.4f;
	mJumpAcceleration = 1.8f;
}

Player::~Player() {
	mInput = NULL;
	
	delete mControls;
	mControls = NULL;
}

void Player::GetInput(unsigned int deltaTime) {
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
	if (mInput->KeyPressed((SDL_Scancode)mControls->mKeyCodes[LAST_PLANE])) {
		int newPlane = mCurrentPlane - 1;
		if (newPlane < 0) newPlane += mLevel->GetTotalPlanes();

		mCurrentPlane = newPlane;

		if (mLevel->CheckCollisions(this, newPlane))
			ApplyCrush();
	}
	if (mInput->KeyPressed((SDL_Scancode)mControls->mKeyCodes[NEXT_PLANE])) {
		int newPlane = mCurrentPlane + 1;
		if (newPlane >= mLevel->GetTotalPlanes()) newPlane -= mLevel->GetTotalPlanes();

		mCurrentPlane = newPlane;

		if (mLevel->CheckCollisions(this, newPlane))
			ApplyCrush();
	}
}

void Player::Tick(unsigned int deltaTime) {
	if (mIsDead) return;
	mVelocity = V2_ZERO;
	
	GetInput(deltaTime);

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

void Player::Render(SDL_Renderer* renderer) {
	mTexture->Render(renderer);
	//SDL_SetRenderDrawColor(renderer, 190, 10, 40, 192);
	//SDL_RenderDrawRect(renderer, &GetCollisionBox());
}

int Player::GetID() {
	return mPlayerID;
}