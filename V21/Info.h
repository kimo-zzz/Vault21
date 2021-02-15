#pragma once

#include "MenuComponent.h"

namespace V21 {
	namespace SDK {
		class Info : public MenuComponent {
		public:
			Info(const char* name, const char* displayName);

			void AddTooltip(const char* tooltip);
			void GetSave(json& j);
			Vector2 GetPosition();
			float GetWidth();
			float NeededWidth();
			void Draw();
			void WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos);
		};
	}
}