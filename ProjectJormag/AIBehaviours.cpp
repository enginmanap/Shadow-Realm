#include "AIBehaviours.h"
#include "Level.h"

namespace Behaviours {

	void CopyCat(unsigned int deltaTime, Shadow* shadow) {
		InputManager* mInput = InputManager::Instance();
		if (mInput->KeyDown(SDL_SCANCODE_A)) {
			shadow->SetMovingLeft(true);
			shadow->SetFlip(SDL_FLIP_NONE);
			shadow->SetDirection(Direction::LEFT);
		} else {
			shadow->SetMovingLeft(false);
		}
		if (mInput->KeyDown(SDL_SCANCODE_D)) {
			shadow->SetMovingRight(true);
			shadow->SetFlip(SDL_FLIP_HORIZONTAL);
			shadow->SetDirection(Direction::RIGHT);
		} else {
			shadow->SetMovingRight(false);
		}
		if (mInput->KeyDown(SDL_SCANCODE_SPACE)) {
			if (shadow->CanJump() && shadow->IsFloored()) {
				shadow->SetCanJump(false);
				shadow->SetJumping(true);
				shadow->SetFloored(false);
				shadow->AddAcceleration(Vector2(0.0f, shadow->GetJumpSpeed()));
			}
		} else {
			shadow->SetCanJump(true);
			shadow->SetJumping(false);
		}
		if (mInput->KeyPressed(SDL_SCANCODE_LEFTBRACKET)) {
			int newPlane = shadow->GetCurrentPlane() - 1;
			if (newPlane < 0) newPlane += ((Level*)shadow->GetParent())->GetTotalPlanes();

			shadow->SetCurrentPlane(newPlane);
		}
		if (mInput->KeyPressed(SDL_SCANCODE_RIGHTBRACKET)) {
			int newPlane = shadow->GetCurrentPlane() + 1;
			if (newPlane >= ((Level*)shadow->GetParent())->GetTotalPlanes()) newPlane -= ((Level*)shadow->GetParent())->GetTotalPlanes();

			shadow->SetCurrentPlane(newPlane);
		}
	}

	void Inverted(unsigned int deltaTime, Shadow* shadow) {
		InputManager* mInput = InputManager::Instance();
		if (mInput->KeyDown(SDL_SCANCODE_D)) {
			shadow->SetMovingLeft(true);
			shadow->SetFlip(SDL_FLIP_NONE);
			shadow->SetDirection(Direction::LEFT);
		} else {
			shadow->SetMovingLeft(false);
		}
		if (mInput->KeyDown(SDL_SCANCODE_A)) {
			shadow->SetMovingRight(true);
			shadow->SetFlip(SDL_FLIP_HORIZONTAL);
			shadow->SetDirection(Direction::RIGHT);
		} else {
			shadow->SetMovingRight(false);
		}
		if (mInput->KeyDown(SDL_SCANCODE_SPACE)) {
			if (shadow->CanJump() && shadow->IsFloored()) {
				shadow->SetCanJump(false);
				shadow->SetJumping(true);
				shadow->SetFloored(false);
				shadow->AddAcceleration(Vector2(0.0f, shadow->GetJumpSpeed()));
			}
		} else {
			shadow->SetCanJump(true);
			shadow->SetJumping(false);
		}
		if (mInput->KeyPressed(SDL_SCANCODE_LEFTBRACKET)) {
			int newPlane = shadow->GetCurrentPlane() - 1;
			if (newPlane < 0) newPlane += ((Level*)shadow->GetParent())->GetTotalPlanes();

			shadow->SetCurrentPlane(newPlane);
		}
		if (mInput->KeyPressed(SDL_SCANCODE_RIGHTBRACKET)) {
			int newPlane = shadow->GetCurrentPlane() + 1;
			if (newPlane >= ((Level*)shadow->GetParent())->GetTotalPlanes()) newPlane -= ((Level*)shadow->GetParent())->GetTotalPlanes();

			shadow->SetCurrentPlane(newPlane);
		}
	}

	void Random(unsigned int deltaTime, Shadow* shadow) {
		InputManager* mInput = InputManager::Instance();
		if (mInput->KeyPressed(SDL_SCANCODE_LEFTBRACKET)) {
			int newPlane = shadow->GetCurrentPlane() - 1;
			if (newPlane < 0) newPlane += ((Level*)shadow->GetParent())->GetTotalPlanes();

			shadow->SetCurrentPlane(newPlane);
		}
		if (mInput->KeyPressed(SDL_SCANCODE_RIGHTBRACKET)) {
			int newPlane = shadow->GetCurrentPlane() + 1;
			if (newPlane >= ((Level*)shadow->GetParent())->GetTotalPlanes()) newPlane -= ((Level*)shadow->GetParent())->GetTotalPlanes();

			shadow->SetCurrentPlane(newPlane);
		}

		int decision = rand() % 50;
		if (decision > 6) {
			return;
		}

		shadow->SetMovingRight(false);
		shadow->SetMovingLeft(false);
		shadow->SetJumping(false);

		switch (decision) {
		case 0:
		case 1:
		case 2:
			shadow->SetMovingRight(true);
			shadow->SetDirection(Direction::RIGHT);
			break;
		case 3:
		case 4:
		case 5:
			shadow->SetMovingLeft(true);
			shadow->SetDirection(Direction::LEFT);
			break;
		case 6:
			if (shadow->IsFloored() && shadow->CanJump()) {
				shadow->SetJumping(true);
				shadow->SetFloored(false);
				shadow->AddAcceleration(Vector2(0.0f, shadow->GetJumpAcceleration()));
			}
			break;
		default:
			break;
		}
	}

	void Mario(unsigned int deltaTime, Shadow* mirror) {

	}
}