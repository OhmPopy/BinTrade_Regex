// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include  <Com/ComCultureInfo.h>
#include  <Com/GetCultureInfo.h>

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		GetCultureInfo::Init();
		ComCultureInfo::CurrentCulture();
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	default: ;
	}
	return TRUE;
}
