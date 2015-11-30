#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ENGINE_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ENGINE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#if defined(__linux)
#  define ENGINE_API
#  define BOOL int
#elif defined(_WIN32) || defined(_WIN64)
#  ifdef ENGINE_EXPORTS
#    define ENGINE_API __declspec(dllexport)
//#define ENGINE_API
#  else
#    define ENGINE_API __declspec(dllimport)
//#define ENGINE_API
#  endif
#endif



// This class is exported from the Engine.dll
class ENGINE_API CEngine {
public:
	CEngine(void);
	// TODO: add your methods here.
};

extern ENGINE_API int nEngine;

ENGINE_API int fnEngine(void);
