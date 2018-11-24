/*
Copyright Kitto.win, All rights reserved.

Purpose:
	Managing Global Vars

*/
#pragma once

#include <d3d11.h>
#include "imgui\imgui.h"

#ifndef MANAGEMENT
#define MANAGEMENT

#define BASE_ADDRESS 0

class CConfigs
{
public:
	bool DumpLocalName = false;
	bool DumpFullList = false;
	bool ShowLocalBase = false;
	bool DumpPlayerList = false;
	bool DumpBoneList = false;

	// Aimbot
	bool EnableAimbot = false;
	int AimbotKey;
	float FOV = 350.f;
	float aimbot_seeker_size = 200.f;
	float AimSpeed = 0.3f;
	int AimBone = 0;
	bool ReduceRecoil = false;
	float AimOffset = 110.f;
	ImVec4 seeker_color = ImVec4(255.f, 0.f, 0.f, 255.f);
	ImVec4 snap_color = ImVec4(255.f, 0.f, 0.f, 255.f);
	ImVec4 object_color = ImVec4(255.f, 255.f, 255.f, 255.f);
	ImVec4 ActorIdColor = ImVec4(255.f, 255.f, 255.f, 255.f);
	ImVec4 DistanceColor = ImVec4(255.f, 255.f, 255.f, 255.f);
	ImVec4 Box_Color = ImVec4(255.f, 0.f, 0.f, 255.f);

	// ESP
	bool EnableESP = false;
	bool IgnoreLocalPlayer = false;
	bool IgnoreSleepers = false;

	bool NameESP = false;
	bool BoundingBox = false;
	bool SkeletonESP = false;
	bool HealthESP = false;
	bool DistanceESP = false;
	bool ActorNames = false;
	bool SnapLines = false;
	bool ActorID = false;
	bool Crosshair = false;
	int CrosshairType = 0;
	int ActorFilter = 0;
	bool debugInfo = 0;
	bool should_load_actor = false;
	bool Aimbot_DrawFov = false;
	bool Aimbot_DrawSeeker = false;
	float ESPFOV = 90.f;
	bool debug_esp = false;

};
class CManagement
{
public:
	HMODULE m_GameModule;
	uintptr_t m_BaseAddress;

	uintptr_t m_codeStart;
	uintptr_t m_codeSize;

	WNDPROC old_window_proc = 0;

	DWORD_PTR* SwapChainTable = nullptr;
	DWORD_PTR* ContextTable = nullptr;
	DWORD_PTR* DeviceTable = nullptr;

	ID3D11Device* Device = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = NULL;

	bool m_bEnableMenu = false;
	ImFont* Verdana;
	ImFont* Icons;

	HWND m_Window;
	CConfigs m_Configs;
	std::string Window_name = "";
	D3DXVECTOR2 seeker_point = D3DXVECTOR2(0,0);
};

extern CManagement GManagement;
#endif

