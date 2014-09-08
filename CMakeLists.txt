cmake_minimum_required(VERSION 2.8.11)

project(jenson)
set(JENSON_BINARY_NAME "jenson")
set(TESTS_BINARY_NAME "jenson-tests")


#
# Build flags & compiler settings
#

# Define the Build flags
option(DebugBuild "Build debug binaries." OFF)
option(CCache "Build using ccache." OFF)
option(Tests "Build the tests executable" OFF)

# Set the CMAKE_BUILD_TYPE (Only DEBUG and RELEASE are supported)
if(DebugBuild)
    set(CMAKE_BUILD_TYPE DEBUG)
    message(STATUS "CMAKE_BUILD_TYPE = DEBUG")
else()
    set(CMAKE_BUILD_TYPE RELEASE)
    message(STATUS "CMAKE_BUILD_TYPE = RELEASE")
endif()

# Optionally enable ccache
if(CCache)
    SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

# Set the compilation flags
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -fomit-frame-pointer -finline-functions -ffast-math")


#
# Find and include external dependencies
#

# Qt
find_package(Qt5Core REQUIRED)
set(CMAKE_AUTOMOC ON)

# Boost
find_package(Boost REQUIRED)
if(Boost_FOUND)
  message(STATUS "Including Boost from: ${Boost_INCLUDE_DIRS}")
  include_directories(${Boost_INCLUDE_DIRS})
endif()


#
# Include subdirectories
#

# Include the subdirectories
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src")

add_subdirectory(src)
if(Tests)
    find_package(Qt5Test REQUIRED)
    add_subdirectory(tests)
endif()
