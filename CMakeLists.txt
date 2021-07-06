#
# vk3DLoader
#

cmake_minimum_required(VERSION 3.16.3)

include(cmake/tools/guards.cmake)
include(cmake/tools/warnings.cmake)

#
# Project configuration
#

project(
    poike
    DESCRIPTION "A Vulkan command-line tool to visualize a 3D model"
    HOMEPAGE_URL "https://github.com/florianvazelle/vk3DLoader"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

message(VERBOSE "*")
message(VERBOSE "* ${PROJECT_NAME} v${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")
message(VERBOSE "* Copyright (c) 2017-2021 Florian Vazelle <ponythugflorian@gmail.com>")
message(VERBOSE "*")

include(cmake/tools/libcpp.cmake)
include(cmake/tools/sanitizer.cmake)

# Vulkan is a required dependency.
include(FindVulkan)
if(NOT VULKAN_FOUND)
    message(FATAL_ERROR "Vulkan SDK not installed.")
endif()

#
# Define library target
#

file(GLOB_RECURSE PROJECT_SOURCES "${CMAKE_SOURCE_DIR}/src/*.hpp")

# ---- Conan package ----

if(CONAN_EXPORTED)
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    include("${CMAKE_SOURCE_DIR}/cmake/conan.cmake")
    conan_cmake_run(CONANFILE conanfile.txt BASIC_SETUP BUILD missing)
endif()

file(GLOB_RECURSE EXTERNALS "${CMAKE_CURRENT_BINARY_DIR}/external/*")


# ---- Create library ----

add_library(${PROJECT_NAME} OBJECT ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${EXTERNALS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/external)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_LIBS} Vulkan::Vulkan)
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_VULKAN)

target_set_warnings(
    ${PROJECT_NAME}
    ENABLE ALL
    AS_ERROR ALL
    DISABLE Annoying missing-field-initializers unused-parameter unused-function
)

# ---- Compile shader into SPIR-V ----

file(GLOB_RECURSE SHADERS "${CMAKE_SOURCE_DIR}/assets/shaders/*.vert" "${CMAKE_SOURCE_DIR}/assets/shaders/*.frag" "${CMAKE_SOURCE_DIR}/assets/shaders/*.comp")

compile_shaders(TARGETS ${SHADERS})

#
# Entry points
#

option(BUILD_APPS "Enable building with entry points." ON)

if(BUILD_APPS)
    add_subdirectory(app)

    option(INSTALL_APPS "Enable installation of the entry points." OFF)

    if (INSTALL_APPS)
        install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} DESTINATION .)
    endif()
endif()

#
# Tests
#

option(BUILD_TESTING "Enable building tests." OFF)

if(BUILD_TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()

#
# Documentation
#

option(BUILD_DOCS "Enable building with documentation." OFF)

if(BUILD_DOCS)
    find_package(Doxygen 1.8)

    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    endif()
endif()
