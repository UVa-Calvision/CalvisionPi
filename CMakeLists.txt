cmake_minimum_required(VERSION 3.14)

# Set project CalvisionPi
project(CalvisionPi VERSION 1.0.0 LANGUAGES CXX)

# Default CMake options
option(BUILD_SERVER "Build server executable (requires fmt and gpiod)" OFF)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Import CppUtils libraries
include("${CMAKE_PREFIX_PATH}/lib/cmake/CppUtils/CppUtilsPreProcTargets.cmake")
include("${CMAKE_PREFIX_PATH}/lib/cmake/CppUtils/CppUtilsCUtilsTargets.cmake")
include("${CMAKE_PREFIX_PATH}/lib/cmake/CppUtils/CppUtilsIOTargets.cmake")
include("${CMAKE_PREFIX_PATH}/lib/cmake/CppUtils/CppUtilsNetworkingTargets.cmake")


# Glob common headers
file(GLOB COMMON_HEADER_LIST CONFIGURE_DEPENDS "${CalvisionPi_SOURCE_DIR}/common/*.h")
file(GLOB COMMON_SOURCE_LIST CONFIGURE_DEPENDS "${CalvisionPi_SOURCE_DIR}/common/*.cpp")

# Everyone can build the client (just sockets)
add_subdirectory(client)

# Only build server if explicitly requested (libraries might not be available)
if (BUILD_SERVER)
    message("Will generate build files for server executable")
    add_subdirectory(server)
endif()
