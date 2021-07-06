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
    DESCRIPTION "A comprehensible Vulkan framework"
    HOMEPAGE_URL "https://github.com/florianvazelle/poike"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

message(VERBOSE "*")
message(VERBOSE "* ${PROJECT_NAME} v${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")
message(VERBOSE "* Copyright (c) 2020-2021 Florian Vazelle <ponythugflorian@gmail.com>")
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

file(GLOB_RECURSE POIKE_PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE POIKE_PROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

# ---- Conan package ----

include("./cmake/conan.cmake")

conan_check(REQUIRED)
conan_cmake_run(
    CONANFILE
    conanfile.txt
    BASIC_SETUP
    CONAN_COMMAND
    ${CONAN_CMD}
    CMAKE_TARGETS
    BUILD
    missing)
include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE EXTERNALS "${CMAKE_CURRENT_BINARY_DIR}/external/*")


# ---- Create library ----

add_library(${PROJECT_NAME} SHARED ${POIKE_PROJECT_SOURCES} ${POIKE_PROJECT_HEADERS} ${EXTERNALS})

target_include_directories(${PROJECT_NAME} PUBLIC ./include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/external)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_LIBS} Vulkan::Vulkan)
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_VULKAN)

target_set_warnings(
    ${PROJECT_NAME}
    ENABLE ALL
    AS_ERROR ALL
    DISABLE Annoying missing-field-initializers unused-parameter unused-function
)

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
