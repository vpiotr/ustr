# Tests CMakeLists.txt

# Find the utest header (it's included in our project)
# No need to find external packages since utest is header-only and included

# Test executable
add_executable(ustr_test ustr_test.cpp)

# Link with ustr (header-only library)
target_link_libraries(ustr_test PRIVATE ustr::ustr)

# Set output directory
set_target_properties(ustr_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add test to CTest
add_test(NAME ustr_tests COMMAND ustr_test)

# Test discovery for better integration with IDEs
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.10)
    include(GoogleTest)
    # Since we're not using Google Test, we'll use a simple approach
    # The test will run as a single unit
endif()

# Custom target for running tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ustr_test
    COMMENT "Running all tests"
)

# Add compile definitions for debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ustr_test PRIVATE DEBUG=1)
endif()

message(STATUS "Test configuration:")
message(STATUS "  Test executable: ustr_test")
message(STATUS "  Output directory: ${CMAKE_BINARY_DIR}/bin")
