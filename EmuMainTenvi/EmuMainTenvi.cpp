#include"../Share/Simple/Simple.h"
#include"../Share/Hook/SimpleHook.h"
#include"../EmuMainTenvi/ConfigTenvi.h"

bool EmuMainTenvi() {
	Rosemary r;

	switch (GetRegion()) {
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
	case TENVI_KRX: {
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
	case TENVI_KR: {
		// GG Init 1
		r.Patch(0x00663C2D, L"90 90 90 90 90");
		// GG Init 2
		// 81 7D ?? ?? ?? ?? ?? 74 ?? FF 75 ?? E8
		r.Patch(0x0047EC60, L"EB");
		// GG HeartBeat
		r.JMP(0x00471E38, 0x00471E69);
		// Ignore Launcher
		r.Patch(0x0047ECAB, L"EB");
		// Login Error Bypass
		r.Patch(0x00472515, L"90 90 90 90 90");
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
		LoadRegionConfig(hinstDLL);
		EmuMainTenvi();
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