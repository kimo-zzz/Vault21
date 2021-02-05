#include "stdafx.h"
#include "CheckBox.h"
#include "Menu.h"
#include "MenuSettings.h"
#include "Renderer.h"
#include "HudManager.h"

namespace HACKUZAN
{
	namespace SDK
	{
		CheckBox::CheckBox(const char* name, const char* displayName, bool defaultValue, std::function<void(CheckBox*, bool)> callback)
		{
			strncpy(this->Name, name, sizeof(this->Name));
			strncpy(this->DisplayName, displayName, sizeof(this->DisplayName));
			this->Value = defaultValue;
			this->Callback = callback;
			this->Tooltip[0] = 0;
		}

		void CheckBox::AddTooltip(const char* tooltip)
		{
			strncpy(this->Tooltip, tooltip, sizeof(this->Tooltip));
		}

		void CheckBox::GetSave(json& j)
		{
			j[this->Name] = json{ {"value", this->Value} };
		}

		Vector2 CheckBox::GetPosition()
		{
			auto& components = this->Parent->Components;
			for (auto i = 0; i < components.size(); i++)
			{
				auto component = components[i];
				if (component == this)
				{
					return this->Parent->GetPosition() + Vector2(this->Parent->GetWidth(), MenuComponent::Height * (i + this->Parent->Children.size()));
				}
			}
		}

		float CheckBox::GetWidth()
		{
			auto value = 0.0f;

			for (auto child : this->Parent->Children)
			{
				value = std::max(value, child->NeededWidth());
			}
			for (auto component : this->Parent->Components)
			{
				value = std::max(value, component->NeededWidth());
			}

			return std::max(MenuComponent::Width, value);
		}

		float CheckBox::NeededWidth()
		{
			return 10.0f + Renderer::DefaultFont->CalcTextSizeA(16, FLT_MAX, 0.0f, this->DisplayName).x + 5.0f + MenuComponent::Height;
		}

		void CheckBox::Draw()
		{
			if (!this->Visible)
			{
				return;
			}

			auto position = this->GetPosition();
			auto rect = Rect(position.X, position.Y, this->GetWidth(), MenuComponent::Height);
			auto box = Rect(rect.Position.X + rect.Width - rect.Height, rect.Position.Y, rect.Height, rect.Height);

			Renderer::AddRectangleFilled(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddRectangle(rect, IM_COL32(0, 0, 0, MenuSettings::BackgroundOpacity));
			Renderer::AddText(this->DisplayName, 14.0f, Rect(rect.Position.X + 10.0f, rect.Position.Y, 0.0f, rect.Height), DT_VCENTER, IM_COL32(255, 255, 255, 255));
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

		void CheckBox::WndProc(UINT msg, WPARAM wparam, Vector2 cursorPos)
		{
			if (msg != WM_LBUTTONUP || !this->Visible || !this->Parent->IsVisible())
			{
				return;
			}

			auto position = this->GetPosition();
			auto box = Rect(position.X + this->GetWidth() - MenuComponent::Height, position.Y, MenuComponent::Height, MenuComponent::Height);

			if (box.Contains(cursorPos)) {
				this->Value = !this->Value;
				if (this->Callback) {
					this->Callback(this, this->Value);
				}
			}
		}
	}
}
