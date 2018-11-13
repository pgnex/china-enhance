#include "../Link.h"
#include "imgui_internal.h"

std::string KeyboardKeys[254] = { "_", "M1", "M2", "Break", "M3", "M4", "M5",
"_", "Backspace", "TAB", "_", "_", "_", "ENTER", "_", "_", "SHIFT", "CTRL", "ALT", "PAUSE",
"CAPS LOCK", "_", "_", "_", "_", "_", "_", "ESC", "_", "_", "_", "_", "SPACEBAR",
"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", "_", "Print", "_", "Print Screen", "Insert",
"Delete", "_", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "_", "_", "_", "_", "_", "_",
"_", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", "Left Windows", "Right Windows", "_", "_", "_", "NUM 0", "NUM 1", "NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6",
"NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "_", "_", "_", "_", "_",
"_", "_", "_", "NUM LOCK", "SCROLL LOCK", "_", "_", "_", "_", "_", "_", "_", "_", "_",
"_", "_", "_", "_", "_", "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", "_", "_", "_",
"_", "_", "_", "_", "_", "_", "_", "Next Track", "Previous Track", "Stop", "Play/Pause", "_", "_",
"_", "_", "_", "_", ";", "+", ",", "-", ".", "/?", "~", "_", "_", "_", "_",
"_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
"_", "_", "_", "_", "_", "_", "_", "[{", "\\|", "}]", "'", "_", "_", "_", "_",
"_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_",
"_", "_", "_", "_", "_", "_", "_", "_", "_", "_", "_" };


void imgui_custom::custom_inline_keyinput(int& key, int& id)
{
	++id;

	// Set same line.
	ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 80);

	// Get the style.
	auto& style = ImGui::GetStyle();

	// Set it to the same color as background
	style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_ChildWindowBg];
	style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_ChildWindowBg];
	style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_ChildWindowBg];

	static auto has_input = 0xDEDFED;

	std::string button_text = "[";
	// Display original text.
	if (key != has_input)
	{
		button_text.append(KeyboardKeys[key]);
	}
	else
	{
		button_text.append("-");
	}
	// Some values
	static bool CurrentKeys[256][99] = { false };
	static bool LastKeys[256][99] = { false };
	static bool bSelected[99] = { false };

	std::copy(&CurrentKeys[0][id], &CurrentKeys[254][id], &LastKeys[0][id]);

	for (int i = 0; i < 255; i++)
	{
		if (GetAsyncKeyState(i)) CurrentKeys[i][id] = true;
		else CurrentKeys[i][id] = false;
	}
	button_text.append("]");

	// Stupid imgui
	button_text.append("##");

	button_text.append(std::to_string(id));

	// Define unique key.
	if (ImGui::Button(button_text.c_str(), ImVec2(80, 20)))
	{
		bSelected[id] = true;
		key = has_input;
	}

	if (bSelected[id])
	{
		for (int i = 0; i < 255; i++) {

			if (CurrentKeys[i][id] && !LastKeys[i][id])
			{
				key = i;
				bSelected[id] = false;
				break;
			}
		}
	}

	// Restore original.

	style.Colors[ImGuiCol_Button] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style.Colors[ImGuiCol_ButtonHovered] = imgui_custom::ConvertFromRGBA(ImVec4(30.f, 30.f, 30.f, 255.f));
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.161f, 0.502f, 0.725f, 1.0f);
}


void imgui_custom::custom_key_button(int& key)
{
	static const constexpr unsigned int long long  has_input = 0x1337;
	std::string button_text;
	// Display original text.
	if (key != has_input)
	{
		button_text = KeyboardKeys[key];
	}
	else
	{
		button_text = "Selected";
	}
	// Some values
	static bool CurrentKeys[256];
	static bool LastKeys[256];
	static bool bSelected = false;

	std::copy(CurrentKeys, CurrentKeys + 255, LastKeys);

	for (int i = 0; i < 255; i++)
	{
		if (GetAsyncKeyState(i)) CurrentKeys[i] = true;
		else CurrentKeys[i] = false;
	}

	// Define unique key.
	if (ImGui::Button(button_text.c_str(), ImVec2(80, 20)))
	{
		bSelected = true;
		key = has_input;
	}

	if (bSelected)
	{
		for (int i = 0; i < 255; i++) {

			if (CurrentKeys[i] && !LastKeys[i])
			{
				key = i;
				bSelected = false;
				break;
			}
		}
	}
}

inline ImVec4 imgui_custom::ConvertFromRGBA(ImVec4 imVec)
{
	return ImVec4(imVec.x / 255.f, imVec.y / 255.f, imVec.z / 255.f, imVec.w / 255.f);
}

inline ImVec4 imgui_custom::ConvertToRGBA(ImVec4 imVec)
{
	return ImVec4(imVec.x * 255.f, imVec.y * 255.f, imVec.z * 255.f, imVec.w * 255.f);
}

void imgui_custom::custom_color_options(ImVec4& col, const char* name)
{
	ImGui::Text(name); ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 30);

	if (ImGui::ColorButton(name, ConvertFromRGBA(col)))
	{
		ImGui::OpenPopup(name);
	}
	if (ImGui::BeginPopup(name))
	{
		ImVec4 toRgb = ConvertFromRGBA(col);
		ImGui::ColorPicker4("", (float*)&toRgb, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
		col = ConvertToRGBA(toRgb);
		ImGui::EndPopup();
	}
}

// thicc overloading
void imgui_custom::custom_color_inline(ImVec4& col, const char* id)
{
	ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 45);

	if (ImGui::ColorButton(id, ConvertFromRGBA(col)))
	{
		ImGui::OpenPopup(id);
	}
	if (ImGui::BeginPopup(id))
	{
		ImVec4 toRgb = ConvertFromRGBA(col);
		ImGui::ColorPicker4("", (float*)&toRgb, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs);
		col = ConvertToRGBA(toRgb);
		ImGui::EndPopup();
	}
}

void imgui_custom::custom_color_inline(ImVec4& col, ImVec4& col2, bool should_draw_second, const char* id)
{
	if (!should_draw_second)
	{
		custom_color_inline(col, id);
	}
	else
	{
		ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 45);

		if (ImGui::ColorButton(id, ConvertFromRGBA(col)))
		{
			ImGui::OpenPopup(id);
		}
		if (ImGui::BeginPopup(id))
		{
			ImVec4 toRgb = ConvertFromRGBA(col);
			ImGui::ColorPicker4("", (float*)&toRgb, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
			col = ConvertToRGBA(toRgb);
			ImGui::EndPopup();
		}

		ImGui::SameLine();

		std::string ids = id + std::string("2");

		if (ImGui::ColorButton(ids.c_str(), ConvertFromRGBA(col2)))
		{
			ImGui::OpenPopup(ids.c_str());
		}
		if (ImGui::BeginPopup(ids.c_str()))
		{
			ImVec4 toRgb = ConvertFromRGBA(col2);
			ImGui::ColorPicker4("", (float*)&toRgb, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
			col2 = ConvertToRGBA(toRgb);
			ImGui::EndPopup();
		}

	}
}


void imgui_custom::create_tab(int& tab, int set, int pos, const char* title)
{

	if (tab == set)
	{
		ImGui::Separator();
	}
	else
	{
		ImGui::Spacing();
	}
	auto& style = ImGui::GetStyle();
#if 1
	if (tab == set)
	{
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.f);
	}
	else
	{
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 0.45f);
	}
#endif

	if (ImGui::Button(title, ImVec2(80, 80)))
	{
		tab = set;
	}

	if (tab == set)
	{
		ImGui::Separator();
	}
	else
	{
		ImGui::Spacing();
	}

}


void imgui_custom::DrawLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float r = (color >> 24) & 0xff;
	float g = (color >> 16) & 0xff;
	float b = (color >> 8) & 0xff;
	float a = (color) & 0xff;

	window->DrawList->AddLine(from, to, ImGui::GetColorU32(ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f)), thickness);
}

void imgui_custom::DrawRectangle(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float r = (color >> 24) & 0xff;
	float g = (color >> 16) & 0xff;
	float b = (color >> 8) & 0xff;
	float a = (color) & 0xff;

	window->DrawList->AddRect(from, to, ImGui::GetColorU32(ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f)), 0, 0, thickness);
}

float imgui_custom::CreateOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float r = (color >> 24) & 0xff;
	float g = (color >> 16) & 0xff;
	float b = (color >> 8) & 0xff;
	float a = (color) & 0xff;

	std::stringstream steam(text);
	std::string line;
	float y = 0.0f;
	int i = 0;

	while (std::getline(steam, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center)
		{
			window->DrawList->AddText(pFont, size + 0.2f, ImVec2(pos.x - textSize.x / 2.0f - 1, pos.y + textSize.y * i - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}
		else
		{
			window->DrawList->AddText(pFont, size + 0.2f, ImVec2(pos.x - 1, pos.y + textSize.y * i - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, a / 255)), line.c_str());
			window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

float imgui_custom::CreateText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();

	float r = (color >> 24) & 0xff;
	float g = (color >> 16) & 0xff;
	float b = (color >> 8) & 0xff;
	float a = (color) & 0xff;

	std::stringstream steam(text);
	std::string line;
	float y = 0.0f;
	int i = 0;

	while (std::getline(steam, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center)
		{
			window->DrawList->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}
		else
		{
			window->DrawList->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(r / 255, g / 255, b / 255, a / 255)), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}



