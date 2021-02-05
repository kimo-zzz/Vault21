#pragma once

#include "MenuComponent.h"

namespace HACKUZAN {
	namespace SDK {
		class CheckBox : public MenuComponent {
		public:
			CheckBox(const char* name, const char* displayName, bool defaultValue, std::function<void(CheckBox*, bool)> callback);

			bool Value;
			std::function<void(CheckBox*, bool)> Callback;

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