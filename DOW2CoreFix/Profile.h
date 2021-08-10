#pragma once

/*
Exports to match original Profiler.dll
*/

#define EXPORT __declspec(dllexport)

namespace Profile {

	unsigned __int64 EXPORT MeasureCurrentCount()
	{
		return 0;
	}

	unsigned __int64 EXPORT MeasureFrequency()
	{
		return 0;
	}
};