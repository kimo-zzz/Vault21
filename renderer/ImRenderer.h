#pragma once

#ifndef RENDER_H
#define RENDER_H

#include "imgui.h"

#include "imgui_impl_win32.h"

#include "d3d9.h"
#include "imgui_impl_dx9.h"

#include "d3d11.h"
#include "imgui_impl_dx11.h"

class image {
	
public:
	std::string filename;
	Vector loc;
	LPDIRECT3DTEXTURE9 texture9;
	ID3D11ShaderResourceView* texture11;
	int width = 50;
	int height = 50;
};

class c_renderer
{
public:

	void init9(LPDIRECT3DDEVICE9 device);
	void free9(void);
	void begin_draw9(void);
	void end_draw9(void);
	bool draw_image9(std::string filename, int in_width, int in_height, Vector loc);

	void init11(ID3D11Device* device, ID3D11DeviceContext* context);
	void free11(void);
	void begin_draw11(void);
	void end_draw11(void);
	bool draw_image11(std::string _filename, int in_width, int in_height, Vector loc);

	ImFont* m_default = nullptr;
	ImFont* m_esp_name = nullptr;
	ImFont* m_weapon_icons = nullptr;

public:
	enum e_flags
	{
		// circle
		circle_normal,
		circle_filled,
		circle_3d,

		// rectangle
		rect_normal,
		rect_filled,

		// triangle
		triangle_normal,
		triangle_filled,

		// text
		text_normal,
		text_with_font
	};

public:
	void draw_text(float_t x, float_t y, const char* text, bool outlined,
		ImColor color = ImColor(255, 255, 255, 255), e_flags flags = text_normal, ImFont* font = nullptr, ...);

	void draw_line(float_t x1, float_t y1, float_t x2, float_t y2,
		ImColor color = ImColor(255, 255, 255, 255), float_t thickness = 1.0f);

	void draw_rect(float_t x, float_t y, float_t w, float_t h,
		ImColor color = ImColor(255, 255, 255, 255), e_flags flags = rect_normal, float_t rounding = 0.0f, uintptr_t points = 12, float_t thickness = 1.0f);

	void draw_triangle(float_t x1, float_t y1, float_t x2, float_t y2,
		float_t x3, float_t y3, ImColor color = ImColor(255, 255, 255, 255), e_flags flags = triangle_normal, float_t thickness = 1.0f);

	void draw_circle(Vector Position, float_t radius,
		ImColor color = ImColor(255, 255, 255, 255), e_flags flags = circle_normal, uintptr_t points = 12, float_t thickness = 1.0f);

	bool draw_image(std::string _filename, int in_width, int in_height, Vector loc);

public:
	float get_width(void) { return m_width; }
	float get_height(void) { return m_height; }

	LPDIRECT3DDEVICE9 get_device9(void) { return m_device9; }
	ID3D11Device* get_device11(void) { return m_device11; }

private:
	LPDIRECT3DDEVICE9 m_device9 = nullptr;

	ID3D11Device* m_device11 = nullptr;
	ID3D11DeviceContext* m_context11 = nullptr;
public:
	HWND m_hwnd = nullptr;

private:
	float m_width = 0.0f;
	float m_height = 0.0f;
};

extern c_renderer render;

#endif // RENDER_H