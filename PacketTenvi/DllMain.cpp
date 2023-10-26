#include"PacketTenvi.h"

// TENVI v127
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
		LoadRegionConfig(hinstDLL);
		PacketHook();
	}
	return TRUE;
}