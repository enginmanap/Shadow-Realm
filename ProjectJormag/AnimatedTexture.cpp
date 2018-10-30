#include "AnimatedTexture.h"

#include <iostream>

namespace Jormag {

	AnimatedTexture::AnimatedTexture(std::string fileName, int x, int y, int w, int h, int numFrames, int animTimeMillis, AnimDir animationDir) : Texture(fileName, x, y, w, h) {
		mNumFrames = numFrames;
		mAnimTimeMillis = animTimeMillis;
		mCurrFrameTime = 0;
		mCurrFrame = 0;

		mIsRunning = true;
		mIsFinished = false;

		mWrapMode = LOOP;
		mAnimationDirection = animationDir;
	}

	AnimatedTexture::~AnimatedTexture() {
		return;
	}

	void AnimatedTexture::Tick(unsigned int deltaTime) {
		if (mIsFinished || !mIsRunning || mNumFrames <= 1)
			return;

		mCurrFrameTime += deltaTime;
		if (mCurrFrameTime >= mAnimTimeMillis / mNumFrames) {
			mCurrFrameTime = 0;
			if (mCurrFrame >= mNumFrames - 1) {
				if (mWrapMode == ONCE)
					mIsFinished = true;
				else
					mCurrFrame = 0;
			} else {
				mCurrFrame++;
			}
		}

		if (mAnimationDirection == HORIZONTAL)
			mSourceRect.x = mCurrFrame * mSourceRect.w;
		else
			mSourceRect.y = mCurrFrame * mSourceRect.h;
	}

	bool AnimatedTexture::IsRunning() {
		return mIsRunning;
	}

	void AnimatedTexture::Start() {
		mIsRunning = true;
	}

	void AnimatedTexture::Pause() {
		mIsRunning = false;
	}

	void AnimatedTexture::Stop() {
		mIsRunning = false;
		mCurrFrameTime = 0;
		mCurrFrame = 0;
	}

	bool AnimatedTexture::AnimationDone() {
		return mIsFinished;
	}

	int AnimatedTexture::GetAnimationRow() {
		if (mAnimationDirection == HORIZONTAL)
			return mSourceRect.y / mSourceRect.h;
		else
			return mSourceRect.x / mSourceRect.w;
	}

	void AnimatedTexture::SetAnimationRow(int row) {
		if (!mIsRunning) return;

		if (mAnimationDirection == HORIZONTAL)
			mSourceRect.y = row * mSourceRect.h;
		else
			mSourceRect.x = row * mSourceRect.w;
	}

	void AnimatedTexture::SetWrapMode(WrapMode wrapMode) {
		mIsFinished = false;
		mCurrFrameTime = 0;
		mCurrFrame = 0;
		mWrapMode = wrapMode;
	}
}