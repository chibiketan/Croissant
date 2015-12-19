set(PROJECT_NAME ENGINE)
get_filename_component(CROISSANT_ENGINE_ROOT_DIR "." ABSOLUTE)

set(ENGINE_SOURCE "${CROISSANT_ENGINE_ROOT_DIR}/Source")
set(ENGINE_INCLUDE "${CROISSANT_ENGINE_ROOT_DIR}/Include")

croissant_files(${PROJECT_NAME} ${ENGINE_SOURCE}
	Core/Application.cpp
	Core/LogManager.cpp
	Debug/MemoryManager.cpp
	Exception/CroissantException.cpp
	FileSystem/Directory.cpp
	FileSystem/File.cpp
	Graphic/OpenGLRenderer.cpp
	Graphic/OpenGLSystemInfo.cpp
	Graphic/OpenGLWrapper.cpp
	Graphic/Window.cpp
	Graphic/WindowEvent.cpp
	Graphic/WindowEventClose.cpp
	Graphic/WindowEventNone.cpp
    Math/Matrix4.cpp
    Math/Vector4.cpp
    Math/Point4.cpp
	Module/ServiceProvider.cpp
	Module/ServiceProviderInternal.cpp
	Threading/AutoResetEvent.cpp
	Threading/Thread.cpp
)

croissant_files(${PROJECT_NAME} ${ENGINE_INCLUDE}
	Engine.hpp
	Core/Application.hpp
	Core/LogManager.hpp
	Debug/MemoryManager.hpp
	Exception/CroissantException.hpp
	FileSystem/Directory.hpp
	FileSystem/File.hpp
	Graphic/OpenGLRenderer.hpp
	Graphic/OpenGLSystemInfo.hpp
	Graphic/OpenGLWrapper.hpp
	Graphic/Window.hpp
	Graphic/WindowEvent.hpp
	Graphic/WindowEventClose.hpp
	Graphic/WindowEventNone.hpp
	Math/Matrix.hpp
    Math/Matrix4.hpp
    Math/Vector4.hpp
    Math/Point4.hpp
	Module/ServiceProvider.hpp
	Module/ServiceProviderInternal.hpp
	Threading/AutoResetEvent.hpp
	Threading/Thread.hpp
)

# Now build our tools

if (${CROISSANT_SHARED_LIB})
	message("Building ${PROJECT_NAME} as shared library")
	add_library(${PROJECT_NAME} SHARED 
		${${PROJECT_NAME}_Sources}
	)
else()
	message("Building ${PROJECT_NAME} as static library")
	add_library(${PROJECT_NAME} 
		${${PROJECT_NAME}_Sources}
    )
endif()

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CROISSANT_ENGINE_ROOT_DIR}/Include
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC opengl32 glu32 gdi32
)

target_compile_features(${PROJECT_NAME}
	PUBLIC
		cxx_defaulted_functions
		cxx_delegating_constructors
		cxx_deleted_functions
		cxx_final
		cxx_lambdas
		cxx_nullptr
		cxx_override
		cxx_rvalue_references
)



set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CROISSANT_ROOT_DIR}/Bin)

# cmake_minimum_required(VERSION 3.1)
# project(ENGINE)


# # Set your project compile flags.
# # E.g. if using the C++ header files
# # you will need to enable C++11 support
# # for your compiler.

# # le dernier includ eest TREEEES moche
# #include_directories(${LLVM_INCLUDE_DIRS} Include D:/MinGW/include/c++/4.9.1  D:/MinGW/include/c++/4.9.1/x86_64-w64-mingw32 D:/MinGW/x86_64-w64-mingw32/include)
# include_directories(Include)
# #add_definitions(${LLVM_DEFINITIONS})

# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # # using Clang
  # add_definitions(-DCOMPILER_CLANG)
# elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # # using GCC
  # add_definitions(-DCOMPILER_GCC)
# elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # # using Intel C++
  # add_definitions(-DCOMPILER_INTEL)
# elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # # using Visual Studio C++
  # add_definitions(-DCOMPILER_MSVC)
# endif()

# add_definitions(-DCROISSANT_HAS_THREAD -DCROISSANT_THREAD_STD -DCROISSANT_MEMORY_DEBUG)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
# # Now build our tools
# add_library(ENGINE SHARED 
	# Source/Core/Application.cpp
	# Source/Core/LogManager.cpp
	# Source/Debug/MemoryManager.cpp
	# Source/Exception/CroissantException.cpp
	# Source/FileSystem/Directory.cpp
	# Source/FileSystem/File.cpp
	# Source/Graphic/OpenGLRenderer.cpp
	# Source/Graphic/Window.cpp
	# Source/Graphic/WindowEvent.cpp
	# Source/Graphic/WindowEventClose.cpp
	# Source/Graphic/WindowEventNone.cpp
	# Source/Module/ServiceProvider.cpp
	# Source/Module/ServiceProviderInternal.cpp
	# Source/Threading/AutoResetEvent.cpp
	# Source/Threading/Thread.cpp
# )

# # Find the libraries that correspond to the LLVM components
# # that we wish to use
# #llvm_map_components_to_libnames(llvm_libs support core irreader)

# # Link against LLVM libraries
# target_link_libraries(ENGINE
	# PUBLIC pthread
# )
