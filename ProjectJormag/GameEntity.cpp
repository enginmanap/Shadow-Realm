#include "GameEntity.h"
#include "Level.h"

GameEntity::GameEntity(AnimatedTexture* texture) : GameObject() {
	mVelocity = V2_ZERO;
	mAcceleration = V2_ZERO;
	mDir = Direction::LEFT;

	mIsMovingLeft = false;
	mIsMovingRight = false;
	mIsJumping = false;
	mCanJump = true;
	mIsFloored = true;
	mIsDead = false;
	mCollisionType = Collisions::Type::ALL;

	mTexture = texture;
	mTexture->SetParent(this);

	mMoveSpeed = 0.0f;
	mJumpSpeed = 0.0f;
	mJumpAcceleration = 0.0f;
}
GameEntity::GameEntity(Level* level, AnimatedTexture* texture) : GameObject() {
	mLevel = level;

	mVelocity = V2_ZERO;
	mAcceleration = V2_ZERO;
	mDir = Direction::Facing::LEFT;

	mIsMovingLeft = false;
	mIsMovingRight = false;
	mIsJumping = false;
	mCanJump = true;
	mIsFloored = true;
	mIsDead = false;
	mCollisionType = Collisions::Type::ALL;

	mTexture = texture;
	mTexture->SetParent(this);

	mMoveSpeed = 0.0f;
	mJumpSpeed = 0.0f;
	mJumpAcceleration = 0.0f;
}

GameEntity::~GameEntity() {
	mLevel = NULL;

	delete mTexture;
	mTexture = NULL;
}

void GameEntity::Tick(unsigned int deltaTime) {
	mTexture->Tick(deltaTime);
}

void GameEntity::Render(SDL_Renderer* renderer) {
	mTexture->Render(renderer);
}

int GameEntity::GetCollisionRadius() {
	int width = mTexture->GetWidth();
	int height = mTexture->GetHeight();
	if (width > height)
		return width;
	else
		return height;
}

SDL_Rect GameEntity::GetCollisionBox() {
	Vector2 pos = GetPos(WORLD) + mVelocity + mAcceleration;
	return { (int)pos.x, (int)pos.y, mTexture->GetWidth(), mTexture->GetHeight() };
}

void GameEntity::ApplyMovement(unsigned int deltaTime) {
	if (mIsMovingLeft)
		mVelocity.x -= mMoveSpeed * (deltaTime / 1000.0f);
	if (mIsMovingRight)
		mVelocity.x += mMoveSpeed * (deltaTime / 1000.0f);
}

void GameEntity::ApplyJump(unsigned int deltaTime) {
	if (mIsFloored) mIsJumping = false;
	if (mIsJumping)
		mAcceleration.y -= mJumpAcceleration * (deltaTime / 1000.0f);
}

void GameEntity::ApplyGravity(unsigned int deltaTime) {
	SDL_Rect box = GetCollisionBox();

	if (mLevel->GetTileAt((float)(box.x), (float)(box.y + box.h), mCurrentPlane)->GetCollisionType() == Collisions::Type::ALL ||
		mLevel->GetTileAt((float)(box.x + box.w), (float )(box.y + box.h), mCurrentPlane)->GetCollisionType() == Collisions::Type::ALL) {
		mIsFloored = true;
		mAcceleration.y = 0.0f;
		mVelocity.y = ((box.y + box.h) % TILESIZE) * -1;
	} else {
		mIsFloored = false;
		mAcceleration.y += GRAVITY * (deltaTime / 1000.0f);
		if (mAcceleration.y > TERMINAL_VELOCITY)
			mAcceleration.y = TERMINAL_VELOCITY;
	}
}

void GameEntity::ApplyCollisions() {
	SDL_Rect box = GetCollisionBox();

	if (box.y > mLevel->GetHeight() * TILESIZE) {
		if (mIsDead) return;
		AudioManager::Instance()->PlaySFX("Sounds/SFX/Player/Death - Fall.ogg", 0, -1);
		SetDead(true, FALL);
	}
		
	if (mLevel->GetTileAt((float)box.x, (float)box.y, mCurrentPlane)->GetCollisionType() == Collisions::Type::ALL ||
		mLevel->GetTileAt((float)(box.x + box.w), (float)box.y, mCurrentPlane)->GetCollisionType() == Collisions::Type::ALL) {
		mAcceleration.y = 0;

		mVelocity.y = 0;
		if (mLevel->CheckCollisions(this, mCurrentPlane)) {
			mAcceleration.x = 0;
			mVelocity.x = 0;
		}
	} else {
		if (mLevel->CheckCollisions(this, mCurrentPlane)) {
			mAcceleration = V2_ZERO;
			mVelocity = V2_ZERO;
		}
	}
}

void GameEntity::ApplyCrush() {
	AudioManager::Instance()->PlaySFX("Sounds/SFX/Player/Death - Crush.ogg", 0, -1);
	SetDead(true, CRUSH);
}

Vector2 GameEntity::GetVelocity() {
	return mVelocity;
}

void GameEntity::AddVelocity(Vector2 velocity) {
	mVelocity += velocity;
}

Vector2 GameEntity::GetAcceleration() {
	return mAcceleration;
}

void GameEntity::AddAcceleration(Vector2 acceleration) {
	mAcceleration += acceleration;
}

Direction::Facing GameEntity::GetDirection() {
	return mDir;
}

void GameEntity::SetDirection(Direction::Facing dir) {
	mDir = dir;
}

bool GameEntity::IsMovingLeft() {
	return mIsMovingLeft;
}

void GameEntity::SetMovingLeft(bool movingLeft) {
	mIsMovingLeft = movingLeft;
}

bool GameEntity::IsMovingRight() {
	return mIsMovingRight;
}

void GameEntity::SetMovingRight(bool movingRight) {
	mIsMovingRight = movingRight;
}

bool GameEntity::IsJumping() {
	return mIsJumping;
}

void GameEntity::SetJumping(bool isJumping) {
	mIsJumping = isJumping;
}

bool GameEntity::CanJump() {
	return mCanJump;
}

void GameEntity::SetCanJump(bool canJump) {
	mCanJump = canJump;
}

bool GameEntity::IsFloored() {
	return mIsFloored;
}

void GameEntity::SetFloored(bool isFloored) {
	mIsFloored = isFloored;
}

bool GameEntity::IsDead() {
	return mIsDead;
}
void GameEntity::SetDead(bool isDead, DeathType causeOfDeath) {
	mIsDead = isDead;
	mCauseOfDeath = causeOfDeath;
}
DeathType GameEntity::GetCauseOfDeath() {
	if (mIsDead)
		return mCauseOfDeath;
	else
		return NONE;
}

float GameEntity::GetMoveSpeed() {
	return mMoveSpeed;
}

float GameEntity::GetJumpSpeed() {
	return mJumpSpeed;
}

float GameEntity::GetJumpAcceleration() {
	return mJumpAcceleration;
}

int GameEntity::GetCurrentPlane() {
	return mCurrentPlane;
}

void GameEntity::SetCurrentPlane(int plane) {
	mCurrentPlane = plane;
}

void GameEntity::SetFlip(SDL_RendererFlip flip) {
	mTexture->SetFlip(flip);
}


void GameEntity::ToggleAnimation() {
	if (mTexture->IsRunning())
		mTexture->Stop();
	else
		mTexture->Start();
}