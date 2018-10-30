#include "CollisionManager.h"

namespace Jormag {

	CollisionManager* CollisionManager::sInstance = NULL;

	CollisionManager* CollisionManager::Instance() {
		if (sInstance == NULL)
			sInstance = new CollisionManager();

		return sInstance;
	}

	void CollisionManager::Free() {
		delete sInstance;
		sInstance = NULL;
	}

	CollisionManager::CollisionManager() {
		return;
	}

	CollisionManager::~CollisionManager() {
		return;
	}

	bool CollisionManager::Check(GameObject* o1, GameObject* o2, CheckType checkType) {
		if (o1->GetCollisionType() == Collisions::Type::NONE || o2->GetCollisionType() == Collisions::Type::NONE)
			return false;

		Vector2 pos1 = o1->GetPos(GameObject::WORLD);
		Vector2 pos2 = o2->GetPos(GameObject::WORLD);

		double distance;
		double collisionLimit;
		SDL_Rect box1;
		SDL_Rect box2;

		switch (checkType) {
		default:
		case CIRCLE:
			distance = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
			collisionLimit = o1->GetCollisionRadius() + o2->GetCollisionRadius();
			return distance < collisionLimit;
		case BOX:
			box1 = o1->GetCollisionBox();
			box2 = o2->GetCollisionBox();
			if (box1.y + box1.h <= box2.y ||
				box1.y >= box2.y + box2.h ||
				box1.x + box1.w <= box2.x ||
				box1.x >= box2.x + box2.w)
				return false;
			else
				return true;
		case PIXEL:
			return false;
		}

	}
}
