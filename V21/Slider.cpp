#include "stdafx.h"
#include "Slider.h"
#include "Menu.h"
#include "MenuSettings.h"
#include "Renderer.h"
#include "HudManager.h"

namespace HACKUZAN {
	namespace SDK {
		Slider::Slider(const char* name, const char* displayName, int defaultValue, int minimumValue, int maximumValue, int step, std::function<void(Slider*, int)> callback) {
			strncpy(this->Name, name, sizeof(this->Name));
			strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
			this->MinimumValue = std::min(minimumValue, maximumValue);
			this->MaximumValue = std::max(maximumValue, minimumValue);
			this->Value = std::min(this->MaximumValue, std::max(this->MinimumValue, defaultValue));
			this->Step = step;
			this->Callback = callback;
			this->Tooltip[0] = 0;
		}

		void Slider::AddTooltip(const char* tooltip)
		{
			strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
		}

		void Slider::GetSave(json& j) {
			j[this->Name] = json{ {"value", this->Value} };
		}

		Vector2 Slider::GetPosition() {
			auto& components = this->Parent->Components;
			for (auto i = 0; i < components.size(); i++) {
				auto component = components[i];
				if (component == this) {
					return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * (i + this->Parent->Children.size()));
				}
			}
		}

		float Slider::GetWidth() {
			auto value = 0.0f;

			for (auto child : this->Parent->Children) {
				value = std::max(value, child->NeededWidth());
			}
			for (auto component : this->Parent->Components) {
				value = std::max(value, component->NeededWidth());
			}

			return std::max(MenuComponent::Width, value);
		}

		float Slider::NeededWidth() {
			return 10.0f + Renderer::DefaultFont->CalcTextSizeA(16, FLT_MAX, 0.0f, this->DisplayName).x + 5.0f + Renderer::DefaultFont->CalcTextSizeA(16, FLT_MAX, 0.0f, std::to_string(this->MaximumValue).c_str()).x + 5.0f;
		}

		void Slider::Draw() {
			if (!this->Visible) {
				return;
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.X, position.Y, this->GetWidth(), MenuComponent::Height);
			auto percentage = ((float)(this->Value - this->MinimumValue) / (this->MaximumValue - this->MinimumValue));

			if (this->Value != this->MinimumValue) {
				Renderer::AddRectangleFilled(Rect(rect.Position.X, rect.Position.Y, rect.Width * percentage, rect.Height), IM_COL32(160, 0, 0, MenuSettings::BackgroundOpacity));
			}
			if (this->Value != this->MaximumValue) {
				Renderer::AddRectangleFilled(Rect(rect.Position.X + rect.Width * percentage, rect.Position.Y, rect.Width * (1 - percentage), rect.Height), IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			}

			Renderer::AddRectangle(rect, IM_COL32(0, 0, 0, 160));
			Renderer::AddLine(Vector2(rect.Position.X + rect.Width * percentage, rect.Position.Y), Vector2(rect.Position.X + rect.Width * percentage, rect.Position.Y + rect.Height), 2.0f, IM_COL32(255, 0, 0, 255));
			Renderer::AddText(this->DisplayName, 14.0f, Rect(rect.Position.X + 10.0f, rect.Position.Y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
			Renderer::AddText(14.0f, Rect(rect.Position.X, rect.Position.Y, rect.Width - 5.0f, rect.Height), DT_RIGHT | DT_VCENTER, IM_COL32(255, 255, 255, 255), "%d", this->Value);

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

		void Slider::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos) {
			if (!this->Visible || !this->Parent->IsVisible()) {
				return;
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.X, position.Y, this->GetWidth(), MenuComponent::Height);

			if ((msg == WM_MOUSEMOVE && this->Interacting) || (msg == WM_LBUTTONDOWN && !this->Interacting && rect.Contains(cursorPos))) {
				this->Value = std::min(this->MaximumValue, std::max(this->MinimumValue, (int)((((cursorPos.X - rect.Position.X) / rect.Width) * (this->MaximumValue - this->MinimumValue) + this->MinimumValue) / this->Step) * this->Step));
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
			}

			if (msg == WM_LBUTTONDOWN && rect.Contains(cursorPos) || msg == WM_LBUTTONUP) {
				this->Interacting = msg == WM_LBUTTONDOWN;
			}
		}
	}
}
