#include"../Share/Simple/Simple.h"
#include"../Share/Hook/SimpleHook.h"
#include<intrin.h>
#pragma intrinsic(_ReturnAddress)

#define EXE_NAME L"RunEmuTenvi"
#define FAST_LOAD L"FastLoad"
#define DELAY_LOAD L"DelayLoad"
#define MUTEX_TENVI L"TenviMutexClient"

std::vector<std::wstring> vFastLoadDlls;
std::vector<std::wstring> vDelayLoadDlls;

bool FastLoad() {
	for (size_t i = 0; i < vFastLoadDlls.size(); i++) {
		if (LoadLibraryW(vFastLoadDlls[i].c_str())) {
			DEBUG(L"FastLoad:" + vFastLoadDlls[i]);
		}
	}
	return true;
}

bool bAlreadyLoaded = false;
bool DelayLoad() {
	if (bAlreadyLoaded) {
		return false;
	}

	bAlreadyLoaded = true;
	for (size_t i = 0; i < vDelayLoadDlls.size(); i++) {
		if (LoadLibraryW(vDelayLoadDlls[i].c_str())) {
			DEBUG(L"DelayLoad:" + vDelayLoadDlls[i]);
		}
	}

	return true;
}

bool IsTenviMutex(LPCWSTR lpName) {
	if (!lpName) {
		return false;
	}

	if (wcsstr(lpName, MUTEX_TENVI)) {
		return true;
	}

	return false;
}

bool CloseMutex(HANDLE hMutex) {
	HANDLE hDuplicatedMutex = NULL;
	if (DuplicateHandle(GetCurrentProcess(), hMutex, 0, &hDuplicatedMutex, 0, FALSE, DUPLICATE_CLOSE_SOURCE)) {
		CloseHandle(hDuplicatedMutex);
		return true;
	}
	return false;
}

decltype(CreateMutexW) *_CreateMutexW = NULL;
HANDLE WINAPI CreateMutexW_Hook(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCWSTR lpName) {
	HANDLE hRet = _CreateMutexW(lpMutexAttributes, bInitialOwner, lpName);

	if (IsTenviMutex(lpName)) {
		CloseMutex(hRet);
		DEBUG(L"Tenvi Mutex Closed!");
	}

	// Tenvi KR does not have mutex
	if (SimpleHook::IsCallerEXE(_ReturnAddress())) {
		if (!bAlreadyLoaded) {
			DelayLoad();
		}
	}

	return hRet;
}

bool EnableHook() {
	SHook(CreateMutexW);
	return true;
}

bool EmuLoaderTenvi(HMODULE hDll) {
	Config conf(EXE_NAME".ini", hDll);
	std::wstring wLoaderDir;

	if (!GetDir(wLoaderDir, hDll)) {
		return false;
	}

	for (size_t i = 1; i <= 10; i++) {
		std::wstring wDllName;
		conf.Read(FAST_LOAD, L"DLL_" + std::to_wstring(i), wDllName);

		if (wDllName.length()) {
			if (wDllName.find('\\') == std::wstring::npos) {
				vFastLoadDlls.push_back(wLoaderDir + L"\\" + wDllName);
			}
			else {
				vFastLoadDlls.push_back(wDllName);
			}
		}
	}

	for (size_t i = 1; i <= 10; i++) {
		std::wstring wDllName;
		conf.Read(DELAY_LOAD, L"DLL_" + std::to_wstring(i), wDllName);

		if (wDllName.length()) {
			if (wDllName.find('\\') == std::wstring::npos) {
				vDelayLoadDlls.push_back(wLoaderDir + L"\\" + wDllName);
			}
			else {
				vDelayLoadDlls.push_back(wDllName);
			}
		}
	}

	EnableHook();
	FastLoad();
	return true;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hinstDLL);
		EmuLoaderTenvi(hinstDLL);
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