set(PROJECT_NAME GetConfig)
get_filename_component(CROISSANT_GETCONFIG_ROOT_DIR "." ABSOLUTE)

set(GETCONFIG_SOURCE "${CROISSANT_GETCONFIG_ROOT_DIR}/Source")
set(GETCONFIG_INCLUDE "${CROISSANT_GETCONFIG_ROOT_DIR}/Include")

croissant_files(${PROJECT_NAME} ${GETCONFIG_SOURCE}
	Main.cpp
)

# Put include files here
croissant_files(${PROJECT_NAME} ${GETCONFIG_INCLUDE}
)

# Now build our tools
add_executable(${PROJECT_NAME}
	${${PROJECT_NAME}_Sources}
)

#target_include_directories(${PROJECT_NAME}
#	PUBLIC ${CROISSANT_GETCONFIG_ROOT_DIR}/Include
#)

#
#target_link_libraries(${PROJECT_NAME}
#	PUBLIC pthread opengl32 glu32 gdi32
#)

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

target_link_libraries(${PROJECT_NAME}
	PUBLIC ENGINE
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CROISSANT_ROOT_DIR}/Bin)