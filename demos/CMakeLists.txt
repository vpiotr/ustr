# Demos CMakeLists.txt

# Find all demo source files
file(GLOB DEMO_SOURCES "*.cpp")

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
