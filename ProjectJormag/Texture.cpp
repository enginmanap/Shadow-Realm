#include "Texture.h"

namespace Jormag {

	Texture::Texture(std::string fileName) : GameObject() {
		mTexture = ResourceManager::Instance()->GetTexture(fileName);

		SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
		mClipped = false;

		mRenderRect.w = mWidth;
		mRenderRect.h = mHeight;
	}

	Texture::Texture(std::string fileName, int x, int y, int w, int h) : GameObject() {
		mTexture = ResourceManager::Instance()->GetTexture(fileName);

		mWidth = w;
		mHeight = h;
		mClipped = true;
		mSourceRect = { x, y, w, h };

		mRenderRect.w = mWidth;
		mRenderRect.h = mHeight;
	}

	Texture::Texture(std::string text, std::string fontPath, int size, SDL_Color color) : GameObject() {
		mTexture = ResourceManager::Instance()->GetText(text, fontPath, size, color);

		SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
		mClipped = false;

		mRenderRect.w = mWidth;
		mRenderRect.h = mHeight;
	}

	Texture::~Texture() {
		mTexture = NULL;
	}

	void Texture::Render(SDL_Renderer* renderer) {
		Vector2 pos = GetPos(WORLD);
		Vector2 scale = GetScale(WORLD);
		Vector2 offset = Camera::Instance()->GetOffset();

		Graphics* g = Graphics::Instance();
		Vector2 screenScale = { g->mScreenWidth / 1920.0f, g->mScreenHeight / 1080.0f };

		if (mIsCentered) {
			mRenderRect.x = (int)(pos.x - mWidth * scale.x * screenScale.x * 0.5f - offset.x);
			mRenderRect.y = (int)(pos.y - mHeight * scale.y * screenScale.y * 0.5f - offset.y);
		} else {
			mRenderRect.x = (int)(pos.x - offset.x);
			mRenderRect.y = (int)(pos.y - offset.y);
		}
		mRenderRect.w = (int)(mWidth * scale.x * screenScale.x);
		mRenderRect.h = (int)(mHeight * scale.y * screenScale.y);

		SDL_SetTextureAlphaMod(mTexture, (Uint8)GetOpacity(WORLD));

		if(mClipped)
			SDL_RenderCopyEx(renderer, mTexture, &mSourceRect, &mRenderRect, GetRotation(WORLD), NULL, GetFlip());
		else
			SDL_RenderCopyEx(renderer, mTexture, NULL, &mRenderRect, GetRotation(WORLD), NULL, GetFlip());
	}
	void Texture::Render(SDL_Renderer* renderer, Vector2 destination) {
		Vector2 scale = GetScale(WORLD);
		Vector2 offset = Camera::Instance()->GetOffset();

		Graphics* g = Graphics::Instance();
		Vector2 screenScale = { g->mScreenWidth / 1920.0f, g->mScreenHeight / 1080.0f };

		mRenderRect.x = (int)(destination.x - offset.x);
		mRenderRect.y = (int)(destination.y - offset.y);
		mRenderRect.w = (int)(mWidth * scale.x);
		mRenderRect.h = (int)(mHeight * scale.y);

		SDL_SetTextureAlphaMod(mTexture, (Uint8)GetOpacity(WORLD));
		
		if (mClipped)
			SDL_RenderCopyEx(renderer, mTexture, &mSourceRect, &mRenderRect, GetRotation(WORLD), NULL, GetFlip());
		else
			SDL_RenderCopyEx(renderer, mTexture, NULL, &mRenderRect, GetRotation(WORLD), NULL, GetFlip());
	}

	int Texture::GetWidth() {
		return mWidth * GetScale(WORLD).x;
	}

	int Texture::GetHeight() {
		return mHeight * GetScale(WORLD).y;
	}

	void Texture::SetCentered(bool centered) {
		mIsCentered = centered;
	}

	void Texture::UseSprite(int id) {
		if (!mClipped) return;
		
		mSourceRect.x = id * mSourceRect.w;
	}

	int Texture::GetCollisionRadius() {
		if (mWidth > mHeight)
			return mWidth / 2;
		else
			return mHeight / 2;
	}

	SDL_Rect Texture::GetCollisionBox()	{
		Vector2 pos = GetPos(WORLD);
		return { (int)pos.x, (int)pos.y, mWidth, mHeight };
	}
}