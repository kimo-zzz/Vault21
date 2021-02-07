#pragma once

#include "stdafx.h"
#include "Rect.h"

namespace HACKUZAN {
	namespace SDK {
		class Menu;

		class MenuComponent {
		public:
			static float Width;
			static float Height;

			bool Visible = false;
			bool Interacting = false;
			char Name[40];
			char DisplayName[40];
			char Tooltip[100];
			Menu* Parent = nullptr;
			json Save;

			virtual void GetSave(json& j) = 0;
			virtual Vector2 GetPosition() = 0;
			virtual float GetWidth() = 0;
			virtual float NeededWidth() = 0;
			virtual void Draw() = 0;
			virtual void WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos) = 0;

			virtual void AddTooltip(const char* tooltip) = 0;

			template <class T>
			T Cast() {
				return (T)this;
			}
		};
	}
}