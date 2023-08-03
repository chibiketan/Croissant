#ifndef CROISSANT_ENGINE_HPP_INC
#  define CROISSANT_ENGINE_HPP_INC
#  pragma once

#  include "defines.hpp"

#  if defined(COMPILER_MSVC)
// On désactive les warning sur les export
#    pragma warning(disable: 4251)
#    pragma warning(disable: 4275)

#    define WIN32_LEAN_AND_MEAN
#    include <Windows.h>
#  endif

#  if defined(CROISSANT_HAS_TRACE)
#    include <iostream>
#    define TRACE(msg) (std::cout << msg << std::endl)
#  else
#    define TRACE(msg)
#  endif

#  if defined(CROISSANT_LINUX)
#    define ENGINE_API
#    define BOOL int

#  elif defined(CROISSANT_WINDOWS)
#    ifdef CROISSANT_SHARED_LIB
#      ifdef ENGINE_EXPORTS
#        define ENGINE_API __declspec(dllexport)
#      else
#        define ENGINE_API __declspec(dllimport)
#      endif
#    else
#      define ENGINE_API
#    endif
#  endif

#  define UNUSED(a) (void)(a)

#  if defined(CROISSANT_HAS_TRACE)
#    include "Core/LogManager.hpp"
#  endif



// TODO move in a more specific file (include only utility ?)

#  define CLASS_NO_COPY(clazz)      \
public:                             \
    clazz(clazz const&) = delete;   \
    clazz& operator=(clazz const&) = delete;

#  define CLASS_NO_MOVE(clazz)  \
public:                         \
    clazz(clazz&&) = delete;    \
    clazz& operator=(clazz&&) = delete;

#  define CLASS_NO_COPY_MOVE(clazz) \
    CLASS_NO_COPY(clazz)            \
    CLASS_NO_MOVE(clazz)



#endif // !CROISSANT_ENGINE_HPP_INC
