#include"AutoResponse.h"

#define EXE_NAME L"RunEmuTenvi"
Region Global_Region = TENVI_JP;

Region GetRegion() {
	return Global_Region;
}

Region GetRegion(HINSTANCE hinstDLL) {
	Config conf(EXE_NAME".ini", hinstDLL);
	std::wstring wRegion;

	if (!conf.Read(EXE_NAME, L"Region", wRegion) || wRegion.length() == 0) {
		DEBUG(L"AutoResponse - JP (Default)");
		return TENVI_JP;
	}

	if (wRegion.compare(L"CN") == 0) {
		DEBUG(L"AutoResponse - CN");
		return TENVI_CN;
	}

	if (wRegion.compare(L"HK") == 0) {
		DEBUG(L"AutoResponse - HK");
		return TENVI_HK;
	}

	if (wRegion.compare(L"KR") == 0) {
		DEBUG(L"AutoResponse - KR");
		return TENVI_KR;
	}

	DEBUG(L"AutoResponse - JP");
	return TENVI_JP;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
		Global_Region = GetRegion(hinstDLL);
		AutoResponseHook();
	}
	return TRUE;
}