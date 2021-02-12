#pragma once

#include "MenuComponent.h"

namespace V21 {
	namespace SDK {
		class List : public MenuComponent {
		public:
			List(const char* name, const char* displayName, std::vector<std::string> items, unsigned int defaultIndex, std::function<void(List*, unsigned int)> callback);

			std::vector<std::string> Items;
			unsigned int Value;
			std::function<void(List*, unsigned int)> Callback;

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