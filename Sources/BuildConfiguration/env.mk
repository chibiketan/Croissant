
ROOT_ENV := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

TARGET ?= DEBUG

ifeq ($(MSYSTEM),MINGW32)
MACHINE := Msys
else ifneq ($(ProgramFiles),)
MACHINE := Windows
else
MACHINE := Linux
endif

# -------------------------------------- variables de compilation
ifeq ($(CXXFLAGS),)
	CXXFLAGS := -msse
else
	CXXFLAGS +=
endif

CXXFLAGS +=  -std=c++11 -Wall -Wextra -Wundef -Wcast-align -Wformat-security -Wformat=2 -Werror-implicit-function-declaration -Wfloat-equal -Wshadow -Wpointer-arith -Wconversion -Wmissing-declarations -Wmissing-noreturn -Wmissing-format-attribute -Wpacked -Wredundant-decls -Winline -Wdouble-promotion -Winit-self -Wcast-qual -Wold-style-cast -Woverloaded-virtual -Wnon-virtual-dtor

include $(ROOT_ENV)/configure.mk

# -------------------------------------- autres variable par MACHINE
ifeq ($(MACHINE),Windows)
SED := sed.exe
MYSHELL := sh.exe
#CXX := x86_64-w64-mingw32-g++
CXX ?= gcc
GREP := grep.exe

APP_EXT := .exe
LIB_EXT := .dll
LIB_PREFIX :=
ARCH := mingw-x64
else ifeq ($(MACHINE),Msys)
SED := sed
MYSHELL := sh
CXX ?= gcc
GREP := grep

APP_EXT := .exe
LIB_EXT := .dll
LIB_PREFIX :=
ARCH := mingw-x64
else
SED := sed
MYSHELL := sh
#CXX ?= g++
GREP := grep

APP_EXT :=
LIB_EXT := .so
LIB_PREFIX := lib
ARCH := Linux
endif

# -------------------------------------- definition de ARCH

ifeq ($(shell $(CXX) -E -P -dD $(CXXFLAGS) $(ROOT_ENV)/spec.cpp | "$(GREP)"  '__x86_64'),)
	ARCH := x86
else
	ARCH := x64
endif


