# Tests CMakeLists.txt

# Find the utest header (it's included in our project)
# No need to find external packages since utest is header-only and included

# Core features test executable
add_executable(ustr_core_features_test ustr_core_features_test.cpp)

# Container test executable
add_executable(ustr_container_test ustr_container_test.cpp)

# Custom classes test executable
add_executable(ustr_custom_classes_test ustr_custom_classes_test.cpp)

# Scoped formatter test executable
add_executable(ustr_scoped_formatter_test ustr_scoped_formatter_test.cpp)

# Link with ustr (header-only library)
target_link_libraries(ustr_core_features_test PRIVATE ustr::ustr)
target_link_libraries(ustr_container_test PRIVATE ustr::ustr)
target_link_libraries(ustr_custom_classes_test PRIVATE ustr::ustr)
target_link_libraries(ustr_scoped_formatter_test PRIVATE ustr::ustr)

# Set output directory
set_target_properties(ustr_core_features_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ustr_container_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ustr_custom_classes_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ustr_scoped_formatter_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add tests to CTest
add_test(NAME ustr_core_features_tests COMMAND ustr_core_features_test)
add_test(NAME ustr_container_tests COMMAND ustr_container_test)
add_test(NAME ustr_custom_classes_tests COMMAND ustr_custom_classes_test)
add_test(NAME ustr_scoped_formatter_tests COMMAND ustr_scoped_formatter_test)

# Test discovery for better integration with IDEs
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.10)
    include(GoogleTest)
    # Since we're not using Google Test, we'll use a simple approach
    # The tests will run as single units
endif()

# Custom target for running tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ustr_core_features_test ustr_container_test ustr_custom_classes_test ustr_scoped_formatter_test
    COMMENT "Running all tests"
)

# Add compile definitions for debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ustr_core_features_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_container_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_custom_classes_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_scoped_formatter_test PRIVATE DEBUG=1)
endif()

message(STATUS "Test configuration:")
message(STATUS "  Test executables: ustr_core_features_test, ustr_container_test, ustr_custom_classes_test, ustr_scoped_formatter_test")
message(STATUS "  Output directory: ${CMAKE_BINARY_DIR}/bin")
