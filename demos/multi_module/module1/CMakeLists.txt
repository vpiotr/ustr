add_library(module1 STATIC module1.cpp)
target_include_directories(module1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)
