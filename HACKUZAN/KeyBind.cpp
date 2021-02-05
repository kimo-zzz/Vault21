#include "stdafx.h"
#include "KeyBind.h"
#include "Menu.h"
#include "MenuSettings.h"
#include "Renderer.h"
#include "HudManager.h"

namespace HACKUZAN {
	namespace SDK {
		static const char* VirtualKeys[] = {
			"Left Mouse",
			"Right Mouse",
			"Cancel",
			"Middle Mouse",
			"X1 Mouse",
			"X2 Mouse",
			"Undefined",
			"Backspace",
			"Tab",
			"Reserved",
			"Reserved",
			"Clear",
			"Enter",
			"Undefined",
			"Undefined",
			"Shift",
			"Ctrl",
			"Alt Gr",
			"Pause",
			"Caps Lock",
			"Hanguel",
			"Undefined",
			"Junja",
			"Final",
			"Kanji",
			"Undefined",
			"Esc",
			"Convert",
			"Non Convert",
			"Accept",
			"Mode Change Request",
			"Space",
			"Page Up",
			"Page Down",
			"End",
			"Home",
			"Left Arrow",
			"Up Arrow",
			"Right Arrow",
			"Down Arrow",
			"Select",
			"Print",
			"Execute",
			"Print Screen",
			"Insert",
			"Delete",
			"Help",
			"0",
			"1",
			"2",
			"3",
			"4",
			"5",
			"6",
			"7",
			"8",
			"9",
			"Undefined",
			"Undefined",
			"Undefined",
			"Undefined",
			"Undefined",
			"Undefined",
			"Undefined",
			"A",
			"B",
			"C",
			"D",
			"E",
			"F",
			"G",
			"H",
			"I",
			"J",
			"K",
			"",
			"M",
			"N",
			"O",
			"P",
			"Q",
			"R",
			"S",
			"T",
			"U",
			"V",
			"W",
			"X",
			"Y",
			"Z",
			"Windows",
			"Windows",
			"Applications",
			"Reserved",
			"Sleep",
			"Numpad 0",
			"Numpad 1",
			"Numpad 2",
			"Numpad 3",
			"Numpad 4",
			"Numpad 5",
			"Numpad 6",
			"Numpad 7",
			"Numpad 8",
			"Numpad 9",
			"*",
			"+",
			"_",
			"-",
			".",
			"/",
			"F1",
			"F2",
			"F3",
			"F4",
			"F5",
			"F6",
			"F7",
			"F8",
			"F9",
			"F10",
			"F11",
			"F12",
			"F13",
			"F14",
			"F15",
			"F16",
			"F17",
			"F18",
			"F19",
			"F20",
			"F21",
			"F22",
			"F23",
			"F24",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Num Lock",
			"Scroll Lock",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Left Shift",
			"Right Shift",
			"Left Control",
			"Right Control",
			"Left Menu",
			"Right Menu",
			"Browser Back",
			"Browser Forward",
			"Browser Refresh",
			"Browser Stop",
			"Browser Search",
			"Browser Favorites",
			"Browser Home",
			"Volume Mute",
			"Volume Down",
			"Volume Up",
			"Next Track",
			"Previous Track",
			"Stop Media",
			"Play/Pause Media",
			"Start Mail",
			"Select Media",
			"Start Application 1",
			"Start Application 2",
			"Reserved",
			"Reserved",
			";",
			"+",
			",",
			"-",
			".",
			"/",
			"#",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Reserved",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"Unassigned",
			"[",
			"\\",
			"]",
			"'",
			"`",
			"Reserved",
			"OEM Specific",
			"OEM 102",
			"OEM Specific",
			"OEM Specific",
			"IME Process",
			"OEM Specific",
			"Packet",
			"Unassigned",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"OEM Specific",
			"Attn",
			"CrSel",
			"ExSel",
			"Erase EOF",
			"Play",
			"Zoom",
			"Reserved",
			"PA1",
			"Clear"
		};

		KeyBind::KeyBind(const char* name, const char* displayName, unsigned char key, bool defaultValue, bool isToggle, std::function<void(KeyBind*, bool)> callback) {
			strncpy(this->Name, name, sizeof(this->Name));
			strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
			this->Key = key;
			this->Value = defaultValue;
			this->IsToggle = isToggle;
			this->Callback = callback;
			this->Tooltip[0] = 0;
		}

		void KeyBind::AddTooltip(const char* tooltip)
		{
			strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
		}

		void KeyBind::GetSave(json& j) {
			j[this->Name] = json{ {"key", this->Key} };
		}

		Vector2 KeyBind::GetPosition() {
			auto& components = this->Parent->Components;
			for (auto i = 0; i < components.size(); i++) {
				auto component = components[i];
				if (component == this) {
					return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * (i + this->Parent->Children.size()));
				}
			}
		}

		float KeyBind::GetWidth() {
			auto value = 0.0f;

			for (auto child : this->Parent->Children) {
				value = std::max(value, child->NeededWidth());
			}
			for (auto component : this->Parent->Components) {
				value = std::max(value, component->NeededWidth());
			}

			return std::max(MenuComponent::Width, value);
		}

		float KeyBind::NeededWidth() {
			return 10.0f + Renderer::DefaultFont->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, this->DisplayName).x + 5.0f + Renderer::DefaultFont->CalcTextSizeA(14.0f, FLT_MAX, 0.0f, "[Down Arrow]").x + 3.0f + MenuComponent::Height;
		}

		void KeyBind::Draw() {
			if (!this->Visible) {
				return;
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.X, position.Y, this->GetWidth(), MenuComponent::Height);
			auto box = Rect(rect.Position.X + rect.Width - rect.Height, rect.Position.Y, rect.Height, rect.Height);

			Renderer::AddRectangleFilled(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddRectangle(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			if (this->Interacting) {
				Renderer::AddText("Press a new key", 14.0f, Rect(rect.Position.X + 10.0f, rect.Position.Y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
			}
			else {
				Renderer::AddText(this->DisplayName, 14.0f, Rect(rect.Position.X + 10.0f, rect.Position.Y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
				Renderer::AddText(14.0f, Rect(rect.Position.X, rect.Position.Y, box.Position.X - rect.Position.X - 3.0f, rect.Height), DT_VCENTER | DT_RIGHT, IM_COL32(255, 0, 0, 255), "[%s]", VirtualKeys[this->Key - 1]);
			}
			Renderer::AddRectangleFilled(box, this->Value ? IM_COL32(160, 0, 0, 255) : IM_COL32(37, 37, 37, 255));
			Renderer::AddRectangle(box, IM_COL32(0, 0, 0, 255));
			Renderer::AddText(this->Value ? "ON" : "OFF", 14.0f, box, DT_CENTER | DT_VCENTER, IM_COL32(255, 255, 255, 255));

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

		void KeyBind::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos) {
			if (msg == WM_KEYUP && this->Interacting) {
				this->Key = wparam;
				this->Interacting = false;
				this->Value = false;
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
				return;
			}

			if (wparam == this->Key && !this->Interacting) {
				if (msg == WM_KEYDOWN && !this->IsToggle && !this->Value) {
					this->Value = true;
					if (this->Callback) {
						this->Callback(this, this->Value);
					}
				}
				else if (msg == WM_KEYUP) {
					if (this->IsToggle) {
						this->Value = !this->Value;
					}
					else {
						this->Value = false;
					}

					if (this->Callback) {
						this->Callback(this, this->Value);
					}
				}
			}

			if (msg != WM_LBUTTONDOWN || !this->Visible || !this->Parent->IsVisible()) {
				return;
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.X, position.Y, this->GetWidth(), MenuComponent::Height);

			if (!rect.Contains(cursorPos)) {
				return;
			}

			auto box = Rect(rect.Position.X + rect.Width - rect.Height, rect.Position.Y, rect.Height, rect.Height);

			if (box.Contains(cursorPos)) {
				this->Value = !this->Value;
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
			}
			else {
				this->Interacting = !this->Interacting;
			}
		}
	}
}
