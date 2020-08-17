cmake_minimum_required(VERSION 2.8)
project(embind_test CXX)

set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_BUILD_TYPE Debug)

if (EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "--bind")
else()
    set( EM_DIR C:/Users/Public/Documents/GitHub/emsdk/upstream/emscripten/system/include)
    include_directories(${EM_DIR})
endif()

add_executable(sample main.cpp)
