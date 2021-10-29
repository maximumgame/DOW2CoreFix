#include "Fmod.h"

#include <Windows.h>
#include <detours.h>

#define RETRIBUTION 1

namespace
{
	typedef int(__stdcall* FMOD_System_SetHardwareChannels)(int, int, int, int, int);
	static int(__stdcall* RealFMOD_System_SetHardwareChannels)(int, int, int, int, int) = NULL;

	int __stdcall FMOD_System_SetHardwareChannels_Detour(int a1, int a2, int a3, int a4, int a5)
	{
		auto result = RealFMOD_System_SetHardwareChannels(a1, a2, a3, a4, a5);
		return result;
	}

	typedef int(__stdcall* FMOD_System_SetSoftwareChannels)(int, int);
	static int(__stdcall* RealFMOD_System_SetSoftwareChannels)(int, int) = NULL;

	int __stdcall FMOD_System_SetSoftwareChannels_Detour(int a1, int a2)
	{
		auto result = RealFMOD_System_SetSoftwareChannels(a1, a2);
		return result;
	}

	void HookFmod()
	{
#if RETRIBUTION
		//dow 2 retribution has a different fmod version
		RealFMOD_System_SetHardwareChannels = (FMOD_System_SetHardwareChannels)DetourFindFunction("fmodex.dll", MAKEINTRESOURCE(278));
		RealFMOD_System_SetSoftwareChannels = (FMOD_System_SetSoftwareChannels)DetourFindFunction("fmodex.dll", MAKEINTRESOURCE(317));
#else
		//dow 2 and chaos rising
		RealFMOD_System_SetHardwareChannels = (FMOD_System_SetHardwareChannels)DetourFindFunction("fmodex.dll", MAKEINTRESOURCE(266));
		RealFMOD_System_SetSoftwareChannels = (FMOD_System_SetSoftwareChannels)DetourFindFunction("fmodex.dll", MAKEINTRESOURCE(304));
#endif

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)RealFMOD_System_SetHardwareChannels, FMOD_System_SetHardwareChannels_Detour);
		DetourAttach(&(PVOID&)RealFMOD_System_SetSoftwareChannels, FMOD_System_SetSoftwareChannels_Detour);
		DetourTransactionCommit();
	}
}

namespace Patches::Fmod
{
	void Apply()
	{
		HookFmod();
	}
}
