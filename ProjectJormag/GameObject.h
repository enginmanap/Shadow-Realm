#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL.h>

#include "MathHelper.h"

namespace Jormag {

	namespace Collisions {
		enum Type {
			NONE,
			ALL
		};
	}

	class GameObject {
	private:
		Vector2 mPos;
		float mRotation;
		Vector2 mScale;
		SDL_RendererFlip mFlip;
		int mOpacity;
		GameObject* mParent;
	protected:
		Collisions::Type mCollisionType;
		
		bool mActive;
	public:
		enum Space { LOCAL, WORLD };
	public:
		GameObject(Vector2 pos = V2_ZERO);
		~GameObject();

		virtual void Tick(unsigned int deltaTime) { };
		virtual void Render(SDL_Renderer* renderer) { };

		// Transformations
		void Translate(Vector2 vector);
		void Rotate(float degrees);

		// Getters & Setters
		Vector2 GetPos(Space space);
		virtual void SetPos(Vector2 pos);

		float GetRotation(Space space);
		virtual void SetRotation(float rotation);

		Vector2 GetScale(Space space);
		virtual void SetScale(Vector2 scale);

		SDL_RendererFlip GetFlip();
		virtual void SetFlip(SDL_RendererFlip flip);

		int GetOpacity(Space space);
		void SetOpacity(int opacity);

		bool IsActive();
		void SetActive(bool active);

		GameObject* GetParent();
		void SetParent(GameObject* parent);

		Collisions::Type GetCollisionType();
		virtual int GetCollisionRadius();
		virtual SDL_Rect GetCollisionBox();
	};
}

#endif