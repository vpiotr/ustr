add_library(module2 STATIC module2.cpp)
target_include_directories(module2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)
