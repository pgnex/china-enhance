/*
Copyright Kitto.win, All rights reserved.

Purpose:
	The Main Entry point for the program

*/

#include "link.h"

#define SPOOF_GAMEOVERLAYRENDERER 0

#if SPOOF_GAMEOVERLAYRENDERER
char __declspec(dllexport) BOverlayNeedsPresent() {
	return 1;
}

char __declspec(dllexport) IsOverlayEnabled()
{
	return 1;
}

DWORD __declspec(dllexport) OverlayHookD3D3(DWORD *a1, int *a2)
{
	return 1;
}

int __declspec(dllexport) SetNotificationInset(int a1, int a2)
{
	return 1;
}

void __declspec(dllexport) SetNotificationPosition(unsigned int a1)
{
}

int  __declspec(dllexport) SteamOverlayIsUsingGamepad()
{
	return 1;
}

int __declspec(dllexport) SteamOverlayIsUsingMouse()
{
	return 1;
}

char __declspec(dllexport) ValveHookScreenshots(char a1)
{
	return 1;
}

void __declspec(dllexport) VirtualFreeWrapper(unsigned int a1)
{
}

float __declspec(dllexport) VulkanSteamOverlayGetScaleFactors(float *a1, float *a2)
{
	return 1;
}

int __declspec(dllexport) VulkanSteamOverlayPresent(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
	return 1;
}

int  __declspec(dllexport) VulkanSteamOverlayProcessCapturedFrame(char a1, int a2, int a3, int a4, int a5, int a6)
{
	return 1;
}
#endif

#define EXPORT 0
#if EXPORT
// Exporting function usable with SetWindowsHookEx
extern "C" __declspec(dllexport) int NextHook(int code, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(NULL, code, wParam, lParam);
}
#endif


BOOL APIENTRY DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {

	if (fdwReason == 1)	
		Hooks::CreateThreads();
	
	return TRUE;
}


