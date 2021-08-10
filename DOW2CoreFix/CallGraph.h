#pragma once

/*
Exports to match original Profiler.dll
*/

#define EXPORT __declspec(dllexport)

class EXPORT CallGraph
{
public:
	__thiscall CallGraph(const char* a) {};
	__thiscall ~CallGraph() {};

	void __thiscall DumpLog() {};
	void virtual __thiscall Visit() {};
};
