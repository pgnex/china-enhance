/*
Copyright china.win, All rights reserved.

Purpose:
	Pubg Cheat

*/

#include "Link.h"

namespace PUBG
{
#pragma region IDA Defs
	typedef unsigned int uint;
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned long ulong;
	typedef          char   int8;
	typedef   signed char   sint8;
	typedef unsigned char   uint8;
	typedef          short  int16;
	typedef   signed short  sint16;
	typedef unsigned short  uint16;
	typedef          int    int32;
	typedef   signed int    sint32;
	typedef unsigned int    uint32;
	typedef          long long ll;
	typedef unsigned long long ull;
	typedef ll              int64;
	typedef ll              sint64;
	typedef ull             uint64;

	// rotate left
	template<class T> T __ROL__(T value, int count)
	{
		const uint nbits = sizeof(T) * 8;

		if (count > 0)
		{
			count %= nbits;
			T high = value >> (nbits - count);
			if (T(-1) < 0) // signed value
				high &= ~((T(-1) << count));
			value <<= count;
			value |= high;
		}
		else
		{
			count = -count % nbits;
			T low = value << (nbits - count);
			value >>= count;
			value |= low;
		}
		return value;
	}

	inline uint8  __ROL1__(uint8  value, int count) { return __ROL__((uint8)value, count); }
	inline uint16 __ROL2__(uint16 value, int count) { return __ROL__((uint16)value, count); }
	inline uint32 __ROL4__(uint32 value, int count) { return __ROL__((uint32)value, count); }
	inline uint64 __ROL8__(uint64 value, int count) { return __ROL__((uint64)value, count); }
	inline uint8  __ROR1__(uint8  value, int count) { return __ROL__((uint8)value, -count); }
	inline uint16 __ROR2__(uint16 value, int count) { return __ROL__((uint16)value, -count); }
	inline uint32 __ROR4__(uint32 value, int count) { return __ROL__((uint32)value, -count); }
	inline uint64 __ROR8__(uint64 value, int count) { return __ROL__((uint64)value, -count); }

	// Partially defined types. They are used when the decompiler does not know
	// anything about the type except its size.
#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64
#if !defined(_MSC_VER)
#define _LONGLONG __int128
#endif

	// Some convenience macros to make partial accesses nicer
#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
#  define LOW_IND(x,part_type)   LAST_IND(x,part_type)
#  define HIGH_IND(x,part_type)  0
#else
#  define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#  define LOW_IND(x,part_type)   0
#endif
	// first unsigned macros:
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define DWORDn(x, n)  (*((_DWORD*)&(x)+n))

#define LOBYTE(x)  BYTEn(x,LOW_IND(x,_BYTE))
#define LOWORD(x)  WORDn(x,LOW_IND(x,_WORD))
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#define HIBYTE(x)  BYTEn(x,HIGH_IND(x,_BYTE))
#define HIWORD(x)  WORDn(x,HIGH_IND(x,_WORD))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))

	// Generate a reference to pair of operands
	template<class T>  int16 __PAIR__(int8  high, T low) { return (((int16)high) << sizeof(high) * 8) | uint8(low); }
	template<class T>  int32 __PAIR__(int16 high, T low) { return (((int32)high) << sizeof(high) * 8) | uint16(low); }
	template<class T>  int64 __PAIR__(int32 high, T low) { return (((int64)high) << sizeof(high) * 8) | uint32(low); }
	template<class T> uint16 __PAIR__(uint8  high, T low) { return (((uint16)high) << sizeof(high) * 8) | uint8(low); }
	template<class T> uint32 __PAIR__(uint16 high, T low) { return (((uint32)high) << sizeof(high) * 8) | uint16(low); }
	template<class T> uint64 __PAIR__(uint32 high, T low) { return (((uint64)high) << sizeof(high) * 8) | uint32(low); }

#pragma endregion

	// Decryption

	bool Init()
	{
		// Get UWorld

	}
}