cmake_minimum_required( VERSION 3.6 )

project(meshlab_sample CXX)
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD 14) # vcglib can not support over c++17

# set(CMAKE_BUILD_TYPE Debug)

add_executable(
    sample main.cpp
    # quadric_simp.h
    # quadric_simp.cpp
)

if (EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "--bind")
else()
    set( EM_DIR C:/Users/Public/Documents/GitHub/emsdk/upstream/emscripten/system/include)
    include_directories(${EM_DIR})
endif()

# set(CMAKE_CXX_FLAGS_RELEASE "/O2 -DQT_NO_DEBUG_OUTPUT /DNDEBUG") # disable qDebug output, disable assert

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
string(REPLACE "/W3" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(TO_CMAKE_PATH $ENV{EIGEN3_INCLUDE_DIR} EIGEN3_INCLUDE_DIR)
include_directories("${EIGEN3_INCLUDE_DIR}")

# ADD_DEFINITIONS(-DQT_NO_KEYWORDS) # Qt signal/slot setting
# find_package(pybind11 REQUIRED)

message("---vcglib setting---")
file(TO_CMAKE_PATH $ENV{VCGLIB_DIR} VCGLIB_DIR)
message(${VCGLIB_DIR})
include_directories(${VCGLIB_DIR})

set( MESHLAB_DIR ${CMAKE_CURRENT_SOURCE_DIR})
message(${MESHLAB_DIR})
include_directories(${MESHLAB_DIR})

# Additional Dependencies
add_definitions(-DMESHLAB_SCALAR=float)

