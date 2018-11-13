#pragma once
//Includes

class vmt_manager
{
public:
	vmt_manager();
	vmt_manager(uintptr_t* ipointer);

	uintptr_t hook_m(uintptr_t n_func, int i);
	void unhook_m(uintptr_t i);

	void restore_o();
	void restore_c();

	uintptr_t get_origin_func(int Index);

private:
	uintptr_t * original_t;
	uintptr_t* custom_t;
	bool i_complete;
	uintptr_t* instance;
	int get_size(uintptr_t* ipointer);
};
