#pragma once

#ifndef MENU_H
#define MENU_H

#include "GameObject.h"
#include "GUIObject.h"

namespace Jormag {
	struct Menu {
		GameObject* mContentPane;
		int mNumChildren;
		GUIObject** mChildren;

		Menu(int numChildren) {
			mNumChildren = numChildren;
			mChildren = new GUIObject*[mNumChildren];
		}

		~Menu() {
			delete mContentPane;
			mContentPane = NULL;

			for (int i = 0; i < mNumChildren; i++) {
				delete mChildren[i];
				mChildren[i] = NULL;
			}
			mChildren = NULL;
		}

		void SetObjectFocus(int indexFocused) {
			for (int i = 0; i < mNumChildren; i++) {
				mChildren[i]->SetFocused(false);
			}
			mChildren[indexFocused]->SetFocused(true);
		}
	};
}

#endif