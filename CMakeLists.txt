cmake_minimum_required(VERSION 3.10)

# Project information
project(ustr 
    VERSION 1.0.0
    DESCRIPTION "Universal String Conversion Library for C++"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.14)
        add_compile_options(/experimental:external /external:W0)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
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
