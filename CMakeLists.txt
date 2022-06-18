cmake_minimum_required(VERSION 3.1)

# cmake -G"Eclipse CDT4 - MinGW Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER_ARG1=-std=gnu++11 ../certi_src

# set the croissant root directory first!
get_filename_component(CROISSANT_ROOT_DIR "." ABSOLUTE)

message("compilation root directory : ${CROISSANT_ROOT_DIR}")

find_package(PythonInterp REQUIRED)

#-------------------------------------------------------------------------------
#   define top-level options for the whole project
#-------------------------------------------------------------------------------
# CROISSANT_TRACE_LEVEL
# 0 Error
# 1 Warning
# 2 Info
# 3 Debug
option(CROISSANT_USE_THREAD "Enable thread support into croissant" ON)
option(CROISSANT_USE_TRACE "Enable trace support into croissant" ON)
#option(CROISSANT_TRACE_LEVEL "Trace level in Croissant" 3)
option(CROISSANT_MEMORY_DEBUG "Enable memory debug implementation into croissant" OFF)
option(CROISSANT_VULKAN_VALIDATION_LAYER "Enable validation layer into vulkan into croissant" ON)
option(CROISSANT_OPENGL_ERROR_THROW "Enable OpenGL call to throw when glGetError return error code" ON)
option(CROISSANT_SHARED_LIB "Enable compile croissant into shared lib" ON)

# set the build type to use
if (NOT CROISSANT_TRACE_LEVEL)
	set(CROISSANT_TRACE_LEVEL 3 CACHE STRING "Trace level in Croissant" FORCE)
endif()
set_property(CACHE CROISSANT_TRACE_LEVEL PROPERTY STRINGS 0 1 2 3)


if (CROISSANT_SHARED_LIB)
	set( BUILD_SHARED_LIBS ON CACHE BOOL "Always build shared libs" )
	set( gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" )
endif()



#-------------------------------------------------------------------------------
#   croissant_apply_target_group(target)
#   Apply IDE group name to target.
#
macro(croissant_apply_target_group target)
    if (NOT ${ORYOL_TARGET_GROUP} STREQUAL "")
        set_target_properties(${target} PROPERTIES FOLDER ${ORYOL_TARGET_GROUP})
    endif()
endmacro()

#-------------------------------------------------------------------------------
#   croissant_files(dirs ...)
#   Parse one or more directories for sources and add them to the current
#   target.
#
macro(croissant_files prj baseDir)
    foreach (fil ${ARGN})
        # determine group folder name
		set(absFile "${baseDir}/${fil}")
		get_filename_component(dir ${fil} DIRECTORY)
		if ("${dir}" STREQUAL "")
        # if (${slashIndex} EQUAL -1)
            source_group("" FILES ${absFile})
        else()
			string(REPLACE / \\ groupName ${dir})
			string(FIND ${groupName} \\ slashIndex)
            source_group(${groupName} FILES ${absFile})
        endif()

        # add to global tracker variables
        list(APPEND ${prj}_Sources ${absFile})

    endforeach()

	# remove duplicate sources
	if (${prj}_Sources)
		list(REMOVE_DUPLICATES ${prj}_Sources)
	endif()

endmacro()


#-------------------------------------------------------------------------------
#   croissant_setup_link_directories()
#   Setup the link library search paths.
#
macro(croissant_setup_link_directories)
    # link_directories(${ORYOL_ROOT_DIR}/lib/${ORYOL_PLATFORM_NAME})
endmacro()

#-------------------------------------------------------------------------------
#   croissant_setup()
#   Performs one-time initialization of the build system. Must be called
#   at the start of the root CMakeLists.txt file.
#
macro(croissant_setup)
	# define configuration types
	set(CMAKE_CONFIGURATION_TYPES Debug Release)

	# set the build type to use
	if (NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Compile Type" FORCE)
	endif()
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
	endif()

	# ---------------------------
	#	Vérification de l'architecture par défaut et de l'endianness
	#	Défini les valeurs suivantes :
	#		HAVE_64_BIT
	#		HAVE_BIG_ENDIAN
	# ---------------------------
	if( NOT APPLE )
	   # check 64 bit
	   if( CMAKE_SIZEOF_VOID_P EQUAL 4 )
		 set( HAVE_64_BIT 0 )
	   else()
		 set( HAVE_64_BIT 1 )
	   endif()

	   # check endianness
	   include( TestBigEndian )
	   test_big_endian( HAVE_BIG_ENDIAN )
	   if( HAVE_BIG_ENDIAN )
		 set( HAVE_BIG_ENDIAN 1 )
	   else( HAVE_BIG_ENDIAN )
		 set( HAVE_BIG_ENDIAN 0 )
	   endif( HAVE_BIG_ENDIAN )
	endif( NOT APPLE )

    if( UNIX )
    	if (HAVE_64_BIT EQUAL 1)
    		message("Target Platform: Unix 64-bit")
    		set(CROISSANT_PLATFORM Linux)
    		set(CROISSANT_PLATFORM_NAME "linux64")
    		set(CROISSANT_LINUX64 1)
    		set(CROISSANT_LINUX 1)
    		list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_LINUX -DCROISSANT_X64)
    		set(CROISSANT_LINUX_PLATFORM_NAME "x64")
    	else()
    		message("Target Platform: Unix 32-bit")
    		set(CROISSANT_PLATFORM Linux)
    		set(CROISSANT_PLATFORM_NAME "linux32")
    		set(CROISSANT_LINUX32 1)
    		set(CROISSANT_LINUX 1)
    		list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_LINUX -DCROISSANT_X32)
    		set(CROISSANT_LINUX_PLATFORM_NAME "x86")
    	endif()
    elseif( APPLE )
    else()
    	# ---------------------------
    	#	Paramétrage de variables suivant si nous sommes sur un windows x86 ou x64
    	#	Défini les valeurs suivantes :
    	#		CROISSANT_PLATFORM
    	#		CROISSANT_PLATFORM_NAME
    	#		CROISSANT_WIN64
    	#		CROISSANT_WIN32
    	#		CROISSANT_WINDOWS
    	#		CROISSANT_PLATFORM_DEFINES
    	#		CROISSANT_WINDOWS_PLATFORM_NAME
    	# ---------------------------
    	if (HAVE_64_BIT EQUAL 1)
    		message("Target Platform: Windows 64-bit")
    		set(CROISSANT_PLATFORM WIN64)
    		set(CROISSANT_PLATFORM_NAME "win64")
    		set(CROISSANT_WIN64 1)
    		set(CROISSANT_WINDOWS 1)
    		list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_WINDOWS -DCROISSANT_X64)
    		set(CROISSANT_WINDOWS_PLATFORM_NAME "x64")
    	else()
    		message("Target Platform: Windows 32-bit")
    		set(CROISSANT_PLATFORM WIN32)
    		set(CROISSANT_PLATFORM_NAME "win32")
    		set(CROISSANT_WIN32 1)
    		set(CROISSANT_WINDOWS 1)
    		list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_WINDOWS -DCROISSANT_X32)
    		set(CROISSANT_WINDOWS_PLATFORM_NAME "x86")
    	endif()
    endif( UNIX )

	# ---------------------------
	#	Détecter le type de compilateur et définir un #define indiquant le compilateur
	# ---------------------------
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	  # using Clang
	  list(APPEND CROISSANT_PLATFORM_DEFINES -DCOMPILER_CLANG)
	  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	  endif()
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	  # using GCC
	  list(APPEND CROISSANT_PLATFORM_DEFINES -DCOMPILER_GCC)
	  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	  endif()
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	  # using Intel C++
	  list(APPEND CROISSANT_PLATFORM_DEFINES -DCOMPILER_INTEL)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	  # using Visual Studio C++
	  list(APPEND CROISSANT_PLATFORM_DEFINES -DCOMPILER_MSVC)
	  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()

# =========================================================================================================================

    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

    if (CROISSANT_ROOT_DIR)
        message("CROISSANT_ROOT_DIR: ${CROISSANT_ROOT_DIR}")
    else()
        message(FATAL_ERROR "Must specify absolute CROISSANT_ROOT_DIR before calling croissant_setup()!")
    endif()

    # set host system variables
    set (CROISSANT_HOST_WINDOWS 0)
    set (CROISSANT_HOST_LINUX 0)
    if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
        set(CROISSANT_HOST_WINDOWS 1)
        message("Croissant host system: Windows")
    elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
        set(CROISSANT_HOST_LINUX 1)
        message("Croissant host system: Linux")
	else()
        message(FATAL_ERROR "Unknown host system.")
    endif()

    # # manually include "toolchain" files for non-crosscompiling scenarios
    # if (NOT CMAKE_TOOLCHAIN_FILE)
        # if (WIN32)
            # include(${ORYOL_ROOT_DIR}/cmake/windows.cmake)
        # elseif (APPLE)
            # include(${ORYOL_ROOT_DIR}/cmake/osx.cmake)
        # else()
            # include(${ORYOL_ROOT_DIR}/cmake/linux.cmake)
        # endif()
    # endif()
    message("CROISSANT_PLATFORM: " ${CROISSANT_PLATFORM})

    # setup standard link directories
    croissant_setup_link_directories()

    # setup the target group variable, used to group targets into folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(TARGET_GROUP "")

    # various global defines
    if (CROISSANT_MEMORY_DEBUG)
		list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_MEMORY_DEBUG)
    else()
        # add_definitions(-DORYOL_ALLOCATOR_DEBUG=0)
    endif()

	if (CROISSANT_VULKAN_VALIDATION_LAYER)
		list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_VULKAN_VALIDATION_LAYER)
	else()
		# add_definitions(-DORYOL_ALLOCATOR_DEBUG=0)
	endif()

	if (CROISSANT_OPENGL_ERROR_THROW)
    	list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_OPENGL_ERROR_THROW)
	endif()

	# ---------------------------
	#	Définir les éléments pour les traces
	# ---------------------------
	if (${CROISSANT_USE_TRACE})
		list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_HAS_TRACE -DCROISSANT_TRACE_LEVEL=${CROISSANT_TRACE_LEVEL})
	endif()



    if (CROISSANT_USE_THREAD)
		# TODO test thread capabilities
		if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
			#list(APPEND CMAKE_REQUIRED_FLAGS "-std=c++14")
			#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
		endif()
		
		include(CheckCXXSourceCompiles)
		message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
		check_cxx_source_compiles(
			"#include <thread>
				int main() { std::thread t; return 0; }"
			CXX11_THREAD_OK)

		if (CXX11_THREAD_OK)
			list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_HAS_THREAD -DCROISSANT_THREAD_STD)
			message(STATUS "Detection de std::thread : OK")
		else()
			message(STATUS "Detection de std::thread : KO")
		endif()

    # else()
        # add_definitions(-DORYOL_FORCE_NO_THREADS=0)
    endif()
    # if (ORYOL_EMSCRIPTEN OR ORYOL_PNACL)
        # add_definitions(-DORYOL_SAMPLE_URL=\"http://localhost/\")
    # else()
        # add_definitions(-DORYOL_SAMPLE_URL=\"${ORYOL_SAMPLE_URL}\")
    # endif()

    # GLM (math lib) definitions
    # add_definitions(-DGLM_FORCE_RADIANS=1)

    # check whether python is installed
    # find_program(PYTHON "python")
    # if (PYTHON)
        # message("PYTHON INTERPRETER FOUND")
    # else()
        # message("PYTHON INTERPRETER NOT FOUND, NO SOURCE CODE GENERATION!")
    # endif()

    # clear the web samples config file
    # oryol_begin_web_samples()

	if (CROISSANT_SHARED_LIB)
		list(APPEND CROISSANT_PLATFORM_DEFINES -DCROISSANT_SHARED_LIB)
	endif()

# defines are now writen to defines.hpp
#	foreach(def ${CROISSANT_PLATFORM_DEFINES})
#		message(STATUS "def : ${def}")
#		add_definitions(${def})
#	endforeach()
	# add_definitions(${CROISSANT_PLATFORM_DEFINES})
endmacro()




#-------------------------------------------------------------------------------------------------------------------------------
#   Content of CMAKE file
#-------------------------------------------------------------------------------------------------------------------------------
project(CROISSANT C CXX)

croissant_setup()

#if (MSVC)
#	# For MSVC, CMake sets certain flags to defaults we want to override.
#	# This replacement code is taken from sample in the CMake Wiki at
#	# http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
#	foreach (flag_var
#			 CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
#			 CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
#		if (NOT CROISSANT_SHARED_LIB)
#			# When Google Test is built as a shared library, it should also use
#			# shared runtime libraries.  Otherwise, it may end up with multiple
#			# copies of runtime library data in different modules, resulting in
#			# hard-to-find crashes. When it is built as a static library, it is
#			# preferable to use CRT as static libraries, as we don't have to rely
#			# on CRT DLLs being available. CMake always defaults to using shared
#			# CRT libraries, so we override that default here.
#			string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
#		endif()
#	endforeach()
#endif()




message(STATUS "CMAKE_CXX_FLAGS : ${CMAKE_CXX_FLAGS}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
set(ENGINE_ROOT_DIR ${CROISSANT_ROOT_DIR}/Sources/Engine2)


add_subdirectory(${CROISSANT_ROOT_DIR}/Sources/3rdparty/gtest)

enable_testing()

add_subdirectory(${CROISSANT_ROOT_DIR}/Sources/Engine2)

add_subdirectory(${CROISSANT_ROOT_DIR}/Sources/GetConfig)

add_subdirectory(${CROISSANT_ROOT_DIR}/Sources/UnitTestGTest)

add_subdirectory(${CROISSANT_ROOT_DIR}/vulkan)

find_package(DevIL REQUIRED)

include_directories(${ENGINE_ROOT_DIR}/../DBlock//Include)
include_directories(${IL_INCLUDE_DIR})

add_executable(DBlock
	${ENGINE_ROOT_DIR}/../Dblock/Source/test.cpp
)

target_link_libraries(DBlock
	PUBLIC ENGINE
	${IL_LIBRARIES}
)


set_target_properties(DBlock PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CROISSANT_ROOT_DIR}/Bin)

# https://cmake.org/cmake/help/v3.6/command/add_custom_command.html?highlight=add_custom_command
# https://cmake.org/cmake/help/v3.2/manual/cmake.1.html#command-line-tool-mode
add_custom_command(TARGET DBlock POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DBlock>/Resources"
				   COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DBlock>/Resources/Images"
                   COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DBlock>/Resources/Shaders"
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ENGINE_ROOT_DIR}/../Dblock/Ressources/test.png" "$<TARGET_FILE_DIR:DBlock>/Resources/Images/test.png"
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ENGINE_ROOT_DIR}/../Dblock/Ressources/Shaders/main.vert" "$<TARGET_FILE_DIR:DBlock>/Resources/Shaders/main.vert"
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ENGINE_ROOT_DIR}/../Dblock/Ressources/Shaders/main.frag" "$<TARGET_FILE_DIR:DBlock>/Resources/Shaders/main.frag"
)

#				   add_custom_command(TARGET MyTest POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${PROJECT_SOURCE_DIR}/libs"
#        $<TARGET_FILE_DIR:MyTest>)