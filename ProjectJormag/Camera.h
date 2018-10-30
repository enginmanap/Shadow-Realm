#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include "Graphics.h"
#include "MathHelper.h"

namespace Jormag {

	class Camera {
	private:
		// Singleton Instance
		static Camera* sInstance;

		// Screen data
		Graphics* mGraphics;

		// Positioning
		Vector2 mOriginOffset;
		Vector2 mOffsetDestination;
		unsigned int mPrefPanTime;
		unsigned int mCurrPanTime;
	private:
		Camera();
		~Camera();

		void PanCamera(unsigned int deltaTime);
	public:
		static Camera* Instance();
		static void Free();

		void Tick(unsigned int deltaTime);

		void SetPanDestination(GameObject* o, unsigned int panTime);
		void SetPanDestination(Vector2 v, unsigned int panTime);
		void SetPanDestination(float x, float y, unsigned int panTime);

		void SetCentre(GameObject* o);
		void SetCentre(Vector2 v);
		void SetCentre(float x, float y);

		Vector2 GetOffset();
		void SetOffset(Vector2 offset);

		void SetPanningTime(unsigned int prefPanTime);
		bool IsAtDestination();
	};
}

#endif