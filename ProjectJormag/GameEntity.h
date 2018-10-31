#pragma once

#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <vector>

#include "Jormag.h"

class Level;

enum DeathType : char {
	NONE,
	ATTACK,
	CRUSH,
	FALL
};

class GameEntity : public GameObject {
protected:
	Level* mLevel;

	Vector2 mVelocity;
	Vector2 mAcceleration;
	Direction::Facing mDir;

	bool mIsMovingLeft;
	bool mIsMovingRight;
	bool mIsJumping;
	bool mCanJump;
	bool mIsFloored;
	bool mIsDead;
	DeathType mCauseOfDeath;
	
	AnimatedTexture* mTexture;

	float mMoveSpeed;
	float mJumpSpeed;
	float mJumpAcceleration;

	int mCurrentPlane;
protected:
	void ApplyMovement(unsigned int deltaTime);
	void ApplyJump(unsigned int deltaTime);
	void ApplyGravity(unsigned int deltaTime);
	void ApplyCollisions();
	void ApplyCrush();
public:
	GameEntity(AnimatedTexture* texture);
	GameEntity(Level* level, AnimatedTexture* texture);
	~GameEntity();

	virtual void Tick(unsigned int deltaTime);
	virtual void Render(SDL_Renderer* renderer);

	virtual int GetCollisionRadius();
	virtual SDL_Rect GetCollisionBox();

	Vector2 GetVelocity();
	void AddVelocity(Vector2 velocity);
	Vector2 GetAcceleration();
	void AddAcceleration(Vector2 velocity);
	Direction::Facing GetDirection();
	void SetDirection(Direction::Facing dir);

	bool IsMovingLeft();
	void SetMovingLeft(bool movingLeft);
	bool IsMovingRight();
	void SetMovingRight(bool movingRight);
	bool IsJumping();
	void SetJumping(bool isJumping);
	bool CanJump();
	void SetCanJump(bool canJump);
	bool IsFloored();
	void SetFloored(bool isFloored);
	bool IsDead();
	void SetDead(bool isDead, DeathType causeOfDeath);
	DeathType GetCauseOfDeath();

	float GetMoveSpeed();
	float GetJumpSpeed();
	float GetJumpAcceleration();

	int GetCurrentPlane();
	void SetCurrentPlane(int plane);

	void SetFlip(SDL_RendererFlip flip);

	void ToggleAnimation();
};

#endif