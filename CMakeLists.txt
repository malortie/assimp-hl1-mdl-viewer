cmake_minimum_required(VERSION 3.0)

if (NOT WIN32)
    message(FATAL_ERROR "At this time, this project only supports Windows.")
endif()

include(FetchContent)

set(HLMDLVIEWER_GAME_EXECUTABLE_DIR "" CACHE STRING 
    "Directory containing the game .exe. This is used to locate the sound folder.")
option(HLMDLVIEWER_USE_NANOGUI  "Use NanoGUI for user interface" ON)

if (HLMDLVIEWER_GAME_EXECUTABLE_DIR)
    # Convert to CMAKE path and strip any slash at the end.
    set(HLMDLVIEWER_GAME_EXECUTABLE_DIR_CMAKE_PATH "")
    file(TO_CMAKE_PATH ${HLMDLVIEWER_GAME_EXECUTABLE_DIR} HLMDLVIEWER_GAME_EXECUTABLE_DIR_CMAKE_PATH)
endif()

macro(ADD_EXTERNAL_PROJECT name repo)
    project(${name})
    
    get_filename_component(fc_base ${name} REALPATH BASE_DIR "${CMAKE_SOURCE_DIR}")

    set(FETCHCONTENT_BASE_DIR ${fc_base})

    FetchContent_Declare(${name}
      GIT_REPOSITORY ${repo}
      GIT_TAG ${ARGV2}
      CMAKE_ARGS -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
    FetchContent_MakeAvailable(${name})
endmacro()

# Add glm.
ADD_EXTERNAL_PROJECT(glm
    https://github.com/g-truc/glm.git
    0.9.9.6
)
set(GLM_INCLUDE_DIRS ${glm_SOURCE_DIR})

# Add Assimp.
ADD_EXTERNAL_PROJECT(assimp
    https://github.com/malortie/assimp.git
)

set(ASSIMP_INCLUDE_DIRS "")
list(APPEND ASSIMP_INCLUDE_DIRS "${assimp_SOURCE_DIR}/include/")
list(APPEND ASSIMP_INCLUDE_DIRS "${assimp_BINARY_DIR}/include/")

# Add Nanogui if enabled.
if (${HLMDLVIEWER_USE_NANOGUI})
    ADD_EXTERNAL_PROJECT(nanogui
        https://github.com/wjakob/nanogui.git
    )

    set(NANOGUI_INCLUDE_DIRS "")
    list(APPEND NANOGUI_INCLUDE_DIRS "${nanogui_SOURCE_DIR}/ext/eigen/")
    list(APPEND NANOGUI_INCLUDE_DIRS "${nanogui_SOURCE_DIR}/ext/glfw/include/")
    list(APPEND NANOGUI_INCLUDE_DIRS "${nanogui_SOURCE_DIR}/ext/glad/include/")
    list(APPEND NANOGUI_INCLUDE_DIRS "${nanogui_SOURCE_DIR}/ext/nanovg/src/")
    list(APPEND NANOGUI_INCLUDE_DIRS "${nanogui_SOURCE_DIR}/include/")
endif()

add_subdirectory(hl_mdlviewer)
