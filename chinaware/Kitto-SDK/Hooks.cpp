/*
Copyright Kitto.win, All rights reserved.

Purpose:
	Handling on DLL_ATTACH and all hooks

*/

#include "Link.h"
#include "Polyhook/PolyHook/PolyHook.hpp"
#include "detours.h"

extern IMGUI_API LRESULT ImGui_ImplDX11_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks {

	PLH::VEHHook* VEH_Present;
	// nvToolsExt64_1
#define DEBUGGING 0

	typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	typedef void(__stdcall *tD3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	typedef void(__stdcall *tD3D11CreateQueryHook) (ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
	tD3D11Present oPresent;
	tD3D11DrawIndexedHook oIndex;
	tD3D11CreateQueryHook oCreateQuery;

	LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	bool file_exists(const char *fileName)
	{
		std::ifstream infile(fileName);
		return infile.good();
	}

	bool bFirstTime = true;

#define USE_IMGUI 1

	void HookFunction(PVOID *oFunction, PVOID pDetour) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(oFunction, pDetour);
		DetourTransactionCommit();
	}

	void UnhookFunction(PVOID* oFunction, PVOID pDetour) {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(oFunction, pDetour);
		DetourTransactionCommit();
	}
	//shader
	ID3D11PixelShader* psRed = NULL;
	ID3D11PixelShader* psGreen = NULL;

	int countnum = 1;
	//Vertex
	ID3D11Buffer *veBuffer;
	UINT Stride = 24;
	UINT veBufferOffset = 0;
	D3D11_BUFFER_DESC vedesc;
	D3D11_BUFFER_DESC pscdesc;

	//index
	ID3D11Buffer *inBuffer;
	DXGI_FORMAT inFormat;
	UINT        inOffset;
	D3D11_BUFFER_DESC indesc;
	ID3D11Buffer *pcsBuffer;

	UINT pscStartSlot;

	enum eDepthState
	{
		ENABLED,
		DISABLED,
		READ_NO_WRITE,
		NO_READ_NO_WRITE,
		_DEPTH_COUNT
	};

	//generate shader func
	HRESULT GenerateShader(ID3D11Device* pD3DDevice, ID3D11PixelShader** pShader, float r, float g, float b)
	{
		char szCast[] = "struct VS_OUT"
			"{"
			" float4 Position : SV_Position;"
			" float4 Color : COLOR0;"
			"};"

			"float4 main( VS_OUT input ) : SV_Target"
			"{"
			" float4 fake;"
			" fake.a = 1.0f;"
			" fake.r = %f;"
			" fake.g = %f;"
			" fake.b = %f;"
			" return fake;"
			"}";
		ID3D10Blob* pBlob;
		char szPixelShader[1000];

		sprintf_s(szPixelShader, szCast, r, g, b);

		ID3DBlob* d3dErrorMsgBlob;

		HRESULT hr = D3DCompile(szPixelShader, sizeof(szPixelShader), "shader", NULL, NULL, "main", "ps_4_0", NULL, NULL, &pBlob, &d3dErrorMsgBlob);

		if (FAILED(hr))
			return hr;

		hr = pD3DDevice->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pShader);

		if (FAILED(hr))
			return hr;

		return S_OK;
	}

	ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(eDepthState::_DEPTH_COUNT)];

	void SetDepthStencilState(eDepthState aState)
	{
		GManagement.DeviceContext->OMSetDepthStencilState(myDepthStencilStates[aState], 1);
	}

	void __stdcall KittoD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
	{
		//if (GetAsyncKeyState(VK_F9) & 1)
				//Log("DrawIndexed called");

			//get stride & vdesc.ByteWidth
		pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
		if (veBuffer)
			veBuffer->GetDesc(&vedesc);
		if (veBuffer != NULL) { veBuffer->Release(); veBuffer = NULL; }

		//get indesc.ByteWidth
		pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
		if (inBuffer)
			inBuffer->GetDesc(&indesc);
		if (inBuffer != NULL) { inBuffer->Release(); inBuffer = NULL; }

		//get pscdesc.ByteWidth
		pContext->PSGetConstantBuffers(pscStartSlot, 1, &pcsBuffer);
		if (pcsBuffer)
			pcsBuffer->GetDesc(&pscdesc);
		if (pcsBuffer != NULL) { pcsBuffer->Release(); pcsBuffer = NULL; }

		//wallhack/chams
		//if (sOptions[0].Function||sOptions[1].Function) //if wallhack/chams option is selected in menu
		if (Stride == 24 || Stride == countnum)
			//if (Stride == ? && indesc.ByteWidth ? && indesc.ByteWidth ? && Descr.Format .. ) //later here you do better model rec, values are different in every game
		{
			SetDepthStencilState(DISABLED);
			pContext->PSSetShader(psRed, NULL, NULL);
			oIndex(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
			pContext->PSSetShader(psGreen, NULL, NULL);
			SetDepthStencilState(READ_NO_WRITE);
		}

		if (GetAsyncKeyState(VK_OEM_4) & 1) //-
			countnum--;
		if (GetAsyncKeyState(VK_OEM_6) & 1) //+
			countnum++;

		return oIndex(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
	}

	void __stdcall KittoD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery)
	{
		//Disable Occlusion which prevents rendering player models through certain objects (used by wallhack to see models through walls at all distances, REDUCES FPS)
		if (pQueryDesc->Query == D3D11_QUERY_OCCLUSION)
		{
			D3D11_QUERY_DESC oqueryDesc = CD3D11_QUERY_DESC();
			(&oqueryDesc)->MiscFlags = pQueryDesc->MiscFlags;
			(&oqueryDesc)->Query = D3D11_QUERY_TIMESTAMP;

			return oCreateQuery(pDevice, &oqueryDesc, ppQuery);
		}

		return oCreateQuery(pDevice, pQueryDesc, ppQuery);
	}

	IMGUI_API LRESULT hkWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		ImGuiIO& io = ImGui::GetIO();
		switch (msg)
		{
		case WM_LBUTTONDOWN:
			io.MouseDown[0] = true;
			break;
		case WM_LBUTTONUP:
			io.MouseDown[0] = false;
			break;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = true;
			break;
		case WM_RBUTTONUP:
			io.MouseDown[1] = false;
			break;
		case WM_MBUTTONDOWN:
			io.MouseDown[2] = true;
			break;
		case WM_MBUTTONUP:
			io.MouseDown[2] = false;
			break;
		case WM_MOUSEWHEEL:
			io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
			break;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			break;
		case WM_KEYDOWN:
			if (wParam < 256)
				io.KeysDown[wParam] = 1;
			break;
		case WM_KEYUP:
			if (wParam < 256)
				io.KeysDown[wParam] = 0;
			break;
		case WM_CHAR:
			// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
			if (wParam > 0 && wParam < 0x10000)
				io.AddInputCharacter((unsigned short)wParam);
			break;
		}

		return CallWindowProc(GManagement.old_window_proc, hwnd, msg, wParam, lParam);
	}

	HRESULT KittoD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
	//	auto protecc = VEH_Present->GetProtectionObject();

		try {

			if (bFirstTime) {

				if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&GManagement.Device)))
				{
					pSwapChain->GetDevice(__uuidof(GManagement.Device), (void**)&GManagement.Device);
					GManagement.Device->GetImmediateContext(&GManagement.DeviceContext);
				}

				ID3D11Texture2D* renderTargetTexture = nullptr;
				if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
				{
					GManagement.Device->CreateRenderTargetView(renderTargetTexture, NULL, &GManagement.pRenderTargetView);
					renderTargetTexture->Release();
				}

#if USE_IMGUI
				ImGui::CreateContext();

				DrawScene::Initialise();


#define PUBG 1
#define RAINBOW 2
#define FORTNITE 3
#define GAME_RUST 4
#define OTHER 5

#define GAME FORTNITE

#if GAME == PUBG
				GManagement.Window = FindWindowA("UnrealWindow", NULL);
#endif
#if GAME == RAINBOW
				GManagement.m_Window = FindWindowA(NULL, "Rainbow Six");
#endif
#if GAME == GAME_RUST
				GManagement.m_Window = FindWindowA(NULL, "Rust");
#endif
#if GAME == OTHER
				GManagement.m_Window = GetForegroundWindow();
#endif
#if GAME == FORTNITE
				GManagement.m_Window = FindWindowA(NULL, "Fortnite  ");
#endif

				ImGui_ImplDX11_Init(GManagement.m_Window, GManagement.Device, GManagement.DeviceContext);

				ImGui_ImplDX11_CreateDeviceObjects();
#endif
	

				GManagement.old_window_proc = (WNDPROC)SetWindowLongPtr(GManagement.m_Window, GWLP_WNDPROC, (LONG_PTR)hkWndProcHandler);

				bFirstTime = false;
			}

			GManagement.DeviceContext->OMSetRenderTargets(1, &GManagement.pRenderTargetView, NULL);

#if GAME == GAME_RUST
			// rust::DoAimbot();
#endif

#if USE_IMGUI
			ImGui_ImplDX11_NewFrame();

			DrawScene::OnScene();

			DrawScene::DrawMenu();

			ImGui::Render();
#endif
		}
		catch (...) {

		}

		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}


	void HookPresent() {

		WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GManagement.m_GameModule, NULL, NULL, NULL, NULL, "DX", NULL };
		RegisterClassExA(&wc);
		HWND window = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

		D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
		D3D_FEATURE_LEVEL obtainedLevel;
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(scd));
		scd.BufferCount = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		scd.OutputWindow = window;
		scd.SampleDesc.Count = 1;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Windowed = ((GetWindowLongPtr(window, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

		// LibOVR 0.4.3 requires that the width and height for the backbuffer is set even if
		// you use windowed mode, despite being optional according to the D3D11 documentation.
		scd.BufferDesc.Width = 1;
		scd.BufferDesc.Height = 1;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 1;

		UINT createFlags = 0;
#ifdef _DEBUG
		// This flag gives you some quite wonderful debug text. Not wonderful for performance, though!
		createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createFlags,
			requestedLevels,
			sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL),
			D3D11_SDK_VERSION,
			&scd,
			&GManagement.SwapChain,
			&GManagement.Device,
			&obtainedLevel,
			&GManagement.DeviceContext);

		if (FAILED(hr))
		{
			MessageBox(window, "Failed", "Module", 0);
		}


		GManagement.SwapChainTable = (DWORD_PTR*)GManagement.SwapChain;
		GManagement.SwapChainTable = (DWORD_PTR*)GManagement.SwapChainTable[0];

		GManagement.ContextTable = (DWORD_PTR*)GManagement.DeviceContext;
		GManagement.ContextTable = (DWORD_PTR*)GManagement.ContextTable[0];

		GManagement.DeviceTable = (DWORD_PTR*)GManagement.Device;
		GManagement.DeviceTable = (DWORD_PTR*)GManagement.DeviceTable[0];

		oPresent = reinterpret_cast<tD3D11Present>(GManagement.SwapChainTable[8]); // present
		oCreateQuery = reinterpret_cast<tD3D11CreateQueryHook>(GManagement.DeviceTable[24]); // createquery
		oIndex = reinterpret_cast<tD3D11DrawIndexedHook>(GManagement.ContextTable[12]); // primitiveindex
#if 0
		// , PresentHook, reinterpret_cast<void**>(&phookD3D11Present)
		VEH_Present = new PLH::VEHHook();
		VEH_Present->SetupHook(*(BYTE**)(&oPresent), (BYTE*)&KittoD3D11Present, PLH::VEHHook::VEHMethod::INT3_BP);
		VEH_Present->Hook();
#else

		HookFunction(reinterpret_cast<PVOID*>(&oPresent), reinterpret_cast<void*>(&KittoD3D11Present));
		// HookFunction(reinterpret_cast<PVOID*>(&oCreateQuery), reinterpret_cast<void*>(&KittoD3D11CreateQuery));
		// HookFunction(reinterpret_cast<PVOID*>(&oIndex), reinterpret_cast<void*>(&KittoD3D11DrawIndexed));
#endif
	}

	bool getCodeInfo(DWORD_PTR baseAddr, DWORD_PTR * codeStart, DWORD_PTR * codeSize)
	{
		IMAGE_DOS_HEADER * dosHd = (IMAGE_DOS_HEADER*)baseAddr;
		if (dosHd && dosHd->e_magic == IMAGE_DOS_SIGNATURE)
		{
			IMAGE_NT_HEADERS * ntHd = (IMAGE_NT_HEADERS*)(baseAddr + dosHd->e_lfanew);
			if (ntHd && ntHd->Signature == IMAGE_NT_SIGNATURE)
			{
				*codeStart = ntHd->OptionalHeader.BaseOfCode + baseAddr;
				*codeSize = ntHd->OptionalHeader.SizeOfCode;

				return true;
			}
		}
		return false;
	}



	DWORD WINAPI Attach(LPVOID lpParam)
	{
		while (!GetModuleHandleA(("dxgi.dll")))
			Sleep(100);

		printf("dxgi.dll found\n");

		// getCodeInfo((DWORD_PTR)GetModuleHandleA(NULL), &GManagement.m_codeStart, &GManagement.m_codeSize);

#if DEBUGGING
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		SetConsoleTitle("Error Log");
#endif

		// Auth
#if 1
		GManagement.Window_name = AUTHENTICATION::GetServerVariable("dog");

		if (!strstr(GManagement.Window_name.c_str(), "Fortnite"))
			ExitProcess(0);
#endif

		GManagement.m_GameModule = GetModuleHandleA(BASE_ADDRESS);

		GManagement.m_BaseAddress = reinterpret_cast<uintptr_t>(GManagement.m_GameModule);

		CreateDirectory("C://Chinaware", NULL);
#if 0
		// create file initially.
		if (!file_exists("C://Chinaware//filter.txt")) {
			std::ofstream filter_file("C://Chinaware//filter.txt");
			if (filter_file.is_open())
				filter_file.close();
		}
#endif

		HookPresent();

		return 0;
	}

	enum Keys : int {
		Insert = 0
	};

	DWORD WINAPI LogKeys(LPVOID lpParam)
	{
		static bool Released[20] = { true };

		while (1) {

			if (GManagement.m_bEnableMenu) {

				POINT mousePosition;
				GetCursorPos(&mousePosition);
				ScreenToClient(GetForegroundWindow(), &mousePosition);

				ImGuiIO& io = ImGui::GetIO();
				io.MousePos.x = mousePosition.x;
				io.MousePos.y = mousePosition.y;

				if (GetAsyncKeyState(VK_LBUTTON))
					io.MouseDown[0] = true;
				else
					io.MouseDown[0] = false;
			}

			if (GetAsyncKeyState(VK_INSERT)) {

				if (Released[Keys::Insert]) {
					GManagement.m_bEnableMenu = !GManagement.m_bEnableMenu;
				}

				Released[Keys::Insert] = false;
			}
			else {
				Released[Keys::Insert] = true;
			}

			Sleep(1);
		}
	}

	void CreateThreads() {

		CreateThread(NULL, NULL, Hooks::Attach, NULL, NULL, NULL);

		CreateThread(NULL, NULL, Hooks::LogKeys, NULL, NULL, NULL);
	}
}