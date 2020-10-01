cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)

set(CMAKE_CXX_STANDARD 11)

# define path of Eigen
file(TO_CMAKE_PATH $ENV{EIGEN3_INCLUDE_DIR} EIGEN3_INCLUDE_DIR)
# define path of vcglib
file(TO_CMAKE_PATH $ENV{VCGLIB_DIR} VCGLIB_DIR)
# define path of meshlab source
file(TO_CMAKE_PATH $ENV{MESHLAB_DIR} MESHLAB_DIR)
add_definitions(-DMESHLAB_SCALAR=float) # you should add this definition if you use meshlab source

file(TO_CMAKE_PATH $ENV{EMSDK_DIR} EMSDK_DIR)

# set output file directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Add Executable
add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${EIGEN3_INCLUDE_DIR}
  ${VCGLIB_DIR}
  ${MESHLAB_DIR}
  ${EMSDK_DIR}
)

target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "--bind")
endif()
