# Demos CMakeLists.txt

# Add UTF-8 support for MSVC for all demo targets
if(MSVC)
    # Use /utf-8 for VS 2015 Update 2+ (MSVC 19.00.24215.1+), fallback to explicit flags for older versions
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.00.24215)
        add_compile_options(/utf-8)
    else()
        add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
    endif()
endif()

# Find all demo source files, excluding removed string iterator demos
file(GLOB DEMO_SOURCES "*.cpp")
list(REMOVE_ITEM DEMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/string_iterators_scanning_demo.cpp")
list(REMOVE_ITEM DEMO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/string_iterators_stl_demo.cpp")

# Create executable for each demo
foreach(DEMO_SOURCE ${DEMO_SOURCES})
    # Get filename without extension
    get_filename_component(DEMO_NAME ${DEMO_SOURCE} NAME_WE)
    
    # Create executable
    add_executable(${DEMO_NAME} ${DEMO_SOURCE})
    
    # Link with ustr (header-only library)
    target_link_libraries(${DEMO_NAME} PRIVATE ustr::ustr)
    
    # Set output directory
    set_target_properties(${DEMO_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    
    message(STATUS "Added demo: ${DEMO_NAME}")
endforeach()

# Add multi-module demo if directory exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/multi_module/CMakeLists.txt")
    add_subdirectory(multi_module)
    message(STATUS "Added multi-module demo")
endif()

# Custom target for running all demos
add_custom_target(run_demos
    COMMENT "Running all demos"
)

# Add each demo to the run_demos target
foreach(DEMO_SOURCE ${DEMO_SOURCES})
    get_filename_component(DEMO_NAME ${DEMO_SOURCE} NAME_WE)
    add_custom_command(TARGET run_demos POST_BUILD
        COMMAND echo "Running ${DEMO_NAME}..."
        COMMAND ${CMAKE_BINARY_DIR}/bin/${DEMO_NAME}
        COMMAND echo "${DEMO_NAME} completed."
        COMMENT "Running demo: ${DEMO_NAME}"
    )
    add_dependencies(run_demos ${DEMO_NAME})
endforeach()

message(STATUS "Demo configuration:")
message(STATUS "  Demo executables will be placed in: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "  Use 'make run_demos' or 'cmake --build . --target run_demos' to run all demos")
