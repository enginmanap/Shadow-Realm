#include "GameObject.h"

namespace Jormag {

	GameObject::GameObject(Vector2 pos) {
		mPos = pos;
		mRotation = 0.0f;
		mScale = V2_ONE;
		mFlip = SDL_FLIP_NONE;
		mOpacity = 255;
		mActive = true;
		mParent = NULL;
		mCollisionType = Collisions::Type::NONE;
	}

	GameObject::~GameObject() {
		mParent = NULL;
	}

	void GameObject::Translate(Vector2 vector) {
		mPos += vector;
	}

	void GameObject::Rotate(float degrees) {
		mRotation += degrees;
		mRotation = fmodf(mRotation, 360.0f);
	}

	Vector2 GameObject::GetPos(Space space) {
		if (space == LOCAL || mParent == NULL)
			return mPos;

		return mParent->GetPos(WORLD) + mPos;
	}
	void GameObject::SetPos(Vector2 pos) {
		mPos = pos;
	}

	float GameObject::GetRotation(Space space) {
		if (space == LOCAL || mParent == NULL)
			return mRotation;

		return fmodf(mParent->GetRotation(WORLD) + mRotation, 360.0f);
	}
	void GameObject::SetRotation(float rotation) {
		mRotation = fmodf(rotation, 360.0f);
	}

	Vector2 GameObject::GetScale(Space space) {
		if (space == LOCAL || mParent == NULL)
			return mScale;

		Vector2 totalScale = mParent->GetScale(WORLD);
		totalScale.x *= mScale.x;
		totalScale.y *= mScale.y;

		return totalScale;
	}
	void GameObject::SetScale(Vector2 scale) {
		mScale = scale;
	}

	SDL_RendererFlip GameObject::GetFlip() {
		return mFlip;
	}
	void GameObject::SetFlip(SDL_RendererFlip flip) {
		mFlip = flip;
	}

	int GameObject::GetOpacity(Space space) {
		if (space == LOCAL || mParent == NULL)
			return mOpacity;

		return mParent->GetOpacity(WORLD);
	}
	void GameObject::SetOpacity(int opacity) {
		mOpacity = opacity;
	}

	bool GameObject::IsActive() {
		return mActive;
	}
	void GameObject::SetActive(bool active) {
		mActive = active;
	}

	GameObject* GameObject::GetParent() {
		return mParent;
	}
	void GameObject::SetParent(GameObject* parent) {
		if (parent == this)
			return;

		if (parent == NULL) {
			mPos = GetPos(WORLD);
			mRotation = GetRotation(WORLD);
			mScale = GetScale(WORLD);
		} else {
			if (mParent != NULL)
				mParent = NULL;
		}
		mParent = parent;
	}

	Collisions::Type GameObject::GetCollisionType() {
		return mCollisionType;
	}

	int GameObject::GetCollisionRadius() {
		return 0;
	}

	SDL_Rect GameObject::GetCollisionBox() {
		return { 0, 0, 1, 1 };
	}
}