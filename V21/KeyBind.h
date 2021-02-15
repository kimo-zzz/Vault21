#pragma once

#include "MenuComponent.h"

namespace V21 {
	namespace SDK {
		class KeyBind : public MenuComponent {
		public:
			KeyBind(const char* name, const char* displayName, unsigned char key, bool defaultValue, bool isToggle, std::function<void(KeyBind*, bool)> callback);

			unsigned char Key;
			bool IsToggle;
			bool Value;
			std::function<void(KeyBind*, bool)> Callback;

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