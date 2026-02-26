cmake_minimum_required(VERSION 3.21)
project(AAD
        VERSION 1.0.0
        LANGUAGES CXX
)
set (LIB_NAME AAD)


# ============================================================
# Global output layout (SINGLE-CONFIG)
# ============================================================
set(_BIN_DIR ${CMAKE_BINARY_DIR}/bin)
set(_LIB_DIR ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${_BIN_DIR})   # executables go here
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${_BIN_DIR})   # shared libraries (DLL) go here
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${_LIB_DIR})   # static libraries (.lib) and import libs go here

# ---------------------------
# Policies and warnings
# ---------------------------
# Use NEW behavior for timestamp of downloaded content
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# ---------------------------
# Enable testing
# ---------------------------
include(CTest)
enable_testing()

# ---------------------------
# C++ standard
# ---------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------
# Source and header files
# ---------------------------
file(GLOB_RECURSE AAD_SOURCES
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/common/*.c*" #covering .c, .cpp and .cxx etc.
)

# ---------------------------
# Create shared library
# ---------------------------
add_library(AADShared SHARED ${AAD_SOURCES})

set_target_properties(AADShared PROPERTIES
        OUTPUT_NAME ${LIB_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${_BIN_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${_LIB_DIR}   # import lib
)
target_compile_definitions(AADShared PRIVATE
        AAD_SHARED_DEFINE
        AAD_BUILDING_DLL
)
target_include_directories(AADShared PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---------------------------
# Create static library
# ---------------------------
add_library(AADStatic STATIC ${AAD_SOURCES})
set_target_properties(AADStatic PROPERTIES
        OUTPUT_NAME "${LIB_NAME}_Static"
        ARCHIVE_OUTPUT_DIRECTORY
        ${_LIB_DIR})
target_compile_definitions(AADStatic PRIVATE AAD_STATIC_DEFINE)
target_include_directories(AADStatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ---------------------------
# SUB-DIRECTORIES
# ---------------------------
add_subdirectory(examples)