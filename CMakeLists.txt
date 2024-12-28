cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(PcntQSign)

# 平台宏定义
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    add_definitions(-D _WIN_PLATFORM_)

    link_libraries(ws2_32 psapi)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    add_definitions(-D _MAC_PLATFORM_)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    add_definitions(-D _LINUX_PLATFORM_)
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "IA64")
    add_definitions(-D _X64_ARCH_)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86")
    add_definitions(-D _X86_ARCH_)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    add_definitions(-D _ARM64_ARCH_)
endif()

file(GLOB SOURCE_FILES "./src/*.cpp")

add_library(PcntQSign SHARED ${SOURCE_FILES})