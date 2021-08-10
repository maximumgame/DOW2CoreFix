#include <Windows.h>
#include <detours.h>

static void (WINAPI* RealGetSystemInfo)(LPSYSTEM_INFO info) = GetSystemInfo;

void WINAPI GetSystemInfoDetour(LPSYSTEM_INFO info)
{
	RealGetSystemInfo(info);

	//dow2 will hang if greater than 12 cores
	if (info->dwNumberOfProcessors > 12)
		info->dwNumberOfProcessors = 12;
}

bool Init(HINSTANCE hModule)
{
	DisableThreadLibraryCalls(hModule);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	//hook GetSystemInfo
	DetourAttach(&(PVOID&)RealGetSystemInfo, GetSystemInfoDetour);
	DetourTransactionCommit();

	return true;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD reason, LPVOID misc)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		return Init(hModule);
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		return true;
	}

	return false;
}