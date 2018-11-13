/*
Copyright Kitto.win, All rights reserved.

Purpose:
	RUST Cheat

*/

#include "Link.h"

namespace rust
{
#pragma region Offsets

#define BASEDLL "UnityPlayer.dll"
#define GOM 0x144FE40

#pragma endregion

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

	void ClampAngles(D3DXVECTOR3&ang) {

		if (ang.x > 89)
			ang.x = 89;

		if (ang.x < 89)
			ang.x = 89;

		ang.y = fmodf(ang.y, 360.f);
	}

	void CalcAngle(D3DXVECTOR3 src, D3DXVECTOR3 dst, D3DXVECTOR3& angles)
	{
		D3DXVECTOR3 delta = src - dst;
		double hyp = Length2D(delta);
		angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
		angles.x = (atan(delta.z / hyp) * 57.295779513082f);
		if (delta.x >= 0.0) angles.y += 180.0f;
		ClampAngles(angles);
	}


	float GetFov(D3DXVECTOR3& viewAngle, D3DXVECTOR3& aimAngle)
	{
		D3DXVECTOR3 delta = aimAngle - viewAngle;
		ClampAngles(delta);
		return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
	}


	std::vector<BaseObject*> GetAllPlayers()
	{
		std::vector<BaseObject*> temp;

		auto ObjectManager = GetGOM();

		BaseObject* List = ObjectManager->TaggedObj;

		while (List && List != ObjectManager->lastTaggedObj) {

			if (List && (List->ObjectClassPtr->Tag == objectsTags::PLAYER)) {
				temp.push_back(List);
			}
			// enumerate list...
			List = List->NextBasePtr;
		}
		// finish looping thru list, check the last object...

		if (ObjectManager->lastTaggedObj->ObjectClassPtr->Tag == objectsTags::PLAYER) {
			temp.push_back(ObjectManager->lastTaggedObj);
		}

		return temp;
	}

	GameObjectManager* GetGOM()
	{
		return *(GameObjectManager**)((uintptr_t)GetModuleHandleA(BASEDLL) + GOM);
	}

	BaseObject* GetLocalBase()
	{
		auto ObjectManager = GetGOM();

		BaseObject* List = ObjectManager->TaggedObj;

		// loop till we hit entity called localplayer
		while ((!strstr(List->ObjectClassPtr->ObjectName, "LocalPlayer")) && List != ObjectManager->lastTaggedObj) {

			// enumerate list...
			List = List->NextBasePtr;
		}

		return List;
	}

	void DumpGOM()
	{
		// lets dump the address of the object manager
		auto ObjectManager = GetGOM();
		std::cout << "Address of GOM: " << ObjectManager << std::endl;
		std::cout << "Dumping All Names: " << std::endl;

		std::ofstream DumpFile("EntityDump.txt");

		BaseObject* first = ObjectManager->TaggedObj;

		// Dump the first object then enumerate
		DumpFile << first->ObjectClassPtr->ObjectName << std::endl;

		static auto next = first->NextBasePtr;

		while (next && next != ObjectManager->lastTaggedObj) {

			DumpFile << next->ObjectClassPtr->ObjectName << std::endl;

			next = next->NextBasePtr;
		}

		DumpFile << ObjectManager->lastTaggedObj->ObjectClassPtr->ObjectName << std::endl;

		std::cout << "Finished Dump..." << std::endl;
	}

	Camera* GetCamera()
	{
		auto GameObjectMgr = GetGOM();

		auto taggedObject = GameObjectMgr->TaggedObj;

		auto CameraBase = (CameraObject*)taggedObject->ObjectClassPtr->Level1EntityClassPtr;

		if (CameraBase && CameraBase->Cam) {

			return CameraBase->Cam;
		}
	}

	bool WorldToScreen(D3DXVECTOR3 origin, D3DXVECTOR2 * out)
	{
		D3DXMATRIX temp;
		// Just gotta get view matrix from camera.
		auto Matrix = GetCamera()->Matrix;

		D3DXMatrixTranspose(&temp, &Matrix);

		D3DXVECTOR3 translationVector = D3DXVECTOR3(temp._41, temp._42, temp._43);
		D3DXVECTOR3 up = D3DXVECTOR3(temp._21, temp._22, temp._23);
		D3DXVECTOR3 right = D3DXVECTOR3(temp._11, temp._12, temp._13);

		float w = D3DXVec3Dot(&translationVector, &origin) + temp._44;

		if (w < 0.098f)
			return false;

		float y = D3DXVec3Dot(&up, &origin) + temp._24;
		float x = D3DXVec3Dot(&right, &origin) + temp._14;

		out->x = (ImGui::GetIO().DisplaySize.x / 2) * (1.f + x / w);
		out->y = (ImGui::GetIO().DisplaySize.y / 2) * (1.f - y / w);

		return true;
	}

	void DumpPlayerList()
	{
		std::ofstream DumpFile("PlayerListDump.txt");
		auto Players = GetAllPlayers();

		for (auto Player : Players)
		{
			auto CurrentPlayerEntity = Player->ObjectClassPtr->Level1EntityClassPtr;

			char buf[32];
			wcstombs(buf, CurrentPlayerEntity->Entity->PlayerEntity->DisplayName->string, sizeof(buf));

			DumpFile << buf << " Is At: " << " x: " << CurrentPlayerEntity->Tranny->VisualState->Position.x << " y: " << CurrentPlayerEntity->Tranny->VisualState->Position.y << " z: " << CurrentPlayerEntity->Tranny->VisualState->Position.z << std::endl;
		}
	}

	bool IsInGame()
	{
		try {
			auto GameObjMgr = GetGOM();

			if (GameObjMgr) {
				auto last = GameObjMgr->TaggedObj;
				if (last) {
					if (last->ObjectClassPtr && last->ObjectClassPtr != nullptr) {

						auto local = GetLocalBase();
						if (local && (!local->ObjectClassPtr->Level1EntityClassPtr->Entity->PlayerEntity->Sleeping))
							return true;
					}
				}
			}
		}
		catch (...) {

		}

		return false;
	}

	struct TransformAccessReadOnly
	{
		ULONGLONG	pTransformData;
	};

	struct TransformData
	{
		ULONGLONG pTransformArray;
		ULONGLONG pTransformIndices;
	};

	struct Matrix34
	{
		D3DXVECTOR4 vec0;
		D3DXVECTOR4 vec1;
		D3DXVECTOR4 vec2;
	};

	template <typename T>
	T SafeRead(T* Data)
	{
		if (Data != nullptr)
			return *Data;
	}

	template <typename T>
	bool SafeReadToBuffer(T* Data, T* Buffer, size_t Size)
	{
		if (Data != nullptr && Buffer != nullptr && Size != 0)
		{
			memcpy(Buffer, Data, Size);
			return true;
		}

		return false;
	}

	void ReloadChangeUI()
	{
		reinterpret_cast<void(__cdecl*)()>((uintptr_t)GetModuleHandleA("panorama.dll") + 0x6450)();
	}
	
	D3DXVECTOR3 INTERNAL_Get_Position(CBoneValue* Tranny) {
		D3DXVECTOR3 tmp;
		reinterpret_cast<void(__fastcall*)(CBoneValue*, D3DXVECTOR3&)>((uintptr_t)GetModuleHandleA(BASEDLL) + 0x6143E0)(Tranny, tmp);
		return tmp;
	}

	D3DXVECTOR3 GetPosition_EXTERNAL(Transgender* Tranny)
	{
		__m128 result;

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		TransformAccessReadOnly pTransformAccessReadOnly = SafeRead((TransformAccessReadOnly*)((uintptr_t)Tranny + 0x38));

		TransformData transformData = SafeRead((TransformData*)(pTransformAccessReadOnly.pTransformData + 0x18));

		size_t sizeMatriciesBuf = sizeof(Matrix34) * Tranny->Index + sizeof(Matrix34);
		size_t sizeIndicesBuf = sizeof(int) * Tranny->Index + sizeof(int);

		// Allocate memory for storing large amounts of data (matricies and indicies)
		Matrix34* pMatriciesBuf = (Matrix34*)malloc(sizeMatriciesBuf);
		int* pIndicesBuf = (int*)malloc(sizeIndicesBuf);

		if (pMatriciesBuf && pIndicesBuf)
		{
			// Read Matricies array into the buffer
			SafeReadToBuffer((Matrix34*)transformData.pTransformArray, pMatriciesBuf, sizeMatriciesBuf);
			// Read Indices array into the buffer
			SafeReadToBuffer((int*)transformData.pTransformIndices, pIndicesBuf, sizeIndicesBuf);

			result = *(__m128*)((ULONGLONG)pMatriciesBuf + 0x30 * Tranny->Index);
			int transformIndex = *(int*)((ULONGLONG)pIndicesBuf + 0x4 * Tranny->Index);

			while (transformIndex >= 0)
			{
				Matrix34 matrix34 = *(Matrix34*)((ULONGLONG)pMatriciesBuf + 0x30 * transformIndex);

				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

				result = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
							tmp7)), *(__m128*)(&matrix34.vec0));

				transformIndex = *(int*)((ULONGLONG)pIndicesBuf + 0x4 * transformIndex);
			}

			free(pMatriciesBuf);
			free(pIndicesBuf);
		}

		return D3DXVECTOR3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
	}

	D3DXVECTOR3 GetBoneByID(BaseEntity* Entity, int Bone)
	{
		auto BoneInfo = Entity->ModelState->SkinnedMultiMesh->BoneDictionary->BoneInfo;

		auto BoneValue = *(CBoneValue**)((uintptr_t)BoneInfo + 0x30 + ((Bone - 1) * 0x18));

		return (INTERNAL_Get_Position(BoneValue));
	}

	std::vector<Skeleton> GetSkeleton(BaseEntity* Entity)
	{
		auto BoneDictionary = Entity->ModelState->SkinnedMultiMesh->BoneDictionary;
		std::vector<Skeleton> skeltmp;

		for (auto i = 0; i < BoneDictionary->BoneCount; i++) {

			Skeleton setup;

			auto boneinfo = BoneDictionary->BoneInfo;

			// Get name
			auto BoneName = *(UnicodeString**)((uintptr_t)boneinfo + 0x28 + (i * 0x18));
			if (BoneName)
				wcstombs(setup.PartName, BoneName->string, sizeof(setup.PartName));

			// Get Location
			auto BoneValue = *(CBoneValue**)((uintptr_t)boneinfo + 0x30 + (i * 0x18));

			if (BoneValue) {
				setup.Position = INTERNAL_Get_Position(BoneValue);
			}

			setup.index = BoneValue->Transform->Index;

			skeltmp.push_back(setup);
		}

		return skeltmp;
	}

	void DumpBones()
	{
		// we want all bone names
		// Lets use local player as reference
		auto Local = GetLocalBase();
		if (Local) {
			auto LocalEntity = Local->ObjectClassPtr->Level1EntityClassPtr->Entity;

			if (LocalEntity) {
				auto Skeleton = LocalEntity->PlayerEntity->ModelState->SkinnedMultiMesh->BoneDictionary;
				if (Skeleton) {

					auto boneinfo = Skeleton->BoneInfo;

					std::ofstream DumpFile("SkeletonDump.txt");

					auto bonecount = Skeleton->BoneCount;
					DumpFile << "// Bone Count: " << bonecount << std::endl;
					DumpFile << "enum Bone_List : int {" << std::endl;

					for (auto i = 0; i < bonecount; i++) {

						// 0x28 + 0x18 * i = name
						// 0x30 + 0x18 * i = value
						auto BoneName = *(UnicodeString**)((uintptr_t)boneinfo + 0x28 + (i * 0x18));

						char buf[32];
						wcstombs(buf, BoneName->string, sizeof(buf));

						DumpFile << " " << buf << " = " << boneinfo->BoneValue->Transform->Index << "," << std::endl;
					}
					DumpFile << "};" << std::endl;

				}
			}
		}

	}


	void DumpGameStuff()
	{
#pragma region Dumps
		if (rust::IsInGame()) {
			if (GManagement.m_Configs.DumpLocalName) {

				try
				{
					auto LocalBase = rust::GetLocalBase();

					if (LocalBase) {
						std::cout << LocalBase->ObjectClassPtr->ObjectName << " At: " << LocalBase << std::endl;
					}
				}
				catch (...)
				{
				}

				GManagement.m_Configs.DumpLocalName = false;
			}

			if (GManagement.m_Configs.DumpFullList)
			{
				try
				{
					rust::DumpGOM();
				}
				catch (...)
				{
				}

				GManagement.m_Configs.DumpFullList = false;
			}

			if (GManagement.m_Configs.DumpPlayerList) {

				try
				{
					rust::DumpPlayerList();
				}
				catch (...)
				{
				}

				GManagement.m_Configs.DumpPlayerList = false;
			}

			if (GManagement.m_Configs.DumpBoneList)
			{
				try {
					DumpBones();
				}
				catch (...) {}

				GManagement.m_Configs.DumpBoneList = false;
			}
		}

#pragma endregion
	}

	void DoAimbot()
	{
		try {

			if (!IsInGame())
				return;

			auto Local = GetLocalBase()->ObjectClassPtr->Level1EntityClassPtr->Entity->PlayerEntity;

			if (GetAsyncKeyState(GManagement.m_Configs.AimbotKey) && GManagement.m_Configs.EnableAimbot) {

				auto pList = GetAllPlayers();

				// Get closest player...
				BaseEntity* best = nullptr;

				float bestFov = GManagement.m_Configs.FOV;

				for (auto Player : pList) {

					auto PlayerEnt = Player->ObjectClassPtr->Level1EntityClassPtr->Entity->PlayerEntity;

					if (PlayerEnt->Health <= 0)
						continue;

					// Get head of each player
					D3DXVECTOR3 HeadCenter = GetBoneByID(PlayerEnt, Bone_List::head);
					D3DXVECTOR2 Screen;
					if (WorldToScreen(HeadCenter, &Screen)) {
						float width = ImGui::GetIO().DisplaySize.x;
						float height = ImGui::GetIO().DisplaySize.y;

						D3DXVECTOR2 dst = Screen - D3DXVECTOR2(width / 2, height / 2);

						float CurFov = Vector2dLength2D(dst);

						if (CurFov < GManagement.m_Configs.FOV && CurFov < bestFov)
						{
							best = PlayerEnt;
							bestFov = CurFov;
						}
					}

				}

				if (best && best != nullptr) {

					if (best->Health <= 0) {
						best = nullptr;
						return;
					}

					D3DXVECTOR3 AimPos;
					// Now we have player lets aim at bone.
					if (GManagement.m_Configs.AimBone == 0) {
						AimPos = GetBoneByID(best, Bone_List::head);
					}
					if (GManagement.m_Configs.AimBone == 1) {
						AimPos = GetBoneByID(best, Bone_List::spine4);
					}


					if (GManagement.m_Configs.ReduceRecoil) {
						Local->PlayerInput->m_recoilAngles = D3DXVECTOR3(0, 0, 0);
					}

					D3DXVECTOR2 screenpos;

					if (WorldToScreen(AimPos, &screenpos)) {

						float TargetX = 0;
						float TargetY = 0;

						float ScreenCenterX = ImGui::GetIO().DisplaySize.x / 2;
						float ScreenCenterY = ImGui::GetIO().DisplaySize.y / 2;

						//X Axis
						if (screenpos.x != 0)
						{
							if (screenpos.x > ScreenCenterX)
							{
								TargetX = -(ScreenCenterX - screenpos.x);
								TargetX /= GManagement.m_Configs.AimSpeed;
								if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
							}

							if (screenpos.x < ScreenCenterX)
							{
								TargetX = screenpos.x - ScreenCenterX;
								TargetX /= GManagement.m_Configs.AimSpeed;
								if (TargetX + ScreenCenterX < 0) TargetX = 0;
							}
						}

						//Y Axis

						if (screenpos.y != 0)
						{
							if (screenpos.y > ScreenCenterY)
							{
								TargetY = -(ScreenCenterY - screenpos.y);
								TargetY /= GManagement.m_Configs.AimSpeed;
								if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
							}

							if (screenpos.y < ScreenCenterY)
							{
								TargetY = screenpos.y - ScreenCenterY;
								TargetY /= GManagement.m_Configs.AimSpeed;
								if (TargetY + ScreenCenterY < 0) TargetY = 0;
							}
						}

						mouse_event(MOUSEEVENTF_MOVE, (DWORD)(TargetX), (DWORD)(TargetY), NULL, NULL);

					}
				}

			}
		}
		catch (...) {

		}

	}


	void DrawESP()
	{
		if (!IsInGame())
			return;

		if (GManagement.m_Configs.Crosshair) {

			if (GManagement.m_Configs.CrosshairType == 0) {

				auto centerx = ImGui::GetIO().DisplaySize.x / 2;
				auto centery = ImGui::GetIO().DisplaySize.y / 2;

				imgui_custom::DrawLine(ImVec2(centerx - 5, centery - 5), ImVec2(centerx + 5, centery + 5), 0xFFFF00FF, 1.f);
				imgui_custom::DrawLine(ImVec2(centerx - 5, centery + 5), ImVec2(centerx + 5, centery - 5), 0xFFFF00FF, 1.f);
			}

		}

		auto localbase = GetLocalBase();

		// Crosshair
		try {
			auto Players = GetAllPlayers();

			for (auto Player : Players)
			{

				if (GManagement.m_Configs.IgnoreLocalPlayer) {
					if (Player == localbase)
						continue;
				}

				auto CurrentPlayerEntity = Player->ObjectClassPtr->Level1EntityClassPtr;

				if (GManagement.m_Configs.IgnoreSleepers && CurrentPlayerEntity->Entity->PlayerEntity->Sleeping) {
					continue;
				}

				if (CurrentPlayerEntity->Entity->PlayerEntity->Health <= 0)
					continue;


				if (GManagement.m_Configs.BoundingBox || GManagement.m_Configs.HealthESP) {

					// Get top left of box
					auto pplayer = Player->ObjectClassPtr->Level1EntityClassPtr->Entity->PlayerEntity;

					D3DXVECTOR3 head = GetBoneByID(pplayer, Bone_List::head);
					// get bottom of box
					D3DXVECTOR3 base = CurrentPlayerEntity->Tranny->VisualState->Position;
					D3DXVECTOR2 w2sh, ws2b;

					if (WorldToScreen(head, &w2sh) && WorldToScreen(base, &ws2b))
					{
						w2sh.y -= 5;
						ws2b.y += 2;
						auto topleft = ImVec2(w2sh.x - (abs(w2sh.y - ws2b.y) / 2), w2sh.y);
						auto botright = ImVec2(ws2b.x + (abs(w2sh.y - ws2b.y) / 2), ws2b.y);

						if (GManagement.m_Configs.BoundingBox)
							imgui_custom::DrawRectangle(topleft, botright, 0xFF0000FF, 1.f);

						if (GManagement.m_Configs.HealthESP) {

							auto Health = (int)CurrentPlayerEntity->Entity->PlayerEntity->Health;

							auto botleft = ImVec2(w2sh.x - (abs(w2sh.y - ws2b.y) / 2), ws2b.y);

							imgui_custom::DrawLine(botleft, topleft, 0x000000FF, 1.f);

							auto dst = abs(botleft.y - topleft.y);

							auto amt = (dst / 100.f) * (100 - Health);
							auto newtop = ImVec2(topleft.x, topleft.y + amt);
							imgui_custom::DrawLine(newtop, botleft, 0x00FF00FF, 1.f);
						}

					}
				}

				D3DXVECTOR2 SP;

				if (WorldToScreen(CurrentPlayerEntity->Tranny->VisualState->Position, &SP)) {
					char buf[32];
					wcstombs(buf, CurrentPlayerEntity->Entity->PlayerEntity->DisplayName->string, sizeof(buf));

					int Pad = 0;
					if (GManagement.m_Configs.NameESP) {
						imgui_custom::CreateOutlinedText(GManagement.Verdana, buf, ImVec2(SP.x, SP.y + Pad), 12.f, 0xFFFFFFFF, 1);
						Pad += 15;
					}

					if (GManagement.m_Configs.DistanceESP) {
						auto delta = CurrentPlayerEntity->Tranny->VisualState->Position - localbase->ObjectClassPtr->Level1EntityClassPtr->Tranny->VisualState->Position;
						auto dst = Length2D(delta);
						std::string distances = "[" + std::to_string((int)dst) + "m]";
						imgui_custom::CreateOutlinedText(GManagement.Verdana, distances.c_str(), ImVec2(SP.x, SP.y + Pad), 12.f, 0xFFFFFFFF, 1);
						Pad += 15;
					}

				}


				if (GManagement.m_Configs.SkeletonESP) {
#pragma region CancerCode
					auto pplayer = Player->ObjectClassPtr->Level1EntityClassPtr->Entity->PlayerEntity;

					D3DXVECTOR3 Head = GetBoneByID(pplayer, Bone_List::head);
					D3DXVECTOR3 Neck = GetBoneByID(pplayer, Bone_List::neck);
					D3DXVECTOR3 LShoulder = GetBoneByID(pplayer, Bone_List::l_clavicle);
					D3DXVECTOR3 RShoulder = GetBoneByID(pplayer, Bone_List::r_clavicle);
					D3DXVECTOR3 LUpperArm = GetBoneByID(pplayer, Bone_List::l_upperarm);
					D3DXVECTOR3 RUpperArm = GetBoneByID(pplayer, Bone_List::r_upperarm);
					D3DXVECTOR3 LForeArm = GetBoneByID(pplayer, Bone_List::l_forearm);
					D3DXVECTOR3 RForeArm = GetBoneByID(pplayer, Bone_List::r_forearm);
					D3DXVECTOR3 LHand = GetBoneByID(pplayer, Bone_List::l_hand);
					D3DXVECTOR3 RHand = GetBoneByID(pplayer, Bone_List::r_hand);
					D3DXVECTOR3 Spine4 = GetBoneByID(pplayer, Bone_List::spine4);
					D3DXVECTOR3 Spine3 = GetBoneByID(pplayer, Bone_List::spine3);
					D3DXVECTOR3 Spine2 = GetBoneByID(pplayer, Bone_List::spine2);
					D3DXVECTOR3 Spine1 = GetBoneByID(pplayer, Bone_List::spine1);
					D3DXVECTOR3 Rhip = GetBoneByID(pplayer, Bone_List::r_hip);
					D3DXVECTOR3 LHip = GetBoneByID(pplayer, Bone_List::l_hip);
					D3DXVECTOR3 RKnee = GetBoneByID(pplayer, Bone_List::r_knee);
					D3DXVECTOR3 LKnee = GetBoneByID(pplayer, Bone_List::l_knee);
					D3DXVECTOR3 RFoot = GetBoneByID(pplayer, Bone_List::r_foot);
					D3DXVECTOR3 LFoot = GetBoneByID(pplayer, Bone_List::l_foot);
					D3DXVECTOR2 w2sHead;
					D3DXVECTOR2 w2sNeck;
					D3DXVECTOR2 w2sLShoulder;
					D3DXVECTOR2 w2sRShoulder;
					D3DXVECTOR2 w2sLUpperArm;
					D3DXVECTOR2 w2sRUpperArm;
					D3DXVECTOR2 w2sLForeArm;
					D3DXVECTOR2 w2sRForeArm;
					D3DXVECTOR2 w2sLHand;
					D3DXVECTOR2 w2sRHand;
					D3DXVECTOR2 w2sSpine4;
					D3DXVECTOR2 w2sSpine3;
					D3DXVECTOR2 w2sSpine2;
					D3DXVECTOR2 w2sSpine1;
					D3DXVECTOR2 w2sRhip;
					D3DXVECTOR2 w2sLHip;
					D3DXVECTOR2 w2sRKnee;
					D3DXVECTOR2 w2sLKnee;
					D3DXVECTOR2 w2sRFoot;
					D3DXVECTOR2 w2sLFoot;

					if (WorldToScreen(Head, &w2sHead) && WorldToScreen(Neck, &w2sNeck) && WorldToScreen(LShoulder, &w2sLShoulder) && WorldToScreen(RShoulder, &w2sRShoulder)
						&& WorldToScreen(LUpperArm, &w2sLUpperArm) && WorldToScreen(RUpperArm, &w2sRUpperArm) && WorldToScreen(LForeArm, &w2sLForeArm) && WorldToScreen(RForeArm, &w2sRForeArm)
						&& WorldToScreen(LHand, &w2sLHand) && WorldToScreen(RHand, &w2sRHand) && WorldToScreen(Spine4, &w2sSpine4) && WorldToScreen(Spine3, &w2sSpine3) && WorldToScreen(Spine2, &w2sSpine2)
						&& WorldToScreen(Spine1, &w2sSpine1) && WorldToScreen(Rhip, &w2sRhip) && WorldToScreen(LHip, &w2sLHip) && WorldToScreen(RKnee, &w2sRKnee) && WorldToScreen(LKnee, &w2sLKnee)
						&& WorldToScreen(RFoot, &w2sRFoot) && WorldToScreen(LFoot, &w2sLFoot)) {

						// Head to spine
						imgui_custom::DrawLine(ImVec2(w2sHead.x, w2sHead.y), ImVec2(w2sNeck.x, w2sNeck.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sNeck.x, w2sNeck.y), ImVec2(w2sSpine4.x, w2sSpine4.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sSpine4.x, w2sSpine4.y), ImVec2(w2sSpine3.x, w2sSpine3.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sSpine3.x, w2sSpine3.y), ImVec2(w2sSpine2.x, w2sSpine2.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sSpine2.x, w2sSpine2.y), ImVec2(w2sSpine1.x, w2sSpine1.y), 0xFF0000FF, 1.f);

						// Left leg
						imgui_custom::DrawLine(ImVec2(w2sSpine1.x, w2sSpine1.y), ImVec2(w2sLHip.x, w2sLHip.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sLHip.x, w2sLHip.y), ImVec2(w2sLKnee.x, w2sLKnee.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sLKnee.x, w2sLKnee.y), ImVec2(w2sLFoot.x, w2sLFoot.y), 0xFF0000FF, 1.f);

						// Right leg
						imgui_custom::DrawLine(ImVec2(w2sSpine1.x, w2sSpine1.y), ImVec2(w2sRhip.x, w2sRhip.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sRhip.x, w2sRhip.y), ImVec2(w2sRKnee.x, w2sRKnee.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sRKnee.x, w2sRKnee.y), ImVec2(w2sRFoot.x, w2sRFoot.y), 0xFF0000FF, 1.f);

						// Left arm
						imgui_custom::DrawLine(ImVec2(w2sSpine4.x, w2sSpine4.y), ImVec2(w2sLShoulder.x, w2sLShoulder.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sLShoulder.x, w2sLShoulder.y), ImVec2(w2sLUpperArm.x, w2sLUpperArm.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sLUpperArm.x, w2sLUpperArm.y), ImVec2(w2sLForeArm.x, w2sLForeArm.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sLForeArm.x, w2sLForeArm.y), ImVec2(w2sLHand.x, w2sLHand.y), 0xFF0000FF, 1.f);

						// Right arm
						imgui_custom::DrawLine(ImVec2(w2sSpine4.x, w2sSpine4.y), ImVec2(w2sRShoulder.x, w2sRShoulder.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sRShoulder.x, w2sRShoulder.y), ImVec2(w2sRUpperArm.x, w2sRUpperArm.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sRUpperArm.x, w2sRUpperArm.y), ImVec2(w2sRForeArm.x, w2sRForeArm.y), 0xFF0000FF, 1.f);
						imgui_custom::DrawLine(ImVec2(w2sRForeArm.x, w2sRForeArm.y), ImVec2(w2sRHand.x, w2sRHand.y), 0xFF0000FF, 1.f);

					}
#pragma endregion
				}



			}
		}
		catch (...) {

		}
	}

}
