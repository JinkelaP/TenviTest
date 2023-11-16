#include"../EmuMainTenvi/ConfigTenvi.h"

#define EXE_NAME L"RunEmuTenvi"

Region Global_Region = TENVI_JP;
std::wstring Global_RegionStr = L"JP";
std::wstring Global_XMLPath;

Region GetRegion() {
	return Global_Region;
}

void SetRegion(Region r) {
	Global_Region = r;
}

std::wstring GetRegionStr() {
	return Global_RegionStr;
}

std::wstring GetXMLPath() {
	return Global_XMLPath;
}

bool LoadRegionConfig(HINSTANCE hinstDLL) {
	Config conf(EXE_NAME".ini", hinstDLL);
	std::wstring wRegion;
	std::wstring wXMLPath;

	if (!conf.Read(EXE_NAME, L"XMLPath", wXMLPath)) {
		wXMLPath = L".\\";
	}

	Global_XMLPath = wXMLPath;

	if (!conf.Read(EXE_NAME, L"Region", wRegion) || wRegion.length() == 0) {
		DEBUG(L"LoadRegionConfig - JP (Default)");
		SetRegion(TENVI_JP);
		return false;
	}

	if (wRegion.compare(L"CN") == 0) {
		DEBUG(L"LoadRegionConfig - CN");
		SetRegion(TENVI_CN);
		Global_RegionStr = wRegion;
		return true;
	}

	if (wRegion.compare(L"HK") == 0) {
		DEBUG(L"LoadRegionConfig - HK");
		SetRegion(TENVI_HK);
		Global_RegionStr = wRegion;
		return true;
	}

	if (wRegion.compare(L"KR") == 0) {
		DEBUG(L"LoadRegionConfig - KR");
		SetRegion(TENVI_KR);
		Global_RegionStr = wRegion;
		return true;
	}

	if (wRegion.compare(L"KRX") == 0) {
		DEBUG(L"LoadRegionConfig - KRX");
		SetRegion(TENVI_KRX);
		Global_RegionStr = wRegion;
		return true;
	}

	DEBUG(L"LoadRegionConfig - JP");
	SetRegion(TENVI_JP);
	return true;
}