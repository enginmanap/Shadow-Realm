#pragma once

#ifndef BEHAVIOURS_H
#define BEHAVIOURS_H

#include "Jormag.h"

#include "Shadow.h"

namespace Behaviours {

	enum Type {
		COPYCAT,
		INVERTED,
		RANDOM,
		MARIO
	};

	void CopyCat(unsigned int deltaTime, Shadow* shadow);
	void Inverted(unsigned int deltaTime, Shadow* shadow);
	void Random(unsigned int deltaTime, Shadow* shadow);
	void Mario(unsigned int deltaTime, Shadow* shadow);
}

#endif