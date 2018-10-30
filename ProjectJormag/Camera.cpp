#include "Camera.h"

namespace Jormag {

	Camera* Camera::sInstance = NULL;

	Camera* Camera::Instance() {
		if (sInstance == NULL)
			sInstance = new Camera();

		return sInstance;
	}

	void Camera::Free() {
		delete sInstance;
		sInstance = NULL;
	}

	Camera::Camera() {
		mGraphics = Graphics::Instance();

		mOriginOffset = { 0.0f, 0.0f };
		mOffsetDestination = { 0.0f, 0.0f };
		mPrefPanTime = 0;
		mCurrPanTime = 0;
	}

	Camera::~Camera() {
		mGraphics = NULL;
	}

	void Camera::Tick(unsigned int deltaTime) {
		PanCamera(deltaTime);
	}

	void Camera::PanCamera(unsigned int deltaTime) {
		float speedX = fabsf(mOriginOffset.x - mOffsetDestination.x) / (mPrefPanTime / deltaTime);
		if (mOriginOffset.x - mOffsetDestination.x < speedX)
			mOriginOffset.x += speedX;
		else if (mOriginOffset.x - mOffsetDestination.x > speedX)
			mOriginOffset.x -= speedX;
		else
			mOriginOffset.x = mOffsetDestination.x;

		float speedY = fabsf(mOriginOffset.y - mOffsetDestination.y) / (mPrefPanTime / deltaTime);
		if (mOriginOffset.y - mOffsetDestination.y < speedY)
			mOriginOffset.y += speedY;
		else if (mOriginOffset.y - mOffsetDestination.y > speedY)
			mOriginOffset.y -= speedY;
		else
			mOriginOffset.y = mOffsetDestination.y;
	}

	void Camera::SetPanDestination(GameObject* o, unsigned int panTime) {
		mOffsetDestination.x = o->GetPos(GameObject::WORLD).x - mGraphics->mScreenWidth / 2;
		mOffsetDestination.y = o->GetPos(GameObject::WORLD).y - mGraphics->mScreenHeight / 2;
		mPrefPanTime = panTime;

		// TODO: Check map bounds
		//if (mOffsetDestination.y + displayMode.h > game->GetMap()->GetHeight() * TILESIZE)
		//	mOffsetDestination.y = (float)game->GetMap()->GetHeight() * TILESIZE - displayMode.h;
		mCurrPanTime = 0;
	}
	void Camera::SetPanDestination(Vector2 v, unsigned int panTime) {
		mOffsetDestination.x = v.x - mGraphics->mScreenWidth / 2;
		mOffsetDestination.y = v.y - mGraphics->mScreenHeight / 2;
		mPrefPanTime = panTime;

		// TODO Check map bounds
		mCurrPanTime = 0;
	}
	void Camera::SetPanDestination(float x, float y, unsigned int panTime) {
		mOffsetDestination.x = x - mGraphics->mScreenWidth / 2;
		mOffsetDestination.y = y - mGraphics->mScreenHeight / 2;
		mPrefPanTime = panTime;

		// TODO Check map bounds
		mCurrPanTime = 0;
	}

	void Camera::SetCentre(GameObject* o) {
		SetPanDestination(o, 0);
		mOriginOffset = mOffsetDestination;
	}
	void Camera::SetCentre(Vector2 v) {
		SetPanDestination(v, 0);
		mOriginOffset = mOffsetDestination;
	}
	void Camera::SetCentre(float x, float y) {
		SetPanDestination(x, y, 0);
		mOriginOffset = mOffsetDestination;
	}

	Vector2 Camera::GetOffset() {
		return mOriginOffset;
	}

	void Camera::SetOffset(Vector2 offset) {
		mOffsetDestination = offset;
	}

	void Camera::SetPanningTime(unsigned int prefPanTime) {
		mPrefPanTime = prefPanTime;
	}

	bool Camera::IsAtDestination() {
		Vector2 distanceToTarget = mOriginOffset - mOffsetDestination;
		return fabsf(distanceToTarget.x + distanceToTarget.y) < Graphics::Instance()->mScreenWidth / 3.0f;
	}
}