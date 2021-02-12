#pragma once

#include "MenuComponent.h"
#include "CheckBox.h" 
#include "Info.h"
#include "Keybind.h"
#include "List.h"
#include "Slider.h"

namespace V21 {
	namespace SDK {
		class Menu : public MenuComponent {
		private:
			Menu(const char* name, const char* displayName);
			~Menu();
			void SetVisible(bool value);
		public:
			static Vector2 BasePosition;
			static Vector2 DragPosition;
			static bool IsDragging;
			static std::vector<Menu*> RootMenus;
			static json MenuSave;

			static void Initialize();
			static void Dispose();
			static void OnDraw();
			static void OnWndProc(UINT msg, WPARAM wparam);
			static Menu* CreateMenu(const char* name, const char* displayName);

			std::vector<Menu*> Children;
			std::vector<MenuComponent*> Components;

			Menu* AddMenu(const char* name, const char* displayName);
			CheckBox* AddCheckBox(const char* name, const char* displayName, bool defaultValue = false, std::function<void(CheckBox*, bool)> callback = nullptr);
			Info* AddInfo(const char* name, const char* displayName);
			KeyBind* AddKeyBind(const char* name, const char* displayName, unsigned char key, bool defaultValue = false, bool isToggle = false, std::function<void(KeyBind*, bool)> callback = nullptr);
			List* AddList(const char* name, const char* displayName, std::vector<std::string> items, unsigned int defaultIndex = 0, std::function<void(List*, unsigned int)> callback = nullptr);
			Slider* AddSlider(const char* name, const char* displayName, int defaultValue = 100, int minimumValue = 0, int maximumValue = 100, int step = 1, std::function<void(Slider*, int)> callback = nullptr);

			bool IsVisible();

			void AddTooltip(const char* tooltip);
			void GetSave(json& j);
			Vector2 GetPosition();
			float GetWidth();
			float NeededWidth();
			void Draw();
			void WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos);

			MenuComponent* operator[](std::string key);
		};
	}
}