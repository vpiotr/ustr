cmake_minimum_required(VERSION 3.10)

# Project information
project(ustr 
    VERSION 1.0.0
    DESCRIPTION "Universal String Conversion Library for C++"
    LANGUAGES CXX
)

# Set C++ standard (allow override from command line)
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    # Windows (MSVC) - Enable most warnings including conversion warnings
    add_compile_options(/W4 /WX- /wd4996)
    # Force UTF-8 encoding for source and execution character sets
    # Use /utf-8 for VS 2015 Update 2+ (MSVC 19.00.24215.1+), fallback to explicit flags for older versions
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.00.24215)
        add_compile_options(/utf-8)
    else()
        add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
    endif()
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.14)
        add_compile_options(/experimental:external /external:W0)
    endif()
else()
    # Linux/GCC or Clang - Enable comprehensive warnings including conversion warnings
    add_compile_options(
        -Wall                    # Enable most warnings
        -Wextra                  # Enable extra warnings
        -Wconversion             # Warn about type conversions that may alter values
        -Wsign-conversion        # Warn about sign conversions
        -Wfloat-conversion       # Warn about float conversions
        -Wcast-qual              # Warn about casts that remove qualifiers
        -Wcast-align             # Warn about casts that increase alignment requirements
        -Wshadow                 # Warn about variable shadowing
        -Wpedantic               # Warn about non-standard C++
        -Wunused                 # Warn about unused variables/functions
        -Wuninitialized          # Warn about uninitialized variables
    )
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O2)
    endif()
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Options
option(USTR_BUILD_TESTS "Build tests" ON)
option(USTR_BUILD_DEMOS "Build demos" ON)
option(USTR_BUILD_DOCS "Build documentation" OFF)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Header-only library target
add_library(ustr INTERFACE)
target_include_directories(ustr INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ustr INTERFACE cxx_std_11)

# Add UTF-8 support for MSVC to the interface library
if(MSVC)
    # Use /utf-8 for VS 2015 Update 2+ (MSVC 19.00.24215.1+), fallback to explicit flags for older versions
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.00.24215)
        target_compile_options(ustr INTERFACE /utf-8)
    else()
        target_compile_options(ustr INTERFACE /source-charset:utf-8 /execution-charset:utf-8)
    endif()
endif()

# Add alias for consistent naming
add_library(ustr::ustr ALIAS ustr)

# Tests
if(USTR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Demos
if(USTR_BUILD_DEMOS)
    add_subdirectory(demos)
endif()

# Documentation
if(USTR_BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    else()
        message(WARNING "Doxygen not found, documentation will not be built")
    endif()
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install header files
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Install library target
install(TARGETS ustr
    EXPORT ustr-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install export targets
install(EXPORT ustr-targets
    FILE ustr-targets.cmake
    NAMESPACE ustr::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ustr
)

# Create config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ustr-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ustr-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ustr
)

# Create version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ustr-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install config files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ustr-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ustr-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ustr
)

# Export targets for build tree
export(EXPORT ustr-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/ustr-targets.cmake
    NAMESPACE ustr::
)

# Register package
export(PACKAGE ustr)

# Print build summary
message(STATUS "")
message(STATUS "USTR Build Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build tests: ${USTR_BUILD_TESTS}")
message(STATUS "  Build demos: ${USTR_BUILD_DEMOS}")
message(STATUS "  Build docs: ${USTR_BUILD_DOCS}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
