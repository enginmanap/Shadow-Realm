#pragma once

#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include "EngineManager.h"

namespace Jormag {
	class StateManager {
	public:
		StateManager() { };
		~StateManager() { };

		virtual void Tick(unsigned int deltaTime) = 0;
		virtual void Render(SDL_Renderer* renderer) = 0;
	};
}

#endif