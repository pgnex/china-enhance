/*
Copyright china.win, All rights reserved.

Purpose:
	Custom wrapper for imgui menu items
*/

#pragma once

#ifndef IMGUI_CUSTOM
#define IMGUI_CUSTOM

#include "../Link.h"

namespace imgui_custom
{
	void custom_key_button(int& key);

	inline ImVec4 ConvertFromRGBA(ImVec4 imVec);

	inline ImVec4 ConvertToRGBA(ImVec4 imVec);

	void custom_color_options(ImVec4& col, const char* name);

	void create_tab(int& tab, int set, int pos, const char* title);

	enum tab_pos
	{
		first,
		middle,
		last
	};

	void custom_color_inline(ImVec4& col, const char* id);
	void custom_color_inline(ImVec4& col, ImVec4& col2, bool should_draw_second, const char* id);
	void custom_inline_keyinput(int& key, int& id);

	void DrawLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness);
	void DrawRectangle(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness);
	float CreateOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center);
	float CreateText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center);
}

#endif