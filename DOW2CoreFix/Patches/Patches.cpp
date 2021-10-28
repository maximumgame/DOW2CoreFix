#include <Windows.h>
#include "Fmod/Fmod.h"
#include "System/System.h"

namespace Patches
{
	void Apply()
	{
		System::Apply();
		Fmod::Apply();
	}
}