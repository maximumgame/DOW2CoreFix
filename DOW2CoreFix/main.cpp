#include <Windows.h>
#include <detours.h>

#include "Patches/Patches.h"

static void (WINAPI* RealGetStartupInfo)(LPSTARTUPINFOA info) = GetStartupInfoA;

void WINAPI GetStartupInfoDetour(LPSTARTUPINFOA info)
{
	RealGetStartupInfo(info);

	Patches::Apply();

	//this should only be called once
	//but to be safe we remove this hook
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)RealGetStartupInfo, GetStartupInfoDetour);
	DetourTransactionCommit();
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