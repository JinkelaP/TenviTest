#include"../Share/Simple/Simple.h"
#include"../Share/Hook/SimpleHook.h"

enum Region {
	TENVI_JP, // JP v127
	TENVI_CN, // CN v126
	TENVI_HK, // HK v102
	TENVI_KR, // KR v107 (Xtream)
};

#define EXE_NAME L"RunEmuTenvi"

Region GetRegion(HINSTANCE hinstDLL) {
	Config conf(EXE_NAME".ini", hinstDLL);
	std::wstring wRegion;

	if (!conf.Read(EXE_NAME, L"Region", wRegion) || wRegion.length() == 0) {
		DEBUG(L"EmuMainTenvi - JP (Default)");
		return TENVI_JP;
	}

	if (wRegion.compare(L"CN") == 0) {
		DEBUG(L"EmuMainTenvi - CN");
		return TENVI_CN;
	}

	if (wRegion.compare(L"HK") == 0) {
		DEBUG(L"EmuMainTenvi - HK");
		return TENVI_HK;
	}

	if (wRegion.compare(L"KR") == 0) {
		DEBUG(L"EmuMainTenvi - KR");
		return TENVI_KR;
	}

	DEBUG(L"EmuMainTenvi - JP");
	return TENVI_JP;
}

bool EmuMainTenvi(HINSTANCE hinstDLL) {
	Rosemary r;

	switch (GetRegion(hinstDLL)) {
	case TENVI_JP: {
		// HSUpdate
		r.Patch(0x005D70D6, L"31 C0 C2 0C 00");
		// EHSvc Loader 2
		r.Patch(0x005D4A98, L"31 C0 C2 18 00");
		// EHSvc Loader 1
		r.Patch(0x005D4D27, L"31 C0 C3");
		// HS Init
		r.Patch(0x005749CE, L"B8 01 00 00 00 C3");
		// Window Mode
		r.Patch(0x005915D6, L"31 C0 40");
		return true;
	}
	case TENVI_CN: {
		// HSUpdate
		r.Patch(0x005EB9CF, L"31 C0 C2 0C 00");
		// EHSvc Loader 2
		r.Patch(0x005E93B8, L"31 C0 C2 18 00");
		// EHSvc Loader 1
		r.Patch(0x005E9647, L"31 C0 C3");
		// HS Init
		r.Patch(0x00580ABE, L"B8 01 00 00 00 C2 04 00");
		// Window Mode
		r.Patch(0x005A3F9A, L"31 C0 40");
		// Disable Launcher
		r.Patch(0x004A122C, L"B8 01 00 00 00 C3");
		// SeData.dll
		r.Patch(0x005851C0, L"B8 01 00 00 00 C3");
		return true;
	}
	case TENVI_HK: {
		// GG Init 1
		r.Patch(0x0068CA3F, L"90 90 90 90 90");
		// GG Init 2
		r.JMP(0x004815AC, 0x004815C8);
		// GG HeartBeat
		r.JMP(0x004724C1, 0x004724F3);
		// Ignore Launcher
		r.JMP(0x004815EC, 0x00481603);
		return true;
	}
	case TENVI_KR: {
		// GG Init 1
		r.Patch(0x00630A3E, L"90 90 90 90 90");
		// GG Init 2
		r.JMP(0x0049313B, 0x004931A0);
		// GG HeartBeat
		r.JMP(0x00483F28, 0x00483F5A);
		// Ignore Launcher
		r.JMP(0x004931C4, 0x004931DB);
		// Login Error Bypass
		r.Patch(0x0048473C, L"90 90 90 90 90");
		return true;
	}
	default: {
		break;
	}
	}
	return false;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hinstDLL);
		EmuMainTenvi(hinstDLL);
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		break;
	}
	default:
	{
		break;
	}
	}
	return TRUE;
}