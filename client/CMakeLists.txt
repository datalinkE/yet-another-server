cmake_minimum_required(VERSION 2.8)

option(USE_CLANG "build application with clang" OFF) # OFF is the default

if(USE_CLANG)
	SET (CMAKE_C_COMPILER              "/usr/bin/clang")
	SET (CMAKE_CXX_COMPILER            "/usr/bin/clang++")

	SET (CMAKE_C_FLAGS_INIT                "-Wall -std=c99")
	SET (CMAKE_C_FLAGS_DEBUG_INIT          "-g")
	SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
	SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
	SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

	SET (CMAKE_CXX_FLAGS_INIT                "-Wall")
	SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
	SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
	SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")
endif(USE_CLANG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_definitions(-std=c++11)

project(yas-client)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

