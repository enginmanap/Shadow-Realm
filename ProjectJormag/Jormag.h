#pragma once

#ifndef JORMAG_H
#define JORMAG_H

#define GRAVITY 9.81f
#define TERMINAL_VELOCITY 53.0f

#include "AnimatedTexture.h"
#include "Graphics.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "Menu.h"
#include "StateManager.h"

using namespace Jormag;

namespace Direction {
	enum Facing {
		LEFT,
		RIGHT
	};
}

#endif