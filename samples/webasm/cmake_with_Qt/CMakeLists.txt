cmake_minimum_required(VERSION 2.8)

project(cmake_sample CXX)
set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_BUILD_TYPE Debug)

add_executable(sample main.cpp)

# if (EMSCRIPTEN)
#     set(CMAKE_EXECUTABLE_SUFFIX ".html") # デフォルトでは.js
# endif()
