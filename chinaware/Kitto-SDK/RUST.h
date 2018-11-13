#pragma once

#ifndef RUST
#define RUST

namespace rust
{

	inline const void** getvtable(const void* inst, size_t offset = 0)
	{
		if (!inst && !offset)
			return NULL;
		return *reinterpret_cast<const void***>((size_t)inst + offset);
	}

	template <typename Fn>
	inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
	{
		if (!inst && offset == 0)
			return NULL;

		return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
	}

#pragma region reversal
	class N0000000A
	{
	public:
		char pad_0000[72]; //0x0000
	}; //Size: 0x0048

	class N00000020
	{
	public:
		class N00000214* N00000021; //0x0000
		char pad_0008[8]; //0x0008
		class N0000061F* N00000023; //0x0010
		char pad_0018[24]; //0x0018
		class N00000036* N00000027; //0x0030
		char pad_0038[1104]; //0x0038
	}; //Size: 0x0488

	class N0000002B
	{
	public:
		char pad_0000[72]; //0x0000
	}; //Size: 0x0048

	class BaseObject
	{
	public:
		class N00000728* N00000016; //0x0000
		BaseObject* NextBasePtr; //0x0008
		class ObjectClass* ObjectClassPtr; //0x0010
		char pad_0018[24]; //0x0018
		class N00000209* N0000001C; //0x0030
		char pad_0038[1104]; //0x0038
	}; //Size: 0x0488

	class GameObjectManager
	{
	public:
		BaseObject * lastTaggedObj; //0x0000
		BaseObject* TaggedObj; //0x0008
		N00000020* LastActiveObj; //0x0010
		N0000002B* ActiveObj; //0x0018
	};

	// Much wow naming

	class UnicodeString
	{
	public:
		char pad_0000[16]; //0x0000
		int32_t size; //0x0010
		wchar_t string[32]; //0x0014
	}; //Size: 0x0054

	class CVisualState
	{
	public:
		char pad_0000[176]; //0x0000
		D3DXVECTOR3 Position; //0x00B0
		float N0000078F; //0x00BC
		float N0000076B; //0x00C0
		D3DXVECTOR3 Rotation; //0x00C4
		char pad_00D0[208]; //0x00D0
	}; //Size: 0x01A0

	class Transgender
	{
	public:
		char pad_0000[56]; //0x0000
		class CVisualState* VisualState; //0x0038
		int32_t Index; //0x0040



	}; //Size: 0x0188

	class CBoneValue
	{
	public:
		char pad_0000[16]; //0x0000
		class Transgender* Transform; //0x0010
		char pad_0018[48]; //0x0018
	}; //Size: 0x0048

	class CBoneInfo
	{
	public:
		char pad_0000[40]; //0x0000
		class UnicodeString* BoneName; //0x0028
		class CBoneValue* BoneValue; //0x0030
		char pad_0038[8]; //0x0038
		int32_t N00000A8D; //0x0040
		char pad_0044[1532]; //0x0044
		class N00000BFD* N00000BDB; //0x0640
		char pad_0648[256]; //0x0648
	}; //Size: 0x0748

	class N000009F9
	{
	public:
		char pad_0000[24]; //0x0000
		class CBoneInfo* BoneInfo; //0x0018
		char pad_0020[32]; //0x0020
		int32_t BoneCount; //0x0040
		char pad_0044[1028]; //0x0044
	}; //Size: 0x0448

	class CSkinMultiMesh
	{
	public:
		char pad_0000[32]; //0x0000
		class N000009F9* BoneDictionary; //0x0020
		class N000009EE* N000009C9; //0x0028
		char pad_0030[280]; //0x0030
	}; //Size: 0x0148

	class CModelState
	{
	public:
		char pad_0000[64]; //0x0000
		class N000008AC* N00000831; //0x0040
		char pad_0048[144]; //0x0048
		class CSkinMultiMesh* SkinnedMultiMesh; //0x00D8
		char pad_00E0[96]; //0x00E0
		D3DXVECTOR3 Velocity; //0x0140
		float N00000874; //0x014C
		float N00000853; //0x0150
		char pad_0154[4]; //0x0154
		float N00000854; //0x0158
		float N0000087A; //0x015C
		float N00000855; //0x0160
		float N0000087D; //0x0164
		float N00000856; //0x0168
		float N00000880; //0x016C
		float N00000857; //0x0170
		float N00000883; //0x0174
		float N00000858; //0x0178
		float N00000888; //0x017C
		float N00000859; //0x0180
		float N0000088B; //0x0184
		char pad_0188[72]; //0x0188
		float N00000863; //0x01D0
		char pad_01D4[8]; //0x01D4
		bool N000008A9; //0x01DC
		char pad_01DD[4]; //0x01DD
		float N00000865; //0x01E1
		float N00000893; //0x01E5
		float N00000866; //0x01E9
		float N00000890; //0x01ED
		float N00000867; //0x01F1
		float N0000088E; //0x01F5
		float N00000868; //0x01F9
		char pad_01FD[84]; //0x01FD
	}; //Size: 0x0251

	class CPlayerInput
	{
	public:
		char pad_0000[48]; //0x0000
		bool m_hadInputBuffer; //0x0030
		char pad_0031[3]; //0x0031
		D3DXVECTOR4 m_bodyRotation; //0x0034
		D3DXVECTOR3 m_bodyAngles; //0x0044
		D3DXVECTOR4 m_headRotation; //0x0050
		D3DXVECTOR3 m_headAngles; //0x0060
		D3DXVECTOR3 m_recoilAngles; //0x006C
		D3DXVECTOR2 m_viewDelta; //0x0078
		D3DXVECTOR3 m_headDelta; //0x0080
		int32_t m_mouseWheelUp; //0x008C
		int32_t m_mouseWheelDn; //0x0090
		float m_mouseX; //0x0094
		float m_mouseY; //0x0098
		D3DXVECTOR3 m_offsetAngles; //0x009C
		uint32_t m_ignoredButtons; //0x00A8
		bool m_hasKeyFocus; //0x00AC
		char pad_00AD[155]; //0x00AD
	};

	class BaseEntity
	{
	public:
		char pad_0000[40]; //0x0000
		class N00000797* m_prefabName; //0x0028
		char pad_0030[328]; //0x0030
		class N000008AC* N000005F0; //0x0178
		class N000008A1* N000005F1; //0x0180
		char pad_0188[64]; //0x0188
		float Health; //0x01C8
		float N0000074F; //0x01CC
		float N000005FB; //0x01D0
		char pad_01D4[444]; //0x01D4
		class N0000086E* N00000633; //0x0390
		class N0000064B* N00000634; //0x0398
		char pad_03A0[8]; //0x03A0
		class N00000656* N00000636; //0x03A8
		char pad_03B0[16]; //0x03B0
		class N00000880* N00000639; //0x03C0
		char pad_03C8[32]; //0x03C8
		class N0000088B* N0000063E; //0x03E8
		class N00000896* N0000063F; //0x03F0
		char pad_03F8[32]; //0x03F8
		class CPlayerInput* PlayerInput; //0x0418
		class CWalkMovement* Walkmovement; //0x0420
		char pad_0428[32]; //0x0428
		class N000008C4* SteamName; //0x0448
		class UnicodeString* DisplayName; //0x0450
		char pad_0458[8]; //0x0458
		class CModelState* ModelState; //0x0460
		char pad_0468[88]; //0x0468
		class CPlayerTick* Playertick; //0x04C0
		char pad_04C8[32]; //0x04C8
		float N00000742; //0x04E8
		D3DXVECTOR3 eye; //0x04EC
		int32_t N00000743; //0x04F8
		int8_t N00001357; //0x04FC
		bool Sleeping; //0x04FD
		char pad_04FE[73]; //0x04FE
		float N0000074D; //0x0547
		char pad_054B[12]; //0x054B
		float N00000796; //0x0557
		char pad_055B[244]; //0x055B

	}; //Size: 0x0D78

	class Level2EntityClass
	{
	public:
		char pad_0000[40]; //0x0000
		BaseEntity* PlayerEntity; //0x0028
		class N00000D20* N00000AE2; //0x0030
		char pad_0038[16]; //0x0038
	}; //Size: 0x0048

	class Level1EntityClass
	{
	public:
		char pad_0000[8]; //0x0000
		Transgender* Tranny; //0x0008
		char pad_0010[8]; //0x0010
		Level2EntityClass* Entity; //0x0018

	}; //Size: 0x0148

	enum objectsTags : int
	{
		MAINCAMERA = 5,
		PLAYER = 6,
		TERRAIN = 20001,
		CORPSE = 20009, //Animals & Players
		MISC = 20006, //Trash cans, Sleeping Bags, Storage Box, etc
		ANIMAL = 20008,
		SKYDOME = 20012,
		RIVERMESH = 20014,
		MONUMENT = 20015 //Airport, Powerplant, etc
	};

	class ObjectClass
	{
	public:
		char pad_0000[8]; //0x0000
		int32_t N0000002C; //0x0008
		char pad_000C[20]; //0x000C
		int32_t N00000046; //0x0020
		char pad_0024[12]; //0x0024
		Level1EntityClass* Level1EntityClassPtr; //0x0030
		int32_t N00000031; //0x0038
		char pad_003C[4]; //0x003C
		int32_t N00000076; //0x0040
		char pad_0044[4]; //0x0044
		int32_t N0000006D; //0x0048
		char pad_004C[12]; //0x004C
		int32_t Layer; //0x0058
		int16_t Tag; //0x005C
		char pad_005E[10]; //0x005E
		char* ObjectName; //0x0068
	};

	class Camera
	{
	public:
		char pad_0000[192]; //0x0000
		D3DXMATRIX Matrix; //0x00C0

	};

	class CameraObject
	{
	public:
		char pad_0000[8]; //0x0000
		class Transgender* Transform; //0x0008
		char pad_0010[8]; //0x0010
		class Camera* Cam; //0x0018
	};

	struct Skeleton
	{
		D3DXVECTOR3 Position;
		char PartName[32];
		int index;
	};

	// Bone Count: 84
	enum Bone_List : int {
		pelvis = 1,
		l_hip,
		l_knee,
		l_foot,
		l_toe,
		l_ankle_scale,
		penis,
		GenitalCensor,
		GenitalCensor_LOD0,
		Inner_LOD0,
		GenitalCensor_LOD1,
		GenitalCensor_LOD2,
		r_hip,
		r_knee,
		r_foot,
		r_toe,
		r_ankle_scale,
		spine1,
		spine1_scale,
		spine2,
		spine3,
		spine4,
		l_clavicle,
		l_upperarm,
		l_forearm,
		l_hand,
		l_index1,
		l_index2,
		l_index3,
		l_little1,
		l_little2,
		l_little3,
		l_middle1,
		l_middle2,
		l_middle3,
		l_prop,
		l_ring1,
		l_ring2,
		l_ring3,
		l_thumb1,
		l_thumb2,
		l_thumb3,
		IKtarget_righthand_min,
		IKtarget_righthand_max,
		l_ulna,
		neck,
		head,
		jaw,
		eyeTranform,
		l_eye,
		l_Eyelid,
		r_eye,
		r_Eyelid,
		r_clavicle,
		r_upperarm,
		r_forearm,
		r_hand,
		r_index1,
		r_index2,
		r_index3,
		r_little1,
		r_little2,
		r_little3,
		r_middle1,
		r_middle2,
		r_middle3,
		r_prop,
		r_ring1,
		r_ring2,
		r_ring3,
		r_thumb1,
		r_thumb2,
		r_thumb3,
		IKtarget_lefthand_min,
		IKtarget_lefthand_max,
		r_ulna,
		l_breast,
		r_breast,
		BoobCensor,
		BreastCensor_LOD0,
		BreastCensor_LOD1,
		BreastCensor_LOD2,
		collision,
		displacement,
	};



#pragma endregion
	// Dumps all names of GOM
	void DumpGOM();

	// Gets the local Player
	BaseObject* GetLocalBase();

	// Gets the GOM
	GameObjectManager* GetGOM();

	void DumpPlayerList();

	void DrawESP();

	bool IsInGame();

	void DumpGameStuff();

	void DoAimbot();
}

#endif