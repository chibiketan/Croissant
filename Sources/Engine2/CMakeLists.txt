set(PROJECT_NAME ENGINE)
get_filename_component(CROISSANT_ENGINE_ROOT_DIR "." ABSOLUTE)

set(ENGINE_SOURCE "${CROISSANT_ENGINE_ROOT_DIR}/Source")
set(ENGINE_INCLUDE "${CROISSANT_ENGINE_ROOT_DIR}/Include")
set(ENGINE_DEFINE_FILE "defines.hpp")

#file(WRITE ${ENGINE_INCLUDE}/${ENGINE_DEFINE_FILE} "")

execute_process(
        COMMAND ${PYTHON_EXECUTABLE} ${CROISSANT_ROOT_DIR}/python/create_header.py ${ENGINE_INCLUDE}/${ENGINE_DEFINE_FILE} ${CROISSANT_PLATFORM_DEFINES}
)

#add_custom_command(
#        PRE_BUILD
#        TARGET ${PROJECT_NAME}
#        #        OUTPUT ${ENGINE_INCLUDE}/defines.h
#        COMMAND ${PYTHON_EXECUTABLE} ${CROISSANT_ROOT_DIR}/python/create_header.py ${ENGINE_INCLUDE}/${ENGINE_DEFINE_FILE} ${CROISSANT_PLATFORM_DEFINES}
#        #        DEPENDS wordlist.txt
#)



croissant_files(${PROJECT_NAME} ${ENGINE_SOURCE}
	Core/AbstractBuffer.cpp
	Core/Application.cpp
	Core/IndexBuffer.cpp
	Core/LogManager.cpp
	Core/Mesh.cpp
	Core/Node.cpp
	Core/VertexBuffer.cpp
	Debug/MemoryManager.cpp
	Exception/CroissantException.cpp
	FileSystem/Directory.cpp
	FileSystem/File.cpp
	Graphic/Camera.cpp
	Graphic/OpenGLBuffer.cpp
	Graphic/OpenGLRenderer.cpp
	Graphic/OpenGLShaderLoader.cpp
	Graphic/OpenGLSystemInfo.cpp
	Graphic/OpenGLWrapper.cpp
	Graphic/Window.cpp
	Graphic/WindowEvent.cpp
	Graphic/WindowEventClose.cpp
	Graphic/WindowEventKey.cpp
	Graphic/WindowEventNone.cpp
	Graphic/WindowMouseMoveEvent.cpp
    Math/EulerAngle.cpp
    Math/Matrix4.cpp
    Math/Point2.cpp
    Math/Point4.cpp
	Math/Quaternion.cpp
    Math/Vector4.cpp
	Module/ServiceProvider.cpp
	Module/ServiceProviderInternal.cpp
	Threading/AutoResetEvent.cpp
	Threading/Thread.cpp
)

croissant_files(${PROJECT_NAME} ${ENGINE_INCLUDE}
    ${ENGINE_DEFINE_FILE}
    Engine.hpp
	Core/AbstractBuffer.hpp
	Core/Application.hpp
	Core/Application.inl
	Core/BufferAccessor.hpp
	Core/BufferAccessor.inl
	Core/Enums.hpp
	Core/IndexBuffer.hpp
	Core/LogManager.hpp
	Core/Mesh.hpp
	Core/Node.hpp
	Core/VertexBuffer.hpp
	Core/VertexBufferDescriptor.hpp
	Core/VertexBufferDescriptor.inl
	Debug/MemoryManager.hpp
	Exception/CroissantException.hpp
	FileSystem/Directory.hpp
	FileSystem/File.hpp
	Graphic/Camera.hpp
	Graphic/Enums.hpp
	Graphic/OpenGLBuffer.hpp
	Graphic/OpenGLRenderer.hpp
	Graphic/OpenGLShaderLoader.hpp
	Graphic/OpenGLSystemInfo.hpp
	Graphic/OpenGLWrapper.hpp
	Graphic/Window.hpp
	Graphic/WindowEvent.hpp
	Graphic/WindowEventClose.hpp
	Graphic/WindowEventKey.hpp
	Graphic/WindowEventNone.hpp
	Graphic/WindowMouseMoveEvent.hpp
	Math/EulerAngle.hpp
	Math/Math.hpp
	Math/Math.inl
	Math/Matrix.hpp
	Math/Matrix4.hpp
	Math/Point2.hpp
	Math/Point4.hpp
	Math/Vector4.hpp
	Math/Quaternion.hpp
	Math/Tuple.hpp
	Module/ServiceProvider.hpp
	Module/ServiceProviderInternal.hpp
	Threading/AutoResetEvent.hpp
	Threading/Thread.hpp
)

if (CROISSANT_HOST_LINUX EQUAL 1)
#    croissant_files(${PROJECT_NAME} ${ENGINE_INCLUDE}
#            Graphic/WindowInternal.Linux.hpp
#            )

#    croissant_files(${PROJECT_NAME} ${ENGINE_SOURCE}
#            Graphic/WindowInternal.Linux.cpp
#            )
elseif(CROISSANT_HOST_WINDOWS EQUAL 1)
    croissant_files(${PROJECT_NAME} ${ENGINE_SOURCE}
            Graphic/WindowInternal.Windows.inl
        )

endif()

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

if(CROISSANT_HOST_WINDOWS EQUAL 1)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC opengl32 glu32 gdi32
    )
elseif(CROISSANT_HOST_LINUX EQUAL 1)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC GL X11
    )
endif()

#target_compile_features(${PROJECT_NAME}
#	PUBLIC
#		cxx_defaulted_functions
#		cxx_delegating_constructors
#		cxx_deleted_functions
#		cxx_final
#		cxx_lambdas
#		cxx_nullptr
#		cxx_override
#		cxx_rvalue_references
#)



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
