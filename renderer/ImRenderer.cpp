#include "stdafx.h"
#include "ImRenderer.h"
#include <d3dx9tex.h>
#pragma comment (lib, "D3dx9.lib")
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

c_renderer render;
static std::list<image> image_list;

void c_renderer::init9(LPDIRECT3DDEVICE9 device)
{
	if (!device)
		return;

	if (!m_device9)
		m_device9 = device;

	ImGui::CreateContext();

	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX9_Init(device);

	if (!m_default)
		m_default = ImGui::GetIO().Fonts->AddFontDefault();
	/* ImGui::GetIO().Fonts->AddFontFromFileTTF("path_to_font", 14.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()); */



}

void c_renderer::free9(void)
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

void c_renderer::begin_draw9(void)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing;

	ImGui::GetStyle().AntiAliasedFill = true;
	ImGui::GetStyle().AntiAliasedLines = true;

	ImGui::Begin("##overlay", nullptr, flags);

	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

	m_width = ImGui::GetIO().DisplaySize.x;
	m_height = ImGui::GetIO().DisplaySize.y;
}

void c_renderer::end_draw9(void)
{
	ImGui::GetOverlayDrawList()->PushClipRectFullScreen();

	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	//for (image i : image_list)
	//	((LPDIRECT3DTEXTURE9)i.texture)->Release();

	//image_list.clear();
}

LPDIRECT3DTEXTURE9 LoadTextureFromFile9(const char* filename, LPDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height, LPDIRECT3DDEVICE9 xD)
{
	// Load texture from disk
	LPDIRECT3DTEXTURE9 pTexture;
	HRESULT hr = D3DXCreateTextureFromFileA(xD, filename, &pTexture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	pTexture->GetLevelDesc(0, &my_image_desc);
	*out_texture = pTexture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return pTexture;
}

bool c_renderer::draw_image9(std::string _filename, int in_width, int in_height, Vector loc) {

	bool has_saved_img_Ptr = false;
	image img_textre;
	for (image a : image_list) {
		if ((a.filename.compare(_filename) == 0) && (a.height == in_height) && (a.width == in_width)) {
			has_saved_img_Ptr = true;
			img_textre = a;
			break;
		}
	}

	if (!has_saved_img_Ptr) {
		img_textre.filename = _filename;
		img_textre.height = in_height;
		img_textre.width = in_width;
		img_textre.loc = loc;
		LoadTextureFromFile9(_filename.c_str(), &img_textre.texture9, &in_width, &in_height, m_device9);
		image_list.push_back(img_textre);
	}
	//std::string wndName = "##texture" + std::to_string(index);
	//const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
	//ImGui::GetStyle().AntiAliasedFill = true;
	//ImGui::GetStyle().AntiAliasedLines = true;
	//ImGui::Begin(wndName.c_str(), nullptr);
	//ImGui::SetWindowSize(ImVec2(in_width, in_height), ImGuiCond_Always);
	//ImGui::SetWindowPos(ImVec2(loc.X, loc.Y), ImGuiCond_Always);
	
	//ImGui::Text("image_list size = %d", image_list.size());
	//ImGui::Text("pointer = %p", img_textre.texture);
	//ImGui::Text("file name = %s", img_textre.filename);
	ImGui::SetCursorScreenPos(ImVec2(loc.X, loc.Y));
	return ImGui::ImageButton((void*)img_textre.texture9, ImVec2(in_width, in_height));
	//ImGui::Text("size = %d x %d", in_width, in_height);
	//ImGui::End();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Simple helper function to load an image into a DX11 texture with common settings
bool LoadTextureFromFile11(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height, ID3D11Device* pDevice)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

bool c_renderer::draw_image11(std::string _filename, int in_width, int in_height, Vector loc) {

	bool has_saved_img_Ptr = false;
	image img_textre;

	for (image a : image_list) {
		if ((a.filename.compare(_filename) == 0) && (a.height == in_height) && (a.width == in_width)) {
			has_saved_img_Ptr = true;
			img_textre = a;
			break;
		}
	}

	if (!has_saved_img_Ptr) {
		img_textre.filename = _filename;
		img_textre.height = in_height;
		img_textre.width = in_width;
		img_textre.loc = loc;
		LoadTextureFromFile11(_filename.c_str(), &img_textre.texture11, &in_width, &in_height, m_device11);
		image_list.push_back(img_textre);
	}

	//std::string wndName = "##texture" + std::to_string(index);
	//const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
	//ImGui::GetStyle().AntiAliasedFill = true;
	//ImGui::GetStyle().AntiAliasedLines = true;
	//ImGui::Begin(wndName.c_str(), nullptr);
	//ImGui::SetWindowSize(ImVec2(in_width, in_height), ImGuiCond_Always);
	//ImGui::SetWindowPos(ImVec2(loc.X, loc.Y), ImGuiCond_Always);

	//ImGui::Text("image_list size = %d", image_list.size());
	//ImGui::Text("pointer = %p", img_textre.texture);
	//ImGui::Text("file name = %s", img_textre.filename);
	ImGui::SetCursorScreenPos(ImVec2(loc.X, loc.Y));
	//ImGui::Image((void*)img_textre.texture11, ImVec2(in_width, in_height));

	return ImGui::ImageButton((void*)img_textre.texture11, ImVec2(in_width, in_height));
	//ImGui::Text("size = %d x %d", in_width, in_height);
	//ImGui::End();
}

void c_renderer::init11(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (!device)
		return;

	if (!m_device11)
		m_device11 = device;

	ImGui::CreateContext();

	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(device, context);

	if (!m_default)
		m_default = ImGui::GetIO().Fonts->AddFontDefault();
	/* ImGui::GetIO().Fonts->AddFontFromFileTTF("path_to_font", 14.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()); */



}

void c_renderer::free11(void)
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

void c_renderer::begin_draw11(void)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	//const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs;
	auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing;

	ImGui::GetStyle().AntiAliasedFill = true;
	ImGui::GetStyle().AntiAliasedLines = true;

	ImGui::Begin("##overlay", nullptr, flags);

	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

	m_width = ImGui::GetIO().DisplaySize.x;
	m_height = ImGui::GetIO().DisplaySize.y;
}

void c_renderer::end_draw11(void)
{
	ImGui::GetOverlayDrawList()->PushClipRectFullScreen();

	ImGui::End();

	ImGui::EndFrame();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//for (image i : image_list)
	//	((LPDIRECT3DTEXTURE9)i.texture)->Release();

	//image_list.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool c_renderer::draw_image(std::string _filename, int in_width, int in_height, Vector loc) {
	if (m_device11)
		return draw_image11(_filename, in_width, in_height, loc);
	if (m_device9)
		return draw_image9(_filename, in_width, in_height, loc);
	return false;
}

void c_renderer::draw_text(float_t x, float_t y, const char* text, bool outlined, ImColor color, e_flags flags, ImFont* font, ...)
{
	switch (flags)
	{
	case c_renderer::text_normal:

		if (outlined)
		{
			ImGui::GetWindowDrawList()->AddText(ImVec2(x, y + 1.0f), ImColor(0, 0, 0, 255), text);
			ImGui::GetWindowDrawList()->AddText(ImVec2(x + 1.0f, y), ImColor(0, 0, 0, 255), text);
		}

		ImGui::GetWindowDrawList()->AddText(ImVec2(x, y), color, text);
		break;
	case c_renderer::text_with_font:

		if (outlined)
		{
			ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x, y + 1.0f), ImColor(0, 0, 0, 255), text);
			ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x + 1.0f, y), ImColor(0, 0, 0, 255), text);
		}

		ImGui::GetWindowDrawList()->AddText(font, font->FontSize, ImVec2(x, y), color, text);
		break;
	default:
		break;
	}
}

void c_renderer::draw_line(float_t x1, float_t y1, float_t x2, float_t y2, ImColor color, float_t thickness)
{
	ImGui::GetWindowDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

void c_renderer::draw_rect(float_t x, float_t y, float_t w, float_t h, ImColor color, e_flags flags, float_t rounding, uintptr_t points, float_t thickness)
{
	switch (flags)
	{
	case c_renderer::rect_normal:
		ImGui::GetWindowDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding, points, thickness);
		break;
	case c_renderer::rect_filled:
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding, points);
		break;
	default:
		break;
	}
}

void c_renderer::draw_triangle(float_t x1, float_t y1, float_t x2, float_t y2, float_t x3, float_t y3, ImColor color, e_flags flags, float_t thickness)
{
	switch (flags)
	{
	case c_renderer::rect_normal:
		ImGui::GetWindowDrawList()->AddTriangle(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color, thickness);
		break;
	case c_renderer::rect_filled:
		ImGui::GetWindowDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color);
		break;
	default:
		break;
	}
}

void c_renderer::draw_circle(float_t x, float_t y, float_t radius, ImColor color, e_flags flags, uintptr_t points, float_t thickness)
{
	switch (flags)
	{
	case circle_normal:
		ImGui::GetWindowDrawList()->AddCircle(ImVec2(x, y), radius, color, points, thickness);
		break;
	case circle_filled:
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(x, y), radius, color, points);
		break;
	case circle_3d:
		break;
	default:
		break;
	}
}