// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.hpp"

#if defined(_WIN32) || defined(_WIN64)

BOOL APIENTRY DllMain( HMODULE,
                       DWORD  ul_reason_for_call,
                       LPVOID
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif

