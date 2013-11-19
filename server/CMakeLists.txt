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

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)

add_definitions(-std=c++11)

project(yas-server)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set (ProtobufTestsProtoSources
    ../proto/Message.proto
    ../proto/ServerStatus.proto
    ../proto/Echo.proto
)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${ProtobufTestsProtoSources})

aux_source_directory(. SRC_LIST)

set(SRC_LIST
    ./main.cpp
    ./ServerApplication.cpp
    ./ServerApplicationImpl.cpp
    ./ServerApplicationImpl_child.cpp
    ./ServerApplicationImpl_helpers.cpp
    ./StreamLog.cpp
    ./ServerApplication.h
    ./ServerApplicationImpl.h
    ./StreamLog.h)

set(SRC_LIST_MESSAGE
    ./message/MessageProcessorBase.h
    ./message/MessageDispatcher.h
    ./message/MessageDispatcher.cpp
    ./message/MessageProcessorVirtualTemplate.h
    ./message/ServerStatusMessageProcessor.h
    ./message/ServerStatusMessageProcessor.cpp
    ./message/MessageProcessorFactoryMethod.h)


set(SRC_SHARED
    ./message/EchoMessageProcessor.h
    ./message/EchoMessageProcessor.cpp
    ./message/MessageProcessorFactoryMethod.h
    ./message/lib/EchoMessageProcessor.fact.cpp)

add_library(EchoMessageProcessor SHARED ${SRC_SHARED} ${PROTO_HDRS})
add_executable(${PROJECT_NAME} ${SRC_LIST} ${SRC_LIST_MESSAGE} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(yas-server ${PROTOBUF_LIBRARY} dl)
