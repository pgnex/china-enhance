/*
// FILENAME: VMTManager.cpp
// DESC: Provides functions for managing
VMT tables, hooking, storing backups,
and restoring the tables when done,
all in an anti-cheat safe way.
*/

//   --  Includes --  //
#include "Link.h"

// Sets up the class and replaces the instance vtable pointer
// with a pointer to our custom vtable
// Takes a pointer to a class casted as a DWORD*

class c_protect_guard
{
public:

	c_protect_guard(void *base, uint32_t len, uint32_t protect)
	{
		this->base = base;
		this->len = len;

		if (!VirtualProtect(base, len, protect, (PDWORD)&old_protect))
			throw std::runtime_error("Failed to protect region!");
	}

	~c_protect_guard()
	{
		VirtualProtect(base, len, old_protect, (PDWORD)&old_protect);
	}

private:

	void *base;
	uint32_t len;
	uint32_t old_protect;
};

vmt_manager::vmt_manager()
{
}

vmt_manager::vmt_manager(uintptr_t* ptr)
{
	instance = ptr;

	original_t = reinterpret_cast<uintptr_t*>(*ptr);

	auto msize = get_size(ptr);

	// allocate memory
	custom_t = new uintptr_t[msize + 1]();

	custom_t[0] = original_t[-1];

	memcpy(&custom_t[1], original_t, msize);

	// custom_t = reinterpret_cast<uintptr_t*>(malloc(get_size(ptr)));

	// memcpy(reinterpret_cast<decltype(&custom_t)>(custom_t), reinterpret_cast<decltype(&original_t)>(original_t), get_size(ptr));

	// *ptr = (uintptr_t)custom_t;

	try
	{
		auto guard = c_protect_guard{ ptr, sizeof(uintptr_t), PAGE_READWRITE };
		*(uintptr_t**)ptr = &custom_t[1];
	}
	catch (...)
	{
		delete[] custom_t;
	}


	i_complete = true;
}


// Counts the amount of entries in a VMT table
// Takes a pointer to a class casted as a DWORD*
int vmt_manager::get_size(uintptr_t* InstancePointer)
{
	uintptr_t* vmt = reinterpret_cast<uintptr_t*>(*InstancePointer);

	int index = 0;

	while (!IS_INTRESOURCE(vmt[index]))
	{
		index++;
	}

	return index * 4;
}

// Hooks the method at the provided index
// Simply replaces the table entry with the new one
uintptr_t vmt_manager::hook_m(uintptr_t NewFunction, int Index)
{
	if (i_complete)
	{
		custom_t[Index + 1] = NewFunction;
		return original_t[Index];
	}

	return NULL;
}

// Restores the original method pointer at the given index
void vmt_manager::unhook_m(uintptr_t Index)
{
	if (i_complete)
		custom_t[Index + 1] = original_t[Index];
	return;
}

// Restores the original pointer in the instance
void vmt_manager::restore_o()
{
	if (i_complete)
	{
		*instance = (uintptr_t)original_t;
	}
	return;
}

// Restores our custom pointer in the instance incase it has
//  been removed
void vmt_manager::restore_c()
{
	if (i_complete)
	{
		*instance = (uintptr_t)custom_t;
	}
	return;
}

uintptr_t vmt_manager::get_origin_func(int i)
{
	return original_t[i];
}

