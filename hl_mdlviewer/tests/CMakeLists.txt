cmake_minimum_required(VERSION 3.0)

project (hl_mdlviewer_tests VERSION "0.0.0")

set(HLMDLVIEWER_TESTS_SHADERS_SOURCE_DIR "${PROJECT_SOURCE_DIR}/shaders")
set(HLMDLVIEWER_TESTS_SHADERS_BINARY_DIR "${PROJECT_BINARY_DIR}/shaders")

file(GLOB_RECURSE HLMDLVIEWER_TESTS_SOURCES
    "${PROJECT_SOURCE_DIR}/*.h"
    "${PROJECT_SOURCE_DIR}/*.hpp"
    "${PROJECT_SOURCE_DIR}/*.c"
    "${PROJECT_SOURCE_DIR}/*.cpp")

source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Source Files" FILES ${HLMDLVIEWER_TESTS_SOURCES})

add_library(${PROJECT_NAME} SHARED ${HLMDLVIEWER_TESTS_SOURCES})

set_source_files_properties(${HLMDLVIEWER_TESTS_SOURCES} PROPERTIES COMPILE_FLAGS "/Yupch.h")
set_source_files_properties("${PROJECT_SOURCE_DIR}/pch.cpp" PROPERTIES COMPILE_FLAGS "/Ycpch.h")

set(HLMDLVIEWER_TESTS_INCLUDE_DIRS ${PROJECT_SOURCE_DIR})

target_include_directories(
${PROJECT_NAME} 
PUBLIC
${HLMDLVIEWER_TESTS_INCLUDE_DIRS}
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    HLMDLVIEWER_SHADERS_SEARCH_PATH=\"${HLMDLVIEWER_TESTS_SHADERS_BINARY_DIR}\")

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(${PROJECT_NAME} hl_mdlviewer_lib)

ADD_POSTBUILD_EVENT_COPY_DIRECTORY(
${PROJECT_NAME} # target
${HLMDLVIEWER_TESTS_SHADERS_SOURCE_DIR} # source directory
${HLMDLVIEWER_TESTS_SHADERS_BINARY_DIR} # target directory
)
