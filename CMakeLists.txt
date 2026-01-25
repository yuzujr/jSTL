cmake_minimum_required(VERSION 3.10)

project(jSTL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE JSTL_SOURCES "src/*.cpp")
add_library(jSTL SHARED ${JSTL_SOURCES})
target_compile_definitions(jSTL PUBLIC JSTL_STD_VER=${CMAKE_CXX_STANDARD})
target_include_directories(jSTL PUBLIC ${CMAKE_SOURCE_DIR}/include)
if(UNIX AND NOT APPLE)
    target_link_libraries(jSTL pthread)
endif()

# test
add_executable(test_headers test/test_headers.cpp)
target_link_libraries(test_headers jSTL)

add_executable(test_thread test/test_thread.cpp)
target_link_libraries(test_thread jSTL)