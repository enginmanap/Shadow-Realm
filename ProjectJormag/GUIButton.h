#pragma once

#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include "GUIObject.h"

namespace Jormag {

	class GUIButton : public GUIObject {
	public:
		GUIButton();
		GUIButton(std::string backTexturePath, std::string focusTexturePath);
		~GUIButton();

		void Tick(unsigned int deltaTime);
	};
}

#endif