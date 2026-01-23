cmake_minimum_required(VERSION 3.10)

project(jSTL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE JSTL_SOURCES "src/*.cpp")
add_library(jSTL SHARED ${JSTL_SOURCES})
target_include_directories(jSTL PRIVATE ${CMAKE_SOURCE_DIR}/include)
if(UNIX AND NOT APPLE)
    target_link_libraries(jSTL pthread)
endif()

# 测试可执行文件
add_executable(test_thread test/test_thread.cpp)
target_include_directories(test_thread PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_thread jSTL)