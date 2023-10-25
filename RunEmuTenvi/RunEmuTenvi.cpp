#include"../Share/Simple/Simple.h"

#define EXE_NAME L"RunEmuTenvi"
#define DLL_NAME L"EmuLoaderTenvi"

bool RunEmu(std::wstring wDroppedTarget) {
	Config conf(EXE_NAME".ini");
	std::wstring wTarget, wCmdLine, wLoaderDLL, wMainDLL;
	bool check = true;

	if (wDroppedTarget.length()) {
		conf.Update(EXE_NAME, L"TargetEXE", wDroppedTarget);

		std::wstring wDeafultLoaderDLL = DLL_NAME".dll";
		conf.Update(EXE_NAME, L"LoaderDLL", wDeafultLoaderDLL);
		conf.Update(EXE_NAME, L"Region", L"JP");
	}

	check &= conf.Read(EXE_NAME, L"TargetEXE", wTarget);
	conf.Read(EXE_NAME, L"CmdLine", wCmdLine);
	check &= conf.Read(EXE_NAME, L"LoaderDLL", wLoaderDLL);

	if (!check) {
		return false;
	}

	std::wstring dir;
	if (!GetDir(dir)) {
		return false;
	}

	Injector injector(wTarget, dir + wLoaderDLL);
	return injector.Run(wCmdLine);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	if (__argc < 2) {
		if (!RunEmu(L"")) {
			MessageBoxW(NULL, L"Please check target exe file path", EXE_NAME, MB_OK);
		}
	}
	else {
		if (!RunEmu(__wargv[1])) {
			MessageBoxW(NULL, L"Please drop target exe file", EXE_NAME, MB_OK);
		}
	}

	return 0;
}