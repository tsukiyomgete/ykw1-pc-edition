cmake_minimum_required(VERSION 3.20)
project(ykw1_pc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(ykw1 ${SOURCES})
target_include_directories(ykw1 PRIVATE src)