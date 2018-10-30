#pragma once

#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "GameObject.h"

namespace Jormag {

	class CollisionManager {
	private:
		static CollisionManager* sInstance;
	public:
		enum CheckType { BOX, CIRCLE, PIXEL };
	private:
		CollisionManager();
		~CollisionManager();
	public:
		static CollisionManager* Instance();
		static void Free();

		bool Check(GameObject* o1, GameObject* o2, CheckType checkType);
	};
}

#endif