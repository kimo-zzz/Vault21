#include "stdafx.h"
#include "Menu.h"
#include "MenuSettings.h"
#include "Renderer.h"
#include "HudManager.h"
#include "Keybind.h"
#include "List.h"
#include "Slider.h"

namespace HACKUZAN {
	namespace SDK {
		Vector2 Menu::BasePosition = Vector2(30.0f, 30.0f);
		Vector2 Menu::DragPosition = Vector2();
		bool Menu::IsDragging = false;
		std::vector<Menu*> Menu::RootMenus;
		json Menu::MenuSave;

		Menu::Menu(const char* name, const char* displayName) {
			strncpy(this->Name, name, sizeof(this->Name));
			strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
			this->Tooltip[0] = 0;
		}

		Menu::~Menu() {
			for (auto child : this->Children) {
				delete child;
			}

			for (auto component : this->Components) {
				delete component;
			}
		}

		void Menu::SetVisible(bool value) {
			this->Visible = value;

			if (!value) {
				this->Interacting = false;
				for (auto menu : this->Children) {
					menu->SetVisible(false);
				}
				for (auto component : this->Components) {
					component->Visible = false;
					component->Interacting = false;
				}
			}
		}

		void Menu::Initialize() {
			std::ifstream file("MenuComponent.save");
			if (file.is_open()) {
				file >> MenuSave;
				auto& savedBasePositionX = MenuSave["HACKUZAN"]["MenuSettings"]["BasePosition"]["X"];
				auto& savedBasePositionY = MenuSave["HACKUZAN"]["MenuSettings"]["BasePosition"]["Y"];
				if (!savedBasePositionX.is_null() && !savedBasePositionY.is_null()) {
					BasePosition = Vector2(savedBasePositionX, savedBasePositionY);
				}
			}

			auto menu = Menu::CreateMenu("HACKUZAN", "HACKUZAN");

			auto menuSettings = menu->AddMenu("MenuSettings", "Menu Settings");
			MenuSettings::BackgroundOpacity = menuSettings->AddSlider("BackgroundOpacity", "Background Opacity", 160, 55, 255, 1, [](Slider*, int value) {
				MenuSettings::BackgroundOpacity = value;
				})->Value;

			MenuSettings::DrawMenu = false;
			menuSettings->AddKeyBind("MenuKey", "Menu Key", VK_SHIFT, false, false, [](KeyBind*, bool value) {
				MenuSettings::DrawMenu = value;
				});

			auto coreMenu = menu->AddMenu("Core", "Core");
			MenuSettings::TicksPerSecond = coreMenu->AddSlider("TicksPerSecond", "Ticks per second", 30, 1, 75, 1, [](Slider*, int value) {
				MenuSettings::TicksPerSecond = value;
				})->Value;

			MenuSettings::DeveloperMode = coreMenu->AddCheckBox("DeveloperMode", "Developer Mode", false, [](CheckBox*, bool value) {
				MenuSettings::DeveloperMode = value;
				})->Value;

			auto hacksMenu = menu->AddMenu("Hacks", "Hacks");
			MenuSettings::AntiAFK = hacksMenu->AddCheckBox("AntiAFK", "Anti AFK", false, [](CheckBox*, bool value) {
				MenuSettings::AntiAFK = value;
				})->Value;
		}

		void Menu::Dispose() {
			for (auto menu : RootMenus) {
				menu->GetSave(MenuSave);
				delete menu;
			}
			RootMenus.clear();

			MenuSave["HACKUZAN"]["MenuSettings"]["BasePosition"]["X"] = BasePosition.X;
			MenuSave["HACKUZAN"]["MenuSettings"]["BasePosition"]["Y"] = BasePosition.Y;

			std::ofstream file("MenuComponent.save");
			if (file.is_open()) {
				file << MenuSave.dump(4);
			}
		}

		void Menu::OnDraw() {
			if (!MenuSettings::DrawMenu) {
				return;
			}

			for (auto menu : RootMenus) {
				menu->Draw();
			}
		}

		void Menu::OnWndProc(UINT msg, WPARAM wparam) {
			if (IsDragging) {
				if (msg == WM_MOUSEMOVE) {
					BasePosition = DragPosition + HudManager::CursorPos2D;
				}
				else if (msg == WM_LBUTTONUP) {
					IsDragging = false;
				}
			}

			for (auto menu : RootMenus) {
				menu->WndProc(msg, wparam, HudManager::CursorPos2D);
			}
		}

		Menu* Menu::CreateMenu(const char* name, const char* displayName) {
			auto menu = new Menu(name, displayName);
			menu->Save = MenuSave[name];
			RootMenus.push_back(menu);
			return menu;
		}

		Menu* Menu::AddMenu(const char* name, const char* displayName) {
			auto menu = new Menu(name, displayName);
			menu->Save = this->Save[name];
			menu->Parent = this;
			this->Children.push_back(menu);
			return menu;
		}

		CheckBox* Menu::AddCheckBox(const char* name, const char* displayName, bool defaultValue, std::function<void(CheckBox*, bool)> callback) {
			auto component = new CheckBox(name, displayName, defaultValue, callback);
			component->Save = this->Save[name];
			auto& savedValue = component->Save["value"];
			if (!savedValue.is_null()) {
				component->Value = savedValue;
			}
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		Info* Menu::AddInfo(const char* name, const char* displayName) {
			auto component = new Info(name, displayName);
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		KeyBind* Menu::AddKeyBind(const char* name, const char* displayName, unsigned char key, bool defaultValue, bool isToggle, std::function<void(KeyBind*, bool)> callback) {
			auto component = new KeyBind(name, displayName, key, defaultValue, isToggle, callback);
			component->Save = this->Save[name];
			auto& savedKey = component->Save["key"];
			if (!savedKey.is_null()) {
				component->Key = savedKey;
			}
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		List* Menu::AddList(const char* name, const char* displayName, std::vector<std::string> items, unsigned int defaultIndex, std::function<void(List*, unsigned int)> callback) {
			auto component = new List(name, displayName, items, defaultIndex, callback);
			component->Save = this->Save[name];
			auto& savedValue = component->Save["value"];
			if (!savedValue.is_null()) {
				component->Value = std::min((unsigned int)savedValue, items.size() - 1);
			}
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		Slider* Menu::AddSlider(const char* name, const char* displayName, int defaultValue, int minimumValue, int maximumValue, int step, std::function<void(Slider*, int)> callback) {
			auto component = new Slider(name, displayName, defaultValue, minimumValue, maximumValue, step, callback);
			component->Save = this->Save[name];
			auto& savedValue = component->Save["value"];
			if (!savedValue.is_null()) {
				component->Value = std::min(component->MaximumValue, std::max(component->MinimumValue, (int)savedValue));
			}
			component->Parent = this;
			this->Components.push_back(component);
			return component;
		}

		bool Menu::IsVisible() {
			return MenuSettings::DrawMenu && (!this->Parent || this->Visible);
		}

		void Menu::AddTooltip(const char* tooltip)
		{
			strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
		}

		void Menu::GetSave(json & j) {
			j[this->Name] = {};

			for (auto child : this->Children) {
				child->GetSave(j[this->Name]);
			}

			for (auto component : this->Components) {
				component->GetSave(j[this->Name]);
			}

			if (j[this->Name].is_null()) {
				j.erase(this->Name);
			}
		}

		Vector2 Menu::GetPosition() {
			if (this->Parent) {
				auto& children = this->Parent->Children;
				for (auto i = 0; i < children.size(); i++) {
					auto child = children[i];
					if (child == this) {
						return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * i);
					}
				}
			}
			else {
				for (auto i = 0; i < RootMenus.size(); i++) {
					auto menu = RootMenus[i];
					if (menu == this) {
						return BasePosition + Vector2(0, MenuComponent::Height * i);
					}
				}
			}
		}

		float Menu::GetWidth() {
			auto value = 0.0f;

			if (this->Parent) {
				for (auto child : this->Parent->Children) {
					value = std::max(value, child->NeededWidth());
				}
				for (auto component : this->Parent->Components) {
					value = std::max(value, component->NeededWidth());
				}
			}
			else {
				for (auto menu : RootMenus) {
					value = std::max(value, menu->NeededWidth());
				}
			}

			return std::max(MenuComponent::Width, value);
		}

		float Menu::NeededWidth() {
			return 10.0f + Renderer::DefaultFont->CalcTextSizeA(16, FLT_MAX, 0.0f, this->DisplayName).x + 5.0f + MenuComponent::Height * 0.45f;
		}

		void Menu::Draw() {
			if (!this->IsVisible()) {
				return;
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.X, position.Y, this->GetWidth(), MenuComponent::Height);

			Renderer::AddRectangleFilled(rect, this->Interacting ? IM_COL32(160, 0, 0, MenuSettings::BackgroundOpacity) : IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddRectangle(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddText(this->DisplayName, 14.0f, Rect(rect.Position.X + 10.0f, rect.Position.Y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));

			if (!this->Children.empty() || !this->Components.empty()) {
				auto p1 = Vector2(rect.Position.X + rect.Width - (int)(rect.Height * 0.2f), rect.Position.Y + (int)(rect.Height * 0.5f));
				auto p2 = Vector2(rect.Position.X + rect.Width - (int)(rect.Height * 0.45f), rect.Position.Y + (int)(rect.Height * 0.65f));
				auto p3 = Vector2(rect.Position.X + rect.Width - (int)(rect.Height * 0.45f), rect.Position.Y + (int)(rect.Height * 0.35f));

				Renderer::AddTriangleFilled(p1, p2, p3, this->Interacting ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 255, 255));

				for (auto child : this->Children) {
					child->Draw();
				}

				for (auto component : this->Components) {
					component->Draw();
				}
			}

			//TODO
			if (this->Tooltip[0] != 0)
			{
				auto textWidth = 10.0f + Renderer::DefaultFont->CalcTextSizeA(14, FLT_MAX, 0.0f, this->DisplayName).x;
				auto mousePos = HudManager::CursorPos2D;
				auto iconRect = Rect(rect.Position.X + textWidth + 5, rect.Position.Y + Height * 0.5f - 10.0f, 20, 20);
				Renderer::AddText("(?)", 16.0f, iconRect, DT_VCENTER, IM_COL32(255, 30, 30, 255));

				if (iconRect.Contains(mousePos))
				{
					auto alpha = std::min(MenuSettings::BackgroundOpacity + 70, 255);
					auto black = IM_COL32(0, 0, 0, alpha);
					auto width = 20.0f + Renderer::DefaultFont->CalcTextSizeA(14, FLT_MAX, 0.0f, this->Tooltip).x;
					auto tooltipRect = Rect(mousePos.X + 20, mousePos.Y - Height * 0.5f, width, Height);
					Renderer::AddRoundedRectangleFilled(tooltipRect, black, 4, ImDrawCornerFlags_All);
					Renderer::AddRoundedRectangle(tooltipRect, black, 1.1f, 4, ImDrawCornerFlags_All);
					Renderer::AddText(this->Tooltip, 14.0f, Rect(tooltipRect.Position.X + 10.0f, tooltipRect.Position.Y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
				}
			}
		}

		void Menu::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos) {
			for (auto child : this->Children) {
				child->WndProc(msg, wparam, cursorPos);
			}

			for (auto component : this->Components) {
				component->WndProc(msg, wparam, cursorPos);
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.X, position.Y, this->GetWidth(), MenuComponent::Height);

			if (msg != WM_LBUTTONDOWN || !this->IsVisible() || !rect.Contains(cursorPos)) {
				return;
			}

			if (!IsDragging) {
				IsDragging = true;
				DragPosition = BasePosition - cursorPos;
			}

			if (!this->Parent) {
				for (auto menu : RootMenus) {
					if (menu != this) {
						menu->SetVisible(false);
					}
				}
			}
			else {
				for (auto menu : this->Parent->Children) {
					if (menu != this) {
						menu->Interacting = false;
						for (auto child : menu->Children) {
							child->SetVisible(false);
						}
						for (auto component : menu->Components) {
							component->Visible = false;
							component->Interacting = false;
						}
					}
				}
				for (auto component : this->Parent->Components) {
					component->Interacting = false;
				}
			}

			if (!this->Children.empty() || !this->Components.empty()) {
				this->Interacting = !this->Interacting;
			}

			for (auto child : this->Children) {
				child->SetVisible(!child->Visible);
			}

			for (auto component : this->Components) {
				component->Visible = !component->Visible;
				component->Interacting = false;
			}
		}

		MenuComponent* Menu::operator[](std::string key) {
			for (auto child : this->Children) {
				if (child->Name == key) {
					return child;
				}
			}

			for (auto component : this->Components) {
				if (component->Name == key) {
					return component;
				}
			}

			return nullptr;
		}
	}
}