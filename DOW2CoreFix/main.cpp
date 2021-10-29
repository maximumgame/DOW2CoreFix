#include <Windows.h>
#include <detours.h>

#include "Patches/Patches.h"

static void (WINAPI* RealGetStartupInfo)(LPSTARTUPINFOA info) = GetStartupInfoA;
static bool hasAttached = false;


void WINAPI GetStartupInfoDetour(LPSTARTUPINFOA info)
{
	RealGetStartupInfo(info);

	if (!hasAttached)
	{
		hasAttached = true;
		Patches::Apply();
	}
}

bool Init(HINSTANCE hModule)
{
	DisableThreadLibraryCalls(hModule);

	//Get us an entry outside of loader lock
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)RealGetStartupInfo, GetStartupInfoDetour);
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