#include"../EmuMainTenvi/ConfigTenvi.h"

#define EXE_NAME L"RunEmuTenvi"

Region Global_Region = TENVI_JP;


Region GetRegion() {
	return Global_Region;
}

void SetRegion(Region r) {
	Global_Region = r;
}

bool LoadRegionConfig(HINSTANCE hinstDLL) {
	Config conf(EXE_NAME".ini", hinstDLL);
	std::wstring wRegion;

	if (!conf.Read(EXE_NAME, L"Region", wRegion) || wRegion.length() == 0) {
		DEBUG(L"LoadRegionConfig - JP (Default)");
		SetRegion(TENVI_JP);
		return false;
	}

	if (wRegion.compare(L"CN") == 0) {
		DEBUG(L"LoadRegionConfig - CN");
		SetRegion(TENVI_CN);
		return true;
	}

	if (wRegion.compare(L"HK") == 0) {
		DEBUG(L"LoadRegionConfig - HK");
		SetRegion(TENVI_HK);
		return true;
	}

	if (wRegion.compare(L"KR") == 0) {
		DEBUG(L"LoadRegionConfig - KR");
		SetRegion(TENVI_KR);
		return true;
	}

	DEBUG(L"LoadRegionConfig - JP");
	SetRegion(TENVI_JP);
	return true;
}