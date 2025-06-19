add_library(module1 STATIC module1.cpp)
target_include_directories(module1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

# Add UTF-8 support for MSVC
if(MSVC)
    # Use /utf-8 for VS 2015 Update 2+ (MSVC 19.00.24215.1+), fallback to explicit flags for older versions
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.00.24215)
        target_compile_options(module1 PRIVATE /utf-8)
    else()
        target_compile_options(module1 PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
    endif()
endif()
