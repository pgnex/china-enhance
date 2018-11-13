/*
Copyright china.win, All rights reserved.

Purpose:
	This header is for linking all libraries and headers

*/

#pragma once

#ifndef LINK
#define LINK

#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <assert.h>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <intrin.h>
#include <emmintrin.h>
#include <chrono>
#include <wtsapi32.h>
#pragma comment(lib, "Wtsapi32.lib")

#ifdef D3D11Wrapper
#include <Gdiplus.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#endif

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/D3DX10math.h"
#pragma comment(lib, "D3DX10.lib")

#define CURL_STATICLIB
#include <curl/curl.h>
#pragma comment (lib,"libcurl_a.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#pragma comment (lib,"Gdiplus.lib")
#pragma comment (lib, "Crypt32.Lib")
#pragma comment (lib, "Normaliz.lib")


#include "Includes.h"

#endif