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

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${EIGEN3_INCLUDE_DIR}
  ${VCGLIB_DIR}
  ${MESHLAB_DIR}
  ${EMSDK_DIR}
)

# set output file directory
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${NGINX_DIR}/html/${PROJECT_NAME})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${NGINX_DIR}/html/${PROJECT_NAME})

# Add Executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    main.cpp
)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "--bind")

    # if defined NGINX_DIR, copy output files to NGINX_DIR
    if(DEFINED ENV{NGINX_DIR})
        file(TO_CMAKE_PATH $ENV{NGINX_DIR} NGINX_DIR)
        
        # make output directory for nginx
        file(MAKE_DIRECTORY ${NGINX_DIR}/html/${PROJECT_NAME})

        # copy neccesarry files to nginx directory after building
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.js
            ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.wasm
            ${NGINX_DIR}/html/${PROJECT_NAME}
        )
        # html file is not related with building, so use file command
        file(COPY
            index.html
            DESTINATION
            ${NGINX_DIR}/html/${PROJECT_NAME}
        )
    endif()
endif()
