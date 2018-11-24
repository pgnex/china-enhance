/*
Copyright Kitto.win, All rights reserved.

Purpose:
	Fortnite Cheat

*/

#include "Link.h"

namespace Fortnite
{
#define M_PI	3.14159265358979323846264338327950288419716939937510

	// maps unsigned 8 bits/channel to D3DCOLOR
#define D3DCOLOR_RGBA(r,g,b,a) \
    ((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))

	// 
	// Kite 221777
	// 213656
	// 213661
	// 213642 NEX
	// 213655

	bool DataCompare(const BYTE* OpCodes, const BYTE* Mask, const char* StrMask)
	{
		while (*StrMask)
		{
			if (*StrMask == 'x' && *OpCodes != *Mask)
				return false;

			++StrMask;
			++OpCodes;
			++Mask;
		}

		return true; 
	}

	uintptr_t FindPattern(uintptr_t StartAddress, size_t CodeLen, BYTE* Mask, const  char* StrMask, unsigned short ignore)
	{
		unsigned short Ign = 0;
		uintptr_t i = 0;

		while (Ign <= ignore)
		{
			if (DataCompare((BYTE*)(StartAddress + i++), Mask, StrMask))
				++Ign;

			else if (i >= CodeLen)
				return 0;
		}

		return StartAddress + i - 1;
	}

	uintptr_t FindPatternRel(uintptr_t StartAddress, size_t CodeLen, BYTE * Mask, const char * StrMash, unsigned short ignore)
	{
		const auto address = FindPattern(StartAddress, CodeLen, Mask, StrMash, 0);
		if (address == 0)
		{
			return 0;
		}

		const auto offset = *reinterpret_cast<unsigned int*>(address + ignore);

		return address + ignore + 4 + offset;
	}

	/*
	_QWORD *__fastcall getobjname(_QWORD *a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5, __int64 a6)
	{
	  _QWORD *v6; // rbx

	  v6 = a1;
	  if ( a2 )
	  {
		a6 = *(_QWORD *)(a2 + 24);
		sub_7FF70BB37CF0(&a6, a1);
	  }
	  else
	  {
		*a1 = 0i64;
		a1[1] = 5i64;
		sub_7FF70A62A9F0();
		sub_7FF70BA3F050(*v6, L"None", 10i64);
	  }
	  return v6;
	}
	*/

	uintptr_t* GetActorName(Actor* Player) {
		return reinterpret_cast<uintptr_t*(__fastcall*)(Actor*, __int64, __int64, __int64, __int64, __int64)>((uintptr_t)GetModuleHandleA(0) + 0x26D3C90)(Player, 0, 0, 0, 0, 0);
	}

	D3DXMATRIX Matrix(D3DXVECTOR3 rot, D3DXVECTOR3 origin = D3DXVECTOR3(0, 0, 0))
	{
		float radPitch = (rot.x * float(M_PI) / 180.f);
		float radYaw = (rot.y * float(M_PI) / 180.f);
		float radRoll = (rot.z * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

	inline float Dot(const D3DXVECTOR3& vector1, D3DXVECTOR3& vector2)
	{
		return vector2.x * vector1.x + vector2.y * vector1.y + vector2.z * vector1.z;
	}

	D3DXVECTOR3 WorldToScreen(D3DXVECTOR3 WorldLocation, D3DXVECTOR3 Rotation, D3DXVECTOR3 CamLocation, float fov)
	{
		D3DXVECTOR3 Screenlocation = D3DXVECTOR3(0, 0, 0);

		D3DMATRIX tempMatrix = Matrix(Rotation); // Matrix

		D3DXVECTOR3 vAxisX, vAxisY, vAxisZ;

		vAxisX = D3DXVECTOR3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = D3DXVECTOR3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = D3DXVECTOR3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		D3DXVECTOR3 vDelta = WorldLocation - CamLocation;
		D3DXVECTOR3 vTransformed = D3DXVECTOR3(Dot(vAxisY, vDelta), Dot(vDelta, vAxisZ), Dot(vDelta, vAxisX));

		if (vTransformed.z < 1.f)
			vTransformed.z = 1.f;

		float FovAngle = fov;

		float ScreenCenterX = ImGui::GetIO().DisplaySize.x / 2.0f;
		float ScreenCenterY = ImGui::GetIO().DisplaySize.y / 2.0f;

		Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

		return Screenlocation;
	}

	//D3DXVECTOR3 get_rotation(CameraManager* Cam)
	//{
	//	using getRotationFn = D3DXVECTOR3(__thiscall*)(CameraManager*);
	//	static getRotationFn getRotation;

	//	if (!getRotation)
	//	getRotation = (getRotationFn)((uintptr_t)GetModuleHandleA(0) + 0x13B3AE0);

	//	return getRotation(Cam);
	//}
	// 13B3AE0
	/*
	__int64 APlayerCameraManager::GetCameraRotation()
	{
	  __int64 result; // rax

	  result = qword_7FF75B0C09B0;
	  if ( !qword_7FF75B0C09B0 )
	  {
		sub_7FF7575CBCE0(&qword_7FF75B0C09B0, &unk_7FF75AB9C880);
		result = qword_7FF75B0C09B0;
	  }
	  return result;
	}

	FRotator APlayerCameraManager::GetCameraRotation() const
	{
		return GetCameraCachePOV().Rotation;
	}
	*/
	void DrawFeet()
	{
		auto LocalPlayer = Fortnite::GetUWorld();
		if (LocalPlayer) {
			auto Instance = LocalPlayer->OwningInstance;
			if (Instance) {
				auto Local = Instance->localPlayers[0];
				if (Local) {
					auto Position = Local->LocalPlayerPosition;
					auto Controller = Local->PlayerController;

					if (Controller) {
						auto Camera = Controller->CameraManager;
						if (Camera) {
							auto CamPos = Camera->CameraPosition;
							auto CamRot = Camera->Rotation;
							auto ScreenPos = WorldToScreen(Position, CamRot, CamPos, Camera->FOV);
							ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y), "Local Player");
						}
					}
				}
			}
		}
	}


	CUWorld* GetUWorld()
	{
#if 1
		static bool bHasInit = false;

		static CUWorld** ppUWorld = nullptr;

		if (!bHasInit) {
			ppUWorld = (CUWorld**)FindPatternRel(GManagement.m_codeStart, GManagement.m_codeSize, (BYTE*)"\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\x8B\x88\x00\x00\x00\x00\x48\x85\xC9", "xxx????xxxxx????xxx????xxx", 3);
			bHasInit = true;
		}
		if (ppUWorld) {
			return *ppUWorld;
		}
		return nullptr;
#else 
		return *reinterpret_cast<CUWorld**>((uintptr_t)GetModuleHandleA(0) + UWorldOffset);
#endif
	}

	void PrintLocal()
	{
		auto UWorld = GetUWorld();
		auto Local = UWorld->OwningInstance->localPlayers[0];
		std::cout << "Local: " << Local << std::endl;
	}

	void PrintUWorldPointer()
	{
		std::cout << "Uworld: " << GetUWorld() << std::endl;
	}

	void DumpPlayerNames()
	{
		auto UWorld = GetUWorld();
		if (UWorld) {
			auto Actors = UWorld->ULevel->Actors;
			for (int i = 0; i < Actors.Num(); i++) {
				auto Actor = Actors[i];
				if (Actor) {
					auto actorIdS = std::to_string(Actor->ActorID);

					if (actorIdS.at(0) == '2')
					std::cout << GetActorName(Actor) << std::endl;
				}
			}

		}
	}

	bool is_pointWIthinFov(D3DXVECTOR2 Point)
	{
		// Calculate middle of screen
		float ScreenCenterX = ImGui::GetIO().DisplaySize.x / 2;
		float ScreenCenterY = ImGui::GetIO().DisplaySize.y / 2;

		// Create a square from top left ot bottom left.
		auto fov_offset = (GManagement.m_Configs.FOV / 2);

		// Co-ordinates
		auto topleft_x = ScreenCenterX - fov_offset;
		auto topleft_y = ScreenCenterY - fov_offset;

		auto botright_x = ScreenCenterX + fov_offset;
		auto botright_y = ScreenCenterY + fov_offset;

		// Construct an imaginary square and check points
		if (Point.x <= topleft_x)
			return false;

		if (Point.x >= botright_x)
			return false;

		if (Point.y <= topleft_y)
			return false;

		if (Point.y >= botright_y)
			return false;

		return true;
	}



	void Draw_Fov()
	{
		if (!GManagement.m_Configs.Aimbot_DrawFov)
			return;

		// Calculate middle of screen
		float ScreenCenterX = ImGui::GetIO().DisplaySize.x / 2;
		float ScreenCenterY = ImGui::GetIO().DisplaySize.y / 2;

		// Create a square from top left ot bottom left.
		auto fov_offset = (GManagement.m_Configs.FOV / 2);

		// Co-ordinates
		auto topleft_x = ScreenCenterX - fov_offset;
		auto topleft_y = ScreenCenterY - fov_offset;

		auto botright_x = ScreenCenterX + fov_offset;
		auto botright_y = ScreenCenterY + fov_offset;

		imgui_custom::DrawRectangle(ImVec2(topleft_x, topleft_y), ImVec2(botright_x, botright_y), 0xFFFFFFFF, 1.f);

		if (is_pointWIthinFov(GManagement.seeker_point) && GManagement.m_Configs.Aimbot_DrawSeeker)
		{
			auto x_seeker_offset = (ImGui::GetIO().DisplaySize.x - GManagement.m_Configs.FOV) / 2.f;
			auto y_seeker_offset = (ImGui::GetIO().DisplaySize.y - GManagement.m_Configs.FOV) / 2.f;

			auto hex_col = D3DCOLOR_RGBA((int)(GManagement.m_Configs.seeker_color.x), ((int)GManagement.m_Configs.seeker_color.y), ((int)GManagement.m_Configs.seeker_color.z), ((int)GManagement.m_Configs.seeker_color.w));

			// Draw from left to right.
			imgui_custom::DrawLine(ImVec2(x_seeker_offset, GManagement.seeker_point.y), ImVec2(ImGui::GetIO().DisplaySize.x - x_seeker_offset, GManagement.seeker_point.y), hex_col, 1.f);
			// Top to bottom
			imgui_custom::DrawLine(ImVec2(GManagement.seeker_point.x, y_seeker_offset), ImVec2(GManagement.seeker_point.x, ImGui::GetIO().DisplaySize.y - y_seeker_offset), hex_col, 1.f);

		}
	}

	void DumpPlayerAddresses()
	{
		auto UWorld = GetUWorld();
		if (UWorld) {
			auto Actors = UWorld->ULevel->Actors;
			for (int i = 0; i < Actors.Num(); i++) {
				auto Actor = Actors[i];
				if (Actor) {

					auto actorIdS = std::to_string(Actor->ActorID);

					if (actorIdS.at(0) == '2')				
					std::cout << "Actor Id: " << actorIdS << " at: " << Actor << std::endl;
				}
			}

		}
	}

	float Length2D(D3DXVECTOR3 vector)
	{
		float sqst = vector.x * vector.x + vector.y * vector.y;

		return std::sqrt(sqst);
	}

	float Vector2dLength2D(D3DXVECTOR2 vector)
	{
		float sqst = vector.x * vector.x + vector.y * vector.y;

		return std::sqrt(sqst);
	}
	void Run_Hake()
	{
		auto UWorld = Fortnite::GetUWorld();

		DWORD hex_boxcol = D3DCOLOR_RGBA((int)(GManagement.m_Configs.Box_Color.x), ((int)GManagement.m_Configs.Box_Color.y), ((int)GManagement.m_Configs.Box_Color.z), 255);
		DWORD hex_Dstcol = D3DCOLOR_RGBA((int)(GManagement.m_Configs.DistanceColor.x), ((int)GManagement.m_Configs.DistanceColor.y), ((int)GManagement.m_Configs.DistanceColor.z),255);
		DWORD hex_Snapcol = D3DCOLOR_RGBA((int)(GManagement.m_Configs.snap_color.x), ((int)GManagement.m_Configs.snap_color.y), ((int)GManagement.m_Configs.snap_color.z), 255);

		if (UWorld) {

			auto Instance = UWorld->OwningInstance;
			if (Instance) {
				auto Local = Instance->localPlayers[0];
				if (Local) {
					auto Controller = Local->PlayerController;

					if (Controller) {
						auto Camera = Controller->CameraManager;
						if (Camera) {
							auto CamPos = Camera->CameraPosition;
							auto CamRot = Camera->Rotation;

							auto Level = UWorld->ULevel;

							if (Level) {

								auto Actors = UWorld->ULevel->Actors;

								// float best_fov = (GManagement.m_Configs.FOV * 3);
								float best_fov = 99999999.f;
								D3DXVECTOR2 best_aim;
								float bestdst = 99999999.f;

								float lowestDst = 999099999.f;

								int32_t best_actorid = 0x0; //0x0018
								Root2* best_root = nullptr;

								for (int i = 0; i < Actors.Num(); i++) {

									auto Actor = Actors[i];

									if (Actor) {

										// Run esp code.
										auto Root = Actor->Root2;

										if (Root) {

											auto pos = Root->Position2;

											auto deltapos = pos - Local->LocalPlayerPosition;
											
											auto deltadistance2d = Length2D(deltapos);

											auto string_actorid = std::to_string(Actor->ActorID);

											char inital_char = string_actorid.at(0);

											if (deltadistance2d < lowestDst && inital_char == '2')
											{
												best_actorid = Actor->ActorID;

												best_root = Root;

												lowestDst = deltadistance2d;
											}
										}
									}
								}

								if (GManagement.m_Configs.debugInfo) {

									ImGui::RenderText(ImVec2(7, 80), std::to_string(best_actorid).c_str());
									auto padding = 95;
									ImGui::RenderText(ImVec2(7, padding), ("1: " + std::to_string(best_root->N00000B18)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("2: " + std::to_string(best_root->N00000AF8)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("3: " + std::to_string(best_root->N00000B1B)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("4: " + std::to_string(best_root->N00000AF9)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("5: " + std::to_string(best_root->N00000B1E)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("6: " + std::to_string(best_root->N00000AFA)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("7: " + std::to_string(best_root->N00000B21)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("8: " + std::to_string(best_root->N00000AFB)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("9: " + std::to_string(best_root->N00000B24)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("10: " + std::to_string(best_root->N00000B28)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("11: " + std::to_string(best_root->N00000AFC)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("12: " + std::to_string(best_root->N00000B2B)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("13: " + std::to_string(best_root->N00000AFD)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("14: " + std::to_string(best_root->N00000B2E)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("15: " + std::to_string(best_root->N00000AFE)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("16: " + std::to_string(best_root->N00000AFF)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("17: " + std::to_string(best_root->N00000B33)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("18: " + std::to_string(best_root->N00000B00)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("19: " + std::to_string(best_root->N00000B36)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("20: " + std::to_string(best_root->N00000B01)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("21: " + std::to_string(best_root->N00000B39)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("22: " + std::to_string(best_root->N00000B02)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("23: " + std::to_string(best_root->N00000B3C)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("24: " + std::to_string(best_root->N00000B3E)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("25: " + std::to_string(best_root->N00000B6E)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("26: " + std::to_string(best_root->N00000B3F)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("27: " + std::to_string(best_root->N00000B71)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("28: " + std::to_string(best_root->N00000B05)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("29: " + std::to_string(best_root->N00000B74)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("30: " + std::to_string(best_root->N00000B06)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("31: " + std::to_string(best_root->N00000B77)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("32: " + std::to_string(best_root->N00000B07)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("33: " + std::to_string(best_root->N00000B08)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("34: " + std::to_string(best_root->N00000B0B)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("35: " + std::to_string(best_root->N00000B0C)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("36: " + std::to_string(best_root->N00000B0C)).c_str());
									padding += 15;

									ImGui::RenderText(ImVec2(7, padding), ("37: " + std::to_string(best_root->N00000B64)).c_str());
									padding += 15;

								}

								for (int i = 0; i < Actors.Num(); i++) {

									auto Actor = Actors[i];

									if (Actor) {

										// Run esp code.
										auto Root = Actor->Root2;

										if (Root) {

											auto pos = Root->Position2;

											pos.z -= 150.f;

											auto ScreenPos = WorldToScreen(pos, CamRot, CamPos, Camera->FOV);

											auto deltapos = pos - Local->LocalPlayerPosition;
											auto deltadistance2d = Length2D(deltapos);

											//auto ScreenPos = WorldToScreen(pos, CamRot, CamPos);
											//auto LocalDetector = WorldToScreen(Local->LocalPlayerPosition, CamRot, CamPos);
											//auto DetectPos = WorldToScreen(Root->Position, CamRot, CamPos);
											//auto delta = pos - Local->LocalPlayerPosition;
											//auto dst = Length2D(delta);
											//if ((DetectPos.x > (LocalDetector.x + 20)) || (DetectPos.x < (LocalDetector.x - 20)) || (DetectPos.y > (LocalDetector.y + 25)) || (DetectPos.y < (LocalDetector.y - 25)))

											if (deltadistance2d / 100.f > 1.f)
											{

												if (Actor->ActorID == best_actorid) {// GManagement.m_Configs.ActorFilter) {

													if (GManagement.m_Configs.ActorNames) {
														auto Name = GetActorName(Actor);
														std::cout << "Value: " << Name << std::endl;
														GManagement.m_Configs.ActorNames = false;
													}

													auto head = pos;
													head.z += 300.f;
													auto ScreenHead = WorldToScreen(head, CamRot, CamPos, Camera->FOV);

													auto aimbone_scaled = head;
													aimbone_scaled.z += GManagement.m_Configs.AimOffset;
													auto Aimw2sScaled = WorldToScreen(aimbone_scaled, CamRot, CamPos, Camera->FOV);
													auto aimpos = D3DXVECTOR2(ScreenHead.x, (Aimw2sScaled.y + ScreenPos.y) / 2.f);

													float ScreenCenterX = ImGui::GetIO().DisplaySize.x / 2;
													float ScreenCenterY = ImGui::GetIO().DisplaySize.y / 2;

													auto dst = (aimpos - D3DXVECTOR2(ScreenCenterX, ScreenCenterY));
													auto delta = pos - Local->LocalPlayerPosition;
													auto distance = Length2D(delta);

													float CurFov = Vector2dLength2D(dst);
													// && distance < bestdst // Distance

													//  CurFov < (GManagement.m_Configs.FOV * 3) // FOV CHeck

													// Check if the point is within the visor.
													if (CurFov < best_fov && is_pointWIthinFov(aimpos)) {
														GManagement.seeker_point = aimpos;
														best_aim = aimpos;
														bestdst = distance;
														best_fov = CurFov;
													}

													int padding = 0;
													if (GManagement.m_Configs.DistanceESP) {
														auto delta = pos - Local->LocalPlayerPosition;
														auto dst = Length2D(delta);
														std::string distances = "[" + std::to_string((int)dst / 100) + "m]";
														imgui_custom::CreateOutlinedText(GManagement.Verdana, distances.c_str(), ImVec2(ScreenPos.x, ScreenPos.y + 5.f), 12.f, hex_Dstcol, 1);
														padding += 15;
													}

													if (GManagement.m_Configs.debug_esp) {
														
														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("1: " + std::to_string(Root->N00000B18)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("2: " + std::to_string(Root->N00000AF8)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("3: " + std::to_string(Root->N00000B1B)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("4: " + std::to_string(Root->N00000AF9)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("5: " + std::to_string(Root->N00000B1E)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("6: " + std::to_string(Root->N00000AFA)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("7: " + std::to_string(Root->N00000B21)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("8: " + std::to_string(Root->N00000AFB)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("9: " + std::to_string(Root->N00000B24)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("10: " + std::to_string(Root->N00000B28)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("11: " + std::to_string(Root->N00000AFC)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("12: " + std::to_string(Root->N00000B2B)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("13: " + std::to_string(Root->N00000AFD)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("14: " + std::to_string(Root->N00000B2E)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("15: " + std::to_string(Root->N00000AFE)).c_str());
														padding += 15;
										
														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("16: " + std::to_string(Root->N00000AFF)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("17: " + std::to_string(Root->N00000B33)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("18: " + std::to_string(Root->N00000B00)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("19: " + std::to_string(Root->N00000B36)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("20: " + std::to_string(Root->N00000B01)).c_str());
														padding += 15;
												
														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("21: " + std::to_string(Root->N00000B39)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("22: " + std::to_string(Root->N00000B02)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("23: " + std::to_string(Root->N00000B3C)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("24: " + std::to_string(Root->N00000B3E)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("25: " + std::to_string(Root->N00000B6E)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("26: " + std::to_string(Root->N00000B3F)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("27: " + std::to_string(Root->N00000B71)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("28: " + std::to_string(Root->N00000B05)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("29: " + std::to_string(Root->N00000B74)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("30: " + std::to_string(Root->N00000B06)).c_str());
														padding += 15;
													
														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("31: " + std::to_string(Root->N00000B77)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("32: " + std::to_string(Root->N00000B07)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("33: " + std::to_string(Root->N00000B08)).c_str());
														padding += 15;
											
														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("34: " + std::to_string(Root->N00000B0B)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("35: " + std::to_string(Root->N00000B0C)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("36: " + std::to_string(Root->N00000B0C)).c_str());
														padding += 15;

														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), ("37: " + std::to_string(Root->N00000B64)).c_str());
														padding += 15;

													}

													if (GManagement.m_Configs.NameESP) {
														ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y + padding), "player");
														padding += 15;
													}


													if (GManagement.m_Configs.SnapLines) {
														imgui_custom::DrawLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y), ImVec2(ScreenPos.x, ScreenPos.y), hex_Snapcol, 1.f);
													}

													if (GManagement.m_Configs.BoundingBox) {

														auto height = ScreenHead.y - ScreenPos.y;
														auto width = height / 2;

														auto topleftX = ScreenHead.x - (width / 2);
														auto topLeftY = ScreenHead.y;
														auto botleftX = ScreenHead.x + (width / 2);
														auto botleftY = ScreenPos.y;

														if (GManagement.m_Configs.BoundingBox)

														imgui_custom::DrawRectangle(ImVec2(topleftX, topLeftY), ImVec2(botleftX, botleftY), hex_boxcol, 1.f);
														// Black
														imgui_custom::DrawRectangle(ImVec2(topleftX + 1, topLeftY - 1), ImVec2(botleftX - 1, botleftY + 1), 0x000000FF, 1.f);

													}

												}

											}


											if (GManagement.m_Configs.ActorID) {
												ImGui::RenderText(ImVec2(ScreenPos.x, ScreenPos.y), std::to_string(Actor->ActorID).c_str());
											}
										}
									}
								}

								Draw_Fov();

								if (GManagement.m_Configs.EnableAimbot && best_aim && GetAsyncKeyState(GManagement.m_Configs.AimbotKey) && GetForegroundWindow() == GManagement.m_Window) {

									try {
										float ScreenCenterX = ImGui::GetIO().DisplaySize.x / 2;
										float ScreenCenterY = ImGui::GetIO().DisplaySize.y / 2;

										float TargetX = 0;
										float TargetY = 0;

										if (best_aim.x > 0.f && best_aim.x < ImGui::GetIO().DisplaySize.x && best_aim.y > 0.f && best_aim.y < ImGui::GetIO().DisplaySize.y)
										{

											//X Axis
											if (best_aim.x != 0)
											{
												if (best_aim.x > ScreenCenterX)
												{
													TargetX = -(ScreenCenterX - best_aim.x);
													TargetX /= GManagement.m_Configs.AimSpeed;
													if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
												}

												if (best_aim.x < ScreenCenterX)
												{
													TargetX = best_aim.x - ScreenCenterX;
													TargetX /= GManagement.m_Configs.AimSpeed;
													if (TargetX + ScreenCenterX < 0) TargetX = 0;
												}
											}

											//Y Axis

											if (best_aim.y != 0)
											{
												if (best_aim.y > ScreenCenterY)
												{
													TargetY = -(ScreenCenterY - best_aim.y);
													TargetY /= GManagement.m_Configs.AimSpeed;
													if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
												}

												if (best_aim.y < ScreenCenterY)
												{
													TargetY = best_aim.y - ScreenCenterY;
													TargetY /= GManagement.m_Configs.AimSpeed;
													if (TargetY + ScreenCenterY < 0) TargetY = 0;
												}
											}

											if (!GManagement.m_Configs.AimSpeed)
											{
												mouse_event(MOUSEEVENTF_MOVE, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);
												return;
											}

											TargetX /= 10;
											TargetY /= 10;

											//Mouse even't will round to 0 a lot of the time, so we can add this, to make it more accurrate on slow speeds.
											if (fabs(TargetX) < 1)
											{
												if (TargetX > 0)
												{
													TargetX = 1;
												}
												if (TargetX < 0)
												{
													TargetX = -1;
												}
											}
											if (fabs(TargetY) < 1)
											{
												if (TargetY > 0)
												{
													TargetY = 1;
												}
												if (TargetY < 0)
												{
													TargetY = -1;
												}
											}

											if (is_pointWIthinFov(best_aim)) {

												// So proximity roasted me
												if (abs(TargetX) > 1 || abs(TargetY) > 1)
													mouse_event(MOUSEEVENTF_MOVE, TargetX, TargetY, NULL, NULL);

												std::string aimpos = "Aim X: " + std::to_string(TargetX) + " Aim Y: " + std::to_string(TargetY);

												if (GManagement.m_Configs.debugInfo)
													ImGui::RenderText(ImVec2(7, 65), aimpos.c_str());
											}

										}

									}
									catch (...) {

									}
								}
							}
						}
					}
				}
			}
		}

	}


}
