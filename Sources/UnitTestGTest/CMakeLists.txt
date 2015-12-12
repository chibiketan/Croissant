set(PROJECT_NAME UTGTEST)
get_filename_component(CROISSANT_UTGTEST_ROOT_DIR "." ABSOLUTE)

set(UTGTEST_SOURCE "${CROISSANT_UTGTEST_ROOT_DIR}/Source")
set(UTGTEST_INCLUDE "${CROISSANT_UTGTEST_ROOT_DIR}/Include")

croissant_files(${PROJECT_NAME} ${UTGTEST_SOURCE}
	main.cpp
	Core/String.cpp
	Event/EventManager.cpp
	FileSystem/Directory.cpp
	FileSystem/File.cpp
	Math/Point.cpp
)

croissant_files(${PROJECT_NAME} ${UTGTEST_INCLUDE}
)

# Now build our tools

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_Sources})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CROISSANT_UTGTEST_ROOT_DIR}/Include ${PROJECT_NAME} ${gtest_SOURCE_DIR}/include ${PROJECT_NAME} ${gtest_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
	ENGINE gtest gtest_main
)

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  add_test(all-tests ${PROJECT_NAME})



set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CROISSANT_ROOT_DIR}/Bin)
