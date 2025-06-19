# Tests CMakeLists.txt

# Find the utest header (it's included in our project)
# No need to find external packages since utest is header-only and included

# Add UTF-8 support for MSVC for all test targets
if(MSVC)
    add_compile_options(/utf-8 /source-charset:utf-8 /execution-charset:utf-8)
endif()

# Core features test executable
add_executable(ustr_core_features_test ustr_core_features_test.cpp)

# Container test executable
add_executable(ustr_container_test ustr_container_test.cpp)

# Custom classes test executable
add_executable(ustr_custom_classes_test ustr_custom_classes_test.cpp)

# Format context test executable
add_executable(ustr_format_context_test ustr_format_context_test.cpp)

# Pair test executable
add_executable(ustr_pair_test ustr_pair_test.cpp)

# Tuple test executable
add_executable(ustr_tuple_test ustr_tuple_test.cpp)

# Custom specialization test executable
add_executable(ustr_custom_specialization_test ustr_custom_specialization_test.cpp)

# Quoted string test executable
add_executable(ustr_quoted_str_test ustr_quoted_str_test.cpp)

# Enum test executable
add_executable(ustr_enum_test ustr_enum_test.cpp)

# Remove string iterator tests
# add_executable(string_iterators_scanning_test string_iterators_scanning_test.cpp)
# add_executable(string_iterators_stl_test string_iterators_stl_test.cpp)

# Link with ustr (header-only library)
target_link_libraries(ustr_core_features_test PRIVATE ustr::ustr)
target_link_libraries(ustr_container_test PRIVATE ustr::ustr)
target_link_libraries(ustr_custom_classes_test PRIVATE ustr::ustr)
target_link_libraries(ustr_format_context_test PRIVATE ustr::ustr)
target_link_libraries(ustr_pair_test PRIVATE ustr::ustr)
target_link_libraries(ustr_tuple_test PRIVATE ustr::ustr)
target_link_libraries(ustr_custom_specialization_test PRIVATE ustr::ustr)
target_link_libraries(ustr_quoted_str_test PRIVATE ustr::ustr)
target_link_libraries(ustr_enum_test PRIVATE ustr::ustr)
# target_link_libraries(string_iterators_scanning_test PRIVATE ustr::ustr)
# target_link_libraries(string_iterators_stl_test PRIVATE ustr::ustr)

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
set_target_properties(ustr_format_context_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ustr_pair_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ustr_tuple_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ustr_custom_specialization_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ustr_quoted_str_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ustr_enum_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
# set_target_properties(string_iterators_scanning_test PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
# )
# set_target_properties(string_iterators_stl_test PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
# )

# Add tests to CTest
add_test(NAME ustr_core_features_tests COMMAND ustr_core_features_test)
add_test(NAME ustr_container_tests COMMAND ustr_container_test)
add_test(NAME ustr_custom_classes_tests COMMAND ustr_custom_classes_test)
add_test(NAME ustr_format_context_tests COMMAND ustr_format_context_test)
add_test(NAME ustr_pair_tests COMMAND ustr_pair_test)
add_test(NAME ustr_tuple_tests COMMAND ustr_tuple_test)
add_test(NAME ustr_custom_specialization_tests COMMAND ustr_custom_specialization_test)
add_test(NAME ustr_quoted_str_tests COMMAND ustr_quoted_str_test)
add_test(NAME ustr_enum_tests COMMAND ustr_enum_test)
# add_test(NAME string_iterators_scanning_tests COMMAND string_iterators_scanning_test)
# add_test(NAME string_iterators_stl_tests COMMAND string_iterators_stl_test)

# Test discovery for better integration with IDEs
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.10)
    include(GoogleTest)
    # Since we're not using Google Test, we'll use a simple approach
    # The tests will run as single units
endif()

# Custom target for running tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ustr_core_features_test ustr_container_test ustr_custom_classes_test ustr_format_context_test ustr_pair_test ustr_tuple_test ustr_custom_specialization_test ustr_quoted_str_test ustr_enum_test
    COMMENT "Running all tests"
)

# Add compile definitions for debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ustr_core_features_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_container_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_custom_classes_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_format_context_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_pair_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_tuple_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_custom_specialization_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_quoted_str_test PRIVATE DEBUG=1)
    target_compile_definitions(ustr_enum_test PRIVATE DEBUG=1)
endif()

message(STATUS "Test configuration:")
message(STATUS "  Test executables: ustr_core_features_test, ustr_container_test, ustr_custom_classes_test, ustr_format_context_test, ustr_pair_test, ustr_tuple_test, ustr_custom_specialization_test, ustr_quoted_str_test, ustr_enum_test")
message(STATUS "  Output directory: ${CMAKE_BINARY_DIR}/bin")
