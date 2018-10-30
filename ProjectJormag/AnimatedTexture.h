#pragma once

#ifndef ANIMATEDTEXTURE_H
#define ANIMATEDTEXTURE_H

#include "Texture.h"

namespace Jormag {

	class AnimatedTexture : public Texture {
	public:
		enum WrapMode { ONCE, LOOP };
		enum AnimDir { HORIZONTAL, VERTICAL };
	private:
		int mNumFrames;
		int mAnimTimeMillis;
		int mCurrFrameTime;
		int mCurrFrame;

		bool mIsRunning;
		bool mIsFinished;

		WrapMode mWrapMode;
		AnimDir mAnimationDirection;
	public:
		AnimatedTexture(std::string fileName, int x, int y, int w, int h, int numFrames, int animTimeMillis, AnimDir animationDirection);
		~AnimatedTexture();

		void Tick(unsigned int deltaTime);

		bool IsRunning();
		void Start();
		void Pause();
		void Stop();
		bool AnimationDone();

		int GetAnimationRow();
		void SetAnimationRow(int row);

		void SetWrapMode(WrapMode wrapMode);
	};
}

#endif