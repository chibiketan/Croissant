project(TestVulkan)
cmake_minimum_required(VERSION 2.8.12)
#add_definitions("-std=c++11")

find_package(VULKAN REQUIRED)

include(
    ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake
)
conan_basic_setup()

add_executable(vulkan main.cpp)
target_include_directories(vulkan PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(vulkan ${CONAN_LIBS} ${Vulkan_LIBRARY})