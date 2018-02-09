set(GTEST_ROOT "${CMAKE_BINARY_DIR}/gtest")

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${GTEST_ROOT}/googletest-download
	)
if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${GTEST_ROOT}/googletest-download
	)
if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Preventing overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build, defines gtest and
# gtest_main targets.
add_subdirectory(${GTEST_ROOT}/googletest-src
	${GTEST_ROOT}/googletest-build
	EXCLUDE_FROM_ALL
	)
