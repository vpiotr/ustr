add_library(module1 STATIC module1.cpp)
target_include_directories(module1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

# Add UTF-8 support for MSVC
if(MSVC)
    target_compile_options(module1 PRIVATE /utf-8 /source-charset:utf-8 /execution-charset:utf-8)
endif()
