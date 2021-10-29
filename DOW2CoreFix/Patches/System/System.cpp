#include "System.h"
#include <Windows.h>
#include <detours.h>

namespace
{
	static void (WINAPI* RealGetSystemInfo)(LPSYSTEM_INFO info) = GetSystemInfo;

	void WINAPI GetSystemInfoDetour(LPSYSTEM_INFO info)
	{
		RealGetSystemInfo(info);

		if (info->dwNumberOfProcessors > 31)
			info->dwNumberOfProcessors = 31;
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

