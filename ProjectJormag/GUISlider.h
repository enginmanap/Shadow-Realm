#pragma once

#ifndef GUISLIDER_H
#define GUISLIDER_H

#include "GUIObject.h"

namespace Jormag {

	class GUISlider : public GUIObject {
	private:
		float mMin, mMax;
		float mCurrent;
	public:
		GUISlider(float min, float max, float initValue);
		GUISlider(float min, float max, float initValue, std::string backTexturePath, std::string focusTexturePath);
		~GUISlider();

		void Tick(unsigned int deltaTime);
		void Render(SDL_Renderer* renderer);

		float GetMinValue();
		float GetMaxValue();
		float GetCurrentValue();
		
		void ModifyValue(float change);
	};
}

#endif