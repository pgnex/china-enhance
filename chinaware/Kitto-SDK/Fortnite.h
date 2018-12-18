#pragma once

#ifndef FORTNITE
#define FORTNITE

namespace Fortnite
{
	constexpr uintptr_t UWorldOffset = 0x593D010;

	/*
	class FTextData {
	public:
		char pad_0x0000[0x28];  //0x0000
		wchar_t* Name;          //0x0028 
		__int32 Length;         //0x0030 
	 
	};
	 
	struct FText {
		FTextData* Data;
		char UnknownData[0x10];
	 
		wchar_t* Get() const {
			if (Data) {
				return Data->Name;
			}
	 
			return nullptr;
		}
	};
	*/
	
	template<class T>
	struct TArray
	{
		friend struct FString;

	public:
		inline TArray()
		{
			Data = nullptr;
			Count = Max = 0;
		};

		inline int Num() const
		{
			return Count;
		};

		inline T& operator[](int i)
		{
			return Data[i];
		};

		inline const T& operator[](int i) const
		{
			return Data[i];
		};

		inline bool IsValidIndex(int i) const
		{
			return i < Num();
		}

	private:
		T* Data;
		int32_t Count;
		int32_t Max;
	};

	class Root1
	{
	public:
		char pad_0000[128]; //0x0000
		float N00000A74; //0x0080
		int32_t N00000AA0; //0x0084
		char pad_0088[24]; //0x0088
		D3DXVECTOR3 Positon1; //0x00A0
		float N00000A96; //0x00AC
		float N00000A79; //0x00B0
		char pad_00B4[4]; //0x00B4
		float N00000A7A; //0x00B8
		char pad_00BC[12]; //0x00BC
		float N00000A7C; //0x00C8
		char pad_00CC[188]; //0x00CC
		int32_t N00000A94; //0x0188
		char pad_018C[324]; //0x018C
	}; //Size: 0x02D0

	class Root2
	{
	public:
		char pad_0000[328]; //0x0000
		D3DXVECTOR3 Position2; //0x0148
		float N00000B18; //0x0154
		float N00000AF8; //0x0158
		// Delta Heights
		float HeightAbsDelta1; //0x015C
		// Delta Heighjts
		float HeightAbsDelta2; //0x0160

		D3DXVECTOR3 TestPosition; //0x0148

		// float N00000B1E; //0x0164
		// float N00000AFA; //0x0168
		// float N00000B21; //0x016C

		float N00000AFB; //0x0170
		float N00000B24; //0x0174
		int32_t N00000B28; //0x0178
		float N00000AFC; //0x017C
		int32_t N00000B2B; //0x0180
		float N00000AFD; //0x0184
		float N00000B2E; //0x0188
		float N00000AFE; //0x018C
		char pad_0190[4]; //0x0190
		int32_t N00000AFF; //0x0194
		int32_t N00000B33; //0x0198
		int32_t N00000B00; //0x019C
		float N00000B36; //0x01A0
		float N00000B01; //0x01A4
		float N00000B39; //0x01A8
		float N00000B02; //0x01AC
		float N00000B3C; //0x01B0
		int32_t N00000B3E; //0x01B4
		float N00000B6E; //0x01B8
		float N00000B3F; //0x01BC
		float N00000B71; //0x01C0
		char pad_01C4[16]; //0x01C4
		int32_t N00000B05; //0x01D4
		int32_t N00000B74; //0x01D8
		int32_t N00000B06; //0x01DC
		float N00000B77; //0x01E0
		float N00000B07; //0x01E4
		char pad_01E8[4]; //0x01E8
		float N00000B08; //0x01EC
		char pad_01F0[20]; //0x01F0
		float N00000B0B; //0x0204
		char pad_0208[4]; //0x0208
		float N00000B0C; //0x020C
		char pad_0210[284]; //0x0210
		float N00000B64; //0x032C
		char pad_0330[52]; //0x0330
}; //Size: 0x0364

#if 0
	class RootComponent
	{
	public:
		char pad_0000[320]; //0x0000
		float N00000401; //0x0140
		float N00000438; //0x0144
		float N00000402; //0x0148
		char pad_014C[44]; //0x014C
		float N00000408; //0x0178
		float N0000042D; //0x017C
		int32_t N00000409; //0x0180
		int32_t N0000042A; //0x0184
		float N0000040A; //0x0188
		float N00000425; //0x018C
		D3DXVECTOR3 Position; //0x0190
		float N00000423; //0x019C
		float N00000434; //0x01A0
		char pad_01A4[48]; //0x01A4
		float N00000412; //0x01D4
		char pad_01D8[4]; //0x01D8
		float N00000413; //0x01DC
		char pad_01E0[116]; //0x01E0
	}; //Size: 0x0254
#endif
	class Actor
	{
	public:
		char pad_0000[24]; //0x0000
		int32_t ActorID; //0x0018
		char pad_001C[292]; //0x001C
		class Root1* Root1; //0x0140
		char pad_0148[16]; //0x0148
		class Root2* Root2; //0x0158
	};

	class CameraManager
	{
	public:
		char pad_0000[3632]; //0x0000
		D3DXVECTOR3 CameraPosition; //0x0E30
		D3DXVECTOR3 Rotation; //0x0E3C
		float FOV; //0x0E48
		float FOV2; //0x0E4C
		float SomeFloat; //0x0E50
		char pad_0E54[572]; //0x0E54
	}; //Size: 0x1E60

	class PlayerController
	{
	public:
		char pad_0000[0x3D0]; //0x0000
		CameraManager* CameraManager; //0x03C8
	}; //Size: 0x0578

	class CLocalPlayer
	{
	public:
		char pad_0000[16]; //0x0000
		class N000000CD* N0000000D; //0x0010
		int32_t N0000000E; //0x0018
		int32_t N000005C4; //0x001C
		char pad_0020[16]; //0x0020
		PlayerController* PlayerController; //0x0030
		char pad_0038[56]; //0x0038
		class N000000A2* N00000019; //0x0070
		char pad_0078[16]; //0x0078
		D3DXVECTOR3 LocalPlayerPosition; //0x0088
	};

	class OwningInstance
	{
	public:
		char pad_0000[48]; //0x0000
		class N0000089E* N0000009B; //0x0030
		TArray<CLocalPlayer*>localPlayers; //0x0038 
	}; //Size: 0x0090

	class N00000074
	{
	public:
		char pad_0000[8]; //0x0000
	}; //Size: 0x0008

	class ULevel
	{
	public:
		char pad_0000[88]; //0x0000
		wchar_t* LevelName; //0x0058
		char pad_0060[56]; //0x0060
		class N00000074 N00000048; //0x0098
		int32_t N00000049; //0x00A0
		int32_t N00000071; //0x00A4
		TArray<Actor*> Actors; //0x00A0
		int32_t N0000004B; //0x00B0
		int32_t N0000006E; //0x00B4

	}; //Size: 0x0148

	class CUWorld
	{
	public:
		char pad_0000[48]; //0x0000
		ULevel* ULevel; //0x0030
		char pad_0038[336]; //0x0038
		OwningInstance* OwningInstance; //0x0188
	};

	CUWorld* GetUWorld();
	void Run_Hake();
	void DrawFeet();
	void DoAimbot();

	void PrintUWorldPointer();
	void PrintLocal();
	void DumpPlayerAddresses();
	void DumpPlayerNames();
	void uworld_offset_calc();
}

#endif