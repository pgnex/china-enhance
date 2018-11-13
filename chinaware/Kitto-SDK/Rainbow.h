#pragma once

#ifndef RAINBOW
#define RAINBOW

/*

game+0x4ECDD40->0xC8->0x8*iterator = entity/actor
entity->0x620->0x10 = viewangle (vector4)
game+0x4E91190->0x388->0x0 = localplayer
target = if (entity != localplayer)


*/
namespace RAINBOW_CHEAT
{
#pragma region Player Based Structures
	class cWeaponInfo {
	public:
		char pad_0000[0x40]; //0x0000
		D3DXVECTOR4 Spreads; //0x0040
		char pad_0050[0x20]; //0x0050
		float Spread; //0x0070
		char pad_0074[0x34]; //0x0074
		float Recoil; //0x00A8
		char pad_00AC[0x94]; //0x00AC
		float KickRecoil; //0x0140
		char pad_0144[0x4]; //0x0144
		float ADSRecoil; //0x0148
	};

	class cWeaponData {
	public:
		char pad_0000[0x110]; //0x0000
		cWeaponInfo* weaponInfo; //0x0110
	}; //Size: 0x0118

	class cWeapon {
	public:
		char pad_0000[0x120]; //0x0000
		cWeaponData* weaponData; //0x0120
	}; //Size: 0x0128

	class cWeaponProcessor {
	public:
		cWeapon * weapon; //0x0000
	}; //Size: 0x0008

	class cWeaponComponent {
	public:
		char pad_0000[0x50]; //0x0000
		cWeaponProcessor* primaryWeaponProcessor; //0x0050
		char pad_0058[0x98]; //0x0058
		cWeaponProcessor* currentWeaponProcessor; //0x00F0
	}; //Size: 0x00F8

	class cChildComponent {
	public:
		char pad_0000[0x148]; //0x0000
		int health; //0x0148
	}; //Size: 0x0148

	class cMainComponent {
	public:
		char pad_0000[0x8]; //0x0000
		cChildComponent* childComponent; //0x0008
		char pad_0010[0x28]; //0x0010
		cWeaponComponent* weaponComponent; //0x0038
	}; //Size: 0x0040

	class cEntityInfo {
	public:
		char pad_0000[0xA8]; //0x0000
		cMainComponent* mainComponent; //0x00A8
	}; //Size: 0x00B0

	class cPlayerName {
	public:
		char playerName[20]; //0x0000
	}; //Size: 0x000C

	class cPlayerInfo
	{
	public:
		char pad_0000[0x146]; //0x0000
		uint8_t teamID; //0x0146
		char pad_0147[0x31]; //0x0147
		cPlayerName* pPlayerName; //0x0178
	};

	class cPPlayerInfo
	{
	public:
		cPlayerInfo * playerInfo; //0x0000
	};

	class cEntity
	{
	public:
		char pad_0000[0x18]; //0x0000
		cEntityInfo* entityInfo; //0x0018
		char pad_0020[0x150]; //0x0020
		D3DXVECTOR3 HeadPosition; //0x0170
		char pad_016C[0x54]; //0x017C
		D3DXVECTOR3 FeetPosition; //0x01D0
		char pad_01CC[0xDC]; //0x01DC
		cPPlayerInfo* pPlayerInfo; //0x02B8
		char pad_02C0[0x360]; // 0x02C0
		void* angleInfo; //0x0620
	};

	class cPEntity
	{
	public:
		char pad_0000[0x20]; //0x0000
		class Entity* entity; //0x0020
	};

	class cEntityList1 {
	public:
		cPEntity * pEntity;
	};

	class cEntityList2 {
	public:
		cEntity * Entity;
	};

	class cGameManager
	{
	public:
		char pad_0000[0xB8]; //0x0000
		cEntityList1* EntityList1; //0x00B8
		int entityList1Size;
		char pad_00C4[0x4]; //0x00C4
		cEntityList2* EntityList2; //0x00C8
		int entityList2Size;

		// 48 8B 05 ?? ?? ?? ?? 48 8D 4C 24 ?? 48 8B 95
		// Here is a sig used to find the address again 
		cGameManager * GetInstance() {
			return *(cGameManager**)((uintptr_t)GetModuleHandleA(0) + 0x4ECDD40);
		}
	};
#pragma endregion

#pragma region Camera Based Structures

	class cCamera {
	public:
		char pad_0000[0xB0]; //0x0000
		D3DXVECTOR3 ViewRight; //0x00B0
		char pad_00BC[0x4]; //0x00BC
		D3DXVECTOR3 ViewUp; //0x00C0
		char pad_00CC[0x4]; //0x00CC
		D3DXVECTOR3 ViewForward; //0x00D0
		char pad_00DC[0x4]; //0x00DC
		D3DXVECTOR3 ViewTranslation; //0x00E0
		char pad_00EC[0x4]; //0x00EC
		float ViewFOVX; //0x0F0
		char pad_00F4[0x10]; //0x00F4
		float ViewFOVY; //0x0104
	}; //Size: 0x0108

	class cEngine {
	public:
		char pad_0000[0x40]; //0x0000
		cCamera* camera; //0x0040
	}; //Size: 0x0048

	class cEngineLink {
	public:
		char pad_0000[0x230]; //0x0000
		cEngine* engine; //0x0230
	}; //Size: 0x0238

	// 48 8B 05 ?? ?? ?? ?? 48 87 10 Sig for GameRenderer
	class cGameRenderer
	{
	public:
		char pad_0000[0x120]; //0x0000
		cEngineLink* engineLink; //0x0120

		cGameRenderer * GetInstance() {
			return *(cGameRenderer**)((uintptr_t)GetModuleHandleA(0) + 0x4DB3CF0);
		}
	};

	// 0.5235987902 << normal recoil value...
#pragma endregion

}
#endif
