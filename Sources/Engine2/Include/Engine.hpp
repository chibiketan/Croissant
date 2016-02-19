#ifndef ENGINE_HPP_INC
#define ENGINE_HPP_INC

#include "defines.hpp"

#if defined(COMPILER_MSVC)
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#endif

#if defined(CROISSANT_HAS_TRACE)
#  include <iostream>
#  define TRACE(msg) (std::cout << msg << std::endl)
#else
#  define TRACE(msg)
#endif

#if defined(CROISSANT_LINUX)
#  define ENGINE_API
#  define BOOL int

#elif defined(CROISSANT_WINDOWS)
#  ifdef CROISSANT_SHARED_LIB
#    ifdef ENGINE_EXPORTS
#      define ENGINE_API __declspec(dllexport)
#    else
#      define ENGINE_API __declspec(dllimport)
#    endif
#  else
#    define ENGINE_API
#  endif
#endif

#define UNUSED(a) (void)(a)

#endif
