#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Camera.h"
#include "ResourceManager.h"

namespace Jormag {
	
	class Texture : public GameObject {
	protected:
		// Wrapped Data
		SDL_Texture* mTexture;

		// Spritesheet Data
		int mWidth;
		int mHeight;
		bool mClipped;

		// Renderer Data
		bool mIsCentered;
		SDL_Rect mSourceRect;
		SDL_Rect mRenderRect;
	public:
		Texture(std::string fileName);
		Texture(std::string fileName, int x, int y, int w, int h);
		Texture(std::string text, std::string fontPath, int size, SDL_Color color);
		~Texture();

		virtual void Render(SDL_Renderer* renderer);
		virtual void Render(SDL_Renderer* renderer, Vector2 destination);

		int GetWidth();
		int GetHeight();

		void SetCentered(bool centered);
		void UseSprite(int id);

		virtual int GetCollisionRadius();
		virtual SDL_Rect GetCollisionBox();
	};
}

#endif