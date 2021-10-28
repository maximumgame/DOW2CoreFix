#include "System.h"
#include <Windows.h>
#include <detours.h>

namespace
{
	static void (WINAPI* RealGetSystemInfo)(LPSYSTEM_INFO info) = GetSystemInfo;

	void WINAPI GetSystemInfoDetour(LPSYSTEM_INFO info)
	{
		RealGetSystemInfo(info);

		//dow2 will hang if greater than 12 cores
		if (info->dwNumberOfProcessors > 12)
			info->dwNumberOfProcessors = 12;
	}
}
namespace Patches::System
{
	void Apply()
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)RealGetSystemInfo, GetSystemInfoDetour);
		DetourTransactionCommit();
	}
}

