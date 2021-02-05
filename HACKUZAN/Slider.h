#pragma once

#include "MenuComponent.h"

namespace HACKUZAN {
	namespace SDK {
		class Slider : public MenuComponent {
		public:
			Slider(const char* name, const char* displayName, int defaultValue, int minimumValue, int maximumValue, int step, std::function<void(Slider*, int)> callback);

			int Value;
			int MinimumValue;
			int MaximumValue;
			int Step;
			std::function<void(Slider*, int)> Callback;

			void AddTooltip(const char* tooltip);
			void GetSave(json& j);
			Vector2 GetPosition();
			float GetWidth();
			float NeededWidth();
			void Draw();
			void WndProc(UINT msg, WPARAM wparam, Vector2 mousePos);
		};
	}
}