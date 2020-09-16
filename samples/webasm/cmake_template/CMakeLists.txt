cmake_minimum_required( VERSION 3.6 )

# Create Project
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} CXX)

set(CMAKE_CXX_STANDARD 11)

file(TO_CMAKE_PATH $ENV{EMSDK_DIR} EMSDK_DIR)

# set output file directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Add Executable
add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${EMSDK_DIR}
)

target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

if (EMSCRIPTEN)
    # set(CMAKE_EXECUTABLE_SUFFIX ".html") # デフォルトでは.js
    set(CMAKE_CXX_FLAGS "--bind")
endif()
