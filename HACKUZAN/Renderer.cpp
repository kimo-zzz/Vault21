#include "stdafx.h"
#include "Renderer.h"
#include "Globals.h"
#include "DeviceHandler.h"
#include "RenderLayer.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "imgui_internal.h"

namespace HACKUZAN {
	ImDrawList* Renderer::DrawList;
	ImFont* Renderer::DefaultFont;

	void Renderer::Initialize() {
		ImGui::CreateContext();
		DrawList = ImGui::GetForegroundDrawList();
		DefaultFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 15.0f);
		ImGui_ImplWin32_Init(Globals::MainWindow);
		ImGui_ImplDX9_Init(DeviceHandler::Instance->Deviceptr->Direct3DDevice);
	}

	void Renderer::Initialize(IDirect3DDevice9* pdevice) {
		ImGui::CreateContext();
		DrawList = ImGui::GetForegroundDrawList();
		DefaultFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 15.0f);
		ImGui_ImplWin32_Init(Globals::MainWindow);
		ImGui_ImplDX9_Init(pdevice);
	}

	void Renderer::Dispose() {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void Renderer::PreReset() {
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void Renderer::PostReset() {
		ImGui_ImplDX9_CreateDeviceObjects();
	}

	void Renderer::NewFrame() {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void Renderer::RenderFrame() {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	void Renderer::AddCircle(Vector2 position, float radius, float thickness, DWORD color) {
		DrawList->AddCircle(*(ImVec2*)& position, radius, color, 100, thickness);
	}

	void Renderer::AddCircle(Vector3 position, float radius, float thickness, DWORD color) {
		if ((color & IM_COL32_A_MASK) == 0)
			return;

		DrawList->_Path.reserve(DrawList->_Path.Size + 100);
		for (auto i = 0; i <= 99; i++) {
			auto angle = (float)i * IM_PI * 1.98f / 99.0f;
			Vector2 pos;
			RenderLayer::WorldToScreen(Vector3(position.X + ImCos(angle) * radius, position.Y, position.Z + ImSin(angle) * radius), pos);
			DrawList->_Path.push_back(*(ImVec2*)& pos);
		}
		DrawList->PathStroke(color, true, thickness);
	}

	void Renderer::AddCircleFilled(Vector2 position, float radius, DWORD color) {
		DrawList->AddCircleFilled(*(ImVec2*)& position, radius, color, 100);
	}

	void Renderer::AddCircleFilled(Vector3 position, float radius, DWORD color) {
		if ((color & IM_COL32_A_MASK) == 0)
			return;

		DrawList->_Path.reserve(DrawList->_Path.Size + 100);
		for (auto i = 0; i <= 99; i++) {
			auto angle = (float)i * IM_PI * 1.98f / 99.0f;
			Vector2 pos;
			RenderLayer::WorldToScreen(Vector3(position.X + ImCos(angle) * radius, position.Y, position.Z + ImSin(angle) * radius), pos);
			DrawList->_Path.push_back(*(ImVec2*)& pos);
		}
		DrawList->PathFillConvex(color);
	}

	void Renderer::AddLine(Vector2 line1, Vector2 line2, float thickness, DWORD color) {
		DrawList->AddLine(*(ImVec2*)& line1, *(ImVec2*)& line2, color, thickness);
	}

	void Renderer::AddRectangle(Vector2 position, float width, float height, DWORD color) {
		DrawList->AddRect(*(ImVec2*)& position, ImVec2(position.X + width, position.Y + height), color);
	}

	void Renderer::AddRectangle(Rect rectangle, DWORD color, float thickness) {
		DrawList->AddRect(ImVec2(rectangle.Position.X, rectangle.Position.Y), ImVec2(rectangle.Position.X + rectangle.Width, rectangle.Position.Y + rectangle.Height), color, 0, ImDrawCornerFlags_All, thickness);
	}

	void Renderer::AddRoundedRectangle(Rect rectangle, DWORD color, float thickness, int rounding,
		int roundSettings)
	{
		DrawList->AddRect(ImVec2(rectangle.Position.X, rectangle.Position.Y), ImVec2(rectangle.Position.X + rectangle.Width, rectangle.Position.Y + rectangle.Height), color, rounding, roundSettings, thickness);
	}

	void Renderer::AddRectangleFilled(Vector2 position, float width, float height, DWORD color) {
		DrawList->AddRectFilled(*(ImVec2*)& position, ImVec2(position.X + width, position.Y + height), color);
	}

	void Renderer::AddRectangleFilled(Rect rectangle, DWORD color) {
		DrawList->AddRectFilled(ImVec2(rectangle.Position.X, rectangle.Position.Y), ImVec2(rectangle.Position.X + rectangle.Width, rectangle.Position.Y + rectangle.Height), color);
	}

	void Renderer::AddRoundedRectangleFilled(Rect rectangle, DWORD color, int rounding,
		int roundSettings)
	{
		DrawList->AddRectFilled(ImVec2(rectangle.Position.X, rectangle.Position.Y), ImVec2(rectangle.Position.X + rectangle.Width, rectangle.Position.Y + rectangle.Height), color, rounding, roundSettings);
	}

	void Renderer::AddText(const char* text, float size, Vector2 position, DWORD color) {
		DrawList->AddText(DefaultFont, size, *(ImVec2*)& position, color, text);
	}

	void Renderer::AddText(float size, Vector2 position, DWORD color, const char* format, ...) {
		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, 256, format, args);
		va_end(args);
		DrawList->AddText(DefaultFont, size, *(ImVec2*)& position, color, buffer);
	}

	void Renderer::AddText(const char* text, float size, Rect rectangle, DWORD flags, DWORD color) {
		auto textSize = DefaultFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text);
		auto position = ImVec2(rectangle.Position.X, rectangle.Position.Y);

		if (flags & DT_CENTER) {
			position.x = rectangle.Position.X + (rectangle.Width - textSize.x) * 0.5f;
		}
		else if (flags & DT_RIGHT) {
			position.x = rectangle.Position.X + rectangle.Width - textSize.x;
		}

		if (flags & DT_VCENTER) {
			position.y = rectangle.Position.Y + (rectangle.Height - textSize.y) * 0.5f;
		}
		else if (flags & DT_BOTTOM) {
			position.y = rectangle.Position.Y + rectangle.Height - textSize.y;
		}

		DrawList->AddText(DefaultFont, size, position, color, text);
	}

	void Renderer::AddText(float size, Rect rectangle, DWORD flags, DWORD color, const char* format, ...) {
		char buffer[256];
		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, 256, format, args);
		va_end(args);

		auto textSize = DefaultFont->CalcTextSizeA(size, FLT_MAX, 0.0f, buffer);
		auto position = ImVec2(rectangle.Position.X, rectangle.Position.Y);

		if (flags & DT_CENTER) {
			position.x = rectangle.Position.X + (rectangle.Width - textSize.x) * 0.5f;
		}
		else if (flags & DT_RIGHT) {
			position.x = rectangle.Position.X + rectangle.Width - textSize.x;
		}

		if (flags & DT_VCENTER) {
			position.y = rectangle.Position.Y + (rectangle.Height - textSize.y) * 0.5f;
		}
		else if (flags & DT_BOTTOM) {
			position.y = rectangle.Position.Y + rectangle.Height - textSize.y;
		}

		DrawList->AddText(DefaultFont, size, position, color, buffer);
	}

	void Renderer::AddTriangle(Vector2 point1, Vector2 point2, Vector2 point3, float thickness, DWORD color) {
		DrawList->AddTriangle(*(ImVec2*)& point1, *(ImVec2*)& point2, *(ImVec2*)& point3, color, thickness);
	}

	void Renderer::AddTriangleFilled(Vector2 point1, Vector2 point2, Vector2 point3, DWORD color) {
		DrawList->AddTriangleFilled(*(ImVec2*)& point1, *(ImVec2*)& point2, *(ImVec2*)& point3, color);
	}
}